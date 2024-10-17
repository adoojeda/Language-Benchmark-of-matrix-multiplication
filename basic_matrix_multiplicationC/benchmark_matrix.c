#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/resource.h>
#include "matrix_multiplication.c"

double get_execution_time(struct timeval start, struct timeval stop) {
    return (stop.tv_sec - start.tv_sec) * 1e3 + (stop.tv_usec - start.tv_usec) * 1e-3;  // Convertir a milisegundos
}

// Función para obtener el uso de memoria
long get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;  // Máximo uso de memoria residente (kilobytes)
}

int main() {
    int sizes[] = {256, 512, 1024};  // Tamaños de matriz a probar
    int runs_per_experiment = 3;  // Número de ejecuciones por experimento

    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); ++s) {
        int size = sizes[s];
        printf("Matrix Size: %d\n", size);

        // Reservar memoria para las matrices
        double **a = malloc(size * sizeof(double *));
        double **b = malloc(size * sizeof(double *));
        double **c = malloc(size * sizeof(double *));
        for (int i = 0; i < size; ++i) {
            a[i] = malloc(size * sizeof(double));
            b[i] = malloc(size * sizeof(double));
            c[i] = malloc(size * sizeof(double));
        }

        double total_time = 0.0;  // Para calcular el tiempo promedio
        long total_memory_usage = 0;  // Para calcular el uso de memoria promedio

        for (int run = 1; run <= runs_per_experiment; ++run) {
            struct timeval start, stop;
            printf("  Run #%d\n", run);

            // Inicializar las matrices
            initialize_matrix(a, b, c, size);

            // Medir el uso de memoria antes de la ejecución
            long memory_before = get_memory_usage();

            // Medir el tiempo de ejecución
            gettimeofday(&start, NULL);
            multiply_matrix(a, b, c, size);
            gettimeofday(&stop, NULL);

            // Medir el uso de memoria después de la ejecución
            long memory_after = get_memory_usage();

            // Calcular el uso de memoria
            long memory_usage = memory_after - memory_before;

            // Calcular el tiempo de ejecución
            double execution_time = get_execution_time(start, stop);
            total_time += execution_time;  // Sumar el tiempo para calcular el promedio
            total_memory_usage += memory_usage;  // Sumar el uso de memoria

            printf("    Execution Time (ms): %0.3f\n", execution_time);
            printf("    Memory Usage (KB): %ld\n", memory_usage);
        }

        double avg_time = total_time / runs_per_experiment;  // Calcular el promedio de tiempo
        long avg_memory = total_memory_usage / runs_per_experiment;  // Promedio de memoria

        printf("  Average Execution Time for %dx%d: %0.3f ms\n", size, size, avg_time);
        printf("  Average Memory Usage for %dx%d: %ld KB\n\n", size, size, avg_memory);

        // Liberar memoria
        for (int i = 0; i < size; ++i) {
            free(a[i]);
            free(b[i]);
            free(c[i]);
        }
        free(a);
        free(b);
        free(c);
    }

    return 0;
}
