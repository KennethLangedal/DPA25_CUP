#include <omp.h>
#include <stdlib.h>

/*
    Here you should write your parallel implementation of
    the Monte Carlo method to estimate the value of pi.
    The output of this function should be the ratio between
    n and m, as described in the exercise sheet (4m/n).

    Feel free to have a look inside the main.c file to
    understand what is going on. However, you should only
    hand in this file.
*/

double monte_carlo_pi(long long n)
{
    long long m = 0;
#pragma omp parallel
    {
        unsigned int seed = omp_get_thread_num();

#pragma omp for reduction(+ : m)
        for (long long i = 0; i < n; i++)
        {
            double x = (double)rand_r(&seed) / (double)RAND_MAX;
            double y = (double)rand_r(&seed) / (double)RAND_MAX;
            m += (x * x + y * y <= 1) ? 1 : 0;
        }
    }

    return (4.0 * (double)m) / (double)n;
}