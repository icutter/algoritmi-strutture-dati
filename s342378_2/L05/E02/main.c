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

// axis = 0 -> orizzontale (col1/val1)
// axis = 1 -> verticale   (col2/val2)
static inline char get_color(tile_t t, int rot, int axis)
{
    if (!rot)
        return axis == 0 ? t.col1 : t.col2;
    else
        return axis == 0 ? t.col2 : t.col1;
}

static inline int get_value(tile_t t, int rot, int axis)
{
    if (!rot)
        return axis == 0 ? t.val1 : t.val2;
    else
        return axis == 0 ? t.val2 : t.val1;
}

int score_line(cell_t **BOARD, int R, int C, tile_t *TILES,
               int index, int axis, int is_row)
{
    int len = is_row ? C : R;

    // primo elemento della linea
    cell_t first = is_row ? BOARD[index][0] : BOARD[0][index];
    tile_t tf = TILES[first.tile];

    // 0 = orizzontale per righe, 1 = verticale per colonne
    char base_color = get_color(tf, first.rot, axis);
    int sum = get_value(tf, first.rot, axis);

    for (int k = 1; k < len; k++)
    {
        cell_t c = is_row ? BOARD[index][k] : BOARD[k][index];
        tile_t t = TILES[c.tile];

        char col = get_color(t, c.rot, axis);
        int val = get_value(t, c.rot, axis);

        if (col != base_color)
            return 0; // linea non valida

        sum += val;
    }

    return sum;
}

int get_score(cell_t **BOARD, int R, int C, tile_t *TILES)
{
    int total = 0;

    // RIGHE -> axis = 0 (orizzontale)
    for (int i = 0; i < R; i++)
        total += score_line(BOARD, R, C, TILES, i, 0, 1);

    // COLONNE -> axis = 1 (verticale)
    for (int j = 0; j < C; j++)
        total += score_line(BOARD, R, C, TILES, j, 1, 0);

    return total;
}

void find_best_score_rec(cell_t **BOARD, int R, int C, tile_t *TILES)
{
    // TODO: Implement recursive function
    // get_score(BOARD, R, C, TILES);
}

int main(int argc, char **argv)
{
    tile_t *TILES;
    int T;

    cell_t **BOARD;
    int R, C;

    read_available_tiles("tiles.txt", &TILES, &T);
    read_board_start("board.txt", &BOARD, &R, &C);

    find_best_score_rec(BOARD, R, C, TILES);

    return 0;
}
