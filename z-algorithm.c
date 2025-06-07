/**
 * @file z-algorithm.c
 * @brief Implementação do algoritmo Z (Z-Algorithm) para busca de padrões em sequências de DNA.
 * 
 * Esta aplicação lê uma sequência de DNA de um arquivo `sequencia.txt`, encontrada no site: 'https://www.ncbi.nlm.nih.gov/nuccore/NC_100739.1?report=fasta', procura por todas as ocorrências
 * de um padrão fixo (por padrão, "ATGC") usando o algoritmo Z e escreve as posições encontradas no
 * arquivo `posicoes.txt`.
 * 
 * @details Este código foi desenvolvido como parte do trabalho T1 da disciplina de Sistemas Embarcados
 * do curso de Engenharia de Computação do IFCE - Campus Fortaleza.
 * 
 * @authors
 *   - Isequiel Henrique do Nascimento (isequiel.nascimento62@aluno.ifce.edu.br)  
 *   - Alan Andrade Nogueira (alan.andrade.nogueira03@aluno.ifce.edu.br)
 * 
 * @date 2025-06-05
 * @version 1.0
 * 
 * @copyright
 * MIT License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * 
 * @platform
 * - Sistema Operacional: Linux ou Windows (compilado via gcc ou compatível)
 * - Requisitos: Compilador C (gcc), arquivos `sequencia.txt` com a sequência de entrada
 * 
 * @how_to_use
 * Compile com: `gcc -o z-algorithm z-algorithm.c`
 * Execute com: `./z-algorithm` en linux ou z-algorithm.exe no windows
 * 
 * @input
 * - Arquivo `sequencia.txt` contendo a sequência de DNA (sem formatação especial).
 * 
 * @output
 * - Arquivo `posicoes.txt` com as posições onde o padrão foi encontrado.
 */

#include <stdio.h>
#include <string.h>

// =============================================================================
// Constantes para limites máximos
// =============================================================================

#define MAX_TEXT_SIZE 8000         /**< Tamanho máximo da sequência de entrada */
#define MAX_PATTERN_SIZE 100       /**< Tamanho máximo do padrão a ser buscado */
#define MAX_OCCURRENCES 1000       /**< Número máximo de ocorrências a registrar */

// =============================================================================
// Função: compute_z_array
// =============================================================================

/**
 * @brief Calcula o vetor Z para uma string utilizando o Z-Algorithm.
 *
 * @param s String de entrada (concatenada: padrão + '$' + texto).
 * @param z Vetor de saída contendo os valores Z calculados.
 * @param length Comprimento da string s.
 */
void compute_z_array(const char *s, int z[], int length) {
    int l = 0, r = 0;
    z[0] = 0;

    for (int i = 1; i < length; i++) {
        if (i <= r)
            z[i] = (r - i + 1 < z[i - l]) ? (r - i + 1) : z[i - l];

        while (i + z[i] < length && s[z[i]] == s[i + z[i]])
            z[i]++;

        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}

// =============================================================================
// Função: z_search
// =============================================================================

/**
 * @brief Realiza a busca de um padrão em um texto usando o Z-Algorithm.
 * 
 * @param text Sequência de DNA a ser analisada.
 * @param pattern Padrão a ser buscado.
 * @param text_len Comprimento do texto.
 * @param pattern_len Comprimento do padrão.
 * @param occurrences Vetor de saída com as posições encontradas.
 * 
 * @return Número de ocorrências encontradas.
 */
int z_search(const char *text, const char *pattern, int text_len, int pattern_len, int occurrences[]) {
    char concat[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];
    int z[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];
    int concat_len = pattern_len + 1 + text_len;

    strncpy(concat, pattern, pattern_len);
    concat[pattern_len] = '$';
    strncpy(concat + pattern_len + 1, text, text_len);

    compute_z_array(concat, z, concat_len);

    int count = 0;
    for (int i = pattern_len + 1; i < concat_len; i++) {
        if (z[i] == pattern_len && count < MAX_OCCURRENCES) {
            occurrences[count++] = i - pattern_len - 1;
        }
    }

    return count;
}

// =============================================================================
// Função: main
// =============================================================================

/**
 * @brief Função principal da aplicação.
 *
 * Lê a sequência de DNA do arquivo "sequencia.txt", realiza a busca do padrão fixo "ATGC"
 * utilizando o Z-Algorithm e grava as posições encontradas no arquivo "posicoes.txt".
 *
 * @details
 * 1. Abre e lê o conteúdo do arquivo de entrada.
 * 2. Remove caracteres de quebra de linha e concatena tudo em uma única string.
 * 3. Executa a busca do padrão com a função z_search().
 * 4. Cria o arquivo de saída e grava as posições encontradas.
 *
 * @return int Retorna 0 em caso de sucesso, ou 1 se ocorrer erro na leitura ou escrita de arquivos.
 */
int main() {
    char text[MAX_TEXT_SIZE + 1] = {0};  // Armazena a sequência de DNA lida do arquivo
    char pattern[MAX_PATTERN_SIZE + 1] = "ATGC";  // Padrão fixo a ser buscado

    // Abrindo o arquivo de entrada contendo a sequência
    FILE *file = fopen("sequencia.txt", "r");
    if (!file) {
        printf("Erro ao abrir sequencia.txt\n");
        return 1;
    }

    // Lê o arquivo linha por linha e concatena em uma única string
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        for (size_t i = 0; i < len; i++) {
            // Ignora quebras de linha
            if (line[i] != '\n' && line[i] != '\r') {
                size_t curr_len = strlen(text);
                if (curr_len < MAX_TEXT_SIZE) {
                    text[curr_len] = line[i];
                    text[curr_len + 1] = '\0';
                }
            }
        }
    }
    fclose(file);  // Fecha o arquivo de entrada

    // Calcula comprimento do texto e do padrão
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    int occurrences[MAX_OCCURRENCES];  // Vetor para armazenar as posições encontradas

    // Executa a busca do padrão no texto
    int count = z_search(text, pattern, text_len, pattern_len, occurrences);

    // Cria o arquivo de saída para gravar as posições
    FILE *out = fopen("posicoes.txt", "w");
    if (!out) {
        printf("Erro ao criar posicoes.txt\n");
        return 1;
    }

    // Escreve os resultados no arquivo
    fprintf(out, "Z-Algorithm encontrou %d ocorrencias:\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(out, "Posicao: %d\n", occurrences[i]);
    }

    fclose(out);  // Fecha o arquivo de saída
    return 0;     // Execução bem-sucedida
}

