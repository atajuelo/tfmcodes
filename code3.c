#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//#define P 200      // Tamaño de cada muestra
#define H 10000  // Número de repeticiones

// Función para generar números aleatorios con distribución normal
// Generador de números normales con Box-Muller (usa dos valores a la vez)
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
    srand(time(NULL)); // Inicializa la semilla de números aleatorios

    double mu = 2.0;
    double var = 1.0;
    double Sn = 0;

    // Asignar memoria dinámicamente para el array S
    double *S = (double *) malloc(H * sizeof(double));
    double *Sp = (double *) malloc(H * sizeof(double));
    if (S == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        return 1;
    }
 	Sn = 0;
    // Simulación
    //for (int j = 0; j < H; j++) {
    	for (int j = 0; j< H; j++){
    		S[j]=0;
    		Sp[j]=0;
    	}
       
        for (int i = 1; i < H; i++) {
            S[i]= S[i-1] + normal_random(mu,var) ;
            Sp[i]=S[i]/i;
            
        }
        
    //}

    // Guardar resultados en un archivo para graficar el histograma
    FILE *file = fopen("trayectorias_g_v2.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        free(S);
        return 1;
    }
    
    for (int i = 1; i < H+1; i++) {
        fprintf(file, " %d %f\n",i, Sp[i]);
    }
    
    fclose(file);
    free(S); // Liberar memoria

    return 0;
}
