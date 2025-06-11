#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <omp.h>

#define NUM_RUNS 4
#define VALIDATE 1

void *your_data_init(int n);

void your_data_free(void *your_data);

void bfs(const int *V, const int *E, int n, int s, int *D, int *P, void *your_data);

void parse_input(FILE *f, int **V, int **E, int *n);

int name_offset(const char *name);

void seq_bfs(const int *V, const int *E, int n, int s, int *D);

void validate_bfs(const int *V, const int *E, int n, int s, const int *D_ref, const int *D, const int *P);

int main(int argc, char **argv)
{
    int nc = argc - 1;
    int *conf = malloc(sizeof(int) * nc);
    conf[0] = 1;
    for (int i = 2; i < argc; i++)
    {
        conf[i - 1] = atoi(argv[i]);
    }

    double *times = malloc(sizeof(double) * nc);

    int *V, *E, n;
    FILE *f = fopen(argv[1], "r");
    parse_input(f, &V, &E, &n);
    fclose(f);

    printf("%s, %d vertices, %d edges\n", argv[1] + name_offset(argv[1]), n, V[n] / 2);
    printf("%3s, %10s, %10s\n", "P", "T", "S_rel");

    int *P = malloc(sizeof(int) * n);
    int *D = malloc(sizeof(int) * n);

    int *D_ref = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++)
        D_ref[i] = INT_MAX;

    seq_bfs(V, E, n, 0, D_ref);

    for (int i = 0; i < nc; i++)
    {
        int nt = conf[i];

        omp_set_num_threads(nt);

        for (int r = 0; r < NUM_RUNS; r++)
        {
            for (int i = 0; i < n; i++)
            {
                D[i] = INT_MAX;
                P[i] = -1;
            }

            void *your_data = your_data_init(n);

            double t0 = omp_get_wtime();
            bfs(V, E, n, 0, D, P, your_data);
            double t1 = omp_get_wtime();

            your_data_free(your_data);

            double elapsed = t1 - t0;
            if (r == 0 || elapsed < times[i])
                times[i] = elapsed;

            if (VALIDATE)
                validate_bfs(V, E, n, 0, D_ref, D, P);
        }

        printf("%3d, %10.4lf, %10.4lf\n",
               conf[i], times[i], times[0] / times[i]);
    }

    free(conf);
    free(times);

    free(V);
    free(E);

    free(P);
    free(D);

    free(D_ref);

    return 0;
}

void parse_input(FILE *f, int **V, int **E, int *n)
{
    char *line = NULL;
    size_t line_n = 0, rc;

    rc = getline(&line, &line_n, f);
    while (line[0] == '%')
        rc = getline(&line, &line_n, f);

    int nz;
    sscanf(line, "%d %d %d\n", n, n, &nz);

    int *I = malloc(sizeof(int) * nz);
    int *J = malloc(sizeof(int) * nz);

    *V = malloc(sizeof(int) * (*n + 1));
    *E = malloc(sizeof(int) * nz * 2);

    for (int i = 0; i <= *n; i++)
        (*V)[i] = 0;

    for (int i = 0; i < nz; i++)
    {
        rc = getline(&line, &line_n, f);
        while (line[0] == '%')
            rc = getline(&line, &line_n, f);

        sscanf(line, "%d %d\n", &I[i], &J[i]);

        (*V)[I[i]]++;
        (*V)[J[i]]++;
    }

    int ps = 0;
    for (int i = 1; i <= *n; i++)
    {
        ps += (*V)[i];
        (*V)[i] = ps - (*V)[i];
    }

    for (int i = 0; i < nz; i++)
    {
        (*E)[(*V)[I[i]]++] = J[i] - 1;
        (*E)[(*V)[J[i]]++] = I[i] - 1;
    }

    free(I);
    free(J);
    free(line);
}

int name_offset(const char *name)
{
    int p = 0, i = 0;
    while (name[i] != '\0')
    {
        if (name[i] == '/')
            p = i + 1;
        i++;
    }
    return p;
}

void seq_bfs(const int *V, const int *E, int n, int s, int *D)
{
    int *queue = malloc(sizeof(int) * n);

    int front = 0, back = 0;
    queue[back++] = s;
    D[s] = 0;
    while (front < back)
    {
        int u = queue[front++];
        for (int i = V[u]; i < V[u + 1]; i++)
        {
            int v = E[i];
            if (D[v] == INT_MAX)
            {
                D[v] = D[u] + 1;
                queue[back++] = v;
            }
        }
    }

    free(queue);
}

void validate_bfs(const int *V, const int *E, int n, int s, const int *D_ref, const int *D, const int *P)
{
    for (int i = 0; i < n; i++)
    {
        if (D[i] != D_ref[i])
        {
            printf("Incorrect distance from %d to %d, was %d but should be %d\n", s, i, D[i], D_ref[i]);
            exit(1);
        }

        if (i == s || D_ref[i] == INT_MAX)
            continue;

        // Assert that previous vertex on the path is actually connected
        int adj = 0;
        for (int j = V[i]; j < V[i + 1]; j++)
        {
            if (E[j] == P[i])
            {
                adj = 1;
                break;
            }
        }
        if (!adj)
        {
            printf("Previous from %d is set to %d, but they are not adjacent\n", i, P[i]);
            exit(1);
        }
    }
}

#include "your_code.c"