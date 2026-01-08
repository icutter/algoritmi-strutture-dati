#include "stack.h"

typedef struct stack_node *link;
struct stack_node
{
    Item val;
    link next;
}; // Nodo locale specifico
struct stack
{
    link head;
};

static link NEW(Item val, link next)
{
    link x = malloc(sizeof *x);
    if (x == NULL)
        return NULL;
    x->val = val;
    x->next = next;
    return x;
}

STACK STACKinit(int maxN)
{
    STACK s = malloc(sizeof *s);
    s->head = NULL;
    return s;
}

int STACKempty(STACK s) { return s->head == NULL; }

void STACKpush(STACK s, Item val)
{
    s->head = NEW(val, s->head);
}

Item STACKpop(STACK s)
{
    if (STACKempty(s))
        return NULL;
    Item tmp = s->head->val;
    link t = s->head->next;
    free(s->head);
    s->head = t;
    return tmp;
}

void STACKfree(STACK s)
{
    while (!STACKempty(s))
        STACKpop(s);
    free(s);
}