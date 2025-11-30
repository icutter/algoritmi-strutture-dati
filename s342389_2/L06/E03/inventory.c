#include "inventory.h"

inv_t *get_item(tabInv_t *inv, char *name)
{
    if (name == NULL || *name == '\0')
    {
        printf("No name provided.\n");
        exit(2);
    }

    inv_t *curr;
    for (int i = 0; i < inv->nInv; i++)
    {
        curr = &inv->vettInv[i];
        if (strcmp(name, curr->nome))
        {
            return curr;
        }
    }

    printf("Could not find the specified item.\n");
    return NULL;
}

tabInv_t *load_inventory_file(char *path)
{
    tabInv_t *invPt = malloc(sizeof(tabInv_t));

    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open inventory file.\n");
        exit(1);
    }

    int n;
    fscanf(fp, "%d", &n);
    invPt->nInv = n;

    invPt->vettInv = malloc(n * sizeof(inv_t));

    for (int i = 0; i < n; i++)
    {
        inv_t *curr = &invPt->vettInv[i];
        fscanf(fp, "%s %s %d %d %d %d %d %d",
               curr->nome,
               curr->tipo,
               &curr->stat.hp,
               &curr->stat.mp,
               &curr->stat.atk,
               &curr->stat.def,
               &curr->stat.mag,
               &curr->stat.spr);
    }

    fclose(fp);
    return invPt;
}

void print_inventory_item(tabInv_t *obj, char *name)
{
    if (name == NULL || *name == '\0')
    {
        printf("No name provided.\n");
        exit(2);
    }

    for (int i = 0; i < obj->nInv; i++)
    {
        inv_t curr = obj->vettInv[i];
        if (strcmp(name, curr.nome) == 0)
        {
            printf("Name:\t%s\t\t", curr.nome);
            printf("Type:\t%s\t\t", curr.tipo);

            stat_t s = curr.stat;
            printf("Stats:\t%d %d %d %d %d %d\n", s.hp, s.mp, s.atk, s.def, s.mag, s.spr);
            printf("\n");
            return;
        }
    }

    printf("Could not find requested item.\n");
}
