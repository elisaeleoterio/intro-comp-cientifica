#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <likwid.h>

#include "utils.h"
#include "matriz.h"

// Monta a matriz de Broyden a partir do vetor X seguindo a estrutura
// f1(x) = -2x1² + 3x1 - 2x2 + 1
// fi(x) = -2xi² + 3xi - xi-1 - 2xi+1 + 1, se 2<= i<= (n-1)
// fn(x) = -2xn² + 3xn - xn-1
double *criaEResolveBroyden(int n, double *x, FILE *output) {

    // TODO Verificar se é a melhor maneira de alcoar a matriz
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

    LIKWID_MARKER_START("Jacobiana");
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
        J[i][i] = 3 - 4*x[i+1];
        J[i][i+1] = -2;
    }
    J[n-1][n-2] = -1;
    J[n-1][n-1] = 3 - 4*x[n-1];
    
    LIKWID_MARKER_STOP("Jacobiana");
    aux = timestamp() - aux;
    *tempJacobiana += aux;
    return J;
}


// Faz a resolução do sistema linear usando Eliminação de Gauss e Retrossubstituição
double *resolverSistemaLinear(double **J, int n, double *F, rtime_t *tempSL, FILE *output) {
    
    LIKWID_MARKER_START("Sistema_Linear");
    rtime_t aux = timestamp();

    for (size_t i = 0; i < n; i++) {
        F[i] = (-1) * F[i];
    }
    
    // Eliminação de Gauss
    for (int i = 0; i < n; ++i) {
        int iMax = i;
        for (int k = i + 1; k < n; ++k)
            if (J[k][i] > J[iMax][i])
                iMax = k;
        if (iMax != i)
        {
            double *tmp, aux;
            tmp = J[i];
            J[i] = J[iMax];
            J[iMax] = tmp;

            aux = F[i];
            F[i] = F[iMax];
            F[iMax] = aux;
        }

        for (int k = i + 1; k < n; ++k) {
            double m = J[k][i] / J[i][i];
            J[k][i] = 0.0;
            for (int j = i + 1; j < n; ++j)
                J[k][j] -= J[i][j] * m;
            F[k] -= F[i] * m;
        }
    }

    double *delta = malloc(n * sizeof(double));
    // Retrossubstituição
    for (int i = n - 1; i >= 0; --i) {
        delta[i] = F[i];
        for (int j = i + 1; j < n; ++j)
            delta[i] -= J[i][j] * delta[j];
        delta[i] /= J[i][i];
    }

    LIKWID_MARKER_STOP("Sistema_Linear");
    aux = timestamp() - aux;
    *tempSL += aux;
    free(F);
    return delta;
}



double *metodoDeNewton(FILE *output, double *x, int max, double epsilon, int n, rtime_t *tempoNewton, rtime_t *tempoJacobiana, rtime_t *tempoSL) {

    LIKWID_MARKER_START("Newton");
    *tempoNewton = timestamp();
    for (size_t i = 0; i < max; i++) {

        imprimirX(output, x, n, i);
        double *F = criaEResolveBroyden(n, x, output);
        
        if (maxVetor(F, n) < epsilon) {
            LIKWID_MARKER_STOP("Newton");
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
            LIKWID_MARKER_STOP("Newton");
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
    LIKWID_MARKER_STOP("Newton");
    *tempoNewton = timestamp() - *tempoNewton;
    return NULL;
}