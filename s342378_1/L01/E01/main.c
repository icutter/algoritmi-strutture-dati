#include <stdio.h>
#include <string.h>
#include <ctype.h>

int regex_length(const char *regexp)
{
    int len = strlen(regexp);
    int i = 0, c = 0;

    while (i < len && regexp[i] != '\0')
    {
        if (regexp[i] == '[')
        {
            do
            {
                i++;
            } while (regexp[i] != ']');
        }
        i++;
        c++;
    }
    return c;
}

int token_length(const char *start)
{
    if (start[0] == '\\')
        return 2;

    if (start[0] == '[')
    {
        int i = 0;
        while (start[i] != '\0' && start[i] != ']')
        {
            i++;
        }
        return i + 1;
    }

    return 1;
}

int validate_character(const char src, const char *regexp, const int n)
{
    if (n == 1 && regexp[0] == '.')
        return isalpha((unsigned char)src);
    if (n == 2 && regexp[0] == '\\')
    {
        switch (regexp[1])
        {
        case 'a':
            return islower((unsigned char)src);
        case 'A':
            return isupper((unsigned char)src);
        }
    }
    if (n > 2 && regexp[0] == '[')
    {
    }
    return src == regexp[0];
}

char *cerca_regexp(char *src, char *regexp)
{
    int reglen = regex_length(regexp);
    if (reglen > strlen(src))
        return NULL;

    for (int i = 0; i < strlen(src) - reglen + 1; i++)
    {
        int j = 0, count = 0;
        while (j < strlen(regexp))
        {
            int token_len = token_length(&regexp[j]);
            if (validate_character(src[i + count], &regexp[j], token_len))
            {
                count++;
                j += token_len;
            }
            else
            {
                break;
            }
        }

        if (count == reglen)
        {
            return &src[i];
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    char *src = "la capanna di zio pino";
    char *regexp = ".nna";

    char *first = cerca_regexp(src, regexp);
    if (first != NULL)
    {
        printf("First regexp occurrence: %c", *first);
    }
    else
    {
        printf("No regexp match was found.");
    }

    return 0;
}
