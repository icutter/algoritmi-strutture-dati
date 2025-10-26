#include <stdio.h>
#include <string.h>
#include <ctype.h>

int regexp_length(const char *regexp)
{
    int len = strlen(regexp);
    int i = 0, count = 0;

    while (i < len && regexp[i] != '\0')
    {
        if (regexp[i] == '\\')
        {
            i += 2; // es: \a
        }
        else if (regexp[i] == '[')
        {
            i++;
            while (i < len && regexp[i] != ']')
                i++;
            if (i < len)
                i++; // salta ']'
        }
        else
        {
            i++;
        }
        count++;
    }

    return count;
}

int token_length(const char *start)
{
    if (start[0] == '\\')
        return 2;

    if (start[0] == '[')
    {
        int i = 0;
        while (start[i] != '\0' && start[i] != ']')
            i++;
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
        default:
            return 0;
        }
    }

    if (n > 2 && regexp[0] == '[')
    {
        int negate = 0;
        int i = 1;

        if (regexp[i] == '^')
        {
            negate = 1;
            i++;
        }

        int match_found = 0;
        /* n is token length; characters are regexp[1] .. regexp[n-2] (']' at n-1) */
        while (i < n - 1 && regexp[i] != ']')
        {
            if (src == regexp[i])
            {
                match_found = 1;
                break;
            }
            i++;
        }

        return negate ? !match_found : match_found;
    }

    return src == regexp[0];
}

char *cerca_regexp(char *src, char *regexp)
{
    int reglen = regexp_length(regexp);
    int srclen = strlen(src);

    if (reglen > srclen)
        return NULL;

    for (int i = 0; i <= srclen - reglen; i++)
    {
        int j = 0, count = 0;
        while (j < (int)strlen(regexp))
        {
            int token_len = token_length(&regexp[j]);
            if (validate_character(src[i + count], &regexp[j], token_len))
            {
                count++;
                j += token_len;
            }
            else
                break;
        }

        if (count == reglen)
            return &src[i];
    }

    return NULL;
}

int main(void)
{
    char *src = "La capanna di zio Pino";
    /* Nota: doppio backslash per avere '\' nella stringa */
    char *regexp = "[^c]an\\aa";

    char *first = cerca_regexp(src, regexp);
    if (first != NULL)
    {
        int reglen = regexp_length(regexp);
        printf("First regexp occurrence: \"%.*s\"\n", reglen, first);
    }
    else
    {
        printf("No regexp match was found.\n");
    }

    return 0;
}
