
// =================================================================================================
// Project:
// Exploring the performance of general matrix-multiplication on an NVIDIA Tesla K40m GPU.
//
// File information:
// Institution.... SURFsara <www.surfsara.nl>
// Author......... Cedric Nugteren <cedric.nugteren@surfsara.nl>
// Changed at..... 2014-11-10
// License........ MIT license
// Tab-size....... 4 spaces
// Line length.... 100 characters
//
// =================================================================================================

// Common include
#include "common.h"

// Global variable with timing results
profile_t timers[NUM_TIMERS];

// =================================================================================================

// Main function. This takes care of creating matrices of various sizes and iterating over the
// different types of BLAS libraries. It also computes the error rate in terms of the L2-norm with
// respect to cuBLAS (the 'golden' reference).
int main(int argc, char* argv[])
{
    // Start of the function
    // printf("\n##\n");
    srand(time(NULL));

    // Compute the peak performance of the GPU
    double peak = GPU_CLOCK * GPU_CORES * GPU_MOD;

    // Print information about the different configurations
    // printf("## --- Configurations ---\n");
    // printf("## myGEMM.cl on '%s', peak: %.1lf GFLOPS\n", GPU_NAME, peak);

    // Loop over the different input/output matrix sizes
    for (int size = MINSIZE; size <= MAXSIZE; size = size * 2) {

        // Set the performance counters to zero
        for (int t = 0; t < NUM_TIMERS; t++) {
            timers[t].t = 0.0;
            timers[t].kf = 0;
        }

        // Set the matrices to be squared (change this to get rectangular matrices)
        const int k = size;
        const int m = size;
        const int n = size;
        // printf("##\n");
        // printf("## --- %dx%dx%d ---\n", k, m, n);

        // Allocate memory for the matrices and fill the inputs with random numbers
        float* A = (float*)malloc(m * k * sizeof(float*));
        float* B = (float*)malloc(k * n * sizeof(float*));
        float* C = (float*)malloc(m * n * sizeof(float*));
        for (int i = 0; i < m * k; i++) {
            A[i] = (float)rand() / (float)RAND_MAX;
        }
        for (int i = 0; i < k * n; i++) {
            B[i] = (float)rand() / (float)RAND_MAX;
        }

        // Set the output matrix to zero (to erase the results of the previous run)
        for (int i = 0; i < m * n; i++) {
            C[i] = 0.0f;
        }


        // Perform the matrix-multiplication
        myclblas(A, B, C, k, m, n, 0);

        // Print the results to screen
        double seconds = wtime(timers[0]);

        // Free up the matrices
        free(A);
        free(B);
        free(C);
    }

    // End of the program
    // printf("##\n");
    // printf("\n");
    return 0;
}

// =================================================================================================

// Timer function: Measure the current time
double timer(void)
{
    auto current_time = std::chrono::system_clock::now();
    auto duration = current_time.time_since_epoch();
    long long nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

    return nanoseconds / 1000000000.0;
}

// Timer function: Get the execution time
double wtime(profile_t timer)
{
    return (timer.t);
}

// Timer function: Get the GFLOPS number
double gflops(profile_t timer)
{
    return ((double)timer.kf / (1000.0 * 1000.0)) / (timer.t);
}

// =================================================================================================

// Load an OpenCL kernel from file
char* readKernelFile(const char* filename, long* _size)
{
    // Open the file
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("-- Error opening file %s\n", filename);
        exit(1);
    }

    // Get its size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Read the kernel code as a string
    char* source = (char*)malloc((size + 1) * sizeof(char));
    fread(source, 1, size * sizeof(char), file);
    source[size] = '\0';
    fclose(file);

    // Save the size and return the source string
    *_size = (size + 1);
    return source;
}

// =================================================================================================
