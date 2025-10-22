#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    r_none,
    r_aiuto,
    r_apri_file,
    r_stampa_corse,
    r_sort_data,
    r_sort_tratta,
    r_sort_partenza,
    r_sort_arrivo,
    r_cerca_tratta,
    r_cerca_partenza,
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
    printf("%sapri_file\t\t<percorso>\n", tab);
    printf("%sstampa_corse\t\t<percorso>\n", tab);
    printf("%ssort_data\n", tab);
    printf("%ssort_tratta\n", tab);
    printf("%ssort_partenza\n", tab);
    printf("%ssort_arrivo\n", tab);
    printf("%scerca_tratta\t\t<nome_tratta> [lineare|binaria]\n", tab);
    printf("%scerca_partenza\t<nome_partenza> [lineare|binaria]\n", tab);
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
    if (are_equal(str, "stampa_corse"))
        return r_stampa_corse;
    if (are_equal(str, "sort_data"))
        return r_sort_data;
    if (are_equal(str, "sort_tratta"))
        return r_sort_tratta;
    if (are_equal(str, "sort_partenza"))
        return r_sort_partenza;
    if (are_equal(str, "sort_arrivo"))
        return r_sort_arrivo;
    if (are_equal(str, "cerca_tratta"))
        return r_cerca_tratta;
    if (are_equal(str, "cerca_partenza"))
        return r_cerca_partenza;
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

