#ifndef LISTA_ESTATICA_H
#define LISTA_ESTATICA_H

#include "vaga.h"

/* -----------------------------------------------------------------
 *  Estruturas
 *
 *  Os nos sao guardados num array pre-alocado via malloc.  O campo
 *  "prox" e um indice no array (-1 significa "sem proximo").
 * ----------------------------------------------------------------- */

typedef struct {
    Vaga vaga;
    int  prox;   /* indice do proximo no; -1 = fim */
} NoEst;

typedef struct {
    NoEst *nos;       /* array de nos alocado dinamicamente */
    int    capacidade;
    int    inicio;    /* indice da cabeca; -1 = vazia */
    int    livre;     /* cabeca da lista de livres    */
    int    tamanho;
} ListaEst;

/* -----------------------------------------------------------------
 *  Operacoes basicas
 * ----------------------------------------------------------------- */

/* capacidade: numero maximo de elementos que a lista suporta */
void inicializarListaEst (ListaEst *l, int capacidade);
void destruirListaEst    (ListaEst *l);
void inserirListaEst     (ListaEst *l, Vaga v);
void imprimirListaEst    (ListaEst *l);
void copiarListaEst      (ListaEst *destino, ListaEst *origem);
Vaga *buscarListaEst     (ListaEst *l, const char *empresa);  /* NULL se nao encontrado */
int   removerListaEst    (ListaEst *l, const char *empresa);  /* 1 se removido, 0 se nao encontrado */

/* -----------------------------------------------------------------
 *  Algoritmos de ordenacao (ordenam por salario crescente)
 * ----------------------------------------------------------------- */

void bubbleSortListaEst    (ListaEst *l);
void selectionSortListaEst (ListaEst *l);
void insertionSortListaEst (ListaEst *l);
void quickSortListaEst     (ListaEst *l);
void mergeSortListaEst     (ListaEst *l);

#endif
