#include "pq.h"

typedef struct pq_node *link;
struct pq_node
{
    Item val;
    link next;
};
struct pqueue
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

PQ PQinit(int maxN)
{
    PQ pq = malloc(sizeof *pq);
    pq->head = NULL;
    return pq;
}

int PQempty(PQ pq) { return pq->head == NULL; }

void PQinsert(PQ pq, Item val)
{
    link x, p;
    Key k = KEYget(val);
    if (pq->head == NULL || KEYcmp(KEYget(pq->head->val), k) < 0)
    {
        pq->head = NEW(val, pq->head);
        return;
    }
    for (x = pq->head->next, p = pq->head;
         x != NULL && KEYcmp(k, KEYget(x->val)) < 0;
         p = x, x = x->next)
        ;
    p->next = NEW(val, x);
}

Item PQextractMax(PQ pq)
{
    if (PQempty(pq))
        return NULL;
    Item tmp = pq->head->val;
    link t = pq->head->next;
    free(pq->head);
    pq->head = t;
    return tmp;
}

void PQdisplay(PQ pq)
{
    for (link x = pq->head; x != NULL; x = x->next)
        ITEMshow(x->val);
}

void PQfree(PQ pq)
{
    while (!PQempty(pq))
        PQextractMax(pq);
    free(pq);
}