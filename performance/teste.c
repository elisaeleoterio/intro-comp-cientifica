#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */

#include "likwid.h"

#define N 10000

/**
 * Programa principal para testar configuraçãp do LIKWID
 */

int main (int argc, char *argv[]) 
{
  double A[N], B[N], C[N];
  
  LIKWID_MARKER_INIT;

  LIKWID_MARKER_START ("Teste_LIKWID");
  for(int i=0; i < N; ++i) {
    C[i] = A[i] + B[i];
  }
  LIKWID_MARKER_STOP ("Teste_LIKWID");
    
  LIKWID_MARKER_CLOSE;

  return 0;
}

