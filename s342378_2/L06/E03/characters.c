#include "characters.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

nodoPg_t *get_character(tabPg_t *pgs, char *code)
{
    if (code == NULL || *code == '\0')
    {
        printf("No code provided.\n");
        exit(2);
    }

    nodoPg_t *current = pgs->headPg;
    while (current != NULL && strcmp(code, current->value.codice) != 0)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Character not found.\n");
        return NULL;
    }

    return current;
}

tabPg_t *load_character_file(char *path)
{
    tabPg_t *pgPt = malloc(sizeof(tabPg_t));

    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open character file.\n");
        exit(1);
    }

    pgPt->headPg = NULL;
    pgPt->tailPg = NULL;
    pgPt->nPg = 0;

    int exitFlag = 0;
    while (exitFlag == 0)
    {
        nodoPg_t *node = malloc(sizeof(nodoPg_t));
        node->next = NULL;

        for (int i = 0; i < 8; i++)
            node->value.equip[i].inUso = 0;

        int s = fscanf(fp, "%6s %50s %50s %d %d %d %d %d %d",
                       node->value.codice,
                       node->value.nome,
                       node->value.classe,
                       &node->value.stat.hp,
                       &node->value.stat.mp,
                       &node->value.stat.atk,
                       &node->value.stat.def,
                       &node->value.stat.mag,
                       &node->value.stat.spr);

        if (s == 9)
        {
            if (pgPt->headPg == NULL)
            {
                pgPt->headPg = node;
            }
            else
            {
                pgPt->tailPg->next = node;
            }

            pgPt->tailPg = node;
            pgPt->nPg++;
        }
        else
        {
            exitFlag = 1;
        }
    }

    return pgPt;
}

void add_character(tabPg_t *pg, char *code, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr)
{
    // TODO: Add character
    // Banale
}

void remove_character(tabPg_t *pg, char *code)
{
    // TODO: Remove character
    // Già svolto nelle esercitazioni precedenti
}

void print_character(nodoPg_t *pg, tabInv_t *invPt)
{
    printf("Code:\t%s\n", pg->value.codice);
    printf("Name:\t%s\n", pg->value.nome);
    printf("Class:\t%s\n", pg->value.classe);

    stat_t s = pg->value.stat;
    printf("Stats:\t%d %d %d %d %d %d\n", s.hp, s.mp, s.atk, s.def, s.mag, s.spr);

    printf("Inventory:\n");

    for (int i = 0; i < 8; i++)
    {
        tabEquip_t eq = pg->value.equip[i];
        if (eq.inUso)
        {
            print_inventory_item(invPt, eq.item->nome);

            s.hp += eq.item->stat.hp;
            s.mp += eq.item->stat.mp;
            s.atk += eq.item->stat.atk;
            s.def += eq.item->stat.def;
            s.mag += eq.item->stat.mag;
            s.spr += eq.item->stat.spr;
        }
    }

    s.hp = MAX(s.hp, 0);
    s.mp = MAX(s.mp, 0);
    s.atk = MAX(s.atk, 0);
    s.def = MAX(s.def, 0);
    s.mag = MAX(s.mag, 0);
    s.spr = MAX(s.spr, 0);

    printf("\n");
    printf("Altered Stats:\t%d %d %d %d %d %d\n", s.hp, s.mp, s.atk, s.def, s.mag, s.spr);
    printf("\n");
}

void add_item(nodoPg_t *pg, tabInv_t *invPt, char *item_name)
{
    for (int i = 0; i < 8; i++)
    {
        tabEquip_t *eq = &pg->value.equip[i];
        if (!eq->inUso)
        {
            inv_t *new_item = get_item(invPt, item_name);
            eq->item = new_item;
            eq->inUso = 1;

            printf("Item was added to the inventory.\n");
            return;
        }
    }

    printf("Inventory is full.\n");
}

void remove_item(nodoPg_t *pg, char *item_name)
{
    for (int i = 0; i < 8; i++)
    {
        tabEquip_t *eq = pg->value.equip[i];
        if (eq->inUso && strcmp(item_name, eq->item->nome) == 0)
        {
            eq->inUso = 0;
        }
    }
}
