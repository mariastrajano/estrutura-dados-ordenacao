#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vaga.h"
#include "ordenacao.h"

void gerarVagas(Vaga v[], int n);

int main(){

    srand(time(NULL));

    int n = 10000;

    Vaga *v = malloc(n * sizeof(Vaga));

    gerarVagas(v,n);

    printf("Geradas %d vagas\n",n);

    quickSort(v,0,n-1);

    printf("Ordenacao concluida\n");

    free(v);

    return 0;
}
