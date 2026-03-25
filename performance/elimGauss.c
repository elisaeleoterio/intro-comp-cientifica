#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "likwid.h"

int main() {

    LIKWID_MARKER_INIT;

    int n;
    printf("Defina o grau da matriz:\n");
    scanf("%d", &n);
    printf("\n");

    double **A = (double **) malloc(sizeof(double *)*n);
    // Ler matriz
    printf("Ler matriz de coeficientes:\n");
    for (size_t i = 0; i < n; i++) {
        A[i] = (double *) malloc(sizeof(double)*n);
        for (size_t j = 0; j < n; j++) {
            scanf("%lf", &A[i][j]);
        }
    }

    double *b = malloc(sizeof(double)*n);
    printf("Ler vetor de Termos Independentes:\n");
    for (size_t i = 0; i < n; i++) {
        scanf("%lf", &b[i]);
    }

    LIKWID_MARKER_START ("Teste_ElimGauss");
    for (int i = 0; i < n; ++i) {
        int iMax = i;
        
        for (int k = i+1; k < n; ++k) {
            if (fabs(A[k][i]) > fabs(A[iMax][i])) {
	            iMax = k;
            }
        }

        if (iMax != i) {
            double *tmp, aux;
            tmp = A[i];
            A[i] = A[iMax];
            A[iMax] = tmp;

            aux = b[i];
            b[i] = b[iMax];
            b[iMax] = aux;
        }

        for (int k = i+1; k < n; ++k) {
            double m = A[k][i] / A[i][i];
            A[k][i]  = 0.0;
            
            for (int j = i+1; j < n; ++j){
                A[k][j] -= A[i][j]*m;
            }
            
            b[k] -= b[i]*m;
        }
    }
    LIKWID_MARKER_STOP ("Teste_ElimGauss");

    LIKWID_MARKER_CLOSE;
}

