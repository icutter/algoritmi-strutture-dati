#include "characters.h"

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
        node->value.equip = malloc(sizeof(tabEquip_t));

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

void print_character(tabPg_t *pg, char *code)
{
    nodoPg_t *current = pg->headPg;
    while (current != NULL && strcmp(code, current->value.nome) != 0)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Character not found.\n");
        exit(1);
    }

    pg_t item = current->value;

    printf("Code:\t%s\n", item.codice);
    printf("Name:\t%s\n", item.nome);
    printf("Class:\t%s\n", item.classe);

    stat_t s = item.stat;
    printf("Stats:\t%d %d %d %d %d %d\n", s.hp, s.mp, s.atk, s.def, s.mag, s.spr);
    printf("\n");
}
