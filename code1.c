#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P 200     // Tamaño de cada muestra
#define H 1000000  // Numero de repeticiones

// Funcion para generar numeros aleatorios con distribución normal con Box-Muller 
double normal_random(double mu, double var) {
    static int use_last = 0; 
    static double z1; 
    
    if (use_last) { 
        use_last = 0;
        return mu + sqrt(var) * z1; 
    }

    double u1, u2, z0;
    do {
        u1 = (double) rand() / RAND_MAX; 
    } while (u1 <= 1e-10);  // Evitar log(0)

    u2 = (double) rand() / RAND_MAX;

    double radius = sqrt(-2.0 * log(u1));
    double theta = 2.0 * M_PI * u2;
    
    z0 = radius * cos(theta);
    z1 = radius * sin(theta);
    
    use_last = 1; // Guardamos z1 para la siguiente llamada
    return mu + sqrt(var) * z0; 
}

int main() {
    srand(time(NULL)); // Generar numeros aleatorios

    double mu = 2.0;
    double var = 1.0;
    double Sn = 0;

    // Asignar memoria dinamicamente
    double *S = (double *) malloc(H * sizeof(double));
    if (S == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        return 1;
    }

    // Simulacion
    for (int j = 0; j < H; j++) {
        Sn = 0;
        for (int i = 0; i < P; i++) {
            Sn += normal_random(mu, var) / P; 
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
