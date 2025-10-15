#include <stdio.h>

int is_even(int n)
{
    return n % 2 == 0;
}

int gcd(int a, int b)
{
    if (a == 0 || a == b)
        return b;
    else if (b == 0)
        return a;

    if (a < b)
    {
        int app = a;
        a = b;
        b = app;
    }

    char mask = is_even(a) << 1 | is_even(b);

    switch (mask)
    {
    case 0b11:
        return 2 * gcd(a / 2, b / 2);
    case 0b10:
        return gcd(a / 2, b);
    case 0b01:
        return gcd(a, b / 2);
    case 0b00:
        return gcd((a - b) / 2, b);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int a, b;

    printf("Insert two integers (format \"a b\")\n");
    scanf("%d %d", &a, &b);

    int result = gcd(a, b);

    printf("Maximum common divider is: %d\n", result);
    return 0;
}
