#include <omp.h>
#include <stdlib.h>

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

void matrix_multiplication(const float *A, const float *B, float *C, int n)
{
#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}