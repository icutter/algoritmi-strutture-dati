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

void genera_playlist(int pos, user_t *users, int N, int *sol)
{
    if (pos == N)
    {
        // stampa una playlist completa
        for (int i = 0; i < N; i++)
        {
            printf("%s ", users[i].songs[sol[i]]);
        }
        printf("\n");
        return;
    }

    for (int i = 0; i < users[pos].count; i++)
    {
        sol[pos] = i;
        genera_playlist(pos + 1, users, N, sol);
    }
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

    printf("\nPlaylist possibili:\n\n");
    int *sol = malloc(user_count * sizeof(int));
    genera_playlist(0, user_arr, user_count, sol);

    free(sol);
    free(user_arr);

    return 0;
}
