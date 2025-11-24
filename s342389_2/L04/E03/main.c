#include <stdio.h>
#include <stdlib.h>

#define PATH_EASY "sets/easy_test_set.txt"
#define PATH_HARD "sets/hard_test_set.txt"
#define PATH_VERY_HARD "sets/very_hard_test_set.txt"

typedef enum
{
    r_zaffiro,
    r_rubino,
    r_topazio,
    r_smeraldo
} stone_t;

typedef struct
{
    int z, r, t, s;
} stoneset_t;

void read_from_file(char *path, stoneset_t **SETS, int *N)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open input file.\n");
        exit(1);
    }

    fscanf("%d", N);

    *SETS = malloc(*N * sizeof(stoneset_t));
    for (int i = 0; i < *N; i++)
    {
        int z, r, t, s;
        fscanf("%d %d %d %d", z, r, t, s);
        (*SETS)[i].z = z;
        (*SETS)[i].r = r;
        (*SETS)[i].t = t;
        (*SETS)[i].s = s;
    }
}

int get_max_len(stoneset_t set)
{
    return set.z + set.r + set.t + set.s;
}

void find_chain_rec(stoneset_t set, char *SOL, int max_len, int pos)
{
    // TODO: Print solutions
    // TODO: Generate valid recursively
}

void find_chain(stoneset_t *SETS, int N)
{
    for (int i = 0; i < N; i++)
    {
        stoneset_t curren_set = SETS[i];
        int max_len = get_max_len(curren_set);
        char *SOL = malloc(max_len * sizeof(char));
        find_chain_rec(curren_set, SOL, max_len, 0);
        free(SOL);
    }
}

int main(int argc, char **argv)
{
    stoneset_t *SETS;
    int N;

    read_from_file(PATH_EASY, &SETS, &N);
    find_chain(SETS, N);

    return 0;
}
