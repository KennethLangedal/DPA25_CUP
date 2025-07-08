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

void *your_data_init(int n)
{
    return NULL;
}

void JP(const int *V, const int *E, int n, const int *P, int *C, void *your_data)
{
}

void your_data_free(void *your_data)
{
}
