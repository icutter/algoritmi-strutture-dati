/* * list_simple.h
 * Libreria per la gestione di Liste Concatenate Semplici (Single Linked Lists).
 * Riferimento: 02 Liste.pdf
 *
 * NOTA: Questa libreria espone la struttura del nodo (non è un ADT opaco),
 * permettendo la manipolazione diretta tipica degli esercizi didattici.
 */

#ifndef LIST_SIMPLE_H
#define LIST_SIMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include "item.h" // Assumiamo la presenza della libreria Item

// Definizione del nodo e del puntatore a nodo
typedef struct node *link;

struct node
{
    Item val;
    link next;
};

/* --- GESTIONE BASE --- */
link NEW(Item val, link next); // Crea un nuovo nodo
link listInit();               // Ritorna NULL (lista vuota)
void listFree(link h);         // Dealloca l'intera lista

/* --- INSERIMENTO --- */
link listInsHead(link h, Item val);   // Inserimento in testa (O(1))
link listInsTail(link h, Item val);   // Inserimento in coda (O(N))
link listInsSorted(link h, Item val); // Inserimento ordinato (O(N))

/* --- CANCELLAZIONE --- */
link listDelHead(link h);       // Cancella la testa
link listDelKey(link h, Key k); // Cancella per chiave (prima occorrenza)

/* --- RICERCA E VISUALIZZAZIONE --- */
Item listSearch(link h, Key k); // Ricerca elemento (ritorna Item o NULL)
void listDisplay(link h);       // Stampa la lista

/* --- VARIANTI RICORSIVE (Tipiche del corso) --- */
void listDisplayR(link h);           // Stampa ricorsiva
int listCountR(link h);              // Conteggio nodi ricorsivo
link listInsTailR(link h, Item val); // Inserimento in coda ricorsivo
link listDelKeyR(link h, Key k);     // Cancellazione ricorsiva

#endif