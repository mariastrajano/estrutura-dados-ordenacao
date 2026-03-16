#include <stdio.h>
#include <stdlib.h>
#include "vaga.h"

void gerarVagas(Vaga v[], int n){

    for(int i=0;i<n;i++){

        sprintf(v[i].empresa,"Empresa_%d",i);

        v[i].salario = 2000 + rand()%20000;

        v[i].distancia = (rand()%300)/10.0;

        v[i].avaliacao = (rand()%50)/10.0;
    }
}
