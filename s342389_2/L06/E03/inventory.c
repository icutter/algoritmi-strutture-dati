#include "inventory.h"

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
    for (int i = 0; i < obj->nInv; i++)
    {
        inv_t curr = obj->vettInv[i];
        if (strcmp(name, curr.nome) == 0)
        {
            printf("Name:\t%s\n", curr.nome);
            printf("Type:\t%s\n", curr.tipo);

            stat_t s = curr.stat;
            printf("Stats:\t%d %d %d %d %d %d\n", s.hp, s.mp, s.atk, s.def, s.mag, s.spr);
            printf("\n");
        }
    }

    printf("Could not find requested item.\n");
}
