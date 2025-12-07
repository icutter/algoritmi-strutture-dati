#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int start;
    int stop;
} att_t;

int get_duration(att_t a)
{
    return a.stop - a.start;
}

int compatible(att_t a, att_t b)
{
    return a.start >= b.stop;
}

int cmp_att(const void *a, const void *b)
{
    att_t *x = (att_t *)a;
    att_t *y = (att_t *)b;
    return x->stop - y->stop;
}

void load_from_file(char *path, att_t **V, int *N)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        printf("File error.\n");
        exit(1);
    }

    fscanf(fp, "%d", N);
    *V = malloc(*N * sizeof(att_t));

    for (int i = 0; i < *N; i++)
        fscanf(fp, "%d %d", &(*V)[i].start, &(*V)[i].stop);

    fclose(fp);
}

void print_solution(att_t *V, int *P, int idx)
{
    if (idx == -1)
        return;
    print_solution(V, P, P[idx]);
    printf("(%d,%d) ", V[idx].start, V[idx].stop);
}

void attSelDP(int N, att_t *V)
{
    qsort(V, N, sizeof(att_t), cmp_att);

    int *DP = calloc(N, sizeof(int));
    int *P = malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
    {
        DP[i] = get_duration(V[i]);
        P[i] = -1;

        for (int j = 0; j < i; j++)
        {
            if (compatible(V[i], V[j]))
            {
                if (DP[j] + get_duration(V[i]) > DP[i])
                {
                    DP[i] = DP[j] + get_duration(V[i]);
                    P[i] = j;
                }
            }
        }
    }

    int best = 0;
    for (int i = 1; i < N; i++)
        if (DP[i] > DP[best])
            best = i;

    printf("Best chain:\n");
    print_solution(V, P, best);
    printf("\nTotal duration = %d\n", DP[best]);

    free(DP);
    free(P);
}

int main()
{
    att_t *V;
    int N;

    load_from_file("att.txt", &V, &N);

    printf("Input activities:\n");
    for (int i = 0; i < N; i++)
        printf("(%d,%d) ", V[i].start, V[i].stop);
    printf("\n");

    attSelDP(N, V);
    free(V);
    return 0;
}
