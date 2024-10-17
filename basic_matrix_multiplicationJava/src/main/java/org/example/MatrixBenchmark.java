package org.example;

public class MatrixBenchmark {

    public static void main(String[] args) {
        int[] sizes = {256, 512, 1024};  // Tamaños de matriz a probar
        int runsPerExperiment = 3;  // Número de ejecuciones por experimento

        for (int size : sizes) {
            System.out.println("Matrix Size: " + size);
            long totalExecutionTime = 0;
            long totalMemoryUsed = 0;

            for (int run = 1; run <= runsPerExperiment; run++) {
                System.out.println("  Run #" + run);

                // Medir uso de memoria antes de la ejecución
                Runtime runtime = Runtime.getRuntime();
                runtime.gc();  // Forzar garbage collection para medir mejor la memoria
                long memoryBefore = runtime.totalMemory() - runtime.freeMemory();

                // Crear y ejecutar la multiplicación de matrices
                Matrix matrix = new Matrix(size);
                long start = System.nanoTime();
                matrix.multiply();
                long end = System.nanoTime();

                // Medir uso de memoria después de la ejecución
                long memoryAfter = runtime.totalMemory() - runtime.freeMemory();
                long memoryUsed = memoryAfter - memoryBefore;
                totalMemoryUsed += memoryUsed;

                // Calcular y mostrar tiempo de ejecución
                long executionTime = end - start;
                totalExecutionTime += executionTime;
                System.out.println("    Execution Time (ms): " + executionTime * 1e-6);
                System.out.println("    Memory Used (MB): " + memoryUsed / (1024 * 1024));
            }

            // Promedio de tiempo de ejecución y memoria
            System.out.println("Average Execution Time for size " + size + " (ms): " + (totalExecutionTime / runsPerExperiment) * 1e-6);
            System.out.println("Average Memory Used for size " + size + " (MB): " + (totalMemoryUsed / runsPerExperiment) / (1024 * 1024));
            System.out.println("-----------------------------------------------------");
        }
    }
}
