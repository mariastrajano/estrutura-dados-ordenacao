#ifndef LISTA_ESTATICA_H
#define LISTA_ESTATICA_H

#include "vaga.h"

/* -----------------------------------------------------------------
 *  Capacidade maxima da lista estatica.
 *  Altere este valor conforme necessario.
 * ----------------------------------------------------------------- */
#define MAX_ESTATICA 10001

/* -----------------------------------------------------------------
 *  Estruturas
 *
 *  Os nos sao guardados num array pre-alocado.  O campo "prox"
 *  e um indice no array (-1 significa "sem proximo").
 * ----------------------------------------------------------------- */

typedef struct {
    Vaga vaga;
    int  prox;   /* indice do proximo no; -1 = fim */
} NoEst;

typedef struct {
    NoEst nos[MAX_ESTATICA]; /* array de nos             */
    int   inicio;            /* indice da cabeca; -1 vazia */
    int   livre;             /* cabeca da lista de livres  */
    int   tamanho;
} ListaEst;

/* -----------------------------------------------------------------
 *  Operacoes basicas
 * ----------------------------------------------------------------- */

void inicializarListaEst (ListaEst *l);
void inserirListaEst     (ListaEst *l, Vaga v);
void imprimirListaEst    (ListaEst *l);
void copiarListaEst      (ListaEst *destino, ListaEst *origem);

/* -----------------------------------------------------------------
 *  Algoritmos de ordenacao (ordenam por salario crescente)
 * ----------------------------------------------------------------- */

void bubbleSortListaEst    (ListaEst *l);
void selectionSortListaEst (ListaEst *l);
void insertionSortListaEst (ListaEst *l);
void quickSortListaEst     (ListaEst *l);
void mergeSortListaEst     (ListaEst *l);

#endif
