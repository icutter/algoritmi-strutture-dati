#include "st.h"

typedef struct st_node *link;
struct st_node
{
    Item val;
    link next;
};
struct symbtab
{
    link head;
    int size;
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

ST STinit(int maxN)
{
    ST st = malloc(sizeof *st);
    st->head = NULL;
    st->size = 0;
    return st;
}

int STcount(ST st) { return st->size; }

void STinsert(ST st, Item val)
{
    st->head = NEW(val, st->head);
    st->size++;
}

Item STsearch(ST st, Key k)
{
    for (link x = st->head; x != NULL; x = x->next)
        if (KEYcmp(KEYget(x->val), k) == 0)
            return x->val;
    return NULL;
}

void STdelete(ST st, Key k)
{
    link x, p;
    if (st == NULL || st->head == NULL)
        return;
    for (x = st->head, p = NULL; x != NULL; p = x, x = x->next)
    {
        if (KEYcmp(k, KEYget(x->val)) == 0)
        {
            if (x == st->head)
                st->head = x->next;
            else
                p->next = x->next;
            free(x);
            st->size--;
            return;
        }
    }
}

void STdisplay(ST st)
{
    for (link x = st->head; x != NULL; x = x->next)
        ITEMshow(x->val);
}

void STfree(ST st)
{
    link x, t;
    for (x = st->head; x != NULL; x = t)
    {
        t = x->next;
        free(x);
    }
    free(st);
}