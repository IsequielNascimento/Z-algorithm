#include <stdio.h>
#include <string.h>

// Definição de constantes (estrutura de dados: defines para limites máximos)
#define MAX_TEXT_SIZE 8000          // Tamanho máximo do texto
#define MAX_PATTERN_SIZE 100        // Tamanho máximo do padrão (pattern)
#define MAX_OCCURRENCES 1000        // Número máximo de ocorrências a registrar

// ============================================================================
// Função: compute_z_array
// Objetivo: Calcula o vetor Z para uma string s[] usando o Z-Algorithm
// ============================================================================
void compute_z_array(const char *s, int z[], int length) {
    int l = 0, r = 0;               // Variáveis para a janela [l, r]
    z[0] = 0;                       // Por definição, Z[0] = 0

    for (int i = 1; i < length; i++) {   // Percorre a string
        if (i <= r)  // Se estiver dentro da janela [l, r]
            // Pega o valor mínimo possível de z[i] para economizar comparações
            z[i] = (r - i + 1 < z[i - l]) ? (r - i + 1) : z[i - l];

        // Expande a janela, comparando caracteres da string
        while (i + z[i] < length && s[z[i]] == s[i + z[i]])
            z[i]++;

        // Atualiza [l, r] se expandiu além da janela atual
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}

// ============================================================================
// Função: z_search
// Objetivo: Busca o padrão (pattern) no texto usando o Z-Algorithm
// ============================================================================
int z_search(const char *text, const char *pattern, int text_len, int pattern_len, int occurrences[]) {
    // Estruturas de dados locais
    char concat[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];  // Concatenar pattern + '$' + text
    int z[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];        // Vetor Z para a string concatenada
    int concat_len = pattern_len + 1 + text_len;        // Comprimento total da string concatenada

    // Concatenar pattern + '$' + text (O '$' é um caractere especial, não ocorre no texto)
    strncpy(concat, pattern, pattern_len);
    concat[pattern_len] = '$';                          // Separador
    strncpy(concat + pattern_len + 1, text, text_len);  // Copia o texto após o '$'

    // Calcula o vetor Z para a string concatenada
    compute_z_array(concat, z, concat_len);

    int count = 0;  // Contador de ocorrências encontradas
    // Itera sobre o vetor Z (começando depois do separador)
    for (int i = pattern_len + 1; i < concat_len; i++) {
        if (z[i] == pattern_len && count < MAX_OCCURRENCES) {
            // Encontrou uma ocorrência do pattern no text
            occurrences[count++] = i - pattern_len - 1;  // Calcula a posição correta no texto
        }
    }
    return count;  // Retorna o número total de ocorrências
}

int main() {
    char text[MAX_TEXT_SIZE + 1] = {0};
    char pattern[MAX_PATTERN_SIZE + 1] = "ATGC";

    FILE *file = fopen("sequencia.txt", "r");
    if (!file) {
        printf("Erro ao abrir sequencia.txt\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        for (size_t i = 0; i < len; i++) {
            if (line[i] != '\n' && line[i] != '\r') {
                size_t curr_len = strlen(text);
                if (curr_len < MAX_TEXT_SIZE) {
                    text[curr_len] = line[i];
                    text[curr_len + 1] = '\0';
                }
            }
        }
    }
    fclose(file);

    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    int occurrences[MAX_OCCURRENCES];

    int count = z_search(text, pattern, text_len, pattern_len, occurrences);
    printf("Z-Algorithm encontrou %d ocorrencias:\n", count);
    for (int i = 0; i < count; i++)
        printf("Posicao: %d\n", occurrences[i]);

    return 0;
}