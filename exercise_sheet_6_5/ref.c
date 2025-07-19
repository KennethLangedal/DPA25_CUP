#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <omp.h>

/*
    Here, you should write your implementation of the Jones-Plassmann parallel
    coloring algorithm. The graph is defined by V, E, and n in the now typical CSR format.
    See the README for an explanation of the graph format. The input array C should hold
    the coloring after your function finishes. The array C has been initialized with all
    -1, and you should use the color classes starting from zero, that is, 0, 1, 2, etc.

    This is a greedy coloring algorithm, and you should color each vertex with the lowest
    available color in the order defined by P. That is, vertices with higher values in P
    should be colored first. To break ties, let the vertex with the lowest ID take priority.
    To be clear, the input array P is given, and you should color the vertices accordingly.
    The purpose of this assignment is to have a fast implementation of Jones-Plassmann, not
    to improve the coloring quality. Your algorithm should be deterministic and always
    produce a valid coloring using the same number of colors (depending on P).

    The data in your_data is what you returned from your_data_init and can be anything
    depending on your implementation of that function. You do not need to use this for
    anything, it is only there to allow you to do any dynamic allocation and initialization
    up-front without paying for it in terms of running time.

    Feel free to take a look inside the main.c file to understand what is going on. However,
    you should only hand in this file. Also, please reach out to us if anything is unclear
    about the setup for this exercise.
*/

typedef struct
{
    int *Count;

    int **local_color_buff;
    int **T_local;
    int *S, *T;
} my_data;

void *your_data_init(int n)
{
    my_data *d = malloc(sizeof(my_data));

    d->Count = malloc(sizeof(int) * n);
    d->S = malloc(sizeof(int) * n);
    d->T = malloc(sizeof(int) * n);

#pragma omp parallel
    {
        int tid = omp_get_thread_num();
#pragma omp single
        {
            d->local_color_buff = malloc(sizeof(int *) * omp_get_num_threads());
            d->T_local = malloc(sizeof(int *) * omp_get_num_threads());
        }
        d->local_color_buff[tid] = malloc(sizeof(int) * n);
        d->T_local[tid] = malloc(sizeof(int) * n);

        for (int i = 0; i < n; i++)
            d->local_color_buff[tid][i] = -1;
    }

    return (void *)d;
}

void JP(const int *V, const int *E, int n, const int *P, int *C, void *your_data)
{
    my_data *d = (my_data *)your_data;

    int s = 0, t = 0;
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nt = omp_get_num_threads();

        int *Count = d->Count;
        int *S = d->S;
        int *T = d->T;

        int *T_local = d->T_local[tid];
        int t_local = 0;

        int *C_buff = d->local_color_buff[tid];

#pragma omp for nowait
        for (int u = 0; u < n; u++)
        {
            int c = 0;
            for (int i = V[u]; i < V[u + 1]; i++)
            {
                int v = E[i];
                if (P[u] < P[v] || (P[u] == P[v] && v < u))
                    c++;
            }
            Count[u] = c;
            if (c == 0)
                T_local[t_local++] = u;
        }

        int p = __atomic_fetch_add(&s, t_local, __ATOMIC_RELAXED);
        for (int i = 0; i < t_local; i++)
            S[p + i] = T_local[i];

#pragma omp barrier

        while (s > 0)
        {
            t_local = 0;
#pragma omp for nowait
            for (int i = 0; i < s; i++)
            {
                int u = S[i];
                // Find color
                for (int j = V[u]; j < V[u + 1]; j++)
                {
                    int v = E[j];
                    if (P[u] < P[v] || (P[u] == P[v] && v < u))
                    {
                        C_buff[C[v]] = u;
                    }
                    else
                    {
                        p = __atomic_sub_fetch(Count + v, 1, __ATOMIC_RELAXED);
                        if (p == 0)
                        {
                            T_local[t_local++] = v;
                        }
                    }
                }

                int c = 0;
                while (C_buff[c] == u)
                    c++;

                C[u] = c;
            }

            p = __atomic_fetch_add(&t, t_local, __ATOMIC_RELAXED);
            for (int i = 0; i < t_local; i++)
                T[p + i] = T_local[i];

#pragma omp barrier

            int *_t = S;
            S = T;
            T = _t;

#pragma omp single
            {
                s = t;
                t = 0;
            }
        }
    }
}

void your_data_free(void *your_data)
{
    my_data *d = (my_data *)your_data;

    free(d->S);
    free(d->T);

#pragma omp parallel
    {
        free(d->local_color_buff[omp_get_thread_num()]);
        free(d->T_local[omp_get_thread_num()]);
    }

    free(d->local_color_buff);
    free(d->T_local);

    free(d);
}
