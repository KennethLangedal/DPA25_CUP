#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

#define NUM_RUNS 4
#define N (1ull << 25)

void sort(uint64_t *A, uint64_t n);

int is_sorted(const uint64_t *A, uint64_t n);

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

    printf("Sorting n=%lld elements\n", N);
    printf("%3s, %10s, %10s\n", "P", "T", "S_rel");

    uint64_t *A = malloc(sizeof(uint64_t) * N);
    uint64_t *B = malloc(sizeof(uint64_t) * N);

    srand(0);

    for (uint64_t i = 0; i < N; i++)
        A[i] = ((uint64_t)rand() << 32) + (uint64_t)rand();

    for (int i = 0; i < nc; i++)
    {
        int nt = conf[i];

        omp_set_num_threads(nt);

        for (int r = 0; r < NUM_RUNS; r++)
        {
            for (uint64_t i = 0; i < N; i++)
                B[i] = A[i];

            double t0 = omp_get_wtime();
            sort(B, N);
            double t1 = omp_get_wtime();

            double elapsed = t1 - t0;
            if (r == 0 || elapsed < times[i])
                times[i] = elapsed;

            if (!is_sorted(B, N))
                printf("Warning, the array is not sorted in ascending order\n");
        }

        printf("%3d, %10.4lf, %10.4lf\n",
               conf[i], times[i], times[0] / times[i]);
    }

    free(conf);
    free(times);

    free(A);
    free(B);

    return 0;
}

int is_sorted(const uint64_t *A, uint64_t n)
{
    for (uint64_t i = 1; i < n; i++)
    {
        if (A[i] < A[i - 1])
            return 0;
    }
    return 1;
}

#include "your_code.c"