#include <stdio.h>
#include <stdlib.h>

int **malloc2dR(FILE *fp, int nr, int nc)
{
    int **m = (int **)malloc(nr * sizeof(int *));
    if (m == NULL)
        exit(1);

    for (int i = 0; i < nr; i++)
    {
        m[i] = (int *)malloc(nc * sizeof(int));
        if (m[i] == NULL)
            exit(1);

        for (int j = 0; j < nc; j++)
        {
            fscanf(fp, "%d", &m[i][j]);
        }
    }

    return m;
}

void print_matrix(int **m, int nr, int nc)
{
    for (int i = 0; i < nr; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            printf("%d\t", m[i][j]);
        }
        printf("\n");
    }
}

void separa(int **mat, int nr, int nc, int **w, int *wc, int **b, int *bc)
{
    int wi = 0, bi = 0;

    int size = nr * nc;
    *bc = size / 2;
    *wc = *bc + (size % 2);

    *w = (int *)malloc(*wc * sizeof(int));
    *b = (int *)malloc(*bc * sizeof(int));

    if (*w == NULL || *b == NULL)
    {
        printf("Allocation failed.");
        exit(1);
    }

    for (int i = 0; i < nr; i++)
    {
        for (int j = 0; j < nc; j++)
        {
            if ((i + j) % 2 == 0)
            {
                (*w)[wi] = mat[i][j];
                wi++;
            }
            else
            {
                (*b)[bi] = mat[i][j];
                bi++;
            }
        }
    }
}

void print_array(int *arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen("mat.txt", "r");
    if (fp == NULL)
    {
        printf("Could not open file.");
        return 1;
    }

    int nr, nc;
    fscanf(fp, "%d %d", &nr, &nc);
    printf("Matrix size: %dx%d\n\n", nr, nc);

    printf("Data in file:\n");
    int **mat = malloc2dR(fp, nr, nc);

    print_matrix(mat, nr, nc);

    int *white_arr, white_len, *black_arr, black_len;
    separa(mat, nr, nc, &white_arr, &white_len, &black_arr, &black_len);

    // Deallocate matrix
    for (int i = 0; i < nr; i++)
    {
        free(mat[i]);
    }
    free(mat);

    printf("White len: %d | ", white_len);
    print_array(white_arr, white_len);
    free(white_arr);

    printf("Black len: %d | ", black_len);
    print_array(black_arr, black_len);
    free(black_arr);
}
