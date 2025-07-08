#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_RUNS 4

void parse_input(FILE *f, int **V, int **E, int *n);

int validate_coloring(const int *V, const int *E, int n, const int *C);

void largest_degree_first(const int *V, const int *E, int n, int *P);

int name_offset(const char *name);

void *your_data_init(int n);

void JP(const int *V, const int *E, int n, const int *P, int *C, void *your_data);

void your_data_free(void *your_data);

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
    printf("%3s, %10s, %10s, %10s\n", "P", "T", "S_rel", "C");

    int *P = malloc(sizeof(int) * n);
    int *C = malloc(sizeof(int) * n);

    for (int i = 0; i < nc; i++)
    {
        int nt = conf[i];

        omp_set_num_threads(nt);

        int c = 0;

        for (int r = 0; r < NUM_RUNS; r++)
        {
            for (int i = 0; i < n; i++)
            {
                C[i] = -1;
            }
            largest_degree_first(V, E, n, P);

            void *your_data = your_data_init(n);

            double t0 = omp_get_wtime();
            JP(V, E, n, P, C, your_data);
            double t1 = omp_get_wtime();

            your_data_free(your_data);

            double elapsed = t1 - t0;
            if (r == 0 || elapsed < times[i])
                times[i] = elapsed;

            int _c = validate_coloring(V, E, n, C);
            if (_c < 0)
                printf("Warning, the coloring has errors\n");
            if (c > 0 && _c != c)
                printf("Warning, inconsistent solutions\n");
            c = _c;
        }

        printf("%3d, %10.4lf, %10.4lf, %10d\n",
               conf[i], times[i], times[0] / times[i], c);
    }

    free(conf);
    free(times);

    free(V);
    free(E);

    free(P);
    free(C);

    return 0;
}

static inline int compare_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void parse_input(FILE *f, int **V, int **E, int *n)
{
    char *line = NULL;
    size_t line_n = 0, rc = 0;

    rc = getline(&line, &line_n, f);
    while (line[0] == '%')
        rc = getline(&line, &line_n, f);

    int nz = 0;
    rc = sscanf(line, "%d %d %d\n", n, n, &nz);

    int *I = malloc(sizeof(int) * nz);
    int *J = malloc(sizeof(int) * nz);

    *V = malloc(sizeof(int) * (*n + 1));
    *E = malloc(sizeof(int) * nz * 2);

    for (int i = 0; i < *n; i++)
        (*V)[i] = 0;

    for (int i = 0; i < nz; i++)
    {
        rc = getline(&line, &line_n, f);
        while (line[0] == '%')
            rc = getline(&line, &line_n, f);

        sscanf(line, "%d %d\n", I + i, J + i);

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

    for (int i = 0; i < *n; i++)
    {
        qsort(*E + (*V)[i], (*V)[i + 1] - (*V)[i], sizeof(int), compare_int);
    }

    free(I);
    free(J);
    free(line);
}

int validate_coloring(const int *V, const int *E, int n, const int *C)
{
    int c = 0;
    for (int u = 0; u < n; u++)
    {
        if (C[u] < 0)
            return -1;

        if (C[u] > c)
            c = C[u];

        for (int i = V[u]; i < V[u + 1]; i++)
        {
            int v = E[i];
            if (C[u] == C[v])
                return -1;
        }
    }
    return c + 1;
}

void largest_degree_first(const int *V, const int *E, int n, int *P)
{
    for (int u = 0; u < n; u++)
    {
        P[u] = V[u + 1] - V[u];
    }
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

#include "your_code.c"
