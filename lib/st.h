#ifndef ST_H
#define ST_H

#include "item.h"

typedef struct symbtab *ST;

ST STinit(int maxN);
int STcount(ST st);
void STinsert(ST st, Item val);
Item STsearch(ST st, Key k);
void STdelete(ST st, Key k);
void STdisplay(ST st);
void STfree(ST st);

#endif