#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "vaga.h"
#include "ordenacao.h"        /* quickSort para preparar entrada ordenada/invertida */
#include "lista_dinamica.h"
#include "lista_estatica.h"

/* declarada em gerar_dados.c */
void gerarVagas(Vaga v[], int n);

/* ================================================================
 *  Tipos de entrada
 * ================================================================ */
typedef enum { ALEATORIO = 0, ORDENADO = 1, INVERTIDO = 2 } TipoEntrada;
static const char *nomeEntrada[] = { "Aleatorio", "Ordenado", "Invertido" };

/* Gera array diferente conforme tipo de entrada */
static void prepararBase(Vaga *src, Vaga *dst, int n, TipoEntrada tipo) {
    memcpy(dst, src, (size_t)n * sizeof(Vaga));
    if (tipo == ORDENADO || tipo == INVERTIDO)
        quickSort(dst, 0, n - 1);          /* ordena crescente por salario */
    if (tipo == INVERTIDO) {
        for (int i = 0, j = n - 1; i < j; i++, j--) {
            Vaga t = dst[i]; dst[i] = dst[j]; dst[j] = t;
        }
    }
}

/* ================================================================
 *  Medicao de tempo  —  retorna segundos
 * ================================================================ */

typedef void (*SortDin)(Lista    *);
typedef void (*SortEst)(ListaEst *);

static double medirDin(SortDin fn, Lista *l) {
    clock_t ini = clock();
    fn(l);
    clock_t fim = clock();
    return (double)(fim - ini) / CLOCKS_PER_SEC * 1000.0; /* ms */
}

static double medirEst(SortEst fn, ListaEst *l) {
    clock_t ini = clock();
    fn(l);
    clock_t fim = clock();
    return (double)(fim - ini) / CLOCKS_PER_SEC * 1000.0; /* ms */
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
    srand(42);   /* semente fixa para reproducibilidade */

    const int VOLUMES[] = { 100, 1000, 10000 };
    /*
     * Repeticoes por volume — algoritmos O(n^2) em lista com N=10000
     * levam ~600 ms cada; 5 sorts x 3 entradas x 5 reps = ~45 s.
     */
    const int REPS[]    = { 50, 20, 5 };
    const int NV = 3, NT = 3;

    g_cmp = cmpSalario;   /* criterio padrao do benchmark */

    FILE *csv = fopen("resultados_listas.csv", "w");
    if (!csv) { fprintf(stderr, "Erro ao criar resultados_listas.csv\n"); return 1; }
    fprintf(csv, "Algoritmo,N,TipoEntrada,Estrutura,TempoMedio_ms,ComparacoesMedia\n");

    for (int v = 0; v < NV; v++) {
        int n    = VOLUMES[v];
        int reps = REPS[v];

        printf("\n===================================================\n");
        printf(" N = %d  (%d repeticoes por cenario)\n", n, reps);
        printf("===================================================\n");

        /* dados base aleatorios */
        Vaga *base = (Vaga *)malloc((size_t)n * sizeof(Vaga));
        if (!base) { fprintf(stderr, "malloc falhou\n"); fclose(csv); return 1; }
        gerarVagas(base, n);

        for (int t = 0; t < NT; t++) {
            TipoEntrada tipo = (TipoEntrada)t;

            /* prepara array ja na ordem desejada */
            Vaga *prep = (Vaga *)malloc((size_t)n * sizeof(Vaga));
            if (!prep) { free(base); fclose(csv); return 1; }
            prepararBase(base, prep, n, tipo);

            printf("\n  [%s]\n", nomeEntrada[t]);
            printf("  %-16s  %12s  %12s  %12s  %12s\n",
                   "Algoritmo", "Din(ms)", "cmp Din", "Est(ms)", "cmp Est");

            for (int s = 0; s < NUM_SORTS; s++) {
                double somaDin = 0.0, somaEst = 0.0;
                long long cmpDin = 0, cmpEst = 0;

                for (int r = 0; r < reps; r++) {
                    /* --- lista dinamica --- */
                    Lista din;
                    inicializarLista(&din);
                    for (int i = 0; i < n; i++) inserirLista(&din, prep[i]);
                    g_comparacoes = 0;
                    somaDin += medirDin(SORTS_DIN[s], &din);
                    if (r == 0) cmpDin = g_comparacoes;
                    liberarLista(&din);

                    /* --- lista estatica --- */
                    ListaEst *est = (ListaEst *)malloc(sizeof(ListaEst));
                    if (!est) { free(prep); free(base); fclose(csv); return 1; }
                    inicializarListaEst(est, n);
                    for (int i = 0; i < n; i++) inserirListaEst(est, prep[i]);
                    g_comparacoes = 0;
                    somaEst += medirEst(SORTS_EST[s], est);
                    if (r == 0) cmpEst = g_comparacoes;
                    destruirListaEst(est);
                    free(est);
                }

                double mDin = somaDin / reps;
                double mEst = somaEst / reps;
                printf("  %-16s  %12.4f  %12lld  %12.4f  %12lld\n",
                       NOMES[s], mDin, cmpDin, mEst, cmpEst);

                fprintf(csv, "%s,%d,%s,Dinamica,%.6f,%lld\n",
                        NOMES[s], n, nomeEntrada[t], mDin, cmpDin);
                fprintf(csv, "%s,%d,%s,Estatica,%.6f,%lld\n",
                        NOMES[s], n, nomeEntrada[t], mEst, cmpEst);
            }

            free(prep);
        }

        free(base);
    }

    fclose(csv);
    printf("\nResultados salvos em resultados_listas.csv\n");
    return 0;
}
