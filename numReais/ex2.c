#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

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

void printFloat_t( Float_t num ) {
    printf("f:%1.9e, ix:0x%08X, sinal:%d, expoente:%d, mantissa:0x%06X\n", num.f, num.i, num.parts.sign, num.parts.exponent - 127, num.parts.mantissa);
}

int main() {
    printf("\nEpsilon: %1.15f\n", FLT_EPSILON);

    Float_t num;
    num.f = 0.0f;
    printf("Valor float: 0.0\n");
    printFloat_t(num);

    num.i = 1;
    printf("Valor inteiro: 1\n");
    printFloat_t(num);

    num.f = 1.17549435e-38;
    printf("Valor float: 1.17549435e-38\n");
    printFloat_t(num);

    num.f = 0.2;
    printf("Valor float: 0.2\n");
    printFloat_t(num);

    num.f = 1.0;
    printf("Valor float: 1.0\n");
    printFloat_t(num);

    num.f = 1.5;
    printf("Valor float: 1.5\n");
    printFloat_t(num);

    num.f = 1.75;
    printf("Valor float: 1.75\n");
    printFloat_t(num);

    num.f = 1.99999988;
    printf("Valor float: 1.99999988\n");
    printFloat_t(num);

    num.f = 2.0;
    printf("Valor float: 2.0\n");
    printFloat_t(num);

    num.f = -16777215;
    printf("Valor float: 16777215\n");
    printFloat_t(num);

    num.f = 3.40282347e+38;
    printf("Valor float: 3.40282347e+38\n");
    printFloat_t(num);

    num.f = (INFINITY);
    printf("Valor float: Infinito positivo\n");
    printFloat_t(num);

}