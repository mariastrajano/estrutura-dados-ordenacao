#include "ordenacao.h"

void trocar(Vaga *a, Vaga *b){
    Vaga temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(Vaga v[], int n){
    for(int i=0;i<n-1;i++)
        for(int j=0;j<n-i-1;j++)
            if(v[j].salario > v[j+1].salario)
                trocar(&v[j],&v[j+1]);
}

void insertionSort(Vaga v[], int n){
    for(int i=1;i<n;i++){

        Vaga chave = v[i];
        int j = i-1;

        while(j>=0 && v[j].salario > chave.salario){
            v[j+1] = v[j];
            j--;
        }

        v[j+1] = chave;
    }
}

void selectionSort(Vaga v[], int n){

    for(int i=0;i<n-1;i++){

        int min = i;

        for(int j=i+1;j<n;j++)
            if(v[j].salario < v[min].salario)
                min = j;

        trocar(&v[i],&v[min]);
    }
}

int particionar(Vaga v[], int inicio, int fim){

    float pivo = v[fim].salario;
    int i = inicio-1;

    for(int j=inicio;j<fim;j++){

        if(v[j].salario <= pivo){
            i++;
            trocar(&v[i],&v[j]);
        }
    }

    trocar(&v[i+1],&v[fim]);

    return i+1;
}

void quickSort(Vaga v[], int inicio, int fim){

    if(inicio < fim){

        int p = particionar(v,inicio,fim);

        quickSort(v,inicio,p-1);
        quickSort(v,p+1,fim);
    }
}

void merge(Vaga v[], int inicio, int meio, int fim){

    int n1 = meio-inicio+1;
    int n2 = fim-meio;

    Vaga L[n1],R[n2];

    for(int i=0;i<n1;i++)
        L[i] = v[inicio+i];

    for(int j=0;j<n2;j++)
        R[j] = v[meio+1+j];

    int i=0,j=0,k=inicio;

    while(i<n1 && j<n2){

        if(L[i].salario <= R[j].salario)
            v[k++] = L[i++];
        else
            v[k++] = R[j++];
    }

    while(i<n1)
        v[k++] = L[i++];

    while(j<n2)
        v[k++] = R[j++];
}

void mergeSort(Vaga v[], int inicio, int fim){

    if(inicio < fim){

        int meio = (inicio+fim)/2;

        mergeSort(v,inicio,meio);
        mergeSort(v,meio+1,fim);

        merge(v,inicio,meio,fim);
    }
}
