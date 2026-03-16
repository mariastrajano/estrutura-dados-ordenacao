#include <stdio.h>
#include <stdlib.h>
#include "vaga.h"

/* -----------------------------------------------------------------
 *  Globais de comparacao (declarados em vaga.h)
 * ----------------------------------------------------------------- */
Comparador g_cmp         = cmpSalario;
long long  g_comparacoes = 0;

int cmpSalario(const Vaga *a, const Vaga *b) {
    if (a->salario   < b->salario)   return -1;
    if (a->salario   > b->salario)   return  1;
    return 0;
}

int cmpDistancia(const Vaga *a, const Vaga *b) {
    if (a->distancia < b->distancia) return -1;
    if (a->distancia > b->distancia) return  1;
    return 0;
}

int cmpAvaliacao(const Vaga *a, const Vaga *b) {
    /* decrescente: avaliacao maior primeiro */
    if (a->avaliacao > b->avaliacao) return -1;
    if (a->avaliacao < b->avaliacao) return  1;
    return 0;
}

void gerarVagas(Vaga v[], int n){

    for(int i=0;i<n;i++){

        sprintf(v[i].empresa,"Empresa_%d",i);

        v[i].salario = 2000 + rand()%20000;

        v[i].distancia = (rand()%300)/10.0;

        v[i].avaliacao = (rand()%50)/10.0;
    }
}
