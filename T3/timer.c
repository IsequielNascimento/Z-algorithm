#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_TEXT_SIZE 1100
#define MAX_PATTERN_SIZE 4
#define MAX_OCCURRENCES 20
#define N_EXECUCOES 1000



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

__uint8_t z_search(const char *text, const char *pattern, int text_len, int pattern_len, char occurrences[]) {
     char concat_buffer[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];
     int z_array_buffer[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];
    int concat_len = pattern_len + 1 + text_len;

     strncpy(concat_buffer, pattern, pattern_len);
    concat_buffer[pattern_len] = '$'; // Adiciona o separador

    // Copia o texto para o buffer de concatenação
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

int main(void) {
    static const char fixed_text[MAX_TEXT_SIZE] = "CCTTTAACGTATTGGAATGCTTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTTT";
    static const char pattern[MAX_PATTERN_SIZE] = {'A','T','G','C'};
    char occurrences[MAX_OCCURRENCES];

    clock_t start = clock();

    for (int i = 0; i < N_EXECUCOES; i++) {
        z_search(fixed_text, pattern, MAX_TEXT_SIZE, MAX_PATTERN_SIZE, occurrences);
    }

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    double average_time = total_time / N_EXECUCOES;

    printf("Tempo TOTAL de %d execucoes: %.10f segundos\n", N_EXECUCOES, total_time);
    printf("Tempo MEDIO por execucao: %.10f segundos\n", average_time);

    return 0;
}
