#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id;
    char col1, col2;
    int val1, val2;
} tile_t;

typedef struct
{
    int tile;
    int rot;
} cell_t;

void read_available_tiles(char *path, tile_t **TILES, int *T)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not read tiles file.\n");
        exit(1);
    }

    fscanf("%d", T);

    *TILES = malloc(*T * sizeof(tile_t));

    for (int i = 0; i < *T; i++)
    {
        char c1, c2;
        int v1, v2;

        fscanf("%c %d %c %d", &c1, &v1, &c2, &v2);

        (*TILES)[i].id = i;
        (*TILES)[i].col1 = c1;
        (*TILES)[i].val1 = v1;
        (*TILES)[i].col2 = c2;
        (*TILES)[i].val2 = v2;
    }

    fclose(fp);
}

void read_board_start(char *path, cell_t ***BOARD, int *R, int *C)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not read board file.\n");
        exit(1);
    }

    fscanf("%d %d", R, C);
    *BOARD = malloc(*R * *C * sizeof(cell_t));

    for (int i = 0; i < *R; i++)
    {
        for (int j = 0; j < *C; j++)
        {
            int tile, rot;
            fscanf("%d/%d", &tile, &rot);

            (*BOARD)[i][j].tile = tile;
            (*BOARD)[i][j].rot = rot;
        }
    }

    fclose(fp);
}

int get_score(cell_t **BOARD, int R, int C)
{
    int total_score = 0;

    for (int i = 0; i < R; i++)
    {
        cell_t current = BOARD[i][0];
        // char color = current.color1;

        for (int j = 1; j < C; j++)
        {
        }
    }

    for (int j = 0; j < C; j++)
    {
        for (int i = 0; i < R; i++)
        {
        }
    }
}

int main(int argc, char **argv)
{
    tile_t *TILES;
    int T;

    cell_t **BOARD;
    int R, C;

    read_available_tiles("tiles.txt", &TILES, &T);
    read_board_start("board.txt", &BOARD, &R, &C);

    return 0;
}