#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int u, v;
} edge_t;

void print_data(edge_t *arr, int N, int E)
{
    printf("\nN: %d\tE: %d\n\n", N, E);

    for (int i = 0; i < E; i++)
    {
        printf("%d %d\n", arr[i].u, arr[i].v);
    }
}

int load_file(edge_t **arr, int *N, int *E)
{
    FILE *fp = fopen("grafo.txt", "r");
    if (fp == NULL)
    {
        printf("Could not open file.\n");
        exit(1);
    }

    fscanf(fp, "%d %d", N, E);

    *arr = malloc(*E * sizeof(edge_t));

    edge_t new_edge;
    for (int i = 0; i < *E; i++)
    {
        if (fscanf(fp, "%d %d", &new_edge.u, &new_edge.v) == 2)
        {
            (*arr)[i] = new_edge;
        }
        else
        {
            printf("Error while reading file.\n");
            return 0;
        }
    }

    return 1;
}

int contains(int *arr, int n, int val)
{
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}

int is_vertex_cover(edge_t *arr, int E, int *cover, int C)
{
    for (int i = 0; i < E; i++)
    {
        if (!contains(cover, C, arr[i].u) && !contains(cover, C, arr[i].v))
            return 0;
    }
    return 1;
}

void print_vertex_covers_rec(edge_t *edges, int N, int E, int idx, int *cover, int count)
{
    if (idx == N)
    {
        // abbiamo considerato tutti i vertici, controlliamo se è un vertex cover
        if (is_vertex_cover(edges, E, cover, count))
        {
            printf("{ ");
            for (int i = 0; i < count; i++)
                printf("%d ", cover[i]);
            printf("}\n");
        }
        return;
    }

    // 1) includo idx nel cover
    cover[count] = idx;
    print_vertex_covers_rec(edges, N, E, idx + 1, cover, count + 1);

    // 2) non includo idx nel cover
    print_vertex_covers_rec(edges, N, E, idx + 1, cover, count);
}

int main(int argc, char *argv[])
{
    edge_t *edges;
    int N; // vertex count
    int E; // edge count

    load_file(&edges, &N, &E);
    // print_data(edges, E, N);

    int *cover = malloc(N * sizeof(int));
    print_vertex_covers_rec(edges, N, E, 0, cover, 0);

    return 0;
}
