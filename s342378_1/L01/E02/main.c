#include <stdio.h>
#include <string.h>

typedef enum
{
    r_none,
    r_aiuto,
    r_date,
    r_partenza,
    r_capolinea,
    r_ritardo,
    r_ritardo_tot,
    r_fine
} comando_e;

void print_commands()
{
    char tab[] = "  ";
    printf("%sdate\t\t{data_inizio} {data_fine}\n", tab);
    printf("%spartenza\t{fermata}\n", tab);
    printf("%scapolinea\t{fermata}\n", tab);
    printf("%sritardo\t{data_inizio} {data_fine}\n", tab);
    printf("%sritardo_tot\t{tratta}\n", tab);
    printf("%sfine\n\n", tab);
}

comando_e read_command(char *str)
{
    if (strcmp(str, "aiuto") == 0)
        return r_aiuto;
    if (strcmp(str, "date") == 0)
        return r_date;
    if (strcmp(str, "partenza") == 0)
        return r_partenza;
    if (strcmp(str, "capolinea") == 0)
        return r_capolinea;
    if (strcmp(str, "ritardo") == 0)
        return r_ritardo;
    if (strcmp(str, "ritardo_tot") == 0)
        return r_ritardo_tot;
    if (strcmp(str, "fine") == 0)
        return r_fine;
    return r_none;
}

void selezionaDati(comando_e cmd)
{
    switch (cmd)
    {
    case r_none:
        printf("Comando non riconosciuto.\n");
        return;
    case r_aiuto:
        print_commands();
        return;
    case r_date:
        return;
    case r_partenza:
        return;
    case r_capolinea:
        return;
    case r_ritardo:
        return;
    case r_ritardo_tot:
        return;
    case r_fine:
        return;
    }
}

int main(int argc, int *argv[])
{
    printf("Inserisci un comando o \"aiuto\" per vedere tutti i comandi.\n");

    while (1)
    {
        char str[20];
        scanf("%s", str);

        comando_e cmd = read_command(str);
        selezionaDati(cmd);
    }

    return 0;
}
