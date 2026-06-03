#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <likwid.h>

#include "utils.h"
#include "matriz.h"

#define MAX_ITER 25

int main(int argc, char *argv[]) {
    
    // Verifica se é para escrever em um arquivo ou para escrever dados por stdout
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

    // // Ler valores especificos do trbalho
    // N: dimensão do SNLB
    // x0: valor inicial das variáveis (um único, igual para todas as variáveis)
    // epsilon: tolerância a ser considerada nos critérios de parada do Método de Newton
    // MAX: número máximo de iterações do Método de Newton
    
    double xo, epsilon;

    scanf("%d %f %f", &n, &epsilon, &xo);

    // Cria vetor das variáveis iniciais
    double *x = malloc(n * sizeof(double));
    verifica_alocacao(x, output);

    // Inicializa vetor com x0 em todos 
    for (size_t i = 0; i < n; i++){
        x[i] = xo;
    }

    fprintf(output, "Vetor X  inicial com x0.\n");
    imprimirVetor(output, x, n);
    fprintf(output, "\n");

    // Resolução da Matriz de Broyden com Método de Newton
    LIKWID_MARKER_INIT;
    rtime_t tempoNewton = 0, tempoJacobiana = 0, tempoSL = 0;
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

    free(x);
    fclose(output);
    return 0;
}