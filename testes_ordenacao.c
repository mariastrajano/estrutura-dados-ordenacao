#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ordenacao.h"
#include "vaga.h"

void gerarVagas(Vaga v[], int n);

void copiar(Vaga origem[], Vaga destino[], int n){
    for(int i=0;i<n;i++)
        destino[i] = origem[i];
}

double medirBubble(Vaga v[], int n){

    clock_t inicio = clock();
    bubbleSort(v,n);
    clock_t fim = clock();

    return (double)(fim-inicio)/CLOCKS_PER_SEC;
}

double medirInsertion(Vaga v[], int n){

    clock_t inicio = clock();
    insertionSort(v,n);
    clock_t fim = clock();

    return (double)(fim-inicio)/CLOCKS_PER_SEC;
}

double medirSelection(Vaga v[], int n){

    clock_t inicio = clock();
    selectionSort(v,n);
    clock_t fim = clock();

    return (double)(fim-inicio)/CLOCKS_PER_SEC;
}

double medirQuick(Vaga v[], int n){

    clock_t inicio = clock();
    quickSort(v,0,n-1);
    clock_t fim = clock();

    return (double)(fim-inicio)/CLOCKS_PER_SEC;
}

double medirMerge(Vaga v[], int n){

    clock_t inicio = clock();
    mergeSort(v,0,n-1);
    clock_t fim = clock();

    return (double)(fim-inicio)/CLOCKS_PER_SEC;
}

int main(){

    srand(time(NULL));

    int tamanhos[] = {100,1000,10000};

    for(int t=0;t<3;t++){

        int n = tamanhos[t];

        printf("\n============================\n");
        printf("TESTE COM %d ELEMENTOS\n",n);
        printf("============================\n");

        Vaga *original = malloc(n*sizeof(Vaga));
        Vaga *teste = malloc(n*sizeof(Vaga));

        gerarVagas(original,n);

        copiar(original,teste,n);
        printf("Bubble Sort: %f s\n",medirBubble(teste,n));

        copiar(original,teste,n);
        printf("Insertion Sort: %f s\n",medirInsertion(teste,n));

        copiar(original,teste,n);
        printf("Selection Sort: %f s\n",medirSelection(teste,n));

        copiar(original,teste,n);
        printf("Quick Sort: %f s\n",medirQuick(teste,n));

        copiar(original,teste,n);
        printf("Merge Sort: %f s\n",medirMerge(teste,n));

        free(original);
        free(teste);
    }

    return 0;
}
