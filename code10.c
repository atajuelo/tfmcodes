#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P 200       // Tamaño de cada muestra
#define H 1000000   // Número de repeticiones
#define s -0.24*P    // Sesgo exponencial

// Función para generar números aleatorios con distribución exponencial
double exponential_random(double mu) {
    double u = (double) rand() / RAND_MAX;
    return -mu * log(1.0 - u);
}

// Función para muestrear un índice basado en los pesos normalizados
int sample_from_weights(double *weights, double total_weight, int n) {
    double r = ((double) rand() / RAND_MAX) * total_weight;
    double cumulative = 0.0;
    for (int i = 0; i < n; i++) {
        cumulative += weights[i];
        if (r <= cumulative) return i;
    }
    return n - 1; // fallback por seguridad
}

int main() {
    srand(time(NULL)); // Inicializa la semilla

    double mu = 2.0;

    double *Sn = (double *) malloc(H * sizeof(double));
    double *w = (double *) malloc(H * sizeof(double));

    if (Sn == NULL || w == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        return 1;
    }

    // Generar muestras y calcular pesos
    double max_val = -INFINITY;
    for (int j = 0; j < H; j++) {
        double sum = 0.0;
        for (int i = 0; i < P; i++) {
            sum += exponential_random(mu);
        }
        Sn[j] = sum / P;
        double val = -s * Sn[j];
        if (val > max_val) max_val = val;
    }

    double wsum = 0.0;
    for (int j = 0; j < H; j++) {
        w[j] = exp(-s * Sn[j] - max_val);  // para evitar overflow
        wsum += w[j];
    }

    FILE *file = fopen("histograma_200_e.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        free(Sn); free(w);
        return 1;
    }

    for (int j = 0; j < H; j++) {
        int idx = sample_from_weights(w, wsum, H);
        fprintf(file, "%.10f\n", Sn[idx]);
    }

    fclose(file);
    free(Sn); free(w);

    return 0;
}
