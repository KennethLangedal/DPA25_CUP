#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

/*
    Here, you should write your parallel implementation of
    a sorting algorithm of your choosing. The input array A
    should be sorted in ascending order after the function
    finishes. The second argument n gives the length of the
    array A. Note that the elements being sorted are
    unsigned 64-bit integers.

    Feel free to take a look inside the main.c file to
    understand what is going on. However, you should only
    hand in this file.
*/

#define BITS 8

void sort(uint64_t *A, uint64_t n)
{
    uint64_t *C = NULL;

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nt = omp_get_num_threads();
        uint64_t s = (n / nt) * tid,
                 t = (n / nt) * (tid + 1);

        if (tid == nt - 1)
            t = n;

        const uint64_t buckets = (1ull << BITS);

        uint64_t *local_buffer = malloc(sizeof(uint64_t) * (t - s));
        uint64_t *local_counts = malloc(sizeof(uint64_t) * buckets);

#pragma omp single
        {
            C = malloc(sizeof(uint64_t) * nt * buckets);
        }

        for (int i = 0; i < 64; i += BITS)
        {
            for (int j = 0; j < buckets; j++)
                local_counts[j] = 0;

            uint64_t mask = ((1ull << BITS) - 1) << i;

            // Count and copy
            for (uint64_t j = s; j < t; j++)
            {
                uint64_t id = (A[j] & mask) >> i;
                local_counts[id]++;
                local_buffer[j - s] = A[j];
            }

            for (int j = 0; j < buckets; j++)
                C[tid * buckets + j] = local_counts[j];

#pragma omp barrier

            // Prefix sum
            uint64_t p = 0;
            for (int j = 0; j < buckets; j++)
            {
                for (int k = 0; k < tid; k++)
                    p += C[k * buckets + j];

                local_counts[j] = p; // Start for bucket j

                for (int k = tid; k < nt; k++)
                    p += C[k * buckets + j];
            }

            // Move elements to final positions
            for (uint64_t j = 0; j < t - s; j++)
            {
                uint64_t id = (local_buffer[j] & mask) >> i;
                A[local_counts[id]++] = local_buffer[j];
            }

#pragma omp barrier
        }

        free(local_buffer);
        free(local_counts);

#pragma omp single
        {
            free(C);
        }
    }
}