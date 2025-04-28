#include <iostream>
#include <cuda_runtime.h>

// Kernel function to add elements of two vectors
__global__ void vectorAdd(int *A, int *B, int *C, int N) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x; // Global thread index
    if (idx < N) {
        C[idx] = A[idx] + B[idx];
    }
}

int main() {
    const int N = 1<<20;  // Size of the vectors (1 million elements)
    int *A, *B, *C;       // Host vectors
    int *d_A, *d_B, *d_C; // Device vectors

    size_t size = N * sizeof(int);

    // Allocate memory for host vectors
    A = (int*)malloc(size);
    B = (int*)malloc(size);
    C = (int*)malloc(size);

    // Allocate memory for device vectors
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Initialize host vectors with random data
    for (int i = 0; i < N; i++) {
        A[i] = rand() % 1000;
        B[i] = rand() % 1000;
    }

    // Copy data from host to device
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    // Define block and grid sizes
    int blockSize = 256;
    int numBlocks = (N + blockSize - 1) / blockSize;

    // Launch the kernel
    vectorAdd<<<numBlocks, blockSize>>>(d_A, d_B, d_C, N);

    // Check for any CUDA errors
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
        return -1;
    }

    // Copy result from device to host
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    // Optionally print a few results to verify
    for (int i = 0; i < 10; i++) {
        std::cout << "C[" << i << "] = " << C[i] << std::endl;
    }

    // Free memory
    free(A);
    free(B);
    free(C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}
