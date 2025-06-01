#include <stdio.h>
#include <string.h>

#define MAX_TEXT_SIZE 8000
#define MAX_PATTERN_SIZE 100
#define MAX_OCCURRENCES 1000  // Número máximo de ocorrências a registrar

// Z-Algorithm: Calcula o vetor Z para a string s[]
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

// Busca com Z-Algorithm
int z_search(const char *text, const char *pattern, int text_len, int pattern_len, int occurrences[]) {
    char concat[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];  // pattern + '$' + text
    int z[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];
    int concat_len = pattern_len + 1 + text_len;

    // Concatenar pattern + '$' + text 
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

// Knuth-Morris-Pratt (KMP): Calcula o prefixo para pattern
void compute_prefix_function(const char *pattern, int m, int pi[]) {
    int k = 0;
    pi[0] = 0;
    for (int q = 1; q < m; q++) {
        while (k > 0 && pattern[k] != pattern[q])
            k = pi[k - 1];
        if (pattern[k] == pattern[q])
            k++;
        pi[q] = k;
    }
}

// Busca com KMP
int kmp_search(const char *text, const char *pattern, int text_len, int pattern_len, int occurrences[]) {
    int pi[MAX_PATTERN_SIZE];
    compute_prefix_function(pattern, pattern_len, pi);
    int q = 0;
    int count = 0;
    for (int i = 0; i < text_len; i++) {
        while (q > 0 && pattern[q] != text[i])
            q = pi[q - 1];
        if (pattern[q] == text[i])
            q++;
        if (q == pattern_len && count < MAX_OCCURRENCES) {
            occurrences[count++] = i - pattern_len + 1;
            q = pi[q - 1];
        }
    }
    return count;
}

int main() {
    char text[MAX_TEXT_SIZE + 1] = "cgactgttatgggttcagtctcgttagtaaataatacaaaatgcccgttcacagctaaggttcatccgtgccgcggtaagtcccgttttcggcagcttca";  // Exemplo de texto (pode ser modificado)
    char pattern[MAX_PATTERN_SIZE + 1] = "atgc";

    int text_len = strlen(text);
    int pattern_len = strlen(pattern);

    int occurrences_z[MAX_OCCURRENCES];
    int occurrences_kmp[MAX_OCCURRENCES];

    

    // Executar busca usando Z-Algorithm
    int count_z = z_search(text, pattern, text_len, pattern_len, occurrences_z);
    printf("Z-Algorithm encontrou %d ocorrências:\n", count_z);
    for (int i = 0; i < count_z; i++)
        printf("Posição: %d\n", occurrences_z[i]);

    // Executar busca usando KMP
    int count_kmp = kmp_search(text, pattern, text_len, pattern_len, occurrences_kmp);
    printf("\nKMP encontrou %d ocorrências:\n", count_kmp);
    for (int i = 0; i < count_kmp; i++)
        printf("Posição: %d\n", occurrences_kmp[i]);

    printf("Tamanho de text[]: %zu bytes\n", sizeof(text));

    printf("Total usado pelas estruturas principais: %zu bytes\n",
       sizeof(text) +
       sizeof(occurrences_z) +
       sizeof(occurrences_kmp));

    return 0;
}
