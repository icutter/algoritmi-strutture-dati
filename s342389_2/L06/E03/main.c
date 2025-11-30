#include "characters.h"
#include "inventory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PG_FILE_PATH "pg.txt"
#define INV_FILE_PATH "inventario.txt"

typedef enum
{
    r_none,

    r_pg_add,
    r_pg_remove,
    r_pg_print,

    r_inv_print,

    r_help,
    r_quit
} comando_e;

void print_commands()
{
    const char tab[] = "  ";
    printf("%spg_add\t<code> <name> etc. etc.\n", tab); // TODO: Add character
    printf("%spg_remove\t<code>\n", tab);
    printf("%spg_print\t<code>\n", tab);
    printf("%sinv_print\t<name>\n", tab);
    printf("%squit\n\n", tab);
}

int are_equal(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

comando_e read_command(char *str)
{
    if (are_equal(str, "pg_add"))
        return r_pg_add;
    if (are_equal(str, "pg_remove"))
        return r_pg_remove;
    if (are_equal(str, "pg_print"))
        return r_pg_print;

    if (are_equal(str, "inv_print"))
        return r_inv_print;

    if (are_equal(str, "help"))
        return r_help;
    if (are_equal(str, "quit"))
        return r_quit;

    return r_none;
}

void seleziona_dati(tabPg_t *tabPg, tabInv_t *tabInv, comando_e cmd, char *cmdstr)
{
    char args[3][256];
    int scanned;

    switch (cmd)
    {
    case r_none:
        printf("Invalid command.\n");
        return;

    case r_pg_add:
        int hp, mp, atk, def, mag, spr;
        sscanf(cmdstr, "%*s %255s %255s %255s %d %d %d %d %d %d", args[0], args[1], args[2], &hp, &mp, &atk, &def, &mag, &spr);
        add_character(tabPg, args[0], args[1], args[2], hp, mp, atk, def, mag, spr);
        return;
    case r_pg_remove:
        sscanf(cmdstr, "%*s %255s", args[0]);
        remove_character(tabPg, args[0]);
        return;
    case r_pg_print:
        sscanf(cmdstr, "%*s %255s", args[0]);
        print_character(tabPg, args[0]);
        return;

    case r_inv_print:
        sscanf(cmdstr, "%*s %255s", args[0]);
        print_inventory_item(tabInv, args[0]);
        return;

    case r_help:
        print_commands();
        return;
    case r_quit:
        return;
    }
}

int main(int argc, char *argv[])
{
    tabPg_t *pgPt = load_character_file(PG_FILE_PATH);
    tabInv_t *invPt = load_inventory_file(INV_FILE_PATH);

    printf("Insert a command or type \"help\" to see all available commands.\n");

    while (1)
    {
        printf("\n$ ");
        char inputstr[512];
        if (!fgets(inputstr, sizeof(inputstr), stdin))
            break;

        // remove trailing newline
        size_t L = strlen(inputstr);
        if (L > 0 && inputstr[L - 1] == '\n')
            inputstr[L - 1] = '\0';

        char cmdstr[64] = {0};
        sscanf(inputstr, "%63s", cmdstr);

        comando_e cmd = read_command(cmdstr);
        seleziona_dati(pgPt, invPt, cmd, inputstr);
    }

    return 0;
}
