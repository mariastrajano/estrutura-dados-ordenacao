# Análise de Algoritmos de Ordenação com Listas Encadeadas

Projeto desenvolvido para a disciplina **Estrutura de Dados**, com o objetivo de analisar o impacto da escolha de **algoritmos de ordenação e estruturas de dados** no desempenho de aplicações que manipulam listas dinâmicas.

O projeto simula um sistema de **listagem de vagas de emprego**, onde novas vagas podem ser adicionadas dinamicamente e a lista pode ser ordenada por diferentes critérios.

## Objetivo

Este projeto tem como objetivo:

- Implementar **listas encadeadas dinâmicas**
- Implementar **listas encadeadas estáticas**
- Implementar diferentes **algoritmos de ordenação**
- Analisar o **desempenho dos algoritmos**
- Gerar **dados experimentais e gráficos**

## Algoritmos de Ordenação Implementados

| Algoritmo | Melhor Caso | Caso Médio | Pior Caso |
|-----------|------------|-----------|-----------|
| Bubble Sort | O(n) | O(n²) | O(n²) |
| Insertion Sort | O(n) | O(n²) | O(n²) |
| Selection Sort | O(n²) | O(n²) | O(n²) |
| Quick Sort | O(n log n) | O(n log n) | O(n²) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) |

## Estruturas de Dados

### Lista Encadeada Dinâmica

Utiliza **alocação dinâmica de memória**, permitindo crescimento flexível da lista.

Funções implementadas:

- Inicialização
- Inserção
- Busca
- Remoção
- Impressão
- Liberação da memória

### Lista Encadeada Estática

Implementada utilizando:

- vetor pré-alocado
- controle manual das posições livres

## Estrutura do Projeto
```
├── main.c
├── gerar_dados.c
├── ordenacao.c
├── ordenacao.h
├── vaga.h
│
├── lista_dinamica.c
├── lista_dinamica.h
│
├── lista_estatica.c
├── lista_estatica.h
│
├── testes_ordenacao.c
├── testes.c
│
├── grafico.py
└── README.md
```

## Compilação

Para compilar o sistema de testes de desempenho:

```bash
gcc testes_ordenacao.c ordenacao.c gerar_dados.c -o testes
```

## Execução

Execute o programa:

```bash
./testes
```

O programa irá:

1. Gerar **dados aleatórios de vagas**
2. Executar os algoritmos de ordenação
3. Repetir cada experimento **100 vezes**
4. Calcular o **tempo médio de execução**
5. Exportar os resultados para um arquivo **CSV**

## Resultados Gerados

Após executar os testes, será criado o arquivo:

```
resultados.csv
```

Exemplo:

```
Algoritmo,N,Tempo
Bubble,100,0.00012
Insertion,100,0.00009
Selection,100,0.00013
Quick,100,0.00001
Merge,100,0.00002
Bubble,1000,0.085
Insertion,1000,0.073
Selection,1000,0.091
Quick,1000,0.002
Merge,1000,0.003
```
## Geração de Gráficos

Os gráficos podem ser gerados utilizando Python.

**Instalar dependências**

```bash
pip install pandas matplotlib
```

**Executar o script**

```bash
python3 grafico.py
```

O script irá gerar um gráfico comparando os tempos de execução dos algoritmos.

## Cenários de Teste

Os algoritmos são avaliados utilizando três volumes de dados:

| Cenário | Elementos |
|---------|------|
| Pequeno | 100 |
| Médio | 1000 |
| Grande | 10000 |

Cada teste é executado **100 vezes** para garantir maior precisão nos resultados.

## Tecnologias Utilizadas

- **C:** implementação das estruturas de dados e algoritmos
- **Python:** geração de gráficos
- **Pandas:** manipulação de dados
- **Matplotlib:** visualização de dados

## Resultado Esperado

Com base na análise teórica e experimental, espera-se que:

- **Quick Sort** apresente melhor desempenho médio
- **Merge Sort** apresente desempenho estável
- Algoritmos **O(n²)** sejam significativamente mais lentos para grandes volumes de dados

## Integrantes do Grupo

- Abner Levi - @abnerlevi
- Grazielly Bibiano - @graziellybn
- Maria Antônia - @mariastrajano
- Matheus Nogueira - @mathsNS
