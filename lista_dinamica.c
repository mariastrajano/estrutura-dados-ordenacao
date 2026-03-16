#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_dinamica.h"

/* conta a comparacao e usa o criterio ativo */
#define CMP(a, b) (g_comparacoes++, g_cmp((a), (b)))

/* =================================================================
 *  Operacoes basicas
 * ================================================================= */

void inicializarLista(Lista *l) {
    l->inicio   = NULL;
    l->tamanho  = 0;
}

void inserirLista(Lista *l, Vaga v) {
    No *novo = (No *)malloc(sizeof(No));
    if (!novo) return;
    novo->vaga = v;
    novo->prox = NULL;

    if (!l->inicio) {
        l->inicio = novo;
    } else {
        /* percorre ate o fim para inserir no final */
        No *cur = l->inicio;
        while (cur->prox) cur = cur->prox;
        cur->prox = novo;
    }
    l->tamanho++;
}

void imprimirLista(Lista *l) {
    No *cur = l->inicio;
    int i = 1;
    while (cur) {
        printf("  %d. %-20s  salario=%.0f  distancia=%.1f  avaliacao=%.1f\n",
               i++, cur->vaga.empresa, cur->vaga.salario,
               cur->vaga.distancia, cur->vaga.avaliacao);
        cur = cur->prox;
    }
}

void liberarLista(Lista *l) {
    No *cur = l->inicio;
    while (cur) {
        No *tmp = cur->prox;
        free(cur);
        cur = tmp;
    }
    l->inicio  = NULL;
    l->tamanho = 0;
}

void copiarLista(Lista *destino, Lista *origem) {
    inicializarLista(destino);
    No *cur = origem->inicio;
    while (cur) {
        inserirLista(destino, cur->vaga);
        cur = cur->prox;
    }
}

Vaga *buscarLista(Lista *l, const char *empresa) {
    No *cur = l->inicio;
    while (cur) {
        if (strcmp(cur->vaga.empresa, empresa) == 0)
            return &cur->vaga;
        cur = cur->prox;
    }
    return NULL;
}

int removerLista(Lista *l, const char *empresa) {
    No *ant = NULL, *cur = l->inicio;
    while (cur) {
        if (strcmp(cur->vaga.empresa, empresa) == 0) {
            if (ant) ant->prox = cur->prox;
            else     l->inicio = cur->prox;
            free(cur);
            l->tamanho--;
            return 1;
        }
        ant = cur;
        cur = cur->prox;
    }
    return 0;
}

/* =================================================================
 *  Auxiliares internos
 * ================================================================= */

/* coleta os dados dos nos num array temporario */
static Vaga *coletarArray(Lista *l) {
    Vaga *arr = (Vaga *)malloc(l->tamanho * sizeof(Vaga));
    if (!arr) return NULL;
    No *cur = l->inicio;
    for (int i = 0; i < l->tamanho; i++, cur = cur->prox)
        arr[i] = cur->vaga;
    return arr;
}

/* escreve os dados do array de volta nos nos */
static void escreverArray(Lista *l, Vaga *arr) {
    No *cur = l->inicio;
    for (int i = 0; i < l->tamanho; i++, cur = cur->prox)
        cur->vaga = arr[i];
}

static void trocar(Vaga *a, Vaga *b) {
    Vaga tmp = *a; *a = *b; *b = tmp;
}

/* =================================================================
 *  Bubble Sort  —  O(n²), troca dados de nos adjacentes
 * ================================================================= */

void bubbleSortLista(Lista *l) {
    if (!l->inicio || !l->inicio->prox) return;
    int trocou;
    do {
        trocou = 0;
        No *p = l->inicio;
        while (p->prox) {
            if (CMP(&p->vaga, &p->prox->vaga) > 0) {
                trocar(&p->vaga, &p->prox->vaga);
                trocou = 1;
            }
            p = p->prox;
        }
    } while (trocou);
}

/* =================================================================
 *  Selection Sort  —  O(n²), encontra o minimo e troca
 * ================================================================= */

