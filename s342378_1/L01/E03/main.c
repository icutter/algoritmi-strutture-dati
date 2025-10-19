#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    r_none,
    r_aiuto,
    r_apri_file,
    r_date,
    r_partenza,
    r_capolinea,
    r_ritardo,
    r_ritardo_tot,
    r_fine
} comando_e;

typedef struct
{
    char codice_tratta[31];
    char partenza[31];
    char destinazione[31];
    char data[11];
    char ora_partenza[9];
    char ora_arrivo[9];
    unsigned int ritardo;
} corsa;

void print_commands()
{
    const char tab[] = "  ";
    printf("%sapri_file\t{percorso}\n", tab);
    printf("%sdate\t\t{data_inizio} {data_fine}\n", tab);
    printf("%spartenza\t{fermata}\n", tab);
    printf("%scapolinea\t{fermata}\n", tab);
    printf("%sritardo\t{data_inizio} {data_fine}\n", tab);
    printf("%sritardo_tot\t{tratta}\n", tab);
    printf("%sfine\n\n", tab);
}

int are_equal(char *a, char *b)
{
    return strcmp(a, b) == 0;
}

comando_e read_command(char *str)
{
    if (are_equal(str, "aiuto"))
        return r_aiuto;
    if (are_equal(str, "apri_file"))
        return r_apri_file;
    if (are_equal(str, "date"))
        return r_date;
    if (are_equal(str, "partenza"))
        return r_partenza;
    if (are_equal(str, "capolinea"))
        return r_capolinea;
    if (are_equal(str, "ritardo"))
        return r_ritardo;
    if (are_equal(str, "ritardo_tot"))
        return r_ritardo_tot;
    if (are_equal(str, "fine"))
        return r_fine;
    return r_none;
}

corsa *load_file(char *path, int *len)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open file.");
        exit(1);
    }

    fscanf(fp, "%d", len);
    corsa *db = malloc(*len * sizeof(corsa));

    for (int i = 0; i < *len; i++)
    {
        fscanf(fp, "%s %s %s %s %s %s %d ",
               db[i].codice_tratta,
               db[i].partenza,
               db[i].destinazione,
               db[i].data,
               db[i].ora_partenza,
               db[i].ora_arrivo,
               &db[i].ritardo);
    }

    fclose(fp);
    return db;
}

void print_corsa(corsa *c)
{
    printf("%s %s %s %s %s %s %d\n", c->codice_tratta, c->partenza, c->destinazione, c->data, c->ora_partenza, c->ora_arrivo, c->ritardo);
}

int is_range_digit(const char *str, int from, int to)
{
    if (from > to)
    {
        printf("Invalid range.");
        exit(2);
    }

    for (int i = from; i <= to; i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }

    return 1;
}

int is_yyyy_mm_dd(const char *str)
{
    // Expect YYYY/MM/DD
    //        0123456789

    if (strlen(str) != 10)
        return 0;

    return is_range_digit(str, 0, 3) && (str[4] == '/') &&
           is_range_digit(str, 5, 6) && (str[7] == '/') &&
           is_range_digit(str, 8, 9);
}

int is_in_range(char *date, char *from, char *to)
{
    return strcmp(date, from) >= 0 && strcmp(date, to) <= 0;
}

void print_date(corsa *db, int n, char *fromDate, char *toDate)
{
    if (!is_yyyy_mm_dd(fromDate) || !is_yyyy_mm_dd(toDate))
    {
        printf("Date formatting is wrong. YYYY/MM/DD required.");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (is_in_range(db[i].data, fromDate, toDate))
        {
            print_corsa(&db[i]);
        }
    }
    printf("\n");
}

void print_partenza(corsa *db, int n, char *fermata)
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(db[i].partenza, fermata) == 0)
        {
            print_corsa(&db[i]);
        }
    }
    printf("\n");
}

void print_capolinea(corsa *db, int n, char *destinazione)
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(db[i].destinazione, destinazione) == 0)
        {
            print_corsa(&db[i]);
        }
    }
    printf("\n");
}

void print_ritardo(corsa *db, int n, char *fromDate, char *toDate)
{
    if (!is_yyyy_mm_dd(fromDate) || !is_yyyy_mm_dd(toDate))
    {
        printf("Date formatting is wrong. YYYY/MM/DD required.");
        return;
    }

    for (int i = 0; i < n; i++)
    {
        if (db[i].ritardo <= 0)
            continue;

        if (is_in_range(db[i].data, fromDate, toDate))
        {
            print_corsa(&db[i]);
        }
    }
    printf("\n");
}

void print_ritardo_tot(corsa *db, int n, char *codice_tratta)
{
    int ritardo_tot = 0;
    for (int i = 0; i < n; i++)
    {
        if (strcmp(db[i].codice_tratta, codice_tratta) == 0)
        {
            ritardo_tot += db[i].ritardo;
        }
    }
    printf("Ritardo totale: %d\n", ritardo_tot);
}

void selezionaDati(corsa **db, int *n, comando_e cmd, char *cmdstr)
{
    char args[2][21];

    switch (cmd)
    {
    case r_none:
        printf("Comando non riconosciuto.\n");
        return;
    case r_aiuto:
        print_commands();
        return;
    case r_apri_file:
        sscanf(cmdstr, "%*s %s", args[0]);
        *db = load_file(args[0], n);
        return;
    case r_date:
        sscanf(cmdstr, "%*s %s %s", args[0], args[1]);
        print_date(*db, *n, args[0], args[1]);
        return;
    case r_partenza:
        sscanf(cmdstr, "%*s %s", args[0]);
        print_partenza(*db, *n, args[0]);
        return;
    case r_capolinea:
        sscanf(cmdstr, "%*s %s", args[0]);
        print_capolinea(*db, *n, args[0]);
        return;
    case r_ritardo:
        sscanf(cmdstr, "%*s %s %s", args[0], args[1]);
        print_ritardo(*db, *n, args[0], args[1]);
        return;
    case r_ritardo_tot:
        sscanf(cmdstr, "%*s %s", args[0]);
        print_ritardo_tot(*db, *n, args[0]);
        return;
    case r_fine:
        exit(0);
        return;
    }
}

int main(int argc, char *argv[])
{
    corsa *db;
    int dbcount = 0;

    printf("Inserisci un comando o \"aiuto\" per vedere tutti i comandi.\n");

    while (1)
    {
        char inputstr[256];
        fgets(inputstr, sizeof(inputstr), stdin);

        char cmdstr[21];
        sscanf(inputstr, "%s", cmdstr);

        comando_e cmd = read_command(cmdstr);
        selezionaDati(&db, &dbcount, cmd, inputstr);
    }

    return 0;
}
