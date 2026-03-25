#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


// Eliminação de Gauss sem pivoteamento parcial e armazenando m na matriz
double **definirMatrizLU(double **A, int n) {
    for (int i = 0; i < n; ++i) {
        for (int k = i+1; k < n; ++k) {
            double m = A[k][i] / A[i][i];
            A[k][i]  = m;
            for (int j = i+1; j < n; ++j) {
                A[k][j] -= A[i][j]*m;
            }    
        }
    }
    return A;
}

// Com pivoteamento parcial
void eliminacaoGauss(double **A, double *b, int n) {
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
}

void retrossubs(double **A, double *b, double *x, int n) {
  for (int i = n-1; i >= 0; --i) {
    x[i] = b[i];
    for (int j = i+1; j < n; ++j)
      x[i] -= A[i][j]*x[j];
    x[i] /= A[i][i];
  }
}

void resolveSL(double **A, double *b, double *x, int n) {
  eliminacaoGauss(A, b, n);
  retrossubs(A, b, x, n); 
}


void imprimirMatrizResultante(double **A, int n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
}

void encontrarYX(double **LU, double *b, int n) {
    // Separa matriz L da matriz U
    double **L = (double *) malloc(sizeof(double *)*n);
    for (size_t i = 0; i < n; i++) {
        L[i] = (double *) malloc(sizeof(double)*n);
    }
    // Passar por todos os itens da matriz
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (i < j) {
                L[i][j] = 0;
            } else if (i > j) {
                L[i][j] = LU[i][j];
            } else {
                L[i][j] = 1;
            }
        }
    }

    double **U = (double *) malloc(sizeof(double *)*n);
    for (size_t i = 0; i < n; i++) {
        U[i] = (double *) malloc(sizeof(double)*n);
    }
    // Passar por todos os itens da matriz
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            if (i > j) {
                U[i][j] = 0;
            } else {
                U[i][j] = LU[i][j];
            }
        }
    }

    // Quantidade de colunas da matriz inversa
    for (size_t i = 0; i < n; i++) {
        
        // Definir b
        for (size_t j = 0; j < n; j++) {
            if (j == i)  {
                b[j] = 1;
            } else {
                b[j] =  0;
            }  
        }
        
        eliminacaoGauss(L, b, n)

        // Eliminação de Gauss sem pivoteamento considerando que:
            // se i > j 
                // A[i][j] = 0
            // se j > i
                // A[i][j] = A[i][j]
            // se não
                // A[i][j] = 1
        // Retrosubstituição retornando y

        // Retrosubstituição de U retornando x

        // Armazena x na primeira coluna da matriz inversa
    }
    
    // Encontrar y:
        // eliminacaoGauss considerando a diagonal principal como 1 e acima da diagona principal como 0
        // Retrosubstituição
        // y da primeira coluna

    // Encontrar x:
        // Retrosubstituição com b = y
        // x da primeira coluna

    // Armazenar o x em uma matriz n x n
    // Repetir o processo até passar por todas as colunas da matriz identidade

}

int main() {

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
    
    printf("Leu a matriz completamente\n");
    
    imprimirMatrizResultante(A, n);

    printf("Antes de LU\n");
    double **LU = definirMatrizLU(A, n);
    printf("Depois de LU\n");

    imprimirMatrizResultante(LU, n);

    double *y = malloc(sizeof(double)*n);
    // eliminacaoGauss(LU, )
    
}