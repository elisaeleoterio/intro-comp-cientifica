#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>

typedef union {
    int32_t i;
    float f;
    struct
    { 
        uint32_t mantissa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
    } parts;
} Float_t;

void imprimirFloat(Float_t num) {
    printf("f:%1.9e, ix:0x%08X, s:%d, e:%d, mx:0x%06X\n", num.f, num.i, num.parts.sign, num.parts.exponent, num.parts.mantissa);
}

float maiorEntre(float a, float b) {
    if (a > b) {
        return a;
    } 
    return b;
}

Float_t epsilon(Float_t num) {
    Float_t epsilon;
    epsilon.f = num.f / 2.0f;   
    while (num.f + epsilon.f / 2.0f > num.f) {
        epsilon.f /= 2.0f;        
    }
    return epsilon;
}

int ulp(Float_t num1, Float_t num2) {
    int num1_inteiro = (*(int *)&num1);
    int num2_inteiro = (*(int *)&num2);

    return abs(num1_inteiro - num2_inteiro);
}

int comparaEpsilonMaq(Float_t a, Float_t b) {
    Float_t dif, maior;

    // Calcula a diferença entre os números;
    dif.f = fabs(a.f - b.f); 
    // Encontra o maior entre os dois
    a.f = fabs(a.f);
    b.f = fabs(b.f);
    maior.f = maiorEntre(a.f, b.f);
    if (dif.f < maior.f * FLT_EPSILON) {
        return 1;
    }
    return 0;
}

int comparaULP(Float_t a, Float_t b, int maxULPs) {
    if (ulp(a, b) - 1 < maxULPs) {
        return 1;
    }
    return 0;
    
}


int main() {
    Float_t num;
    num.f = 1.0;
    num = epsilon(num);
     

    Float_t num1, num2;
    num1.f = 0.000001;
    num2.f = 0.0;
    printf("UPL: %d\n", ulp(num1, num2));


    //// Comparação com Epsilon e ULP

    /**** Mesma Gandeza: ****/
    // 16777215 e 16777216
    printf("/-------------Comparação de números de mesma grandeza-------------/\n");
    num1.f = 16777215;
    num2.f = 16777216;
    imprimirFloat(num1);
    imprimirFloat(num2);

    printf("Comparação com Epsilon da Máquina:\n");
    if (comparaEpsilonMaq(num1, num2)) {
        printf("Iguais - Epsilon Máquina.\n");
    } else {
        printf("Diferentes - Epsilon Máquina.\n");
    }
    
    printf("Comparação com ULP (Units in the Last Place):\n");
    if (comparaULP(num1, num2, 2)) {
        printf("Iguais - ULP\n");
    } else {
        printf("Diferentes - ULP\n");
    }

    /**** Gandeza Diferente: ****/
    // 16777215 e 0.001

    printf("/-------------Comparação de números de grandeza diferente-------------/\n");
    num1.f = 16777215;
    num2.f = 0.001;
    imprimirFloat(num1);
    imprimirFloat(num2);

    printf("Comparação com Epsilon da Máquina:\n");
    if (comparaEpsilonMaq(num1, num2)) {
        printf("Iguais - Epsilon Máquina.\n");
    } else {
        printf("Diferentes - Epsilon Máquina.\n");
    }
    
    printf("Comparação com ULP (Units in the Last Place):\n");
    if (comparaULP(num1, num2, 2)) {
        printf("Iguais - ULP\n");
    } else {
        printf("Diferentes - ULP\n");
    }

    /**** Números muito pequenos: ****/
    // 0.0 e 0.000001

    printf("/-------------Comparação de números muito pequenos-------------/\n");
    num1.f = 0.0;
    num2.f = 0.000001;
    imprimirFloat(num1);
    imprimirFloat(num2);

    printf("Comparação com Epsilon da Máquina:\n");
    if (comparaEpsilonMaq(num1, num2)) {
        printf("Iguais - Epsilon Máquina.\n");
    } else {
        printf("Diferentes - Epsilon Máquina.\n");
    }
    
    printf("Comparação com ULP (Units in the Last Place):\n");
    if (comparaULP(num1, num2, 2)) {
        printf("Iguais - ULP\n");
    } else {
        printf("Diferentes - ULP\n");
    }


}