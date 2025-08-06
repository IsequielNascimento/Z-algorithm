#include <stdio.h>
#include <time.h>

#define MAX_TEXT_SIZE 1100
#define MAX_PATTERN_SIZE 4
#define MAX_OCCURRENCES 20
#define N_EXECUCOES 1000

static char concat_buffer[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];
static int z_array_buffer[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE];

void compute_z_array(const char *s, int z[], int length) {
    int l = 0, r = 0;
    z[0] = 0;
    for (int i = 1; i < length; i++) {
        int zi = 0;

        if (i <= r) {
            int k = z[i - l];
            if (r - i + 1 < k) k = r - i + 1;
            zi = k;
        }

        while (i + zi < length && s[zi] == s[i + zi]) {
            zi++;
        }

        z[i] = zi;

        if (i + zi - 1 > r) {
            l = i;
            r = i + zi - 1;
        }
    }
}

__uint8_t z_search(int concat_len, int pattern_len, char occurrences[]) {
    compute_z_array(concat_buffer, z_array_buffer, concat_len);

    char count = 0;
    for (int i = pattern_len + 1; i < concat_len && count < MAX_OCCURRENCES; i++) {
        if (z_array_buffer[i] == pattern_len) {
            occurrences[count++] = i - pattern_len - 1;
        }
    }
    return count;
}

int main(void) {
    static const char fixed_text[MAX_TEXT_SIZE] = "CCTTTAACGTATTGGAATGCTTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTTT";
    static const char pattern[MAX_PATTERN_SIZE] = {'A','T','G','C'};
    char occurrences[MAX_OCCURRENCES];

    for (int i = 0; i < MAX_PATTERN_SIZE; i++) {
        concat_buffer[i] = pattern[i];
    }
    concat_buffer[MAX_PATTERN_SIZE] = '$';
    for (int i = 0; i < MAX_TEXT_SIZE; i++) {
        concat_buffer[MAX_PATTERN_SIZE + 1 + i] = fixed_text[i];
    }

    int concat_len = MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE;

    clock_t start = clock();

    for (int i = 0; i < N_EXECUCOES; i++) {
        z_search(concat_len, MAX_PATTERN_SIZE, occurrences);
    }

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    double average_time = total_time / N_EXECUCOES;

    printf("Tempo TOTAL de %d execucoes: %.10f segundos\n", N_EXECUCOES, total_time);
    printf("Tempo MEDIO por execucao: %.10f segundos\n", average_time);

    return 0;
}

