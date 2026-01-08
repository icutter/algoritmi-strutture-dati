#ifndef QUEUE_H
#define QUEUE_H

#include "item.h"

typedef struct queue *QUEUE;

QUEUE QUEUEinit(int maxN);
int QUEUEempty(QUEUE q);
void QUEUEput(QUEUE q, Item val);
Item QUEUEget(QUEUE q);
void QUEUEfree(QUEUE q);

#endif