#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//#define P 50     // Tamaño de cada muestra
#define H 1000  // Número de repeticiones

// Función para generar números aleatorios con distribución exponencial
double exponential_random(double mu) {
    double u = (double) rand() / RAND_MAX;  // Genera un número aleatorio entre [0, 1]
    return -mu * log(1.0 - u);  // Genera un valor con distribución exponencial
}

int main() {
    srand(time(NULL)); // Inicializa la semilla de números aleatorios

    double mu = 2.0;  // Media de la distribución exponencial
    double Sn = 0;

    // Asignar memoria dinámicamente para el array S
    double *S = (double *) malloc(H * sizeof(double));
    double *Sp = (double *) malloc(H * sizeof(double));
    if (S == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        return 1;
    }

  // Simulación
    //for (int j = 0; j < H; j++) {
    for (int j = 0; j< H; j++){
    		S[j]=0;
    		Sp[j]=0;
    	}
       
    for (int i = 1; i < H; i++) {
            S[i]= S[i-1] + exponential_random(mu) ;
            Sp[i]=S[i]/i;
            
        }

    // Guardar resultados en un archivo para graficar el histograma
    FILE *file = fopen("trayectorias_e_v2.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        free(S);
        return 1;
    }
    
    for (int i = 1; i < H+1; i++) {
        fprintf(file, "%d %f\n",i, Sp[i]);
    }
    
    fclose(file);
    free(S); // Liberar memoria

    return 0;
}
