#include <stdio.h>
#include <stdlib.h>

#define VALOR 0.6f
#define NUM_ELEMENTOS 10000000

float somaSequencia( float *dados, unsigned int tam ) {
    float soma = 0.0;
    while ( tam-- ) {
        soma += dados[tam];
    }
    return soma;
}

float somaPar( float *dados, unsigned int tam ) {
    if (tam == 2)
        return dados[0] + dados[1];

    if (tam == 1)
        return dados[0];

    unsigned int div = tam / 2;
    return somaPar(dados, div) + somaPar(dados+div, tam-div);
}

float KahanSum(float *dados, unsigned int tam) {
    float soma = 0.0;
    float comp = 0.0;

    for (size_t i = 0; i < tam; i++) {
        float y = dados[i] - comp;
        float t = soma + y;
        comp = (t - soma) - y;
        soma = t;
    }

    return soma;
}



void main() {
    // Preenche um vetor
    float *dados = (float*) malloc(NUM_ELEMENTOS * sizeof(float));
    for (unsigned int i = 0; i < NUM_ELEMENTOS; ++i)
        dados[i] = VALOR;
    
    float soma1 = somaSequencia( dados, NUM_ELEMENTOS );
    printf("Soma sequencia: %1.15f\n", soma1);
    
    float soma2 = somaPar( dados, NUM_ELEMENTOS );
    printf("Soma par: %1.15f\n", soma2);

    float soma3 = KahanSum(dados, NUM_ELEMENTOS);
    printf("Soma Kahan: %1.15f\n", soma3);

    
    free (dados);
}