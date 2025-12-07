#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int start;
    int stop;
} att_t;

void load_from_file(char *path, att_t **V, int *N)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open file.\n");
        exit(1);
    }

    fscanf(fp, "%d", N);
    *V = malloc(*N * sizeof(att_t));

    for (int i = 0; i < *N; i++)
    {
        fscanf(fp, "%d %d", &(*V)[i].start, &(*V)[i].stop);
    }
}

void print_activities(att_t *V, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (i != 0)
        {
            printf(", ");
        }

        printf("(%d,%d)", V[i].start, V[i].stop);
    }
    printf("\n");
}

int get_duration(att_t a)
{
    return a.stop - a.start;
}

int get_chain_duration(att_t *V, int N)
{
    int tot = 0;
    for (int i = 0; i < N; i++)
    {
        tot += get_duration(V[i]);
    }
    return tot;
}

int are_compatible(att_t a, att_t b)
{
    return b.stop < a.start || b.start > a.stop;
}

int is_compatible(att_t *V, int N)
{
    for (int i = 0; i < N - 1; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (!are_compatible(V[i], V[j]))
                return 0;
        }
    }
    return 1;
}

void maxDurata(int pos, att_t *val, att_t *sol, att_t *best, int *best_len, int *best_dur, int n, int start)
{
    if (!is_compatible(sol, pos))
    {
        return;
    }

    int current_dur = get_chain_duration(sol, pos);
    if (current_dur > *best_dur)
    {
        memcpy(best, sol, pos * sizeof(att_t));
        *best_len = pos;
        *best_dur = current_dur;
    }

    for (int i = start; i < n; i++)
    {
        sol[pos] = val[i];
        maxDurata(pos + 1, val, sol, best, best_len, best_dur, n, i + 1);
    }
}

void attSel(int N, att_t *V)
{
    att_t *TEMP = malloc(N * sizeof(att_t));
    att_t *BEST = malloc(N * sizeof(att_t));

    TEMP[0] = V[0];
    BEST[0] = V[0];
    int best_len = 1;
    int best_dur = get_duration(V[0]);

    maxDurata(1, V, TEMP, BEST, &best_len, &best_dur, N, 1);
    printf("\nBest set of activities:\n");
    print_activities(BEST, best_len);
}

int main(int argc, char **argv)
{
    att_t *V;
    int N;

    load_from_file("att.txt", &V, &N);
    print_activities(V, N);

    attSel(N, V);

    return 0;
}
