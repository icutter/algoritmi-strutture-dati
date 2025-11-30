#include "inventory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pg_t
{
    char codice[7];
    char nome[51];
    char classe[51];
    tabEquip_t *equip;
    stat_t stat;
} pg_t;

typedef struct Node
{
    pg_t value;
    struct Node *next;
} nodoPg_t;

typedef struct tabPg_t
{
    nodoPg_t *headPg;
    nodoPg_t *tailPg;
    int nPg;
} tabPg_t;

extern tabPg_t *load_character_file(char *path);
extern void add_character(tabPg_t *pg, char *code, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr);
extern void remove_character(tabPg_t *pg, char *code);
extern void print_character(tabPg_t *pg, char *code);