void print_corsa_to_file(FILE *fp, corsa *c)
{
    fprintf(fp, "%s %s %s %s %s %s %d\n", c->codice_tratta, c->partenza, c->destinazione, c->data, c->ora_partenza, c->ora_arrivo, c->ritardo);
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
    // Index  0123456789

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

int starts_with(const char *str, const char *prefix)
{
    size_t len_prefix = strlen(prefix);
    size_t len_str = strlen(str);
    return len_str >= len_prefix && strncmp(str, prefix, len_prefix) == 0;
}

void print_corse(corsa *db, int n)
{
    for (int i = 0; i < n; i++)
    {
        print_corsa(&db[i]);
    }
}

void write_corse_to_file(corsa *db, int n, char *path)
{
    FILE *fp = fopen(path, "w+");
    if (fp == NULL)
    {
        printf("Could not open or create file.\n");
        exit(1);
    }

    fprintf(fp, "%d\n", n);

    for (int i = 0; i < n; i++)
    {
        print_corsa_to_file(fp, &db[i]);
    }

    fclose(fp);
}

// ********************
// Comparison functions
// ********************

int comp_data(corsa a, corsa b)
{
    return strcmp(a.data, b.data);
}

int comp_ora_partenza(corsa a, corsa b)
{
    return strcmp(a.ora_partenza, b.ora_partenza);
}

int comp_tratta(corsa a, corsa b)
{
    return strcmp(a.codice_tratta, b.codice_tratta);
}

int comp_partenza(corsa a, corsa b)
{
    return strcmp(a.partenza, b.partenza);
}

int comp_arrivo(corsa a, corsa b)
{
    return strcmp(a.destinazione, b.destinazione);
}

void sort(corsa *db, int n, int (*comp_func)(corsa a, corsa b))
{
    const int l = 0, r = n - 1;
    int i, j;
    corsa x;

    for (i = l + 1; i <= r; i++)
    {
        x = db[i];
        j = i - 1;

        while (j >= l && comp_func(x, db[j]) < 0)
        {
            db[j + 1] = db[j];
            j--;
        }

        db[j + 1] = x;
    }
}

int comp_fuzzy_field(const char *field, const char *query)
{
    size_t len = strlen(query);
    int cmp = strncmp(field, query, len);
    if (cmp == 0)
        return 0;
    return strcmp(field, query);
}

int comp_fuzzy_codice_tratta(corsa c, char *str)
{
    return comp_fuzzy_field(c.codice_tratta, str);
}

int comp_fuzzy_partenza(corsa c, char *str)
{
    return comp_fuzzy_field(c.partenza, str);
}

void ricerca_lineare(corsa *db, int n, char *str, int (*comp_func)(corsa, char *))
{
    for (int i = 0; i < n; i++)
    {
        if (comp_func(db[i], str) == 0)
        {
            print_corsa(&db[i]);
            return;
        }
    }
    printf("Elemento non trovato.\n");
}

void ricerca_binaria(corsa *db, int n, char *str, int (*comp_func)(corsa, char *))
{
    int l = 0, r = n - 1;
    int found = -1;

    while (l <= r)
    {
        int m = (l + r) / 2;
        int cmp = comp_func(db[m], str);

        if (cmp == 0)
        {
            found = m;
            // cerca il primo elemento valido a sinistra
            while (found > 0 && comp_func(db[found - 1], str) == 0)
                found--;
            break;
        }
        else if (cmp < 0)
            l = m + 1;
        else
            r = m - 1;
    }

    if (found == -1)
    {
        printf("Elemento non trovato.\n");
        return;
    }

    print_corsa(&db[found]);
}

void seleziona_dati(corsa **db, int *n, comando_e cmd, char *cmdstr)
{
    if (*db == NULL && cmd != r_apri_file && cmd != r_aiuto && cmd != r_fine)
    {
        printf("Nessun file aperto. Usa 'apri_file <percorso>'.\n");
        return;
    }

    char args[2][21];
    int scanned;

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
        printf("Il file %s è stato caricato correttamente.\n", args[0]);
        return;
    case r_stampa_corse:
        if (sscanf(cmdstr, "%*s %s", args[0]) == 1)
        {
            // stampa su file
            write_corse_to_file(*db, *n, args[0]);
        }
        else
        {
            // stampa a video
            print_corse(*db, *n);
        }
        return;
    case r_sort_data:
        // Algoritmo di ordinamento stabile
        sort(*db, *n, comp_ora_partenza);
        sort(*db, *n, comp_data);
        return;
    case r_sort_tratta:
        sort(*db, *n, comp_tratta);
        return;
    case r_sort_partenza:
        sort(*db, *n, comp_partenza);
        return;
    case r_sort_arrivo:
        sort(*db, *n, comp_arrivo);
        return;
    case r_cerca_tratta:
        scanned = sscanf(cmdstr, "%*s %s %s", args[0], args[1]);
        sort(*db, *n, comp_tratta);

        if (scanned == 0)
        {
            printf("Inserire almeno un argomento.\n");
            return;
        }

        if (scanned == 2 && strcmp(args[1], "binaria") == 0)
        {
            ricerca_binaria(*db, *n, args[0], comp_fuzzy_codice_tratta);
            return;
        }

        if (scanned == 1 || (scanned == 2 && strcmp(args[1], "lineare") == 0))
        {
            ricerca_lineare(*db, *n, args[0], comp_fuzzy_codice_tratta);
            return;
        }

        printf("Argomenti non validi.\n");
        return;
    case r_cerca_partenza:
        scanned = sscanf(cmdstr, "%*s %s %s", args[0], args[1]);
        sort(*db, *n, comp_partenza);

        if (scanned == 0)
        {
            printf("Inserire almeno un argomento.\n");
            return;
        }

        if (scanned == 2 && strcmp(args[1], "binaria") == 0)
        {
            ricerca_binaria(*db, *n, args[0], comp_fuzzy_partenza);
            return;
        }

        if (scanned == 1 || (scanned == 2 && strcmp(args[1], "lineare") == 0))
        {
            ricerca_lineare(*db, *n, args[0], comp_fuzzy_partenza);
            return;
        }

        printf("Argomenti non validi.\n");
        return;
    case r_fine:
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    corsa *db = NULL;
    int dbcount = 0;

    printf("Inserisci un comando o \"aiuto\" per vedere tutti i comandi.\n");

    while (1)
    {
        printf("\n$ ");

        char inputstr[256];
        fgets(inputstr, sizeof(inputstr), stdin);

        char cmdstr[21];
        sscanf(inputstr, "%s", cmdstr);

        comando_e cmd = read_command(cmdstr);
        seleziona_dati(&db, &dbcount, cmd, inputstr);
    }

    return 0;
}
