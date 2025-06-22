#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P 200
#define H 1000000
#define s -0.25*P

// Generador normal con Box-Muller
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
    } while (u1 <= 1e-10);

    u2 = (double) rand() / RAND_MAX;

    double radius = sqrt(-2.0 * log(u1));
    double theta = 2.0 * M_PI * u2;

    z0 = radius * cos(theta);
    z1 = radius * sin(theta);

    use_last = 1;
    return mu + sqrt(var) * z0;
}

// Remuestreo proporcional a los pesos
int sample_from_weights(double *w, double total, int n) {
    double r = ((double) rand() / RAND_MAX) * total;
    double cumulative = 0.0;
    for (int i = 0; i < n; i++) {
        cumulative += w[i];
        if (r <= cumulative) return i;
    }
    return n - 1;
}

int main() {
    srand(time(NULL));

    double mu = 2.0;
    double var = 1.0;

    double *Sn = (double *) malloc(H * sizeof(double));
    double *w = (double *) malloc(H * sizeof(double));
    if (w == NULL || Sn == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        return 1;
    }

    // Generar muestras y calcular pesos
    double max_val = -INFINITY;
    for (int j = 0; j < H; j++) {
        double sum = 0;
        for (int i = 0; i < P; i++) {
            sum += normal_random(mu, var);
        }
        Sn[j] = sum / P;
        double val = -s * Sn[j];
        if (val > max_val) max_val = val;
    }

    double wsum = 0;
    for (int j = 0; j < H; j++) {
        w[j] = exp(-s * Sn[j] - max_val);  // estabilidad numérica
        wsum += w[j];
    }

    // Generar archivo con muestras remuestreadas de la distribución tildada
    FILE *file = fopen("muestras_tildadas_0.25.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        free(w); free(Sn);
        return 1;
    }

    for (int j = 0; j < H; j++) {
        int idx = sample_from_weights(w, wsum, H);
        fprintf(file, "%.10f\n", Sn[idx]);  // Guardar la muestra tildada
    }

    fclose(file);
    free(w); free(Sn);

    return 0;
}
