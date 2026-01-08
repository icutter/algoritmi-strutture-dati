#ifndef PQ_H
#define PQ_H

#include "item.h"

typedef struct pqueue *PQ;

PQ PQinit(int maxN);
int PQempty(PQ pq);
void PQinsert(PQ pq, Item val);
Item PQextractMax(PQ pq);
void PQdisplay(PQ pq);
void PQfree(PQ pq);

#endif