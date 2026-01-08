#ifndef STACK_H
#define STACK_H

#include "item.h" // Dipendenza fondamentale

typedef struct stack *STACK;

STACK STACKinit(int maxN);
int STACKempty(STACK s);
void STACKpush(STACK s, Item val);
Item STACKpop(STACK s);
void STACKfree(STACK s);

#endif