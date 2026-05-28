#define _POSIX_C_SOURCE 199309L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <likwid.h>

#include "utils.h"

rtime_t timestamp (void) {
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
  return ( (rtime_t) tp.tv_sec*1.0e3 + (rtime_t) tp.tv_nsec*1.0e-6 );
}

// Encontra o maior elemento de um vetor
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

void verifica_alocacao(double *x, FILE *output) {
    if (!x) {
        if (output != stdout){
            fclose(output);
        }
        fprintf(stderr, "Erro de alocação.\n");
        exit(1);
    }
}