#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P 1000000 // Número de puntos de Monte Carlo la N mayuscula

// Función para generar números aleatorios uniformes en [0, 1]
double uniform_random() {
    return (double) rand() / RAND_MAX;
}

int main() {
    srand(time(NULL)); // Semilla de números aleatorios
    
    double integral = 0;
    double integral_squared = 0;

    // Monte Carlo
    for (int i = 0; i < P; i++) {
        double x = uniform_random();
        double g = cos(M_PI * x / 2);
        integral += g;
        integral_squared += g * g;
    }

  
    integral /= P;
    integral_squared /= P;

    // Estimación de la varianza
    double variance = integral_squared - (integral * integral);

    printf("Estimación de la integral: %f\n", integral);
    printf("Varianza: %.15f\n", variance);

    return 0;
}
