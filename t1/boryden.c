#include <stdint.h>
#include <stdio.h>
#include <matheval.h>

// || F(X(0)) || = max{|e-2|, |-2|} = 2
double maxResult(double **F, double *x) {
    return 2;
}

// || 𝚫(0) || = max{|0.075766|, |-0.924234|} = 0.924234
double maxVetor(double *delta);

double *metodoDeNewton(double *x, double **F, double **J, int max, double epsilon, int n) {
    for (size_t i = 0; i < max; i++) {
        if (maxResult(F, x) < epsilon) {
            return x;
        }
        double *delta = resolverSistemaLinear(J, delta, F, x); // delta é um vetor de variáveis
        for (size_t i = 0; i < n; i++) {
            x[i] = x[i] + delta[i];
        }
        if (maxVetor(delta) < epsilon) {
            return x;
        }
    }
}


// Derivada da matriz F
double **calcularJacobiana();

double **montarBroyden();



int main() {

    // Ler entradas
    int n, max;
    float  xo, epsilon;

    printf("Insira o tamanho da matriz: ");
    scanf("%d", &n);
    printf("\n");

    printf("Insira o valor inicial das variáveis (x0): ");
    scanf("%f", &xo);
    printf("\n");

    printf("Insira o valor de epsilon: ");
    scanf("%f", &epsilon);
    printf("\n");

    printf("Insira o valor máximo de iterações: ");
    scanf("%d", &max);
    printf("\n");

}