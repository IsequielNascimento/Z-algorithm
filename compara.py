import os
import re

def ler_posicoes(caminho_arquivo):
    posicoes = []
    try:
        with open(caminho_arquivo, "r") as f:
            for linha in f:
                linha = linha.strip()
                if linha.startswith("Posicao:"):
                    try:
                        numero = int(linha.split(":")[1].strip())
                        posicoes.append(numero)
                    except ValueError:
                        print(f"⚠️ Erro ao converter linha: {linha}")
        return posicoes
    except FileNotFoundError:
        print(f"❌ Erro: Arquivo não encontrado - {caminho_arquivo}")
        return None

def comparar_listas(lista1, lista2):
    erro = False
    max_len = max(len(lista1), len(lista2))

    for i in range(max_len):
        if i >= len(lista1):
            print(f"Linha {i+1} existe apenas no arquivo KMP: {lista2[i]}")
            erro = True
        elif i >= len(lista2):
            print(f"Linha {i+1} existe apenas no arquivo Z: {lista1[i]}")
            erro = True
        elif lista1[i] != lista2[i]:
            print(f"❌ Erro na linha {i+1}: Z = {lista1[i]}, KMP = {lista2[i]}")
            erro = True

    if not erro:
        print("✅ Todos os resultados são consistentes entre Z e KMP.")

def main():
    z_path = "posicoes.txt"
    kmp_path = os.path.join("KMP", "posicoes.txt")

    z_posicoes = ler_posicoes(z_path)
    kmp_posicoes = ler_posicoes(kmp_path)

    if z_posicoes is None or kmp_posicoes is None:
        print("⚠️ Erro na leitura dos arquivos. Verifique os caminhos.")
        return

    comparar_listas(z_posicoes, kmp_posicoes)

if __name__ == "__main__":
    main()
