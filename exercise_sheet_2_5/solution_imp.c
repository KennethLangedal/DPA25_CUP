#include <omp.h>
#include <stdlib.h>
#include <immintrin.h>

/*
    Here you should write your parallel implementation of
    matrix multiplication. The output should be stored in
    the C matrix provided as a parameter to the function.

    You can also make the following assumptions about the
    input matrices to this function.
        * The data in C has already been zero-initialized.
        * The value of n will always be a multiple of 16
          and non-negative.
        * The memory for A, B, and C will be aligned to
          32-byte boundaries.
        * All matrices are stored using the row-major
          layout. You find A[i][j] by using A[i * n + j].

    Feel free to have a look inside the main.c file to
    understand what is going on. However, you should only
    hand in this file.
*/

typedef float v8sf __attribute__((vector_size(32)));

static inline void kernel(const float *A, const float *B, float *C, int n, int k)
{
    v8sf c00 = _mm256_load_ps(C + (n * 0) + 0);
    v8sf c01 = _mm256_load_ps(C + (n * 0) + 8);
    v8sf c10 = _mm256_load_ps(C + (n * 1) + 0);
    v8sf c11 = _mm256_load_ps(C + (n * 1) + 8);
    v8sf c20 = _mm256_load_ps(C + (n * 2) + 0);
    v8sf c21 = _mm256_load_ps(C + (n * 2) + 8);
    v8sf c30 = _mm256_load_ps(C + (n * 3) + 0);
    v8sf c31 = _mm256_load_ps(C + (n * 3) + 8);

#pragma GCC unroll 32
    for (int i = 0; i < k; i++)
    {
        v8sf b0 = _mm256_load_ps(B + i * 16 + 0);
        v8sf b1 = _mm256_load_ps(B + i * 16 + 8);

        v8sf a0 = _mm256_broadcast_ss(A + (n * 0) + i);
        c00 = _mm256_fmadd_ps(b0, a0, c00);
        c01 = _mm256_fmadd_ps(b1, a0, c01);

        v8sf a1 = _mm256_broadcast_ss(A + (n * 1) + i);
        c10 = _mm256_fmadd_ps(b0, a1, c10);
        c11 = _mm256_fmadd_ps(b1, a1, c11);

        v8sf a2 = _mm256_broadcast_ss(A + (n * 2) + i);
        c20 = _mm256_fmadd_ps(b0, a2, c20);
        c21 = _mm256_fmadd_ps(b1, a2, c21);

        v8sf a3 = _mm256_broadcast_ss(A + (n * 3) + i);
        c30 = _mm256_fmadd_ps(b0, a3, c30);
        c31 = _mm256_fmadd_ps(b1, a3, c31);
    }

    _mm256_store_ps(C + (n * 0) + 0, c00);
    _mm256_store_ps(C + (n * 0) + 8, c01);
    _mm256_store_ps(C + (n * 1) + 0, c10);
    _mm256_store_ps(C + (n * 1) + 8, c11);
    _mm256_store_ps(C + (n * 2) + 0, c20);
    _mm256_store_ps(C + (n * 2) + 8, c21);
    _mm256_store_ps(C + (n * 3) + 0, c30);
    _mm256_store_ps(C + (n * 3) + 8, c31);
}

void matrix_multiplication(const float *A, const float *B, float *C, int n)
{
#pragma omp parallel
    {
        int L2N = n / 8;
        int L1N = n / 8;
        float *L1B = aligned_alloc(32, sizeof(float) * L1N * 16);

        for (int aj = 0; aj < n; aj += L1N)
        {
#pragma omp for collapse(2)
            for (int ai = 0; ai < n; ai += L2N)
            {
                for (int cj = 0; cj < n; cj += 16)
                {
                    // Copy B
                    for (int i = 0; i < L1N; i++)
                        for (int j = 0; j < 16; j++)
                            L1B[i * 16 + j] = B[(aj + i) * n + cj + j];

                    for (int ci = ai; ci < ai + L2N; ci += 4)
                    {
                        kernel(A + ci * n + aj, L1B, C + ci * n + cj, n, L1N);
                    }
                }
            }
        }
        free(L1B);
    }
}