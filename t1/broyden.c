#define _POSIX_C_SOURCE 199309L // Descobrir para que serve isso
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <likwid.h>

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

double *resolverSistemaLinear(double **J, int n, double *x, rtime_t *tempSL) {
    LIKWID_MARKER_START("Sistema Linear.");
    rtime_t aux = timestamp();
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

    LIKWID_MARKER_STOP("Sistema Linear.");
    aux = timestamp() - aux;
    *tempSL += aux;
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

void imprimirVetor(FILE *output, double *vetor, int n) {
    for (size_t i = 0; i < n; i++) {
        fprintf(output, "{%.2f} ", vetor[i]);
    }
    fprintf(output, "\n");
}

void imprimirMatriz(FILE *output, double **J, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            fprintf(output, "{%.2f} ", J[i][j]);
        }
        fprintf(output, "\n");
    }
    fprintf(output, "\n");
}

void imprimirX(FILE *output, double *x, int n, size_t i) {
    fprintf(output, "Iteração %ld.\n", i);
    for (size_t j = 0; j < n; j++) {
        fprintf(output, "x%ld: %.4f\n", j, x[j]);
    }
}

void imprimirTempos(FILE *output, rtime_t tempoNewton, rtime_t tempoJacobiana, rtime_t tempoSL) {
    fprintf(output, "###########\n");
    fprintf(output, "# Tempo Total: %lf (em milisegundos)\n", tempoNewton);
    fprintf(output, "# Tempo Jacobiana: %lf (em milisegundos)\n", tempoJacobiana);
    fprintf(output, "# Tempo SL: %lf (em milisegundos)\n", tempoSL);
    fprintf(output, "###########\n");
}

double *metodoDeNewton(FILE *output, double *x, int max, double epsilon, int n, rtime_t *tempoNewton, rtime_t *tempoJacobiana, rtime_t *tempoSL) {
    // cria matriz de broyden com variáveis iniciais
    double *F = resolveBroyden(n, x);
    fprintf(output, "Vetor resultante da matriz de Broyden avaliada em X.\n");
    imprimirVetor(output, F, n);
    fprintf(output, "\n");
    
    LIKWID_MARKER_START("Método de Newton Inteiro.");
    *tempoNewton = timestamp();
    // Implementação do Algoritmo de Newton
    for (size_t i = 0; i < max; i++) {
        
        imprimirX(output, x, n, i);
        
        if (maxVetor(F, n) < epsilon) {
            LIKWID_MARKER_STOP("Método de Newton Inteiro.");
            *tempoNewton = timestamp() - *tempoNewton;
            return x;
        }

        double **J = calcularJacobiana(x, n, tempoJacobiana);
        fprintf(output, "Matriz Jacobiana avaliada em X.\n");
        imprimirMatriz(output, J, n);

        double *delta = resolverSistemaLinear(J, n, x, tempoSL); // delta é um vetor de variáveis
        fprintf(output, "Vetor delta.\n");
        imprimirVetor(output, delta, n);
        fprintf(output, "\n");

        for (size_t i = 0; i < n; i++) {
            x[i] = x[i] + delta[i];
        }
        if (maxVetor(delta, n) < epsilon) {
            LIKWID_MARKER_STOP("Método de Newton Inteiro.");
            *tempoNewton = timestamp() - *tempoNewton;
            return x;
        }
    }
    LIKWID_MARKER_STOP("Método de Newton Inteiro.");
    *tempoNewton = timestamp() - *tempoNewton;
    return NULL;
}

int main(int argc, char *argv[]) {
    FILE *output = stdout;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                output = fopen(argv[i + 1], "wb+");
                if (!output) {
                    return 1;
                }
            }
        }
    }
    
    // Ler entradas
    int n, max = MAX_ITER;
    float  xo = 0.0, epsilon = 0.0;

    // fprintf(output, "Insira o tamanho da matriz: ");
    scanf("%d", &n);

    // fprintf(output, "Insira o valor inicial das variáveis (x0): ");
    // scanf("%f", &xo);

    // fprintf(output, "Insira o valor de epsilon: ");
    // scanf("%f", &epsilon);


    // Cria vetor das variáveis iniciais
    double *x = malloc(n * sizeof(double));
    if (!x) {
        fprintf(stderr, "Erro de alocação.\n");
        if (output != stdout){
            fclose(output);
        }
        return 1;
    }
    // Inicializa vetor com x0 em todos 
    for (size_t i = 0; i < n; i++){
        x[i] = xo;
    }
    fprintf(output, "Vetor X com x0.\n");
    imprimirVetor(output, x, n);
    fprintf(output, "\n");

    LIKWID_MARKER_INIT;

    rtime_t tempoNewton, tempoJacobiana, tempoSL;
    x = metodoDeNewton(output, x, max, epsilon, n, &tempoNewton, &tempoJacobiana, &tempoSL);

    LIKWID_MARKER_CLOSE;

    imprimirTempos(output, tempoNewton, tempoJacobiana, tempoSL);
    
    if (x == NULL) {
        fprintf(output, "Não foi possível encontrar uma solução.\n");
        return 1;
    } else {
        fprintf(output, "Solução para sistema encontrado.\n");
        imprimirVetor(output, x, n);
    }

    fclose(output);
    return 0;
}