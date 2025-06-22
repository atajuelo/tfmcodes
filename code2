#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P 200     // Tamaño de cada muestra
#define H 1000000  // Número de repeticiones

// Función para generar números aleatorios con distribución exponencial
double exponential_random(double mu) {
    double u = (double) rand() / RAND_MAX;  // Genera un número aleatorio entre [0, 1]
    return -mu * log(1.0 - u);  
}

int main() {
    srand(time(NULL)); // Generador de números aleatorios

    double mu = 2.0;  // Media
    double Sn = 0;

    // Asignar memoria dinamicamente
    double *S = (double *) malloc(H * sizeof(double));
    if (S == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        return 1;
    }

    // Calculo de trayectorias
    for (int j = 0; j < H; j++) {
        Sn = 0;
        for (int i = 0; i < P; i++) {
            Sn += exponential_random(mu) / P; 
        }
        S[j] = Sn;
    }

    // Guarda resultados 
    FILE *file = fopen("histograma_200.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        free(S);
        return 1;
    }
    
    for (int i = 0; i < H; i++) {
        fprintf(file, "%f\n", S[i]);
    }
    
    fclose(file);
    free(S); // Liberar memoria

    return 0;
}
