#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "vaga.h"
#include "lista_dinamica.h"
#include "lista_estatica.h"

/* declarada em gerar_dados.c */
void gerarVagas(Vaga v[], int n);

/* ================================================================
 *  Medicao de tempo  —  retorna segundos
 * ================================================================ */

typedef void (*SortDin)(Lista    *);
typedef void (*SortEst)(ListaEst *);

static double medirDin(SortDin fn, Lista *l) {
    clock_t ini = clock();
    fn(l);
    clock_t fim = clock();
    return (double)(fim - ini) / CLOCKS_PER_SEC;
}

static double medirEst(SortEst fn, ListaEst *l) {
    clock_t ini = clock();
    fn(l);
    clock_t fim = clock();
    return (double)(fim - ini) / CLOCKS_PER_SEC;
}

/* ================================================================
 *  Populacao das listas a partir de um array de Vagas
 * ================================================================ */

static void popularDin(Lista *l, Vaga v[], int n) {
    inicializarLista(l);
    for (int i = 0; i < n; i++) inserirLista(l, v[i]);
}

static void popularEst(ListaEst *l, Vaga v[], int n) {
    inicializarListaEst(l, n);
    for (int i = 0; i < n; i++) inserirListaEst(l, v[i]);
}

/* ================================================================
 *  Benchmark
 * ================================================================ */

static const char *NOMES[] = {
    "Bubble Sort",
    "Selection Sort",
    "Insertion Sort",
    "Quick Sort",
    "Merge Sort"
};

static SortDin SORTS_DIN[] = {
    bubbleSortLista,
    selectionSortLista,
    insertionSortLista,
    quickSortLista,
    mergeSortLista
};

static SortEst SORTS_EST[] = {
    bubbleSortListaEst,
    selectionSortListaEst,
    insertionSortListaEst,
    quickSortListaEst,
    mergeSortListaEst
};

#define NUM_SORTS 5

int main(void) {
    srand((unsigned)time(NULL));

    int tamanhos[] = {100, 1000, 10000};
    int numTam     = 3;

    for (int t = 0; t < numTam; t++) {
        int n = tamanhos[t];

        printf("\n======================================\n");
        printf(" TESTE COM %d ELEMENTOS\n", n);
        printf("======================================\n");
        printf("%-16s  %-10s  %-10s\n", "Algoritmo", "Dinamica", "Estatica");
        printf("%-16s  %-10s  %-10s\n", "---------", "--------", "--------");

        /* gera dados originais uma vez so */
        Vaga *original = (Vaga *)malloc(n * sizeof(Vaga));
        if (!original) { fprintf(stderr, "malloc falhou\n"); return 1; }
        gerarVagas(original, n);

        for (int s = 0; s < NUM_SORTS; s++) {

            /* --- lista dinamica --- */
            Lista din;
            popularDin(&din, original, n);
            double tempoDin = medirDin(SORTS_DIN[s], &din);
            liberarLista(&din);

            /* --- lista estatica --- */
            ListaEst *est = (ListaEst *)malloc(sizeof(ListaEst));
            if (!est) { fprintf(stderr, "malloc falhou\n"); free(original); return 1; }
            popularEst(est, original, n);
            double tempoEst = medirEst(SORTS_EST[s], est);
            destruirListaEst(est);
            free(est);

            printf("%-16s  %8.6f s  %8.6f s\n",
                   NOMES[s], tempoDin, tempoEst);
        }

        free(original);
    }

    return 0;
}
