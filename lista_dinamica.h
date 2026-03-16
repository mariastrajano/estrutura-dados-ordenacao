#ifndef LISTA_DINAMICA_H
#define LISTA_DINAMICA_H

#include "vaga.h"

/* -----------------------------------------------------------------
 *  Estruturas
 * ----------------------------------------------------------------- */

typedef struct No {
    Vaga        vaga;
    struct No  *prox;
} No;

typedef struct {
    No  *inicio;
    int  tamanho;
} Lista;

/* -----------------------------------------------------------------
 *  Operacoes basicas
 * ----------------------------------------------------------------- */

void inicializarLista (Lista *l);
void inserirLista     (Lista *l, Vaga v);
void imprimirLista    (Lista *l);
void liberarLista     (Lista *l);
void copiarLista      (Lista *destino, Lista *origem);

/* -----------------------------------------------------------------
 *  Algoritmos de ordenacao (ordenam por salario crescente)
 * ----------------------------------------------------------------- */

void bubbleSortLista    (Lista *l);
void selectionSortLista (Lista *l);
void insertionSortLista (Lista *l);
void quickSortLista     (Lista *l);
void mergeSortLista     (Lista *l);

#endif
