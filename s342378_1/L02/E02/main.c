#include <stdio.h>

void malloc2dP(int ***mat)
{
}

int **malloc2dR()
{
}

void separa(int **mat, int nr, int nc)
{
}

int main(int argc, int *argv[])
{
    FILE *fp = fopen("mat.txt", "r");
    if (fp == NULL)
    {
        printf("Could not open file.");
        return 1;
    }

    int nr, nc;
    fscanf(fp, "%d %d", &nr, &nc);
    printf("Matrix size: %dx%d\n", nr, nc);

    // int **mat = malloc2dR();
}
