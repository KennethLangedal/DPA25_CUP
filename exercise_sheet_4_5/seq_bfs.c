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
    int *S, *T;
} seq_bfs_data;

void *your_data_init(int n)
{
    seq_bfs_data *my_data = malloc(sizeof(seq_bfs_data));

    my_data->S = malloc(sizeof(int) * n);
    my_data->T = malloc(sizeof(int) * n);

    return (void *)my_data;
}

void bfs(const int *V, const int *E, int n, int s, int *D, int *P, void *your_data)
{
    seq_bfs_data *my_data = (seq_bfs_data *)your_data;

    // Two frontiers, S ant T
    int *S = my_data->S, *T = my_data->T;

    // Two counters (r, w) for the size of each frontier
    int r = 0, w = 0;

    // Add s to current frontier
    S[r++] = s;

    // Set distance value for s
    D[s] = 0;

    // While current frontier is not empty
    while (r > 0)
    {
        // Explore from each vertex
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
                    T[w++] = v;
                }
            }
        }

        int *t = S;
        S = T;
        T = t;

        r = w;
        w = 0;
    }
}

void your_data_free(void *your_data)
{
    seq_bfs_data *my_data = (seq_bfs_data *)your_data;

    free(my_data->S);
    free(my_data->T);

    free(my_data);
}
