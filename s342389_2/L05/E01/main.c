#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int start;
    int stop;
} att;

int get_duration(att a)
{
    return a.stop - a.start;
}

int maxDurata(att *V, int N, int pos, att *TEMP, int tmp)
{
    if (pos >= N)
        return 0;

    int max_index = -1;
    int max_duration = 0;

    for (int i = pos + 1; i < N; i++)
    {

        int cur_duration = maxDurata(V, N, i, TEMP, tmp + 1);
        if (cur_duration > max_duration)
        {
            max_duration = cur_duration;
            max_index = i;
        }
    }

    return max_duration;
}

void attSel(int N, att *V)
{
    att TEMP[1000];
    maxDurata(V, N, 0, TEMP, 0);
}

void load_from_file(char *path, att **V, int *N)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open file.\n");
        exit(1);
    }

    fscanf(fp, "%d", N);
    *V = malloc(*N * sizeof(att));

    for (int i = 0; i < *N; i++)
    {
        fscanf(fp, "%d %d", &(*V)[i].start, &(*V)[i].stop);
    }
}

void print_activities(att *V, int len)
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

int main(int argc, char **argv)
{
    att *V;
    int N;

    load_from_file("att.txt", &V, &N);
    print_activities(V, N);
    attSel(N, V);

    return 0;
}