void selectionSortLista(Lista *l) {
    for (No *i = l->inicio; i; i = i->prox) {
        No *min = i;
        for (No *j = i->prox; j; j = j->prox)
            if (CMP(&j->vaga, &min->vaga) < 0) min = j;
        if (min != i) trocar(&i->vaga, &min->vaga);
    }
}

/* =================================================================
 *  Insertion Sort  —  O(n²), insere cada elemento na posicao certa
 * ================================================================= */

void insertionSortLista(Lista *l) {
    if (!l->inicio || l->tamanho <= 1) return;

    int   n   = l->tamanho;
    Vaga *arr = coletarArray(l);
    if (!arr) return;

    for (int i = 1; i < n; i++) {
        Vaga chave = arr[i];
        int  j     = i - 1;
        while (j >= 0 && CMP(&arr[j], &chave) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = chave;
    }

    escreverArray(l, arr);
    free(arr);
}

/* =================================================================
 *  Quick Sort  —  O(n log n) medio, particao de Lomuto
 * ================================================================= */

/* coloca a mediana de v[ini], v[meio], v[fim] em v[fim] (pivo Lomuto) */
static void mediana3(Vaga *v, int ini, int fim) {
    int meio = ini + (fim - ini) / 2;
    if (CMP(&v[ini], &v[meio]) > 0) trocar(&v[ini], &v[meio]);
    if (CMP(&v[ini], &v[fim])  > 0) trocar(&v[ini], &v[fim]);
    if (CMP(&v[meio], &v[fim]) > 0) trocar(&v[meio], &v[fim]);
    trocar(&v[meio], &v[fim]); /* mediana -> fim */
}

static void quickSortArr(Vaga *v, int ini, int fim) {
    if (ini >= fim) return;
    mediana3(v, ini, fim);
    Vaga  pivo = v[fim];
    int   i    = ini - 1;
    for (int j = ini; j < fim; j++) {
        if (CMP(&v[j], &pivo) <= 0) {
            i++;
            trocar(&v[i], &v[j]);
        }
    }
    trocar(&v[i + 1], &v[fim]);
    int p = i + 1;
    quickSortArr(v, ini, p - 1);
    quickSortArr(v, p + 1, fim);
}

void quickSortLista(Lista *l) {
    if (!l->inicio || l->tamanho <= 1) return;
    Vaga *arr = coletarArray(l);
    if (!arr) return;
    quickSortArr(arr, 0, l->tamanho - 1);
    escreverArray(l, arr);
    free(arr);
}

/* =================================================================
 *  Merge Sort  —  O(n log n), divide e conquista
 * ================================================================= */

static void merge(Vaga *v, int ini, int meio, int fim) {
    int   n1 = meio - ini + 1;
    int   n2 = fim  - meio;
    Vaga *L  = (Vaga *)malloc(n1 * sizeof(Vaga));
    Vaga *R  = (Vaga *)malloc(n2 * sizeof(Vaga));
    if (!L || !R) { free(L); free(R); return; }

    for (int i = 0; i < n1; i++) L[i] = v[ini  + i];
    for (int j = 0; j < n2; j++) R[j] = v[meio + 1 + j];

    int i = 0, j = 0, k = ini;
    while (i < n1 && j < n2)
        v[k++] = (CMP(&L[i], &R[j]) <= 0) ? L[i++] : R[j++];
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];

    free(L);
    free(R);
}

static void mergeSortArr(Vaga *v, int ini, int fim) {
    if (ini < fim) {
        int meio = (ini + fim) / 2;
        mergeSortArr(v, ini, meio);
        mergeSortArr(v, meio + 1, fim);
        merge(v, ini, meio, fim);
    }
}

void mergeSortLista(Lista *l) {
    if (!l->inicio || l->tamanho <= 1) return;
    Vaga *arr = coletarArray(l);
    if (!arr) return;
    mergeSortArr(arr, 0, l->tamanho - 1);
    escreverArray(l, arr);
    free(arr);
}
