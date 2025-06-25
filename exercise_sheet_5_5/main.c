#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_RUNS 4

void parse_input(FILE *f, int **V, int **E, int *n);

int validate_matching(const int *V, const int *E, int n, const int *M);

void make_bipartite(const int *V, const int *E, int n, int *V_b, int *E_b);

int name_offset(const char *name);

void *your_data_init(int nx, int ny);

void maximum_matching(const int *V, const int *E, int nx, int ny, int *M, void *your_data);

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

    int n_b = n * 2;
    int *V_b = malloc(sizeof(int) * ((n * 2) + 1));
    int *E_b = malloc(sizeof(int) * V[n] * 2);

    make_bipartite(V, E, n, V_b, E_b);

    printf("%s, %d vertices, %d edges\n", argv[1] + name_offset(argv[1]), n_b, V_b[n_b] / 2);
    printf("%3s, %10s, %10s, %10s\n", "P", "T", "S_rel", "M");

    int *M = malloc(sizeof(int) * n_b);

    for (int i = 0; i < nc; i++)
    {
        int nt = conf[i];

        omp_set_num_threads(nt);

        int m = 0;

        for (int r = 0; r < NUM_RUNS; r++)
        {
            for (int i = 0; i < n_b; i++)
            {
                M[i] = -1;
            }

            void *your_data = your_data_init(n, n);

            double t0 = omp_get_wtime();
            maximum_matching(V_b, E_b, n, n, M, your_data);
            double t1 = omp_get_wtime();

            your_data_free(your_data);

            double elapsed = t1 - t0;
            if (r == 0 || elapsed < times[i])
                times[i] = elapsed;

            int _m = validate_matching(V_b, E_b, n_b, M);
            if (_m < 0)
                printf("Warning, the matching has errors\n");
            if (m > 0 && _m != m)
                printf("Warning, inconsistent solutions\n");
            m = _m;
        }

        printf("%3d, %10.4lf, %10.4lf, %10d\n",
               conf[i], times[i], times[0] / times[i], m);
    }

    free(conf);
    free(times);

    free(V);
    free(E);

    free(V_b);
    free(E_b);

    free(M);

    return 0;
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

    free(I);
    free(J);
    free(line);
}

int validate_matching(const int *V, const int *E, int n, const int *M)
{
    int size = 0;
    for (int i = 0; i < n; i++)
    {
        if (M[i] < 0)
            continue;

        size++;
        if (i != M[M[i]])
            return -1;
    }
    return size / 2;
}

void make_bipartite(const int *V, const int *E, int n, int *V_b, int *E_b)
{
    for (int i = 0; i <= n; i++)
    {
        V_b[i] = V[i];
        V_b[n + i] = V[n] + V[i];
    }

    for (int i = 0; i < V[n]; i++)
    {
        E_b[i] = E[i] + n;
        E_b[V[n] + i] = E[i];
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
