/*
 * testes_ordenacao.c
 *
 * Benchmark dos 5 algoritmos sobre arrays de Vaga.
 *
 * Entradas testadas : Aleatorio | Ordenado | Invertido
 * Volumes           : 100 | 1000 | 10000
 * Repeticoes        : 100 (N=100), 50 (N=1000), 10 (N=10000)
 *   — reducao justificada: algoritmos O(n²) tornam-se proibitivos
 *     com 100 reps para N=10000 (Bubble demoraria ~60s).
 *
 * Saida: resultados.csv  (para grafico.py)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ordenacao.h"
#include "vaga.h"

void gerarVagas(Vaga v[], int n);

/* ------------------------------------------------------------------ */

static void copiar(Vaga src[], Vaga dst[], int n) {
    for (int i = 0; i < n; i++) dst[i] = src[i];
}

/* ordena ascending por salario (usa quickSort ja implementado) */
static void ordenarAsc(Vaga v[], int n) { quickSort(v, 0, n - 1); }

/* inverte o array in-place */
static void inverter(Vaga v[], int n) {
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        Vaga tmp = v[i]; v[i] = v[j]; v[j] = tmp;
    }
}

typedef enum { ALEATORIO = 0, ORDENADO = 1, INVERTIDO = 2 } TipoEntrada;
static const char *nomeEntrada[] = { "Aleatorio", "Ordenado", "Invertido" };

/* prepara o array conforme o tipo de entrada */
static void prepararEntrada(Vaga base[], Vaga dst[], int n, TipoEntrada tipo) {
    copiar(base, dst, n);
    if (tipo == ORDENADO)   ordenarAsc(dst, n);
    if (tipo == INVERTIDO) { ordenarAsc(dst, n); inverter(dst, n); }
}

/* mede tempo de um sort em ms */
typedef void (*FnSort)(Vaga[], int, int);  /* quickSort/mergeSort */
typedef void (*FnSortN)(Vaga[], int);      /* bubble/insertion/selection */

static double medirN(FnSortN fn, Vaga base[], int n, TipoEntrada tipo) {
    Vaga *buf = malloc(n * sizeof(Vaga));
    prepararEntrada(base, buf, n, tipo);
    clock_t ini = clock();
    fn(buf, n);
    clock_t fim = clock();
    free(buf);
    return (double)(fim - ini) / CLOCKS_PER_SEC * 1000.0; /* ms */
}

static double medirQM(FnSort fn, Vaga base[], int n, TipoEntrada tipo) {
    Vaga *buf = malloc(n * sizeof(Vaga));
    prepararEntrada(base, buf, n, tipo);
    clock_t ini = clock();
    fn(buf, 0, n - 1);
    clock_t fim = clock();
    free(buf);
    return (double)(fim - ini) / CLOCKS_PER_SEC * 1000.0;
}

/* ------------------------------------------------------------------ */

int main(void) {
    srand(42); /* semente fixa para reproducibilidade */

    const int VOLUMES[] = { 100, 1000, 10000 };
    const int REPS[]    = { 100,  50,    10  };
    const int NV        = 3;
    const int NT        = 3; /* tipos de entrada */

    FILE *csv = fopen("resultados.csv", "w");
    if (!csv) { fprintf(stderr, "Erro ao criar resultados.csv\n"); return 1; }
    fprintf(csv, "Algoritmo,N,TipoEntrada,TempoMedio_ms\n");

    for (int v = 0; v < NV; v++) {
        int n    = VOLUMES[v];
        int reps = REPS[v];

        printf("\n===========================================\n");
        printf(" N = %d  (%d repeticoes por cenario)\n", n, reps);
        printf("===========================================\n");
        printf("%-16s  %-10s  %-12s\n", "Algoritmo", "Entrada", "Tempo medio");
        printf("%-16s  %-10s  %-12s\n", "---------", "-------", "-----------");

        /* gera dados aleatorios base uma unica vez */
        Vaga *base = malloc(n * sizeof(Vaga));
        gerarVagas(base, n);

        for (int t = 0; t < NT; t++) {
            TipoEntrada tipo = (TipoEntrada)t;

            /* --- Bubble Sort --- */
            double soma = 0;
            for (int r = 0; r < reps; r++) soma += medirN(bubbleSort, base, n, tipo);
            double media = soma / reps;
            printf("%-16s  %-10s  %.4f ms\n", "Bubble Sort", nomeEntrada[t], media);
            fprintf(csv, "Bubble Sort,%d,%s,%.6f\n", n, nomeEntrada[t], media);

            /* --- Insertion Sort --- */
            soma = 0;
            for (int r = 0; r < reps; r++) soma += medirN(insertionSort, base, n, tipo);
            media = soma / reps;
            printf("%-16s  %-10s  %.4f ms\n", "Insertion Sort", nomeEntrada[t], media);
            fprintf(csv, "Insertion Sort,%d,%s,%.6f\n", n, nomeEntrada[t], media);

            /* --- Selection Sort --- */
            soma = 0;
            for (int r = 0; r < reps; r++) soma += medirN(selectionSort, base, n, tipo);
            media = soma / reps;
            printf("%-16s  %-10s  %.4f ms\n", "Selection Sort", nomeEntrada[t], media);
            fprintf(csv, "Selection Sort,%d,%s,%.6f\n", n, nomeEntrada[t], media);

            /* --- Quick Sort --- */
            soma = 0;
            for (int r = 0; r < reps; r++) soma += medirQM(quickSort, base, n, tipo);
            media = soma / reps;
            printf("%-16s  %-10s  %.4f ms\n", "Quick Sort", nomeEntrada[t], media);
            fprintf(csv, "Quick Sort,%d,%s,%.6f\n", n, nomeEntrada[t], media);

            /* --- Merge Sort --- */
            soma = 0;
            for (int r = 0; r < reps; r++) soma += medirQM(mergeSort, base, n, tipo);
            media = soma / reps;
            printf("%-16s  %-10s  %.4f ms\n", "Merge Sort", nomeEntrada[t], media);
            fprintf(csv, "Merge Sort,%d,%s,%.6f\n", n, nomeEntrada[t], media);
        }

        free(base);
    }

    fclose(csv);
    printf("\nResultados salvos em resultados.csv\n");
    return 0;
}
