#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stat_t
{
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} stat_t;

typedef struct inv_t
{
    char nome[51];
    char tipo[51];
    stat_t stat;
} inv_t;

typedef struct tabEquip_t
{
    int inUso;
    inv_t *vettEq;
} tabEquip_t;

typedef struct tabInv_t
{
    inv_t *vettInv;
    int nInv;
    int maxInv;
} tabInv_t;

extern tabInv_t *load_inventory_file(char *path);
extern void print_inventory_item(tabInv_t *obj, char *name);
