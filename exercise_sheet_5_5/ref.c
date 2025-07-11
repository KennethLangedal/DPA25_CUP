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

typedef struct
{
    int *visited, *degree;
    int *C, *S, *T, **T_local;
} data;

void *your_data_init(int nx, int ny)
{
    data *my_data = malloc(sizeof(data));

    my_data->visited = malloc(sizeof(int) * (nx + ny));
    for (int i = 0; i < nx + ny; i++)
        my_data->visited[i] = 0;

    my_data->degree = malloc(sizeof(int) * nx);
    for (int i = 0; i < nx; i++)
        my_data->degree[i] = 0;

    my_data->C = malloc(sizeof(int) * 3);
    for (int i = 0; i < 3; i++)
        my_data->C[i] = 0;

    my_data->S = malloc(sizeof(int) * nx);
    for (int i = 0; i < nx; i++)
        my_data->S[i] = 0;

    my_data->T = malloc(sizeof(int) * nx);
    for (int i = 0; i < nx; i++)
        my_data->T[i] = 0;

#pragma omp parallel
    {
#pragma omp single
        {
            my_data->T_local = malloc(sizeof(int *) * omp_get_num_threads());
        }

        my_data->T_local[omp_get_thread_num()] = malloc(sizeof(int) * nx);
    }

    return my_data;
}

void match_and_update(const int *V, const int *E, int u, int *visited, int *M, int *degree)
{
    if (__atomic_exchange_n(visited + u, 1, __ATOMIC_RELAXED))
        return;

    for (int i = V[u]; i < V[u + 1]; i++)
    {
        int v = E[i];
        if (__atomic_exchange_n(visited + u, 1, __ATOMIC_RELAXED))
            continue;

        M[u] = v;
        M[v] = u;
        for (int j = V[v]; j < V[v + 1]; j++)
        {
            int w = E[j];
            if (__atomic_sub_fetch(degree + w, 1, __ATOMIC_RELAXED) == 1)
                match_and_update(V, E, w, visited, M, degree);
        }
        break;
    }
}

// Nondeterministic parallel greedy maximal matching
void karp_sipser(const int *V, const int *E, int nx, int *M, int *visited, int *degree)
{
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < nx; i++)
            degree[i] = V[i + 1] - V[i];

#pragma omp for nowait
        for (int i = 0; i < nx; i++)
        {
            if (degree[i] == 1)
                match_and_update(V, E, i, visited, M, degree);
        }

#pragma omp for nowait
        for (int i = 0; i < nx; i++)
        {
            if (M[i] < 0)
                match_and_update(V, E, i, visited, M, degree);
        }
    }
}

// Vertex-disjoint DFS with lookahead and fairness
int vertex_disjoint_dfs(const int *V, const int *E, int u, int it, int *M, int *visited)
{
    // Lookahead
    for (int i = V[u]; i < V[u + 1]; i++)
    {
        int v = E[i];
        if (M[v] >= 0 || visited[v] >= it)
            continue;

        if (__atomic_exchange_n(visited + v, it, __ATOMIC_RELAXED) < it)
        {
            M[u] = v;
            M[v] = u;
            return 1;
        }
    }

    // Fairness
    if (it & 1)
    {
        for (int i = V[u]; i < V[u + 1]; i++)
        {
            int v = E[i];
            if (visited[v] >= it)
                continue;

            if (__atomic_exchange_n(visited + v, it, __ATOMIC_RELAXED) < it &&
                vertex_disjoint_dfs(V, E, M[v], it, M, visited))
            {
                M[u] = v;
                M[v] = u;
                return 1;
            }
        }
    }
    else
    {
        for (int i = V[u + 1] - 1; i >= V[u]; i--)
        {
            int v = E[i];
            if (visited[v] >= it)
                continue;

            if (__atomic_exchange_n(visited + v, it, __ATOMIC_RELAXED) < it &&
                vertex_disjoint_dfs(V, E, M[v], it, M, visited))
            {
                M[u] = v;
                M[v] = u;
                return 1;
            }
        }
    }

    return 0;
}

void pothen_fan(const int *V, const int *E, int nx, int ny, int *M, int *C, int *S, int *T, int **_T_local, int *visited)
{
#pragma omp parallel firstprivate(S, T)
    {
        int nt = omp_get_num_threads();
        int tid = omp_get_thread_num();

        int *T_local = _T_local[tid];

        int w_local = 0, c = 0;

        // Find unmatched vertices

#pragma omp for nowait
        for (int u = 0; u < nx; u++)
        {
            if (M[u] < 0)
                T_local[w_local++] = u;
        }

        int p = __atomic_fetch_add(C + c, w_local, __ATOMIC_RELAXED);
        for (int i = 0; i < w_local; i++)
            S[p + i] = T_local[i];

#pragma omp barrier

        int r = nx + 1;
        int it = 2;

        // Loop while we find new agmenting paths

        while (r > C[c])
        {
            r = C[c];
            c = (c + 1) % 3;
            w_local = 0;

#pragma omp for nowait
            for (int i = 0; i < r; i++)
            {
                int u = S[i];

                // Search for augmenting path and queue up vertices that are still unmatched

                if (!vertex_disjoint_dfs(V, E, u, it, M, visited))
                    T_local[w_local++] = u;
            }

            p = __atomic_fetch_add(C + c, w_local, __ATOMIC_RELAXED);
            for (int i = 0; i < w_local; i++)
                T[p + i] = T_local[i];

#pragma omp barrier

            int *t = S;
            S = T;
            T = t;

            it++;

            if (tid == 0)
                C[(c + 1) % 3] = 0;
        }
    }
}

void maximum_matching(const int *V, const int *E, int nx, int ny, int *M, void *your_data)
{
    data *my_data = (data *)your_data;

    karp_sipser(V, E, nx, M, my_data->visited, my_data->degree);
    pothen_fan(V, E, nx, ny, M, my_data->C, my_data->S, my_data->T, my_data->T_local, my_data->visited);
}

void your_data_free(void *your_data)
{
    data *my_data = (data *)your_data;

    free(my_data->visited);
    free(my_data->degree);
    free(my_data->C);
    free(my_data->S);
    free(my_data->T);

#pragma omp parallel
    {
        free(my_data->T_local[omp_get_thread_num()]);
    }

    free(my_data->T_local);

    free(my_data);
}
