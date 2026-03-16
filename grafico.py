"""
grafico.py — Gera graficos de desempenho a partir de resultados.csv

Uso:
    pip install pandas matplotlib
    python grafico.py

Saida: graficos PNG em ./graficos/
"""

import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

CSV_FILE = "resultados.csv"
OUT_DIR  = "graficos"

CORES = {
    "Bubble Sort":    "#e74c3c",
    "Insertion Sort": "#e67e22",
    "Selection Sort": "#f1c40f",
    "Quick Sort":     "#2ecc71",
    "Merge Sort":     "#3498db",
}

LINHAS = {
    "Aleatorio":  "o-",
    "Ordenado":   "s--",
    "Invertido":  "^:",
}

def carregar():
    df = pd.read_csv(CSV_FILE)
    df.columns = [c.strip() for c in df.columns]
    return df


def grafico_por_entrada(df):
    """Um grafico por tipo de entrada: X=N, Y=tempo, linhas=algoritmo."""
    os.makedirs(OUT_DIR, exist_ok=True)
    entradas = df["TipoEntrada"].unique()

    for entrada in entradas:
        sub = df[df["TipoEntrada"] == entrada]
        fig, ax = plt.subplots(figsize=(8, 5))

        for algo, cor in CORES.items():
            dados = sub[sub["Algoritmo"] == algo].sort_values("N")
            if dados.empty:
                continue
            ax.plot(dados["N"], dados["TempoMedio_ms"],
                    "o-", color=cor, label=algo, linewidth=2, markersize=5)

        ax.set_title(f"Desempenho dos Algoritmos — Entrada: {entrada}")
        ax.set_xlabel("N (numero de elementos)")
        ax.set_ylabel("Tempo medio (ms)")
        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.xaxis.set_major_formatter(ticker.ScalarFormatter())
        ax.legend()
        ax.grid(True, which="both", linestyle="--", alpha=0.5)
        plt.tight_layout()
        nome = f"{OUT_DIR}/algoritmos_{entrada.lower()}.png"
        plt.savefig(nome, dpi=150)
        plt.close()
        print(f"  Salvo: {nome}")


def grafico_por_algoritmo(df):
    """Um grafico por algoritmo: X=N, Y=tempo, linhas=tipo de entrada."""
    os.makedirs(OUT_DIR, exist_ok=True)
    algoritmos = df["Algoritmo"].unique()

    for algo in algoritmos:
        sub = df[df["Algoritmo"] == algo]
        fig, ax = plt.subplots(figsize=(8, 5))

        for entrada, estilo in LINHAS.items():
            dados = sub[sub["TipoEntrada"] == entrada].sort_values("N")
            if dados.empty:
                continue
            ax.plot(dados["N"], dados["TempoMedio_ms"],
                    estilo, label=entrada, linewidth=2, markersize=6)

        ax.set_title(f"{algo} — Comparacao por Tipo de Entrada")
        ax.set_xlabel("N (numero de elementos)")
        ax.set_ylabel("Tempo medio (ms)")
        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.xaxis.set_major_formatter(ticker.ScalarFormatter())
        ax.legend()
        ax.grid(True, which="both", linestyle="--", alpha=0.5)
        plt.tight_layout()
        nome = f"{OUT_DIR}/{algo.lower().replace(' ', '_')}_entradas.png"
        plt.savefig(nome, dpi=150)
        plt.close()
        print(f"  Salvo: {nome}")


def grafico_comparativo_n(df, n_alvo):
    """Barras comparando todos os algoritmos para um N fixo."""
    os.makedirs(OUT_DIR, exist_ok=True)
    sub      = df[df["N"] == n_alvo]
    entradas = ["Aleatorio", "Ordenado", "Invertido"]
    algos    = list(CORES.keys())

    x      = range(len(algos))
    width  = 0.25
    fig, ax = plt.subplots(figsize=(10, 5))

    for i, entrada in enumerate(entradas):
        tempos = []
        for algo in algos:
            row = sub[(sub["Algoritmo"] == algo) & (sub["TipoEntrada"] == entrada)]
            tempos.append(row["TempoMedio_ms"].values[0] if not row.empty else 0)
        bars = ax.bar([xi + i * width for xi in x], tempos, width,
                      label=entrada, alpha=0.85)

    ax.set_title(f"Comparacao de Algoritmos — N = {n_alvo}")
    ax.set_xlabel("Algoritmo")
    ax.set_ylabel("Tempo medio (ms)")
    ax.set_xticks([xi + width for xi in x])
    ax.set_xticklabels(algos, rotation=15, ha="right")
    ax.legend()
    ax.grid(axis="y", linestyle="--", alpha=0.5)
    plt.tight_layout()
    nome = f"{OUT_DIR}/comparativo_n{n_alvo}.png"
    plt.savefig(nome, dpi=150)
    plt.close()
    print(f"  Salvo: {nome}")


def main():
    if not os.path.exists(CSV_FILE):
        print(f"Arquivo '{CSV_FILE}' nao encontrado.")
        print("Execute primeiro: gcc -O2 -o testes_ord.exe gerar_dados.c ordenacao.c testes_ordenacao.c && .\\testes_ord.exe")
        return

    df = carregar()
    print(f"Dados carregados: {len(df)} linhas\n")

    print("Gerando graficos por tipo de entrada...")
    grafico_por_entrada(df)

    print("Gerando graficos por algoritmo...")
    grafico_por_algoritmo(df)

    print("Gerando graficos comparativos em barra...")
    for n in df["N"].unique():
        grafico_comparativo_n(df, int(n))

    print(f"\nPronto! {len(os.listdir(OUT_DIR))} graficos salvos em ./{OUT_DIR}/")


if __name__ == "__main__":
    main()
