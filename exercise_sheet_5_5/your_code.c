#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <omp.h>

/*
    Here, you should write your parallel implementation of
    a bipartite maximum matching algorithm. The graph is
    defined by V, E, nx, and ny. See the README for an
    explanation of the graph format. The input array M
    should hold the maximum matching after your function
    finishes. We represent a matched edge {u,v} by setting
    M[u] = v and M[v] = u. You can assume this array has
    been initialized to minus one. The data in your_data is
    what you returned from your_data_init and can be anything
    depending on your implementation of that function. You do
    not need to use this for anything, it is only there to allow you to
    do any dynamic allocation and initialization up-front without
    paying for it in terms of running time.

    Feel free to take a look inside the main.c file to
    understand what is going on. However, you should only
    hand in this file. Also, please reach out to us if anything
    is unclear about the setup for this exercise.
*/

void *your_data_init(int nx, int ny)
{
    return NULL;
}

void maximum_matching(const int *V, const int *E, int nx, int ny, int *M, void *your_data)
{
}

void your_data_free(void *your_data)
{
}
