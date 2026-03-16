#ifndef VAGA_H
#define VAGA_H

typedef struct {
    char empresa[50];
    float salario;
    float distancia;
    float avaliacao;
} Vaga;

/* -----------------------------------------------------------------
 *  Comparador: negativo se a < b, 0 se iguais, positivo se a > b
 * ----------------------------------------------------------------- */
typedef int (*Comparador)(const Vaga *, const Vaga *);

int cmpSalario  (const Vaga *a, const Vaga *b); /* crescente           */
int cmpDistancia(const Vaga *a, const Vaga *b); /* crescente           */
int cmpAvaliacao(const Vaga *a, const Vaga *b); /* decrescente (melhor)*/

/* Comparador ativo e contador de comparacoes (definidos em gerar_dados.c) */
extern Comparador  g_cmp;
extern long long   g_comparacoes;

#endif
