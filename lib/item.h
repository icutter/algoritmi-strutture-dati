#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *Key;
typedef struct item *Item;

Item ITEMnew(char *name, int num);
void ITEMfree(Item val);
void ITEMshow(Item val);
Key KEYget(Item val);
int KEYcmp(Key k1, Key k2);

#endif