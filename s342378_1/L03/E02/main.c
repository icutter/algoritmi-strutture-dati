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

typedef enum
{
    r_add_record,
    r_add_from_file,
    r_search_code,
    r_remove_code,
    r_remove_date_range,
    r_write_to_file,
    r_print_list,
    r_help,
    r_quit,
    r_err,
} cmd_e;

int are_equal(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

void print_commands()
{
    const char tab[] = "  ";
    printf("%sadd_record\t\t<code> <name> <surname> <birth_date> <address> <city> <cap>\n", tab);
    printf("%sadd_from_file\t\t<path>\n", tab);
    printf("%ssearch_code\t\t<code>\n", tab);
    printf("%sremove_code\t\t<code>\n", tab);
    printf("%sremove_date_range\t<start_date> <end_date>\n", tab);
    printf("%swrite_to_file\t\t<path>\n", tab);
    printf("%sprint_list\n", tab);
    printf("%squit\n", tab);
}

cmd_e read_command(char *str)
{
    if (are_equal(str, "add_record"))
        return r_add_record;
    if (are_equal(str, "add_from_file"))
        return r_add_from_file;
    if (are_equal(str, "search_code"))
        return r_search_code;
    if (are_equal(str, "remove_code"))
        return r_remove_code;
    if (are_equal(str, "remove_date_range"))
        return r_remove_date_range;
    if (are_equal(str, "write_to_file"))
        return r_write_to_file;
    if (are_equal(str, "print_list"))
        return r_print_list;
    if (are_equal(str, "help"))
        return r_help;
    if (are_equal(str, "quit"))
        return r_quit;

    return r_err;
}

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
    *head = NULL;
}

void print_item(item_t item)
{
    char date_str[11];
    format_date(date_str, item.data_di_nascita);
    printf("%s %s %s %s %s %s %d\n",
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

    return 1;
}

int add_record(node_t **head, char *record)
{
    item_t new_item;
    if (parse_item(&new_item, record))
    {
        add_to_sorted_list(head, new_item);
        printf("Record added successfully.\n");
        return 1;
    }
    printf("Failed to add record.\n");
    return 0;
}

int add_from_file(node_t **head, char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL)
    {
        printf("Could not open file.\n");
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
    return 1;
}

int search_by_code(node_t *head, char *code)
{
    node_t *current = head;
    while (current != NULL)
    {
        if (strcmp(current->value.codice, code) == 0)
        {
            print_item(current->value);
            return 1;
        }
        current = current->next;
    }

    return 0;
}

int remove_code(node_t **head, char *code)
{
    node_t *current = *head;
    node_t *prev = NULL;
    while (current)
    {
        if (strcmp(current->value.codice, code) == 0)
        {
            if (prev == NULL)
                *head = current->next;
            else
                prev->next = current->next;
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

int remove_date_range(node_t **head, char *from, char *to)
{
    date_t dfrom, dto;
    if (!parse_date(&dfrom, from) || !parse_date(&dto, to))
    {
        printf("Invalid date range format.\n");
        return 0;
    }

    node_t *current = *head;
    node_t *prev = NULL;
    int removed = 0;

    while (current)
    {
        int cmpFrom = comp_date(current->value.data_di_nascita, dfrom);
        int cmpTo = comp_date(current->value.data_di_nascita, dto);
        if (cmpFrom >= 0 && cmpTo <= 0)
        {
            node_t *toFree = current;
            if (prev == NULL)
            {
                *head = current->next;
                current = *head;
            }
            else
            {
                prev->next = current->next;
                current = prev->next;
            }
            free(toFree);
            removed++;
            continue;
        }
        prev = current;
        current = current->next;
    }

    return removed;
}

int write_to_file(node_t *head, char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp)
    {
        printf("Could not open file for writing: %s\n", path);
        return 0;
    }
    node_t *current = head;
    char date_str[DATE_STR_LEN];
    while (current)
    {
        format_date(date_str, current->value.data_di_nascita);
        /* CODE NOME COGNOME DD/MM/YYYY VIA CITTA CAP\n */
        fprintf(fp, "%s %s %s %s %s %s %d\n",
                current->value.codice,
                current->value.nome,
                current->value.cognome,
                date_str,
                current->value.via,
                current->value.citta,
                current->value.cap);
        current = current->next;
    }
    fclose(fp);
    return 1;
}

void try_command(node_t **head, cmd_e cmd, char *inputstr)
{
    char args[2][51];

    switch (cmd)
    {
    case r_add_record:
        add_record(head, inputstr + strlen("add_record "));
        return;
    case r_add_from_file:
        sscanf(inputstr, "%*s %50s", args[0]);
        add_from_file(head, args[0]);
        return;
    case r_search_code:
        sscanf(inputstr, "%*s %5s", args[0]);
        search_by_code(*head, args[0]);
        return;
    case r_remove_code:
        sscanf(inputstr, "%*s %5s", args[0]);
        remove_code(head, args[0]);
        return;
    case r_remove_date_range:
        sscanf(inputstr, "%*s %10s %10s", args[0], args[1]);
        remove_date_range(head, args[0], args[1]);
        return;
    case r_write_to_file:
        sscanf(inputstr, "%*s %50s", args[0]);
        write_to_file(*head, args[0]);
        return;
    case r_print_list:
        print_list(*head);
        return;
    case r_help:
        print_commands();
        return;
    case r_quit:
        clear_list(head);
        exit(0);
        return;
    case r_err:
        printf("Command was not recognized.\n");

        return;
    }
}

int main(int argc, char *argv[])
{
    node_t *head = NULL;

    char buffer[128];

    printf("Type a command or \"help\" to get the list of available commands.\n");

    while (1)
    {
        printf("\n$ ");
        char inputstr[512];
        if (!fgets(inputstr, sizeof(inputstr), stdin))
            break;

        printf("\n");

        // remove trailing newline
        size_t L = strlen(inputstr);
        if (L > 0 && inputstr[L - 1] == '\n')
            inputstr[L - 1] = '\0';

        char cmdstr[64] = {0};
        sscanf(inputstr, "%63s", cmdstr);

        cmd_e cmd = read_command(cmdstr);
        try_command(&head, cmd, inputstr);
    }

    return 0;
}
