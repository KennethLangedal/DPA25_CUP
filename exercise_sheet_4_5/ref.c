#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <omp.h>

/*
    Here, you should write your parallel implementation of
    BFS. The graph is defined by V, E, and n. See the README
    for an explanation of the graph format. The input array D
    should hold the minimum distance from s to every vertex
    in the graph. You can assume that D has been initialized
    to INT_MAX and P to -1. The data in your_data is what you
    returned from your_data_init and can be anything depending
    on your implementation of that function. You do not need
    to use this for anything, it is only there to allow you to
    do any dynamic allocation and initialization up-front without
    paying for it in terms of running time.

    Feel free to take a look inside the main.c file to
    understand what is going on. However, you should only
    hand in this file. Also, please reach out to us if anything
    is unclear about the setup for this exercise.
*/

typedef struct
{
    int *S, *T, *W;
    int **_T_local;
} par_bfs_data;

void *your_data_init(int n)
{
    par_bfs_data *my_data = malloc(sizeof(par_bfs_data));

    my_data->S = malloc(sizeof(int) * n);
    my_data->T = malloc(sizeof(int) * n);
    my_data->W = malloc(sizeof(int) * 3);
    for (int i = 0; i < 3; i++)
        my_data->W[i] = 0;

#pragma omp parallel
    {
        int nt = omp_get_num_threads();
        int tid = omp_get_thread_num();
#pragma omp single
        {
            my_data->_T_local = malloc(sizeof(int *) * nt);
        }
        my_data->_T_local[tid] = malloc(sizeof(int) * n);
    }

    return (void *)my_data;
}

void bfs(const int *V, const int *E, int n, int s, int *D, int *P, void *your_data)
{
    par_bfs_data *my_data = (par_bfs_data *)your_data;

    // Two frontiers, S ant T
    int *S = my_data->S, *T = my_data->T;

    // Only one counter for the size of the current frontier
    // We use 3 counters for the next frontier to avoid one barrier
    int r = 0;

    // Add s to current frontier
    S[r++] = s;

    // Set distance value for s
    D[s] = 0;

#pragma omp parallel firstprivate(S, T, r)
    {
        int tid = omp_get_thread_num();
        int *T_local = my_data->_T_local[tid];

        // The 3 next counters
        int *W = my_data->W;
        int c = 0;

        // While current frontier is nonempty
        while (r > 0)
        {
            int w_local = 0;
            // Explore from each vertex
#pragma omp for nowait
            for (int i = 0; i < r; i++)
            {
                int u = S[i];
                for (int j = V[u]; j < V[u + 1]; j++)
                {
                    int v = E[j];

                    // Check if unvisited
                    if (D[v] == INT_MAX)
                    {
                        // Update D and P
                        D[v] = D[u] + 1;
                        P[v] = u;

                        // Add to next frontier
                        T_local[w_local++] = v;
                    }
                }
            }

            int p = __atomic_fetch_add(W + c, w_local, __ATOMIC_RELAXED);

            for (int i = 0; i < w_local; i++)
                T[p + i] = T_local[i];

            int *t = S;
            S = T;
            T = t;

#pragma omp barrier

            r = W[c];
            c = (c + 1) % 3; // Change to next counter
            if (tid == 0)
                W[(c + 1) % 3] = 0; // Reset the third counter
        }
    }
}

void your_data_free(void *your_data)
{
    par_bfs_data *my_data = (par_bfs_data *)your_data;

    free(my_data->S);
    free(my_data->T);
    free(my_data->W);

#pragma omp parallel
    {
        int tid = omp_get_thread_num();

        free(my_data->_T_local[tid]);
    }

    free(my_data->_T_local);

    free(my_data);
}
