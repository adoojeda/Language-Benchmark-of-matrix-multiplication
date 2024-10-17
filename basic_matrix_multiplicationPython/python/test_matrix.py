import time
import numpy as np
from memory_profiler import memory_usage
import psutil
from matrix import matmul

def test_matmul(matrix_size, runs=3):
    A = np.random.randint(10, size=(matrix_size, matrix_size))
    B = np.random.randint(10, size=(matrix_size, matrix_size))
    
    times = []
    memory_used = []

    for _ in range(runs):
        # Medir el uso de memoria durante la ejecución de matmul
        mem_usage = memory_usage((matmul, (A, B)), max_usage=True)
        memory_used.append(mem_usage)  # Guarda el uso máximo de memoria

        # Medir el tiempo de ejecución
        start_time = time.time()
        matmul(A, B)
        end_time = time.time()
        
        times.append(end_time - start_time)

    avg_time = (sum(times) / runs) * 1000  # Convertir a milisegundos
    avg_memory = (sum(memory_used) / runs) / 1.024  # Convertir de MiB a MB
    cpu_usage = psutil.cpu_percent(interval=None)  # Uso de CPU en el momento de la medición
    
    return avg_time, avg_memory, cpu_usage

if __name__ == "__main__":
    sizes = [256, 512, 1024]
    for size in sizes:
        avg_time, avg_memory, cpu_usage = test_matmul(size)
        print(f"Avg time for {size}x{size}: {avg_time:.2f} milliseconds")
        print(f"Avg memory usage for {size}x{size}: {avg_memory:.2f} MB")
        print(f"CPU usage during execution for {size}x{size}: {cpu_usage:.2f}%")
