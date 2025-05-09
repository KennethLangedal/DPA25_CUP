#include <omp.h>
#include <stdlib.h>

/*
    Here you should write your parallel implementation of
    the Monte Carlo method to estimate the value of pi.
    The output of this function should be the ratio between
    n and m as described in the exercise sheet (4m/n).

    Feel free to have a look inside the main.c file to
    understand what is going on. However, you should only
    write code in this function if you want to have your code
    evaluated on our large machine.
*/

#define A 48271ll
#define M ((1ll << 31) - 1ll)

static double my_rand(long long *seed)
{
    long long r = (*seed * A) % M;
    *seed = r;
    return (double)r / (double)(M - 1);
}

double monte_carlo_pi(long long n)
{
    long long m = 0;
#pragma omp parallel
    {
        long long seed = omp_get_thread_num() + 1;

#pragma omp for reduction(+ : m)
        for (long long i = 0; i < n; i++)
        {
            double x = my_rand(&seed);
            double y = my_rand(&seed);
            m += (x * x + y * y <= 1) ? 1 : 0;
        }
    }

    return (4.0 * (double)m) / (double)n;
}