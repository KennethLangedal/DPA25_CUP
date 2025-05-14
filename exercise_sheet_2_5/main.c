#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <limits.h>

#define NUM_RUNS 4
#define N (16ll * 64ll)

void matrix_multiplication(const float *A, const float *B, float *C, int n);

int validate(const float *A, const float *B, const float *C, int n, int k);

int main(int argc, char **argv)
{
    int nc = argc;
    int *conf = malloc(sizeof(int) * nc);
    conf[0] = 1;
    for (int i = 1; i < argc; i++)
    {
        conf[i] = atoi(argv[i]);
    }

    double *times = malloc(sizeof(double) * nc);

    printf("Using n=%lld\n", N);
    printf("%3s, %10s, %10s, %10s\n", "P", "T", "S_rel", "GFLOPS");

    float *A = aligned_alloc(32, sizeof(float) * N * N);
    float *B = aligned_alloc(32, sizeof(float) * N * N);
    float *C = aligned_alloc(32, sizeof(float) * N * N);

    srand(0);

    for (int i = 0; i < N * N; i++)
        A[i] = ((double)rand() / (double)RAND_MAX) - 0.5f;
    for (int i = 0; i < N * N; i++)
        B[i] = ((double)rand() / (double)RAND_MAX) - 0.5f;

    for (int i = 0; i < nc; i++)
    {
        int nt = conf[i];

        omp_set_num_threads(nt);

        double pi = 0.0;

        for (int r = 0; r < NUM_RUNS; r++)
        {
            for (int i = 0; i < N * N; i++)
                C[i] = 0.0f;

            double t0 = omp_get_wtime();
            matrix_multiplication(A, B, C, N);
            double t1 = omp_get_wtime();

            double elapsed = t1 - t0;
            if (r == 0 || elapsed < times[i])
                times[i] = elapsed;

            if (!validate(A, B, C, N, 20))
                printf("Warning, detected error in solution!\n");
        }

        double flops = (double)(N * N * N * 2) / times[i];
        printf("%3d, %10.4lf, %10.4lf, %10.4lf\n",
               conf[i], times[i], times[0] / times[i], flops / 1e9);
    }

    free(conf);
    free(times);

    free(A);
    free(B);
    free(C);

    return 0;
}

int validate(const float *A, const float *B, const float *C, int n, int k)
{
    float *R = malloc(sizeof(float) * n);
    float *BR = malloc(sizeof(float) * n);

    int valid = 1;

    for (int t = 0; t < k; t++)
    {
        for (int i = 0; i < n; i++)
            R[i] = rand() & 1;

        for (int i = 0; i < n; i++)
        {
            BR[i] = 0.0f;
            for (int j = 0; j < n; j++)
                BR[i] += B[i * n + j] * R[j];
        }

        for (int i = 0; i < n; i++)
        {
            float cr = 0.0f;
            for (int j = 0; j < n; j++)
                cr += C[i * n + j] * R[j];

            float axbr = 0.0f;
            for (int j = 0; j < n; j++)
                axbr += A[i * n + j] * BR[j];

            if (fabs(axbr - cr) > 1.0f)
                valid = 0;
        }
    }

    free(R);
    free(BR);

    return valid;
}

#include "your_code.c"