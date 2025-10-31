#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int u, v;
} edge_t;

int powerset(int pos, int *val, int *sol, int n, int cnt)
{
    int j;
    if (pos >= n)
    {
        printf("{ \t");
        for (j = 0; j < n; j++)
            if (sol[j] != 0)
                printf("%d \t", val[j]);
        printf("} \n");
        return cnt + 1;
    }
    sol[pos] = 0;
    cnt = powerset(pos + 1, val, sol, n, cnt);
    sol[pos] = 1;
    cnt = powerset(pos + 1, val, sol, n, cnt);
    return cnt;
}

void load_file(edge_t *arr, int *N, int *E)
{
    FILE *fp = fopen("grafo.txt", "r");
    if (fp == NULL)
    {
        printf("Could not open file.\n");
        exit(1);
    }

    fscanf(fp, "%d %d", N, E);

    arr = malloc(*N * sizeof(edge_t));

    edge_t new_edge;
    for (int i = 0; i < *N; i++)
    {
        if (fscanf(fp, "%d %d", &new_edge.u, &new_edge.v) == 2)
        {
            arr[i] = new_edge;
        }
        else
        {
            printf("Error while reading file.\n");
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
    edge_t *arr;
    int N; // vertex count
    int E; // edge count

    load_file(arr, &N, &E);

    return 0;
}
