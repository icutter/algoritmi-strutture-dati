#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int count_occurrences(int *a, int N, int x)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (a[i] == x)
        {
            count++;
        }
    }
    return count;
}

int majority(const int *a, int N)
{
    if (N == 1)
    {
        return a[0];
    }

    int mid = N / 2;
    int left = majority(a, mid);
    int right = majority(a + mid, N - mid);

    if (left == right)
    {
        return left;
    }

    int l_count = count_occurrences(a, N, left);
    int r_count = count_occurrences(a, N, right);

    if (l_count > N / 2)
        return left;
    if (r_count > N / 2)
        return right;

    return -1;
}

int main(int argc, char *argv[])
{
    const int vet[] = {4, 1, 2, 5, 2, 5, 2};

    int major = majority(vet, ARRAY_SIZE(vet));
    if (major >= 0)
    {
        printf("Major element: %d\n", major);
    }
    else
    {
        printf("No major element was found.\n");
    }

    return 0;
}
