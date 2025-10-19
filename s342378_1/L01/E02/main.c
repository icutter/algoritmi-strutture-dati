#include <stdio.h>
#include <stdlib.h>
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
    const char tab[] = "  ";
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

void print_date(FILE *fp, int rows, char *fromDate, char *toDate)
{
    char str[61], date[11];

    for (int i = 0; i < rows; i++)
    {
        fgets(str, 60, fp);
        if (sscanf(str, "%*s %*s %*s %10s %*s %*s %*d", date))
        {
            if (strcmp(date, fromDate) > 0 && strcmp(date, toDate) < 0)
            {
                printf("%s\n", str);
            }
        }
    }
    printf("\n");
}

void print_partenza(FILE *fp)
{
}

void print_capolinea(FILE *fp)
{
}

void print_ritardo(FILE *fp)
{
}

void print_ritardo_tot(FILE *fp)
{
}

void seleziona_dati(FILE *fp, int rows, comando_e cmd)
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
        print_date(fp, rows, "2017/10/04", "2020/10/27");
        return;
    case r_partenza:
        print_partenza(fp);
        return;
    case r_capolinea:
        print_capolinea(fp);
        return;
    case r_ritardo:
        print_ritardo(fp);
        return;
    case r_ritardo_tot:
        print_ritardo_tot(fp);
        return;
    case r_fine:
        exit(0);
        return;
    }
}

void discard_line(FILE *fp)
{
    int c;
    while ((c = fgetc(fp)) != EOF && c != '\n')
        ;
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen("corse.txt", "r");
    if (fp == NULL)
    {
        printf("Could not open file.");
        return 1;
    }

    int rowCount;
    fscanf(fp, "%d", &rowCount);

    printf("Inserisci un comando o \"aiuto\" per vedere tutti i comandi.\n");

    while (1)
    {
        char str[31];
        scanf("%s", str);

        rewind(fp);
        discard_line(fp);

        comando_e cmd = read_command(str);
        seleziona_dati(fp, rowCount, cmd);
    }

    return 0;
}
