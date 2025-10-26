#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct
{
    short year, month, day;
} date_t;

typedef struct
{
    char codice[51];
    char nome[51];
    char cognome[51];
    date_t data_di_nascita;
    char via[51];
    char citta[51];
    int cap;
} item_t;

typedef struct
{
    item_t value;
    node_t *next;
} node_t;

int parse_date(date_t *date, char *str)
{
    int d, m, y;
    char sep1, sep2;

    // Try parse from dd/mm/yyyy format
    if (sscanf(str, "%2d%c%2d%c%4d", &d, &sep1, &m, &sep2, &y) != 5)
        return 0;

    // Check separators
    if (sep1 != '/' || sep2 != '/')
        return 0;

    // Check ranges
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 0)
        return 0;

    date->day = d;
    date->month = m;
    date->year = y;

    return 1;
}

int comp_date(date_t a, date_t b)
{
    int yearDiff = a.year - b.year;
    int monthDiff = a.month - b.month;
    int dayDiff = a.day - b.day;

    return yearDiff != 0 ? yearDiff : (monthDiff != 0 ? monthDiff : dayDiff);
}

void clear_list(node_t **head)
{
    node_t *current = *head;
    while (current)
    {
        node_t *tmp = current;
        current = current->next;
        free(tmp);
    }
}

int create_node(node_t **node, char *date)
{
    *node = malloc(sizeof(node_t));

    char *codeStr;
    char *dateStr;

    // sscanf(date, "%5s %s %s %10s %s %s %d",

    // );
}

int insert_sorted(node_t **head, item_t value)
{
    // node_t *new_node = malloc(sizeof(node_t));

    // if (*head == NULL)
    // {
    //     new_node->value = new_item;
    // }
}

int new_list_from_file(node_t **head, char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open file.");
        return 1;
    }

    int scanned = 0;

    while (1)
    {
        item_t *new_item = malloc(sizeof(item_t));
        if (new_item == NULL)
        {
        }

        scanned = fscanf(fp, "%5s %s %s %10s %s %s %d",
                         new_item->codice,
                         new_item->nome,
                         new_item->cognome,
                         new_item->data_di_nascita,
                         new_item->via,
                         new_item->citta,
                         &new_item->cap);
    }
}

int main(int argc, char *argv[])
{
    node_t *head = NULL;
    char path[] = "anag1.txt";

    new_list_from_file(&head, path);

    return 0;
}
