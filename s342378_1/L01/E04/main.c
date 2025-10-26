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

int are_equal(const char *a, const char *b)
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
        printf("Could not open file.\n");
        return NULL;
    }

    if (fscanf(fp, "%d", len) != 1)
    {
        printf("Invalid file format (missing count).\n");
        fclose(fp);
        return NULL;
    }

    corsa *db = malloc(*len * sizeof(corsa));
    if (db == NULL)
    {
        printf("Memory allocation failure.\n");
        fclose(fp);
        exit(1);
    }

    for (int i = 0; i < *len; i++)
    {
        if (fscanf(fp, "%30s %30s %30s %10s %8s %8s %u",
                   db[i].codice_tratta,
                   db[i].partenza,
                   db[i].destinazione,
                   db[i].data,
                   db[i].ora_partenza,
                   db[i].ora_arrivo,
                   &db[i].ritardo) != 7)
        {
            printf("Invalid file format at record %d.\n", i);
            free(db);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    return db;
}

int is_range_digit(const char *str, int from, int to)
{
    if (from > to)
    {
        printf("Invalid range.\n");
        exit(2);
    }

    for (int i = from; i <= to; i++)
    {
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }

    return 1;
}

int is_yyyy_mm_dd(const char *str)
{
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

void print_corsa(const corsa *c)
{
    printf("%s %s %s %s %s %s %u\n", c->codice_tratta, c->partenza, c->destinazione, c->data, c->ora_partenza, c->ora_arrivo, c->ritardo);
}

void print_corsa_to_file(FILE *fp, const corsa *c)
{
    fprintf(fp, "%s %s %s %s %s %s %u\n", c->codice_tratta, c->partenza, c->destinazione, c->data, c->ora_partenza, c->ora_arrivo, c->ritardo);
}

void print_sorted(corsa **sorted_db, int n)
{
    for (int i = 0; i < n; i++)
    {
        print_corsa(sorted_db[i]);
    }
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
        return;
    }

    fprintf(fp, "%d\n", n);

    for (int i = 0; i < n; i++)
    {
        print_corsa_to_file(fp, &db[i]);
    }

    fclose(fp);
}

// Comparison functions (work on actual corsa values)
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

// Insertion sort on array of pointers to corsa
void sort(corsa **arr, int n, int (*comp_func)(corsa a, corsa b))
{
    for (int i = 1; i < n; i++)
    {
        corsa *key = arr[i];
        int j = i - 1;
        while (j >= 0 && comp_func(*key, *arr[j]) < 0)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
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

// Linear / binary search operate on arrays of pointers
void ricerca_lineare(corsa **db, int n, char *str, int (*comp_func)(corsa, char *))
{
    for (int i = 0; i < n; i++)
    {
        if (comp_func(*db[i], str) == 0)
        {
            print_corsa(db[i]);
            return;
        }
    }
    printf("Elemento non trovato.\n");
}

void ricerca_binaria(corsa **db, int n, char *str, int (*comp_func)(corsa, char *))
{
    int l = 0, r = n - 1;
    int found = -1;

    while (l <= r)
    {
        int m = (l + r) / 2;
        int cmp = comp_func(*db[m], str);

        if (cmp == 0)
        {
            found = m;
            while (found > 0 && comp_func(*db[found - 1], str) == 0)
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

    print_corsa(db[found]);
}

void free_sorted_arrays(corsa **a, corsa **b, corsa **c, corsa **d)
{
    if (a)
        free(a);
    if (b)
        free(b);
    if (c)
        free(c);
    if (d)
        free(d);
}

void seleziona_dati(corsa **db, int *n, comando_e cmd, char *cmdstr,
                    corsa ***db_data, corsa ***db_tratta, corsa ***db_partenza, corsa ***db_arrivo)
{
    if (*db == NULL && cmd != r_apri_file && cmd != r_aiuto && cmd != r_fine)
    {
        printf("Nessun file aperto. Usa 'apri_file <percorso>'.\n");
        return;
    }

    char args[2][256];
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
        sscanf(cmdstr, "%*s %255s", args[0]);
        // if a DB is already loaded, free it and the sorted arrays
        if (*db != NULL)
        {
            free_sorted_arrays(*db_data ? *db_data : NULL,
                               *db_tratta ? *db_tratta : NULL,
                               *db_partenza ? *db_partenza : NULL,
                               *db_arrivo ? *db_arrivo : NULL);
            free(*db);
            *db = NULL;
            *n = 0;
        }

        *db = load_file(args[0], n);
        if (*db == NULL)
        {
            printf("Errore nel caricamento del file %s.\n", args[0]);
            return;
        }

        // allocate pointer arrays
        *db_data = malloc(*n * sizeof(corsa *));
        *db_tratta = malloc(*n * sizeof(corsa *));
        *db_partenza = malloc(*n * sizeof(corsa *));
        *db_arrivo = malloc(*n * sizeof(corsa *));

        if (!*db_data || !*db_tratta || !*db_partenza || !*db_arrivo)
        {
            printf("Memory allocation failure for pointer arrays.\n");
            free_sorted_arrays(*db_data, *db_tratta, *db_partenza, *db_arrivo);
            free(*db);
            *db = NULL;
            *n = 0;
            exit(1);
        }

        // initialize pointer arrays to original order
        for (int i = 0; i < *n; i++)
        {
            (*db_data)[i] = &(*db)[i];
            (*db_tratta)[i] = &(*db)[i];
            (*db_partenza)[i] = &(*db)[i];
            (*db_arrivo)[i] = &(*db)[i];
        }

        // Sorting per data e ora (stable via two sorts: time then date)
        sort(*db_data, *n, comp_ora_partenza);
        sort(*db_data, *n, comp_data);

        // Sorting per tratta
        sort(*db_tratta, *n, comp_tratta);

        // Sorting per fermata di partenza
        sort(*db_partenza, *n, comp_partenza);

        // Sorting per fermata di arrivo
        sort(*db_arrivo, *n, comp_arrivo);

        printf("Il file %s è stato caricato correttamente (%d record).\n", args[0], *n);
        return;
    case r_stampa_corse:
        if (sscanf(cmdstr, "%*s %255s", args[0]) == 1)
        {
            // stampa su file (original order)
            write_corse_to_file(*db, *n, args[0]);
        }
        else
        {
            // stampa a video (original order)
            print_corse(*db, *n);
        }
        return;
    case r_sort_data:
        print_sorted(*db_data, *n);
        return;
    case r_sort_tratta:
        print_sorted(*db_tratta, *n);
        return;
    case r_sort_partenza:
        print_sorted(*db_partenza, *n);
        return;
    case r_sort_arrivo:
        print_sorted(*db_arrivo, *n);
        return;
    case r_cerca_tratta:
        scanned = sscanf(cmdstr, "%*s %255s %255s", args[0], args[1]);

        if (scanned == 0)
        {
            printf("Inserire almeno un argomento.\n");
            return;
        }

        if (scanned == 2 && strcmp(args[1], "binaria") == 0)
        {
            ricerca_binaria(*db_tratta, *n, args[0], comp_fuzzy_codice_tratta);
            return;
        }

        if (scanned == 1 || (scanned == 2 && strcmp(args[1], "lineare") == 0))
        {
            ricerca_lineare(*db_tratta, *n, args[0], comp_fuzzy_codice_tratta);
            return;
        }

        printf("Argomenti non validi.\n");
        return;
    case r_cerca_partenza:
        scanned = sscanf(cmdstr, "%*s %255s %255s", args[0], args[1]);

        if (scanned == 0)
        {
            printf("Inserire almeno un argomento.\n");
            return;
        }

        if (scanned == 2 && strcmp(args[1], "binaria") == 0)
        {
            ricerca_binaria(*db_partenza, *n, args[0], comp_fuzzy_partenza);
            return;
        }

        if (scanned == 1 || (scanned == 2 && strcmp(args[1], "lineare") == 0))
        {
            ricerca_lineare(*db_partenza, *n, args[0], comp_fuzzy_partenza);
            return;
        }

        printf("Argomenti non validi.\n");
        return;
    case r_fine:
        // free memory before exiting
        free_sorted_arrays(*db_data ? *db_data : NULL,
                           *db_tratta ? *db_tratta : NULL,
                           *db_partenza ? *db_partenza : NULL,
                           *db_arrivo ? *db_arrivo : NULL);
        if (*db)
            free(*db);
        exit(0);
    default:
        return;
    }
}

int main(int argc, char *argv[])
{
    corsa *db = NULL;
    int dbcount = 0;

    corsa **db_data = NULL;
    corsa **db_tratta = NULL;
    corsa **db_partenza = NULL;
    corsa **db_arrivo = NULL;

    printf("Inserisci un comando o \"aiuto\" per vedere tutti i comandi.\n");

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
        seleziona_dati(&db, &dbcount, cmd, inputstr, &db_data, &db_tratta, &db_partenza, &db_arrivo);
    }

    // cleanup on EOF
    free_sorted_arrays(db_data, db_tratta, db_partenza, db_arrivo);
    if (db)
        free(db);
    return 0;
}
