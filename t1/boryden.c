#define _POSIX_C_SOURCE 199309L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <time.h>

#define MAX_ITER 25

typedef double rtime_t;

rtime_t timestamp (void)
{
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
  return ( (rtime_t) tp.tv_sec*1.0e3 + (rtime_t) tp.tv_nsec*1.0e-6 );
}

double *resolveBroyden(int n, double *x) {
    double *F = malloc(n * sizeof(double));
    F[0] = (3 - 2*x[0])*x[0] - 2*x[1] + 1;
    for(int i = 1; i < n - 1; i++) {
        F[i] = (3 - 2*x[i])*x[i] - x[i-1] - 2*x[i+1] + 1;
    }
    F[n-1] = (3 - 2*x[n-1])*x[n-1] - x[n-2] + 1;
    
    return F;
}

// Derivada da matriz F
double **calcularJacobiana(double *x, int n) {
    
    // Alocação dinâmica da matriz jacobiana
    double **J = malloc(n * sizeof(double*));
    for (size_t i = 0; i < n; i++) {
        J[i] = malloc(n * sizeof(double));
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
    
    return J;
}

double *resolverSistemaLinear(double **J, int n, double *x) {

    // Vetor de coeficientes independentes negados 
    double *F = resolveBroyden(n, x);
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

    return delta;
}

// || F(X(0)) || = max{|e-2|, |-2|} = 2
// || 𝚫(0) || = max{|0.075766|, |-0.924234|} = 0.924234
double maxVetor(double *vetor, int n) {
    double max = vetor[0];
    for (size_t i = 0; i < n; i++) {
        if (vetor[i] > max) {
            max = vetor[i];
        }
    }
    return max;   
}

void imprimirVetor(double *vetor, int n) {
    for (size_t i = 0; i < n; i++) {
        printf("{%.2f} ", vetor[i]);
    }
    printf("\n");
}

void imprimirMatriz(double **J, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            printf("{%.2f} ", J[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double *metodoDeNewton(double xo, int max, double epsilon, int n, rtime_t *tempo) {
    
    // Cria vetor das variáveis iniciais
    double *x = malloc(n * sizeof(double));
    for (size_t i = 0; i < n; i++){
        x[i] = xo;
    }
    printf("Vetor X com x0.\n");
    imprimirVetor(x, n);
    printf("\n");

    // cria matriz de broyden com variáveis iniciais
    double *F = resolveBroyden(n, x);
    printf("Vetor resultante da matriz de Broyden avaliada em X.\n");
    imprimirVetor(F, n);
    printf("\n");
    
    *tempo = timestamp();
    // Implementação do Algoritmo de Newton
    for (size_t i = 0; i < max; i++) {
        printf("Iteração número %ld\n", i);
        if (maxVetor(F, n) < epsilon) {
            *tempo = timestamp() - *tempo;
            return x;
        }
        double **J = calcularJacobiana(x, n);
        printf("Matriz Jacobiana avaliada em X.\n");
        imprimirMatriz(J, n);

        double *delta = resolverSistemaLinear(J, n, x); // delta é um vetor de variáveis
        printf("Vetor delta.\n");
        imprimirVetor(delta, n);
        printf("\n");

        for (size_t i = 0; i < n; i++) {
            x[i] = x[i] + delta[i];
        }
        if (maxVetor(delta, n) < epsilon) {
            *tempo = timestamp() - *tempo;
            return x;
        }
    }
    *tempo = timestamp() - *tempo;
    return NULL;
}

int main() {

    // Ler entradas
    int n, max = MAX_ITER;
    float  xo, epsilon;

    printf("Insira o tamanho da matriz: ");
    scanf("%d", &n);

    printf("Insira o valor inicial das variáveis (x0): ");
    scanf("%f", &xo);

    printf("Insira o valor de epsilon: ");
    scanf("%f", &epsilon);

    // printf("Insira o valor máximo de iterações: ");
    // scanf("%d", &max);

    rtime_t tempo;
    double *x = metodoDeNewton(xo, max, epsilon, n, &tempo);
    printf("Tempo de execução total método de Newton: %.2f\n", tempo);
    if (x == NULL) {
        printf("Não foi possível encontrar uma solução.\n");
        return 1;
    } else {
        printf("Solução para sistema encontrado.\n");
        for (size_t i = 0; i < n; i++) {
            printf("x[%ld]: %.2f\n", i, x[i]);
        }
    }
}