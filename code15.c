#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define L 100              // Tamaño de la red 
#define N (L * L)         // Número total de espines
#define J 1.0
#define KB 1.0
#define STEPS 100000      // Pasos de medición (cada uno es un paso Monte Carlo)
#define STEPS_T 1000*N  // Pasos para termalizar (en unidades individuales)
#define TEMP_MIN 1.0
#define TEMP_MAX 4.0
#define TEMP_STEP 0.01

int spin[N];
int left[N], right[N], up[N], down[N];
FILE *file;    // Archivo para resultados promediados

// Calcular las conexiones de los vecinos (contorno periódico)
void contorno() {
    for (int x = 0; x < L; x++) {
        for (int y = 0; y < L; y++) {
            int i = x + y * L;
            left[i]  = (x == 0) ? i + (L - 1) : i - 1;
            right[i] = (x == (L - 1)) ? i - (L - 1) : i + 1;
            up[i]    = (y == 0) ? i + L * (L - 1) : i - L;
            down[i]  = (y == (L - 1)) ? i - L * (L - 1) : i + L;
        }
    }
}

// Inicialización aleatoria de espines
void initialize() { 
    for (int i = 0; i < N; i++) {
        spin[i] = (rand() % 2) * 2 - 1;  // +1 o -1
    }
}

// Calcular la magnetización total
double magnetization() {
    double M = 0.0;
    for (int i = 0; i < N; i++) {
        M += spin[i];
    }
    return M;
}

// Calcular la energía total del sistema
double energia() {
    double E = 0.0;
    for (int i = 0; i < N; i++) {
        E += -J * spin[i] * (spin[right[i]] + spin[down[i]]);
    }
    return E;
}

// Simulación de Monte Carlo usando el algoritmo de Metropolis
void simulacion() {
    double h[5];  // Probabilidades de aceptación
    double E = energia();       // Energía inicial
    double M = magnetization(); // Magnetización inicial

    for (double T = TEMP_MAX; T >= TEMP_MIN; T -= TEMP_STEP) {
        double E_sum = 0.0, E2_sum = 0.0;
        double M_sum = 0.0, M2_sum = 0.0, Mabs_sum = 0.0;

        // Precalcular los factores de Boltzmann (no se usan en el muestreo uniforme)
        for (int j = -4, i = 0; j <= 4; j += 2, i++) {
            h[i] = 1.0;  // Probabilidad uniforme (todos los cambios son aceptados)
        }

        // Termalización (en pasos individuales)
        for (int step = 0; step < STEPS_T; step++) {
            int c = rand() % N;
            double cb = spin[c] * (spin[right[c]] + spin[left[c]] + spin[up[c]] + spin[down[c]]);
            int index = (int)((cb + 4) / 2);

            // Aceptación siempre (probabilidad uniforme)
            if ((double)rand() / RAND_MAX < 1.0) {  // Cambiar la probabilidad de aceptación
                spin[c] = -spin[c];
                E += 2 * J * cb;
                M += 2 * spin[c];
            }
        }

        // Medición (una vez por paso Monte Carlo)
        for (int step = 0; step < STEPS; step++) {
            for (int i = 0; i < N; i++) {
                int c = rand() % N;
                double cb = spin[c] * (spin[right[c]] + spin[left[c]] + spin[up[c]] + spin[down[c]]);
                int index = (int)((cb + 4) / 2);

                // Aceptación siempre (probabilidad uniforme)
                if ((double)rand() / RAND_MAX < 1.0) {  // Cambiar la probabilidad de aceptación
                    spin[c] = -spin[c];
                    E += 2 * J * cb;
                    //M += 2 * spin[c];
                }
            }

            double E_n = E / N;
            //double M_n = M / N;
            //double Mabs = fabs(M_n);

            E_sum    += E_n;
            //E2_sum   += E_n * E_n;
            //M_sum    += M_n;
            //M2_sum   += M_n * M_n;
           // Mabs_sum += Mabs;
        }

        // Calcular promedios
        double E_avg = E_sum / STEPS;
        //double E2_avg = E2_sum / STEPS;
        //double M_avg = M_sum / STEPS;
        //double M2_avg = M2_sum / STEPS;
        //double Mabs_avg = Mabs_sum / STEPS;

        // Calor específico y susceptibilidad
        //double Cv = (E2_avg - E_avg * E_avg) / (T * T);
        //double chi = (M2_sum / (double)STEPS - (M_sum / (double)STEPS) * (M_sum / (double)STEPS)) / T;

        // Guardar resultados promediados
        fprintf(file, "%.5f %.5f\n", T, E_avg);
        printf("T = %.5f, E_avg = %.5f\n",
               T, E_avg);
    }
}

int main() {
    srand(time(NULL));

    // Abrir archivo de promedios
    char filename[100];
    sprintf(filename, "ising_%d_mc_uniform.dat", L);
    file = fopen(filename, "w");
    if (!file) {
        perror("Error al abrir el archivo de resultados");
        return 1;
    }

    contorno();
    initialize();
    simulacion();

    fclose(file);
    printf("Simulación completa.\nResultados promediados en '%s'\n", filename);

    return 0;
}
