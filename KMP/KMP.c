#include <stdio.h>
#include <string.h>

#define MAX_TEXT_SIZE 8000
#define MAX_PATTERN_SIZE 100
#define MAX_OCCURRENCES 1000

void constructLps(const char *pat, int m, int lps[]) {
    int len = 0;
    lps[0] = 0;

    int i = 1;
    while (i < m) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

int search(const char *pat, const char *txt, int result_indices[], int max_results) {
    int m = strlen(pat);
    int n = strlen(txt);
    int lps[MAX_PATTERN_SIZE];
    int res_count = 0;

    constructLps(pat, m, lps);

    int i = 0;
    int j = 0;

    while (i < n) {
        if (txt[i] == pat[j]) {
            i++;
            j++;
            if (j == m) {
                if (res_count < max_results) {
                    result_indices[res_count++] = i - j;
                }
                j = lps[j - 1];
            }
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return res_count;
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

    int result_indices[MAX_OCCURRENCES];
    int count = search(pattern, text, result_indices, MAX_OCCURRENCES);

    FILE *out = fopen("posicoes.txt", "w");
    if (!out) {
        printf("Erro ao criar posicoes.txt\n");
        return 1;
    }

    fprintf(out, "KMP encontrou %d ocorrencias:\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(out, "Posicao: %d\n", result_indices[i]);
    }

    fclose(out);
    return 0;
}
