#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define L 100
#define NRUNS 1000000
#define TMAX 100.0

double urand() {
    return (double)rand() / (double)RAND_MAX;
}

// Función para muestrear índices según pesos
int sample_from_weights(double *weights, double total_weight, int n) {
    double r = urand() * total_weight;
    double cumulative = 0.0;
    for (int i = 0; i < n; i++) {
        cumulative += weights[i];
        if (r <= cumulative)
            return i;
    }
    return n - 1; // fallback
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <s>\n", argv[0]);
        return 1;
    }

    double s = atof(argv[1]);

    double p = 0.6;
    double q = 0.4;
    double rate_total = p + q;

    srand(time(NULL));

    FILE *fa = fopen("A_tilted.dat", "w");
    FILE *fb = fopen("B_tilted.dat", "w");

    if (!fa || !fb) {
        perror("Error abriendo archivos");
        return 1;
    }

    double *A = malloc(NRUNS * sizeof(double));
    double *B = malloc(NRUNS * sizeof(double));
    double *weightsA = malloc(NRUNS * sizeof(double));
    double *weightsB = malloc(NRUNS * sizeof(double));

    if (!A || !B || !weightsA || !weightsB) {
        perror("Error al asignar memoria");
        return 1;
    }

    double maxA = -INFINITY;
    double maxB = -INFINITY;

    for (int run = 0; run < NRUNS; run++) {
        int pos = 0;
        double t = 0.0;
        int N_total = 0, N_right = 0, N_left = 0;

        while (t < TMAX) {
            double dt = -log(urand()) / rate_total;
            if (t + dt > TMAX) {
                dt = TMAX - t;
                t = TMAX;
                break;
            }
            t += dt;

            double r = urand();
            if (r < p / rate_total) {
                pos = (pos + 1) % L;
                N_right++;
            } else {
                pos = (pos - 1 + L) % L;
                N_left++;
            }
            N_total++;
        }

        int J = N_right - N_left;

        A[run] = (double)N_total / TMAX;
        B[run] = (double)J / TMAX;

        double valA = -s * N_total;
        double valB = -s * J;

        if (valA > maxA) maxA = valA;
        if (valB > maxB) maxB = valB;
    }

    double wsumA = 0.0, wsumB = 0.0;
    for (int i = 0; i < NRUNS; i++) {
        weightsA[i] = exp(-s * A[i] * TMAX - maxA);
        weightsB[i] = exp(-s * B[i] * TMAX - maxB);
        wsumA += weightsA[i];
        wsumB += weightsB[i];
    }

    // Generar muestras de A y B bajo P_s
    for (int k = 0; k < NRUNS; k++) {
        int idxA = sample_from_weights(weightsA, wsumA, NRUNS);
        int idxB = sample_from_weights(weightsB, wsumB, NRUNS);

        fprintf(fa, "%.10f\n", A[idxA]);
        fprintf(fb, "%.10f\n", B[idxB]);
    }

    fclose(fa);
    fclose(fb);
    free(A); free(B); free(weightsA); free(weightsB);

    return 0;
}
