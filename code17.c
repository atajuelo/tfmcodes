#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define L 100              // Número de sitios (condiciones periódicas)
#define NRUNS 1000000      // Número de simulaciones independientes
#define TMAX 100.0         // Tiempo máximo de simulación
#define S -0.2       // Parámetro de tilting

double urand() {
    return rand() / (RAND_MAX + 1.0);
}

int main() {
    srand(time(NULL));

    // Probabilidades originales
    double p0 = 0.6;
    double q0 = 0.4;

    // Probabilidades sesgadas por tilting de Doob
    double p = p0 * exp(-S);
    double q = q0 * exp(S);

    double rate_total = p + q;

    if (isnan(rate_total) || rate_total <= 0.0 || isinf(rate_total)) {
        fprintf(stderr, "rate_total inválido: %f\n", rate_total);
        return 1;
    }

    FILE *fa = fopen("A_doob_s0.2.dat", "w");
    FILE *fb = fopen("B_doob_s0.2.dat", "w");

    if (!fa || !fb) {
        perror("Error abriendo archivos de salida");
        return 1;
    }

    fprintf(fa, "# A = N / T\n");
    fprintf(fb, "# B = (N+ - N-) / T\n");

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

        double A = N_total / TMAX;
        double B = (N_right - N_left) / TMAX;

        fprintf(fa, "%.8f\n", A);
        fprintf(fb, "%.8f\n", B);
    }

    fclose(fa);
    fclose(fb);

    printf("Simulación completada.\n");

    return 0;
}
