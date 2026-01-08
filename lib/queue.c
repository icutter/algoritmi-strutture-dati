#include "queue.h"

typedef struct queue_node *link;
struct queue_node
{
    Item val;
    link next;
};
struct queue
{
    link head;
    link tail;
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

QUEUE QUEUEinit(int maxN)
{
    QUEUE q = malloc(sizeof *q);
    q->head = q->tail = NULL;
    return q;
}

int QUEUEempty(QUEUE q) { return q->head == NULL; }

void QUEUEput(QUEUE q, Item val)
{
    if (q->head == NULL)
    {
        q->tail = NEW(val, q->head);
        q->head = q->tail;
        return;
    }
    q->tail->next = NEW(val, q->tail->next);
    q->tail = q->tail->next;
}

Item QUEUEget(QUEUE q)
{
    if (QUEUEempty(q))
        return NULL;
    Item tmp = q->head->val;
    link t = q->head->next;
    free(q->head);
    q->head = t;
    if (q->head == NULL)
        q->tail = NULL;
    return tmp;
}

void QUEUEfree(QUEUE q)
{
    while (!QUEUEempty(q))
        QUEUEget(q);
    free(q);
}