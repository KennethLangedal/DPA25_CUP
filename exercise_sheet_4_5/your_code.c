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

void *your_data_init(int n)
{
    return NULL;
}

void bfs(const int *V, const int *E, int n, int s, int *D, int *P, void *your_data)
{
}

void your_data_free(void *your_data)
{
}
