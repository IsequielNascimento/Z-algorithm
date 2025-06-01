#include <stdio.h>
#include <string.h>

#define TAM_TEXTO 8000
#define TAM_PADRAO 100
#define MAX_OCORRENCIAS 500

char texto[TAM_TEXTO];
char padrao[TAM_PADRAO];
int z_array[TAM_TEXTO + TAM_PADRAO + 1];
int ocorrencias[MAX_OCORRENCIAS];
int ocorrencias_naive[MAX_OCORRENCIAS];

// Função para construir o Z-array
void calcularZArray(const char *s, int *z) {
    int n = strlen(s);
    int l = 0, r = 0;
    z[0] = 0;

    for (int i = 1; i < n; i++) {
        if (i <= r)
            z[i] = (z[i - l] < r - i + 1) ? z[i - l] : (r - i + 1);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++;
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}

// Função Z-Algorithm para encontrar padrão no texto
int buscarComZ(const char *texto, const char *padrao, int *ocorrencias) {
    int count = 0;
    char concat[TAM_PADRAO + 1 + TAM_TEXTO];
    sprintf(concat, "%s$%s", padrao, texto);
    calcularZArray(concat, z_array);

    int lenPadrao = strlen(padrao);
    int lenConcat = strlen(concat);

    for (int i = lenPadrao + 1; i < lenConcat; i++) {
        if (z_array[i] == lenPadrao) {
            if (count < MAX_OCORRENCIAS)
                ocorrencias[count++] = i - lenPadrao - 1;
        }
    }

    return count;
}

// Busca Ingênua para comparação
int buscarNaive(const char *texto, const char *padrao, int *ocorrencias) {
    int count = 0;
    int n = strlen(texto);
    int m = strlen(padrao);

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && texto[i + j] == padrao[j]) {
            j++;
        }
        if (j == m && count < MAX_OCORRENCIAS) {
            ocorrencias[count++] = i;
        }
    }

    return count;
}

// Função para exibir resultados
void exibirOcorrencias(const char *nome, int *ocorrencias, int count) {
    printf("\nOcorrências encontradas por %s (%d):\n", nome, count);
    for (int i = 0; i < count; i++) {
        printf("Posição %d\n", ocorrencias[i]);
    }
}

int main() {
    // Exemplo de dados (pode ser alterado para testes maiores)
    strcpy(texto, "atgccgactgttatgggttcagtctcgttagtaaataatacaaaccgttcacagctaaggttcatccgtgccgcggtaagtcccgttttcggcagcttcaatgccgactgttatgggttcagtctcgttagtaaataatacaaaccgttcacagctaaggttcatccgtgccgcggtaagtcccgttttcggcagcttcaatgccgactgttatgggttcagtctcgttagtaaataatacaaaccgttcacagctaaggttcatccgtgccgcggtaagtcccgttttcggcagcttcaatgccgactgttatgggttcagtctcgttagtaaataatacaaaccgttcacagctaaggttcatccgtgccgcggtaagtcccgttttcggcagcttca");
    strcpy(padrao, "atgc");

    printf("Texto: %s\n", texto);
    printf("Padrão: %s\n", padrao);

    // Executa Z-Algorithm
    int countZ = buscarComZ(texto, padrao, ocorrencias);
    exibirOcorrencias("Z-Algorithm", ocorrencias, countZ);

    // Executa busca ingênua
    int countNaive = buscarNaive(texto, padrao, ocorrencias_naive);
    exibirOcorrencias("Busca Ingênua", ocorrencias_naive, countNaive);

    return 0;
}
