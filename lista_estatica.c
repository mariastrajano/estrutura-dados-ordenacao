#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista_estatica.h"

/* =================================================================
 *  Operacoes basicas
 * ================================================================= */

void inicializarListaEst(ListaEst *l) {
    l->inicio   = -1;
    l->tamanho  = 0;
    /* encadeia todos os slots como lista de nos livres */
    for (int i = 0; i < MAX_ESTATICA - 1; i++)
        l->nos[i].prox = i + 1;
    l->nos[MAX_ESTATICA - 1].prox = -1;
    l->livre = 0;
}

/* aloca um slot da lista de livres; retorna indice ou -1 se cheio */
static int alocarSlot(ListaEst *l) {
    if (l->livre == -1) return -1;
    int idx   = l->livre;
    l->livre  = l->nos[idx].prox;
    return idx;
}

void inserirListaEst(ListaEst *l, Vaga v) {
    if (l->tamanho >= MAX_ESTATICA) return;
    int idx = alocarSlot(l);
    if (idx == -1) return;

    l->nos[idx].vaga = v;
    l->nos[idx].prox = -1;

    if (l->inicio == -1) {
        l->inicio = idx;
    } else {
        /* percorre ate o fim */
        int cur = l->inicio;
        while (l->nos[cur].prox != -1) cur = l->nos[cur].prox;
        l->nos[cur].prox = idx;
    }
    l->tamanho++;
}

void imprimirListaEst(ListaEst *l) {
    int cur = l->inicio;
    int i   = 1;
    while (cur != -1) {
        Vaga *v = &l->nos[cur].vaga;
        printf("  %d. %-20s  salario=%.0f  distancia=%.1f  avaliacao=%.1f\n",
               i++, v->empresa, v->salario, v->distancia, v->avaliacao);
        cur = l->nos[cur].prox;
    }
}

void copiarListaEst(ListaEst *destino, ListaEst *origem) {
    memcpy(destino, origem, sizeof(ListaEst));
}

/* =================================================================
 *  Auxiliares internos
 * ================================================================= */

/* coleta indices dos nos (em ordem de lista) */
static int *coletarIndices(ListaEst *l) {
    int *idx = (int *)malloc(l->tamanho * sizeof(int));
    if (!idx) return NULL;
    int cur = l->inicio;
    for (int i = 0; i < l->tamanho; i++) {
        idx[i] = cur;
        cur    = l->nos[cur].prox;
    }
    return idx;
}

static void trocarEst(ListaEst *l, int a, int b) {
    Vaga tmp       = l->nos[a].vaga;
    l->nos[a].vaga = l->nos[b].vaga;
    l->nos[b].vaga = tmp;
}

/* coleta os dados dos nos num array temporario */
static Vaga *coletarArrayEst(ListaEst *l) {
    Vaga *arr = (Vaga *)malloc(l->tamanho * sizeof(Vaga));
    if (!arr) return NULL;
    int cur = l->inicio;
    for (int i = 0; i < l->tamanho; i++) {
        arr[i] = l->nos[cur].vaga;
        cur    = l->nos[cur].prox;
    }
    return arr;
}

/* escreve de volta o array nos nos (em ordem de lista) */
static void escreverArrayEst(ListaEst *l, Vaga *arr) {
    int cur = l->inicio;
    for (int i = 0; i < l->tamanho; i++) {
        l->nos[cur].vaga = arr[i];
        cur = l->nos[cur].prox;
    }
}

/* =================================================================
 *  Bubble Sort  —  troca dados via indices adjacentes
 * ================================================================= */

void bubbleSortListaEst(ListaEst *l) {
    if (l->tamanho <= 1) return;
    int *idx  = coletarIndices(l);
    int  n    = l->tamanho;
    int trocou;
    do {
        trocou = 0;
        for (int i = 0; i < n - 1; i++) {
            if (l->nos[idx[i]].vaga.salario > l->nos[idx[i+1]].vaga.salario) {
                trocarEst(l, idx[i], idx[i+1]);
                trocou = 1;
            }
        }
        n--;
    } while (trocou);
    free(idx);
}

/* =================================================================
 *  Selection Sort  —  encontra minimo e troca
 * ================================================================= */

void selectionSortListaEst(ListaEst *l) {
    if (l->tamanho <= 1) return;
    int *idx = coletarIndices(l);
    int  n   = l->tamanho;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (l->nos[idx[j]].vaga.salario < l->nos[idx[min]].vaga.salario)
                min = j;
        if (min != i) trocarEst(l, idx[i], idx[min]);
    }
    free(idx);
}

/* =================================================================
 *  Insertion Sort  —  insere cada elemento na posicao certa
 * ================================================================= */

void insertionSortListaEst(ListaEst *l) {
    if (l->tamanho <= 1) return;
    int   n   = l->tamanho;
    Vaga *arr = coletarArrayEst(l);
    if (!arr) return;
    for (int i = 1; i < n; i++) {
        Vaga chave = arr[i];
        int  j     = i - 1;
        while (j >= 0 && arr[j].salario > chave.salario) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = chave;
    }
    escreverArrayEst(l, arr);
    free(arr);
}

/* =================================================================
 *  Quick Sort  —  particao de Lomuto sobre array de dados
 * ================================================================= */

static void trocarArr(Vaga *a, Vaga *b) { Vaga t = *a; *a = *b; *b = t; }

static void quickArr(Vaga *v, int ini, int fim) {
    if (ini >= fim) return;
    float pivo = v[fim].salario;
    int i = ini - 1;
    for (int j = ini; j < fim; j++)
        if (v[j].salario <= pivo) { i++; trocarArr(&v[i], &v[j]); }
    trocarArr(&v[i+1], &v[fim]);
    int p = i + 1;
    quickArr(v, ini, p - 1);
    quickArr(v, p + 1, fim);
}

void quickSortListaEst(ListaEst *l) {
    if (l->tamanho <= 1) return;
    Vaga *arr = coletarArrayEst(l);
    if (!arr) return;
    quickArr(arr, 0, l->tamanho - 1);
    escreverArrayEst(l, arr);
    free(arr);
}

/* =================================================================
 *  Merge Sort  —  divide e conquista sobre array de dados
 * ================================================================= */

static void mergeArr(Vaga *v, int ini, int meio, int fim) {
    int   n1 = meio - ini + 1, n2 = fim - meio;
    Vaga *L  = (Vaga *)malloc(n1 * sizeof(Vaga));
    Vaga *R  = (Vaga *)malloc(n2 * sizeof(Vaga));
    if (!L || !R) { free(L); free(R); return; }
    for (int i = 0; i < n1; i++) L[i] = v[ini + i];
    for (int j = 0; j < n2; j++) R[j] = v[meio + 1 + j];
    int i = 0, j = 0, k = ini;
    while (i < n1 && j < n2)
        v[k++] = (L[i].salario <= R[j].salario) ? L[i++] : R[j++];
    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];
    free(L); free(R);
}

static void mergeSortArr(Vaga *v, int ini, int fim) {
    if (ini < fim) {
        int meio = (ini + fim) / 2;
        mergeSortArr(v, ini, meio);
        mergeSortArr(v, meio + 1, fim);
        mergeArr(v, ini, meio, fim);
    }
}

void mergeSortListaEst(ListaEst *l) {
    if (l->tamanho <= 1) return;
    Vaga *arr = coletarArrayEst(l);
    if (!arr) return;
    mergeSortArr(arr, 0, l->tamanho - 1);
    escreverArrayEst(l, arr);
    free(arr);
}
