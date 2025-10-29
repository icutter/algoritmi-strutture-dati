#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CODE_STR_LEN 6
#define DATE_STR_LEN 11

typedef struct
{
    short day, month, year;
} date_t;

typedef struct
{
    char codice[CODE_STR_LEN];
    char nome[51];
    char cognome[51];
    date_t data_di_nascita;
    char via[51];
    char citta[51];
    int cap;
} item_t;

typedef struct node
{
    item_t value;
    struct node *next;
} node_t;

int parse_code(char *dest, char *str)
{
    // AXXXX
    // 01234

    if (strlen(str) != 5)
        return 0;

    if (str[0] != 'A')
        return 0;

    for (int i = 1; i <= 4; i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }

    strcpy(dest, str);
    return 1;
}

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

void format_date(char *buffer, date_t date)
{
    sprintf(buffer, "%d/%d/%d", date.day, date.month, date.year);
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

void print_item(item_t item)
{
    char date_str[11];
    format_date(date_str, item.data_di_nascita);
    printf("%s %s %s %s %s %s %c\n",
           item.codice,
           item.nome,
           item.cognome,
           date_str,
           item.via,
           item.citta,
           item.cap);
}

void print_list(node_t *head)
{
    node_t *current = head;
    while (current)
    {
        print_item(current->value);
        current = current->next;
    }
}

int parse_item(item_t *item, char *str)
{
    char codice_str[CODE_STR_LEN];
    char data_str[DATE_STR_LEN];

    int res = sscanf(str, "%5s %s %s %10s %s %s %d",
                     codice_str,
                     item->nome,
                     item->cognome,
                     data_str,
                     item->via,
                     item->citta,
                     &item->cap);

    int err = res != 7 || !parse_code(item->codice, codice_str) || !parse_date(&item->data_di_nascita, data_str);
    return !err;
}

int add_to_sorted_list(node_t **head, item_t item)
{
    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = item;
    new_node->next = NULL;

    // Check first item
    if (*head == NULL || comp_date(item.data_di_nascita, (*head)->value.data_di_nascita) < 0)
    {
        new_node->next = *head;
        *head = new_node;
        return 0;
    }

    node_t *current = *head;
    while (current->next != NULL && comp_date(current->next->value.data_di_nascita, item.data_di_nascita) <= 0)
    {
        current = current->next;
    }

    // Insert node
    node_t *tmp = current->next;
    current->next = new_node;
    new_node->next = tmp;
}

int add_from_file(node_t **head, char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open file.");
        return 0;
    }

    item_t new_item;

    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        if (parse_item(&new_item, line))
        {
            add_to_sorted_list(head, new_item);
        }
    }

    fclose(fp);
}

int main(int argc, char *argv[])
{
    node_t *head = NULL;
    char path[] = "anag1.txt";

    add_from_file(&head, path);

    print_list(head);

    return 0;
}
