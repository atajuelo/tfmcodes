#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define L 100            // Número de sitios (condiciones periódicas)
#define NRUNS 1000000       // Número de simulaciones independientes
#define TMAX 100.0       // Tiempo máximo de simulación

double urand() {
    return rand() / (RAND_MAX + 1.0);
}

int main() {
    double p = 0.6;
    double q = 0.4;
    double rate_total = p + q;

    srand(time(NULL));

    FILE *fa = fopen("A_2.dat", "w");
    FILE *fb = fopen("B_2.dat", "w");

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

            // Ajustar dt para no pasarse de TMAX
            if (t + dt > TMAX) {
                dt = TMAX - t;
                t = TMAX;  // Tiempo exacto
                break;     // No hacer salto si se pasa
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

        double A = N_total / TMAX;            // Dividir siempre por TMAX (tiempo fijo)
        double B = (N_right - N_left) / TMAX;

        fprintf(fa, "%.8f\n", A);
        fprintf(fb, "%.8f\n", B);
    }

    fclose(fa);
    fclose(fb);

    printf("Simulación completada. Datos guardados en 'A.dat' y 'B.dat'.\n");

    return 0;
}
