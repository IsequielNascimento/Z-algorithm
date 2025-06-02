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

// ============================================================================
// Função: compute_prefix_function
// Objetivo: Calcula o prefixo para o padrão (pattern) no KMP
// ============================================================================
void compute_prefix_function(const char *pattern, int m, int pi[]) {
    int k = 0;      // Comprimento do maior prefixo que também é sufixo
    pi[0] = 0;      // Por definição, pi[0] = 0

    for (int q = 1; q < m; q++) {  // Itera sobre o padrão
        // Ajusta k se há divergência entre pattern[k] e pattern[q]
        while (k > 0 && pattern[k] != pattern[q])
            k = pi[k - 1];          // Volta para o próximo maior prefixo

        if (pattern[k] == pattern[q])
            k++;                    // Aumenta k se os caracteres coincidem

        pi[q] = k;                  // Armazena o comprimento do prefixo atual
    }
}

// ============================================================================
// Função: kmp_search
// Objetivo: Busca o padrão (pattern) no texto usando o KMP
// ============================================================================
int kmp_search(const char *text, const char *pattern, int text_len, int pattern_len, int occurrences[]) {
    int pi[MAX_PATTERN_SIZE];       // Vetor de prefixos (pi) para o padrão
    compute_prefix_function(pattern, pattern_len, pi);  // Pré-processamento do padrão

    int q = 0;                      // Número de caracteres do padrão já casados
    int count = 0;                  // Contador de ocorrências encontradas

    for (int i = 0; i < text_len; i++) {  // Percorre o texto
        // Ajusta 'q' se há divergência entre pattern[q] e text[i]
        while (q > 0 && pattern[q] != text[i])
            q = pi[q - 1];          // Volta para o próximo maior prefixo

        if (pattern[q] == text[i])
            q++;                    // Avança q se os caracteres coincidem

        if (q == pattern_len && count < MAX_OCCURRENCES) {
            // Encontrou uma ocorrência do padrão no texto
            occurrences[count++] = i - pattern_len + 1;  // Calcula a posição de início
            q = pi[q - 1];          // Prepara q para continuar a busca
        }
    }
    return count;  // Retorna o número total de ocorrências
}







// ============================================================================
// Função principal: main
// ============================================================================
int main() {
    char text[MAX_TEXT_SIZE + 1] = {0};  // Inicializa o buffer vazio
    char pattern[MAX_PATTERN_SIZE + 1] = "ATGC";

    // ======== Leitura do arquivo sequencia.txt ==========
    FILE *file = fopen("sequencia.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo sequencia.txt\n");
        return 1;
    }

    // Leitura e remoção de quebras de linha
    char line[256];  // Buffer temporário para cada linha
    while (fgets(line, sizeof(line), file)) {
        size_t len = strlen(line);
        for (size_t i = 0; i < len; i++) {
            if (line[i] != '\n' && line[i] != '\r') {
                size_t curr_len = strlen(text);
                if (curr_len < MAX_TEXT_SIZE) {
                    text[curr_len] = line[i];
                    text[curr_len + 1] = '\0';
                } else {
                    printf("A sequência excedeu o tamanho maximo de %d caracteres.\n", MAX_TEXT_SIZE);
                    fclose(file);
                    return 1;
                }
            }
        }
    }
    fclose(file);  // Fecha o arquivo após a leitura
    // ====================================================

    int text_len = strlen(text);
    int pattern_len = strlen(pattern);

    int occurrences_z[MAX_OCCURRENCES];
    int occurrences_kmp[MAX_OCCURRENCES];

    // Executa a busca com Z-Algorithm
    int count_z = z_search(text, pattern, text_len, pattern_len, occurrences_z);
    printf("\nZ-Algorithm encontrou %d ocorrencias:\n", count_z);
    for (int i = 0; i < count_z; i++)
        printf("Posicao: %d\n", occurrences_z[i]);

    // Executa a busca com KMP
    int count_kmp = kmp_search(text, pattern, text_len, pattern_len, occurrences_kmp);
    printf("\nKMP encontrou %d ocorrencias:\n", count_kmp);
    for (int i = 0; i < count_kmp; i++)
        printf("Posicao: %d\n", occurrences_kmp[i]);

    return 0;
}
