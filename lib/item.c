#include "item.h"

struct item
{
    char *name;
    int num;
};

Item ITEMnew(char *name, int num)
{
    Item val = malloc(sizeof(struct item));
    if (val == NULL)
        return NULL;
    val->name = strdup(name);
    val->num = num;
    return val;
}

void ITEMfree(Item val)
{
    if (val == NULL)
        return;
    if (val->name != NULL)
        free(val->name);
    free(val);
}

void ITEMshow(Item val)
{
    if (val != NULL)
        printf("Item: %s [%d]\n", val->name, val->num);
}

Key KEYget(Item val)
{
    return (val != NULL) ? val->name : NULL;
}

int KEYcmp(Key k1, Key k2)
{
    return strcmp(k1, k2);
}