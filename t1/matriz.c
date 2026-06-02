#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <likwid.h>

#include "utils.h"
#include "matriz.h"

#define TAM_BANDA_JACOBIANA 3
#define TAM_SUPERDIAGONAL 1
#define TAM_SUBDIAGONAL 1

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
    
    char marker_jacob[50];
    snprintf(marker_jacob, sizeof(marker_jacob), "Jacobiana_%d", n);
    LIKWID_MARKER_START(marker_jacob);
    rtime_t aux = timestamp();

    // Tridiagonal de Broyden => Tam = 3
    int k1 = TAM_SUBDIAGONAL;
    int k2 = TAM_SUPERDIAGONAL;
    int tam = TAM_BANDA_JACOBIANA; 

    // Vetor para armazenar as 3-diagonais diferentes de 0 da matriz Jacobiana
    double *J = malloc(n * tam * sizeof(double));

    // Conversor das coordenadas indice = i * tam + (j − i + k1​)

    J[0] = 3 - 4*x[0];
    J[1 + k1] = -2; //(2)
    for (int i = 1; i < n - 1; i++) {
        J[i * tam ] = -1;
        J[i * tam + k1] = 3 - 4*x[i+1];
        J[i * tam + 1 + k1] = -2;
    }
    J[(n - 1) * tam + k1 - 1] = -1;
    J[(n - 1) * tam + k1] = 3 - 4*x[n-1];
    
    LIKWID_MARKER_STOP(marker_jacob);
    aux = timestamp() - aux;
    *tempJacobiana += aux;
    return J;
}


// TODO Otimizar usando Gauss-Seidel por ser uma matriz tridiagonal
double *resolverSistemaLinear(double *J, int n, double *x, rtime_t *tempSL, FILE *output) {
    
    char marker_sl[50];
    snprintf(marker_sl, sizeof(marker_sl), "Linear_%d", n);
    LIKWID_MARKER_START(marker_sl);
    rtime_t aux = timestamp();


    // J[0] = 3 - 4*x[0];; // Diagonal
    // J[1 + k1] = -2;;    // Superdiagonal

    // J[i * tam ] = -1; // Subdiagonal
    // J[i * tam + k1] = 3 - 4*x[i+1]; // Diagonal
    // J[i * tam + 1 + k1] = -2; // Superdiagnal

    // J[(n - 1) * tam + k1 - 1] = -1; // Subdiagonal
    // J[(n - 1) * tam + k1] = 3 - 4*x[n-1]; // Diagonal

//         void gaussSeidel (double *d, double *a, double *c,
// double *b, double *x, uint n, double tol)
// {
// double erro = 1.0 + tol;
// while (erro < tol) { // 5(n-2)+6 ≈ 5n FLOPs / iteração
// x[ 0 ] = (b[ 0 ] – c[ 0 ] * x[ 1 ]) / d[ 0 ];
// for (int i=1; i < n-1; ++i)
// x[ i ] = (b[ i ] – a[ i-1 ] * x[ i-1] – c[ i ] * x[ i+1 ]) / d[ i ];
// x[ n-1 ] = (b[ n-1 ] – a[ n-2 ] * x[ n-2 ] ) / d[ n-1 ];
// // Calcula erro == norma máxima de ( x(k) – x(k-1) )
// }

    

    // // Vetor de coeficientes independentes == Matriz de Broyden em X negados 
    // double *F = criaEResolveBroyden(n, x, output);
    // for (size_t i = 0; i < n; i++) {
    //     F[i] = (-1) * F[i];
    // }
    
    // // Eliminação de Gauss
    // for (int i = 0; i < n; ++i) {
    //     int iMax = i;
    //     for (int k = i + 1; k < n; ++k)
    //         if (J[k][i] > J[iMax][i])
    //             iMax = k;
    //     if (iMax != i)
    //     {
    //         double *tmp, aux;
    //         tmp = J[i];
    //         J[i] = J[iMax];
    //         J[iMax] = tmp;

    //         aux = F[i];
    //         F[i] = F[iMax];
    //         F[iMax] = aux;
    //     }

    //     for (int k = i + 1; k < n; ++k) {
    //         double m = J[k][i] / J[i][i];
    //         J[k][i] = 0.0;
    //         for (int j = i + 1; j < n; ++j)
    //             J[k][j] -= J[i][j] * m;
    //         F[k] -= F[i] * m;
    //     }
    // }

    // double *delta = malloc(n * sizeof(double));
    // // Retrossubstituição
    // for (int i = n - 1; i >= 0; --i) {
    //     delta[i] = F[i];
    //     for (int j = i + 1; j < n; ++j)
    //         delta[i] -= J[i][j] * delta[j];
    //     delta[i] /= J[i][i];
    // }

    LIKWID_MARKER_STOP(marker_sl);
    aux = timestamp() - aux;
    *tempSL += aux;
    free(F);
    return delta;
}



double *metodoDeNewton(FILE *output, double *x, int max, double epsilon, int n, rtime_t *tempoNewton, rtime_t *tempoJacobiana, rtime_t *tempoSL) {
    // cria matriz de broyden com variáveis iniciais
    double *F = criaEResolveBroyden(n, x, output);
    fprintf(output, "Vetor resultante da matriz de Broyden avaliada em X.\n");
    imprimirVetor(output, F, n);
    fprintf(output, "\n");

    char marker_newton[50];
    snprintf(marker_newton, sizeof(marker_newton), "Newton_%d", n);
    
    LIKWID_MARKER_START(marker_newton);
    *tempoNewton = timestamp();

    // Implementação do Algoritmo de Newton
    for (size_t i = 0; i < max; i++) {
        imprimirX(output, x, n, i);
        
        if (maxVetor(F, n) < epsilon) {
            LIKWID_MARKER_STOP(marker_newton);
            *tempoNewton = timestamp() - *tempoNewton;
            return x;
        }

        double **J = calcularJacobiana(x, n, tempoJacobiana);
        // fprintf(output, "Matriz Jacobiana avaliada em X.\n");
        // imprimirMatriz(output, J, n);

        double *delta = resolverSistemaLinear(J, n, x, tempoSL, output); // delta é um vetor de variáveis
        // fprintf(output, "Vetor delta.\n");
        // imprimirVetor(output, delta, n);
        // fprintf(output, "\n");

        for (size_t i = 0; i < n; i++) {
            x[i] = x[i] + delta[i];
        }
        if (maxVetor(delta, n) < epsilon) {
            LIKWID_MARKER_STOP(marker_newton);
            *tempoNewton = timestamp() - *tempoNewton;
            return x;
        }

        for (int j = 0; j < n; j++) {
            free(J[j]); // Libera cada linha
        }
        free(J);        // Libera o vetor principal
        free(delta);
    }
    LIKWID_MARKER_STOP(marker_newton);
    *tempoNewton = timestamp() - *tempoNewton;
    return NULL;
}