#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <likwid.h>
#include <math.h>

#include "utils.h"
#include "matriz.h"

#define MAX_ITER 1000
#define TOLERANCIA 1e-6

/* VERSÃO NÃO OTIMIZADA */

// Monta a matriz de Broyden a partir do vetor X seguindo a estrutura
// f1(x) = -2x1² + 3x1 - 2x2 + 1
// fi(x) = -2xi² + 3xi - xi-1 - 2xi+1 + 1, se 2<= i<= (n-1)
// fn(x) = -2xn² + 3xn - xn-1
double *criaEResolveBroyden(int n, double *x, FILE *output) {
    double *F = malloc(n * sizeof(double));
    verifica_alocacao(F, output);

    // Monta a matriz
    F[0] = (3 - 2*x[0])*x[0] - 2*x[1] + 1;
    for(int i = 1; i < n - 1; i++) {
        F[i] = (3 - 2*x[i])*x[i] - x[i-1] - 2*x[i+1] + 1;
    }
    F[n-1] = (3 - 2*x[n-1])*x[n-1] - x[n-2] + 1;
    
    return F;
}

// Calcula a matriz com as derivadas parciais da matriz de broyden
double **calcularJacobiana(double *x, int n, rtime_t *tempJacobiana) {

    char marker_jac[50];
    snprintf(marker_jac, sizeof(marker_jac), "Jacobiana_%d", n);

    LIKWID_MARKER_START(marker_jac);
    rtime_t aux = timestamp();

    // Alocação dinâmica da matriz jacobiana
    double **J = calloc(n, sizeof(double*));
    for (size_t i = 0; i < n; i++) {
        J[i] = calloc(n, sizeof(double));
    }

    // Criar matriz jacobiana
    J[0][0] = 3 - 4*x[0];
    J[0][1] = -2;
    
    for (int i = 1; i < n - 1; i++) {
        J[i][i-1] = -1;
        J[i][i] = 3 - 4*x[i];
        J[i][i+1] = -2;
    }
    J[n-1][n-2] = -1;
    J[n-1][n-1] = 3 - 4*x[n-1];
    
    LIKWID_MARKER_STOP(marker_jac);
    aux = timestamp() - aux;
    *tempJacobiana += aux;
    return J;
}

// Faz a resolução do sistema linear usando Gauss-Seidel sem otimização para matriz k-diagonal
double *resolverSistemaLinear(double **J, int n, double *F, rtime_t *tempSL, FILE *output) {
    
    char marker_sl[50];
    snprintf(marker_sl, sizeof(marker_sl), "Sistema_Linear_%d", n);
    LIKWID_MARKER_START(marker_sl);
    rtime_t aux = timestamp();

    for (size_t i = 0; i < n; i++) {
        F[i] = (-1) * F[i];
    }

    double *delta = calloc(n, sizeof(double));
    
    for (int k = 0; k < MAX_ITER; k++) {
        double erro_maximo = 0.0;

        for (int i = 0; i < n; i++) {
            double soma = 0.0;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    soma += J[i][j] * delta[j];
                }
            }
            double novo_valor = (F[i] - soma) / J[i][i];
            double erro_atual = fabs(novo_valor - delta[i]);
            if (erro_atual > erro_maximo) {
                erro_maximo = erro_atual;
            }
            delta[i] = novo_valor;
        }
        if (erro_maximo < TOLERANCIA) {
            break; 
        }
    }

    LIKWID_MARKER_STOP(marker_sl);
    aux = timestamp() - aux;
    *tempSL += aux;
    return delta;
}


// só vai convergir se |3 - 4x| > 3
double *metodoDeNewton(FILE *output, double *x, int max, double epsilon, int n, rtime_t *tempoNewton, rtime_t *tempoJacobiana, rtime_t *tempoSL) {

    char marker_new[50];
    snprintf(marker_new, sizeof(marker_new), "Newton_%d", n);
    LIKWID_MARKER_START(marker_new);
    *tempoNewton = timestamp();
    for (size_t i = 0; i < max; i++) {

        imprimirX(output, x, n, i);
        
        double *F = criaEResolveBroyden(n, x, output);
        
        if (maxVetor(F, n) < epsilon) {
            LIKWID_MARKER_STOP(marker_new);
            *tempoNewton = timestamp() - *tempoNewton;
            free(F);
            return x;
        }

        double **J = calcularJacobiana(x, n, tempoJacobiana);

        double *delta = resolverSistemaLinear(J, n, F, tempoSL, output);

        for (size_t i = 0; i < n; i++) {
            x[i] = x[i] + delta[i];
        }
        if (maxVetor(delta, n) < epsilon) {
            LIKWID_MARKER_STOP(marker_new);
            *tempoNewton = timestamp() - *tempoNewton;
            free(F);
            free(delta);
            for (int j = 0; j < n; j++) {
                free(J[j]);
            }
            free(J);
            return x;
        }

        for (int j = 0; j < n; j++) {
            free(J[j]);
        }
        free(J);
        free(delta);
        free(F);
    }
    LIKWID_MARKER_STOP(marker_new);
    *tempoNewton = timestamp() - *tempoNewton;
    return NULL;
}