#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char songs[5][256];
    int count;
} user_t;

void print_songs(user_t *users, int N)
{
    printf("%d\n", N);

    for (int i = 0; i < N; i++)
    {
        int songs = users[i].count;
        printf("\t%d\n", songs);

        for (int j = 0; j < songs; j++)
        {
            printf("\t\t%s\n", users[i].songs[j]);
        }
    }
}

int comb_ripet(int pos, int *val, int *sol, int n, int k, int start, int count)
{
    int i;
    if (pos >= k)
    {
        for (i = 0; i < k; i++)
            printf("%d ", sol[i]);
        printf("\n");
        return count + 1;
    }
    for (i = start; i < n; i++)
    {
        sol[pos] = val[i];
        count = comb_ripet(pos + 1, val, sol, n, k, start, count);
        start++;
    }
    return count;
}

void load_file(char *path, user_t **arr, int *N)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Error while loading file.\n");
        exit(1);
    }

    fscanf(fp, "%d", N);
    *arr = malloc(*N * sizeof(user_t));

    for (int i = 0; i < *N; i++)
    {
        int s;
        fscanf(fp, "%d", &s);
        (*arr)[i].count = s;

        for (int j = 0; j < s; j++)
        {
            fscanf(fp, "%255s", (*arr)[i].songs[j]);
        }
    }
}

int main(int argc, char *argv[])
{
    user_t *user_arr;
    int user_count;

    load_file("brani.txt", &user_arr, &user_count);
    print_songs(user_arr, user_count);
    printf("\n\n");

    comb_ripet(0);

    return 0;
}
