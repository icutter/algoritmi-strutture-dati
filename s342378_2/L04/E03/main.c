#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    stone_t a;
    stone_t b;
} rules_t;

typedef int stoneset_t[4];
rules_t *RULES;

int get_max_len(stoneset_t set)
{
    return set[0] + set[1] + set[2] + set[3];
}

rules_t *init_sequence_rules()
{
    rules_t *R = malloc(4 * sizeof(rules_t));
    R[r_zaffiro] = (rules_t){r_zaffiro, r_rubino};
    R[r_rubino] = (rules_t){r_smeraldo, r_topazio};
    R[r_topazio] = (rules_t){r_zaffiro, r_rubino};
    R[r_smeraldo] = (rules_t){r_smeraldo, r_topazio};

    return R;
}

void read_from_file(char *path, stoneset_t **SETS, int *N)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open input file.\n");
        exit(1);
    }

    fscanf(fp, "%d", N);

    *SETS = malloc(*N * sizeof(stoneset_t));
    for (int i = 0; i < *N; i++)
    {
        int z, r, t, s;
        fscanf(fp, "%d %d %d %d", &z, &r, &t, &s);
        (*SETS)[i][r_zaffiro] = z;
        (*SETS)[i][r_rubino] = r;
        (*SETS)[i][r_topazio] = t;
        (*SETS)[i][r_smeraldo] = s;
    }
}

int find_chain_rec(stoneset_t set, stone_t last_stone, int upper_bound, int pos)
{
    if (pos >= upper_bound || get_max_len(set) <= 0)
    {
        return 0;
    }

    stoneset_t next_set;
    stone_t next_stone;
    int first = 0, second = 0;

    next_stone = RULES[last_stone].a;
    if (set[next_stone] > 0)
    {
        memcpy(next_set, set, sizeof(stoneset_t));
        next_set[next_stone]--;
        first = find_chain_rec(next_set, next_stone, upper_bound, pos + 1);
    }

    next_stone = RULES[last_stone].b;
    if (set[next_stone] > 0)
    {
        memcpy(next_set, set, sizeof(stoneset_t));
        next_set[next_stone]--;
        second = find_chain_rec(next_set, next_stone, upper_bound, pos + 1);
    }

    return 1 + (first > second ? first : second);
}

void find_chain(stoneset_t *SETS, int N)
{
    for (int i = 0; i < N; i++)
    {
        int max_len = 0;
        for (int j = 0; j < 4; j++)
        {
            stoneset_t current_set;
            memcpy(current_set, SETS[i], sizeof(stoneset_t));
            const int upper_bound = get_max_len(current_set);

            if (current_set[j] <= 0)
                continue;

            current_set[j]--;

            int val = find_chain_rec(current_set, (stone_t)j, upper_bound, 1);
            max_len = val > max_len ? val : max_len;
        }

        printf("%d. Collana massima di lunghezza %d\n", i, max_len);
    }
}

int main(int argc, char **argv)
{
    RULES = init_sequence_rules();

    stoneset_t *SETS;
    int N;

    read_from_file(PATH_EASY, &SETS, &N);

    find_chain(SETS, N);

    return 0;
}
