#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <likwid.h>
#include <math.h>

#include "utils.h"
#include "matriz.h"

#define MAX_GAUSS_SEIDEL 1000


// Monta a matriz de Broyden resolvida a partir do vetor X seguindo a estrutura
// f1(x) = -2x1² + 3x1 - 2x2 + 1
// fi(x) = -2xi² + 3xi - xi-1 - 2xi+1 + 1, se 2<= i<= (n-1)
// fn(x) = -2xn² + 3xn - xn-1
double *criaEResolveBroyden(int n, double *x, FILE *output) {

    double *F = malloc(n * sizeof(double));
    verifica_alocacao(F, output);

    F[0] = (3 - 2*x[0])*x[0] - 2*x[1] + 1;
    for(int i = 1; i < n - 1; i++) {
        F[i] = (3 - 2*x[i])*x[i] - x[i-1] - 2*x[i+1] + 1;
    }
    F[n-1] = (3 - 2*x[n-1])*x[n-1] - x[n-2] + 1;
    
    return F;
}

// Calcula a matriz Jacobiana com as derivadas parciais da matriz de Broyden
double *calcularJacobiana(double *x, int n, rtime_t *tempJacobiana) {
    
    LIKWID_MARKER_START("Jacobiana");
    rtime_t aux = timestamp();

    // Vetor para armazenar a diagonal principal 
    // (única que não é constante) diferentes de 0 da matriz Jacobiana
    double *d = malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        d[i] = 3 - 4 * x[i];
    }
    LIKWID_MARKER_STOP("Jacobiana");
    aux = timestamp() - aux;
    *tempJacobiana += aux;
    return d;
}


// Resolve o Sistema Linear J(X(i))𝚫(i) = -F(X(i))
double *resolverSistemaLinear(double *d, int n, double *F, rtime_t *tempSL, FILE *output, double epsilon, int max_iter) {
    
    LIKWID_MARKER_START("Sistema_Linear");
    rtime_t aux = timestamp();

    double *delta = calloc(n, sizeof(double));
    double erro = 1.0 + epsilon;
    int iter = 0;
    while (erro > epsilon && iter < max_iter) {
        double max_dif = 0.0;
        double old;
        old = delta[0];
        delta[0] = (-F[0] + 2 * delta[1])/d[0];

        if (fabs(delta[0] - old) > max_dif) {
            max_dif = fabs(delta[0] - old);
        }

        for (int i = 1; i < n - 1; i++) {
            old = delta[i];
            delta[i] = (-F[i] + 1 * delta[i - 1] + 2 * delta[i + 1])/ d[i];
            if (fabs(delta[i] - old) > max_dif) {
                max_dif = fabs(delta[i] - old);
            }
        }
        delta[n - 1] = (-F[n - 1] + 1 * delta[n - 2]) / d[n - 1];
        if (fabs(delta[n - 1] - old) > max_dif) {
            max_dif = fabs(delta[n - 1] - old);
        }

        erro = max_dif;
        iter ++; 
    }

    LIKWID_MARKER_STOP("Sistema_Linear");
    aux = timestamp() - aux;
    *tempSL += aux;
    return delta;
}


// TODO Rever liberações de memória
double *metodoDeNewton(FILE *output, double *x, int max, double epsilon, int n, rtime_t *tempoNewton, rtime_t *tempoJacobiana, rtime_t *tempoSL) {
    // TODO verificar parâmetros
    
    
    LIKWID_MARKER_START("Newton");
    *tempoNewton = timestamp();
    
    // Implementação do Algoritmo de Newton com máximo de 25 iterações
    for (size_t i = 0; i < max; i++) {
        imprimirX(output, x, n, i);
        
        // cria matriz de broyden com variáveis iniciais
        double *F = criaEResolveBroyden(n, x, output);

        if (maxVetor(F, n) < epsilon) {
            LIKWID_MARKER_STOP("Newton");
            *tempoNewton = timestamp() - *tempoNewton;
            free(F);
            return x;
        }

        double *J = calcularJacobiana(x, n, tempoJacobiana);

        // TODO Rever se a passagem de parâmetros está certa
        double *delta = resolverSistemaLinear(J, n, F, tempoSL, output, epsilon, MAX_GAUSS_SEIDEL);

        for (size_t i = 0; i < n; i++) {
            x[i] = x[i] + delta[i];
        }

        if (maxVetor(delta, n) < epsilon) {
            LIKWID_MARKER_STOP("Newton");
            *tempoNewton = timestamp() - *tempoNewton;
            free(J);
            free(delta);
            free(F);
            return x;
        }

        free(J);
        free(delta);
        free(F);
    }
    LIKWID_MARKER_STOP("Newton");
    *tempoNewton = timestamp() - *tempoNewton;
    return NULL;
}