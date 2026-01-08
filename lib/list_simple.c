/* * list_simple.c
 * Implementazione delle operazioni su liste concatenate.
 */

#include "list_simple.h"

// Creazione di un nuovo nodo
link NEW(Item val, link next)
{
    link x = malloc(sizeof *x);
    if (x == NULL)
    {
        printf("Errore di allocazione memoria\n");
        return NULL;
    }
    x->val = val;
    x->next = next;
    return x;
}

// Inizializzazione
link listInit()
{
    return NULL;
}

// Deallocazione completa
void listFree(link h)
{
    link t;
    while (h != NULL)
    {
        t = h->next;
        // Se Item è un puntatore complesso, usare ITEMfree(h->val);
        free(h);
        h = t;
    }
}

/* ==========================================
 * OPERAZIONI ITERATIVE
 * ========================================== */

// Inserimento in testa: O(1)
link listInsHead(link h, Item val)
{
    return NEW(val, h);
}

// Inserimento in coda: O(N) perché bisogna scorrere
link listInsTail(link h, Item val)
{
    link x;
    if (h == NULL)
        return NEW(val, NULL);

    // Scorro fino all'ultimo nodo
    for (x = h; x->next != NULL; x = x->next)
        ;

    x->next = NEW(val, NULL);
    return h;
}

// Inserimento Ordinato (assumendo KEYcmp definita in item.h): O(N)
link listInsSorted(link h, Item val)
{
    link x, p;
    Key k = KEYget(val);

    // Caso 1: Lista vuota o inserimento in testa
    if (h == NULL || KEYcmp(KEYget(h->val), k) > 0)
        return NEW(val, h);

    // Caso 2: Scansione per trovare la posizione
    for (p = h, x = h->next; x != NULL; p = x, x = x->next)
    {
        if (KEYcmp(KEYget(x->val), k) > 0)
            break;
    }

    p->next = NEW(val, x);
    return h;
}

// Cancellazione elemento con chiave k
link listDelKey(link h, Key k)
{
    link x, p;
    if (h == NULL)
        return NULL;

    // Caso: Cancellazione in testa
    if (KEYcmp(KEYget(h->val), k) == 0)
    {
        x = h->next;
        free(h);
        return x; // Ritorna la nuova testa
    }

    // Caso: Cancellazione nel corpo
    for (p = h, x = h->next; x != NULL; p = x, x = x->next)
    {
        if (KEYcmp(KEYget(x->val), k) == 0)
        {
            p->next = x->next;
            free(x);
            break;
        }
    }
    return h;
}

Item listSearch(link h, Key k)
{
    link x;
    for (x = h; x != NULL; x = x->next)
    {
        if (KEYcmp(KEYget(x->val), k) == 0)
            return x->val;
    }
    return NULL; // Non trovato (usare ITEMsetVoid() se previsto)
}

void listDisplay(link h)
{
    link x;
    printf("List: ");
    for (x = h; x != NULL; x = x->next)
    {
        ITEMshow(x->val); // Stampa definita in item.h
        printf(" -> ");
    }
    printf("NULL\n");
}

/* ==========================================
 * OPERAZIONI RICORSIVE
 * (Spesso richieste negli esercizi d'esame)
 * ========================================== */

void listDisplayR(link h)
{
    if (h == NULL)
    {
        printf("NULL\n");
        return;
    }
    ITEMshow(h->val);
    printf(" -> ");
    listDisplayR(h->next);
}

int listCountR(link h)
{
    if (h == NULL)
        return 0;
    return 1 + listCountR(h->next);
}

// Inserimento in coda ricorsivo
link listInsTailR(link h, Item val)
{
    if (h == NULL)
        return NEW(val, NULL);
    h->next = listInsTailR(h->next, val);
    return h;
}

// Cancellazione ricorsiva
link listDelKeyR(link h, Key k)
{
    link x;
    if (h == NULL)
        return NULL;

    if (KEYcmp(KEYget(h->val), k) == 0)
    {
        x = h->next;
        free(h);
        return x;
    }

    h->next = listDelKeyR(h->next, k);
    return h;
}