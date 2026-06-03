#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <likwid.h>

#include "utils.h"

double *criaEResolveBroyden(int n, double *x, FILE *output);
double **calcularJacobiana(double *x, int n, rtime_t *tempJacobiana);
double *resolverSistemaLinear(double **J, int n, double *F, rtime_t *tempSL, FILE *output);
double *metodoDeNewton(FILE *output, double *x, int max, double epsilon, int n, rtime_t *tempoNewton, rtime_t *tempoJacobiana, rtime_t *tempoSL);
#endif