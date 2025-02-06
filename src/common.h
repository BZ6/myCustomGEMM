
// =================================================================================================
// Project:
// Exploring the performance of general matrix-multiplication on an NVIDIA Tesla K40m GPU.
//
// File information:
// Institution.... SURFsara <www.surfsara.nl>
// Author......... Cedric Nugteren <cedric.nugteren@surfsara.nl>
// Changed at..... 2014-11-17
// License........ MIT license
// Tab-size....... 4 spaces
// Line length.... 100 characters
//
// =================================================================================================

// Common C includes
#include <chrono>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Include kernel constants
#include "settings.h"

// =================================================================================================

// Repeat all kernels multiple times to get an average timing result
#define NUM_RUNS 50

// Squared matrices are tested within a certain range (e.g. 1024x1024, 2048x2048, 4096x4096)
#define MINSIZE (4)
#define MAXSIZE (512 * MINSIZE)

// Set the alpha and beta values for the cuBLAS and clBlas libraries. Note that the myGEMM kernels
// for simplicity only support alpha values of 1 and beta values of 0.
#define ALPHA 1.0f
#define BETA 0.0f

// Define the current GPU's parameters
#define GPU_NAME "AMD Radeon Vega 8"
#define GPU_CLOCK 0.745 // Core clock in GHz
#define GPU_CORES 2880 // Total number of CUDA cores
#define GPU_MOD 2 // Fused multiply-add

// OpenCL settings
#define MAX_NUM_DEVICES 16
#define MAX_DEVICE_NAME 1024
#define CURRENT_DEVICE 0

// Define OpenCL compiler options, such as "-cl-nv-maxrregcount=127" or  "-cl-fast-relaxed-math"
#define COMPILER_OPTIONS "-cl-std=CL1.2"

// =================================================================================================

// Timer structure
typedef struct {
    double t; // Time
    int long long kf; // KFlops
} profile_t;

// Number of timers
#define NUM_TIMERS 2

// Global variable holding the timing results
extern profile_t timers[NUM_TIMERS];

// =================================================================================================

// Forward declarations of BLAS functions
void myclblas(float* A, float* B, float* C, int K, int M, int N, int timerID);

// Forward declarations of the timer functions
double timer(void);
double wtime(profile_t timer);
double gflops(profile_t timer);

// Other forward declarations
char* readKernelFile(const char* filename, long* _size);

// =================================================================================================
