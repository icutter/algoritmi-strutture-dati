#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CODE_STR_LEN 6
#define DATE_STR_LEN 11

typedef struct
{
    short year, month, day;
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
    perror("Function is not implemented.");
    exit(1);
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

int parse_item(item_t **item, char *str)
{
    if (*item != NULL)
    {
        free(*item);
    }
    *item = malloc(sizeof(item_t));
    if (*item == NULL)
    {
        perror("Memory allocation failure.");
        return 0;
    }

    char codice_str[CODE_STR_LEN];
    char data_str[DATE_STR_LEN];

    int res = sscanf(str, "%5s %s %s %10s %s %s %d",
                     codice_str,
                     (*item)->nome,
                     (*item)->cognome,
                     data_str,
                     (*item)->via,
                     (*item)->citta,
                     &(*item)->cap);

    if (res != 7)
    {
        return 0;
    }

    if (!parse_code((*item)->codice, codice_str))
    {
        return 0;
    }

    if (!parse_date(&(*item)->data_di_nascita, data_str))
    {
        return 0;
    }

    return 1;
}

int add_to_sorted_list(node_t **head, item_t item)
{
    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = item;

    // Check first item

    node_t *current = *head;
    while (comp_date(current->value.data_di_nascita, new_node->value.data_di_nascita))
    {
        current = current->next;
    }

    // Insert node
}

int add_from_file(node_t **head, char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open file.");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        item_t *new_item = malloc(sizeof(item_t));
        if (parse_item(&new_item, line))
        {
            // add_to_sorted_list();
        }
    }
}

int main(int argc, char *argv[])
{
    node_t *head = NULL;
    char path[] = "anag1.txt";

    add_from_file(&head, path);

    return 0;
}
