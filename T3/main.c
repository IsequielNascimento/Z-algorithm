
/* ---------------- Código criado pela equipe (Z-Algorithm + UART redirect) ---------------- */
#include <string.h>
#include <stdio.h>

#define MAX_TEXT_SIZE 1198              /**< Tamanho fixo da sequência de DNA embarcada */
#define MAX_PATTERN_SIZE 4              /**< Tamanho do padrão buscado (ATGC) */
#define MAX_OCCURRENCES 20              /**< Número máximo de ocorrências a registrar */

/**< Buffer que armazena a string concatenada pattern + '$' + text */
static char concat_buffer[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];

/**< Vetor que armazena os valores do vetor Z após execução do algoritmo */
static int z_array_buffer[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];

/**
 * @brief Calcula o vetor Z da string concatenada (padrão + '$' + texto).
 * @param s string concatenada
 * @param z vetor de saída com os valores Z
 * @param length comprimento da string concatenada
 */
void compute_z_array(const char *s, int z[], int length) {
    int l = 0, r = 0;
    z[0] = 0;
    for (int i = 1; i < length; i++) {
        if (i <= r)
            z[i] = (r - i + 1 < z[i - l]) ? (r - i + 1) : z[i - l];
        else
            z[i] = 0;

        while (i + z[i] < length && s[z[i]] == s[i + z[i]])
            z[i]++;

        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}

/**
 * @brief Busca o padrão no texto utilizando o Z-Algorithm.
 * @param text sequência de DNA
 * @param pattern padrão buscado ("ATGC")
 * @param text_len comprimento da sequência
 * @param pattern_len comprimento do padrão
 * @param occurrences vetor com as posições encontradas
 * @return número de ocorrências
 */
__uint8_t z_search(const char *text, const char *pattern, int text_len, int pattern_len, char occurrences[]) {
    __uint8_t concat_len = pattern_len + 1 + text_len;
    strncpy(concat_buffer, pattern, pattern_len);
    concat_buffer[pattern_len] = '$';
    strncpy(concat_buffer + pattern_len + 1, text, text_len);

    compute_z_array(concat_buffer, z_array_buffer, concat_len);

    char count = 0;
    for (int i = pattern_len + 1; i < concat_len; i++) {
        if (z_array_buffer[i] == pattern_len && count < MAX_OCCURRENCES) {
            occurrences[count++] = i - pattern_len - 1;
        }
    }
    return count;
}

/**
 * @brief Função principal do programa embarcado STM32.
 *        Inicializa os periféricos, executa a busca Z e imprime os resultados via UART.
 */
char main(void) {

    static const char fixed_text[MAX_TEXT_SIZE] = {
        "CCTTTAACGTATTGGAATGCTTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTTT"
    };
    static const char pattern[MAX_PATTERN_SIZE] = {'A','T','G','C'};

    char occurrences[MAX_OCCURRENCES]; /**< Vetor que armazena as posições encontradas do padrão no texto */
    char count = z_search(fixed_text, pattern, MAX_TEXT_SIZE, MAX_PATTERN_SIZE, occurrences);

    printf("\nZ-Algorithm encontrou %d ocorrencias:\n", count);
    for (char i = 0; i < count; i++) {
        printf("Posicao: %d\n", occurrences[i]);
    }
    printf("Execucao do Z-Algorithm concluida.\n");

/* ---------------- Fim do Código criado pela equipe (Z-Algorithm + UART redirect) ---------------- */


}
