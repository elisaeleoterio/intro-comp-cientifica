#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <likwid.h>

#ifndef UTILS_H
#define UTILS_H

#define _POSIX_C_SOURCE 199309L

typedef double rtime_t;

rtime_t timestamp (void);
double maxVetor(double *vetor, int n);
void verifica_alocacao(double *x, FILE *output);

void imprimirTempos(FILE *output, rtime_t tempoNewton, rtime_t tempoJacobiana, rtime_t tempoSL);
void imprimirX(FILE *output, double *x, int n, size_t i);
void imprimirMatriz(FILE *output, double **J, int n);
void imprimirVetor(FILE *output, double *vetor, int n);
#endif