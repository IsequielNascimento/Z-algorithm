#include <stdio.h>
#include <string.h> 

/* 
 MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE = (4 + 1 + 1630) = 1635
 (1635 chars * 1 byte/char) + (1635 ints * 4 bytes/int) = 1635 + 6540 = 8175 bytes (~ 8KB)
*/
#define MAX_TEXT_SIZE 1630 // Tamanho fixo do texto
#define MAX_PATTERN_SIZE 4 // Padrão "ATGC"
#define MAX_OCCURRENCES 10 // lista as posições das ocorrências

// 
// extern UART_HandleTypeDef huart2;
// int _write(int file, char *ptr, int len) {
//     HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
//     return len;
// }

void compute_z_array(const char *s, int z[], int length) {
    int l = 0, r = 0;
    z[0] = 0; // z[0] iniciado com 0.

    // Loop principal para calcular os valores Z para cada posição i
    for (int i = 1; i < length; i++) { 
        // Se i está dentro do Z-box atual (l, r), usar o valor espelhado
        if (i <= r) {
            // z[i - l] é o valor Z para a posição correspondente dentro do Z-box [l, r]
            // r - i + 1 é comprimento restante do Z-box a partir de i
            // Utilizasse o mínimo para não ultrapassar os limites do Z-box
            z[i] = (r - i + 1 < z[i - l]) ? (r - i + 1) : z[i - l];
        } else {
            // Se i está fora do Z-box, inicializar z[i] com 0
            z[i] = 0;
        }

        // Estende a correspondência do prefixo a partir de z[i]
        // Compara caracteres no prefixo (s[z[i]]) com caracteres na substring começando em i (s[i + z[i]])
        while (i + z[i] < length && s[z[i]] == s[i + z[i]]) { 
            z[i]++; // Incrementa z[i] enquanto os caracteres corresponderem
        }

        // Se o Z-box atual (i, i + z[i] - 1) se estende além do Z-box mais à direita conhecido (r)
        if (i + z[i] - 1 > r) {
            l = i;                   // Atualiza l para o início do novo Z-box
            r = i + z[i] - 1;        // Atualiza r para o final do novo Z-box
        }
    }
}

// Funções para manipular a estrutura de dados

// As variáveis 'concat_buffer' e 'z_array_buffer' são a "estrutura de dados principal".
static char concat_buffer[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE]; 
static int z_array_buffer[MAX_PATTERN_SIZE + 1 + MAX_TEXT_SIZE]; 

// Função z_search
int z_search(const char *text, const char *pattern, int text_len, int pattern_len, int occurrences[]) {
    int concat_len = pattern_len + 1 + text_len;

    // Copia o padrão para o buffer de concatenação
    strncpy(concat_buffer, pattern, pattern_len);
    concat_buffer[pattern_len] = '$'; // Adiciona o separador

    // Copia o texto para o buffer de concatenação 
    strncpy(concat_buffer + pattern_len + 1, text, text_len);

    compute_z_array(concat_buffer, z_array_buffer, concat_len);

    int count = 0;
    // Loop de pesquisa de ocorrências
    for (int i = pattern_len + 1; i < concat_len; i++) { // Pelo menos um laço de repetição 
        if (z_array_buffer[i] == pattern_len && count < MAX_OCCURRENCES) {
            occurrences[count++] = i - pattern_len - 1;
        }
    }

    return count;
}

int main(void)
{

    // Dados de texto de tamanho fixo 
    static const char fixed_text[MAX_TEXT_SIZE] = {
        "ATGCCCTTTAACGTATTGGATTACGCGCTTAGAAACGTTGCAGGTTGCGAATTTAACGCCATTTTCCTAACCACTGACCAATCTTTCAAGATTGCAGGAATTGAATGCGGTGTTCATGGTCACAATGGCATTAATGGCAGTCGGGGCAATCCAAAACAGTTCAAAAAGTTGGGACGCCTCAATACTGGCCACACTCACACGGCTAGTATTTATGGCGGCGTTTATACCGCTGGCGTTGCTGGTAGTCTCGATATGGGTTACAACGTTGGCGCTAGTAGCTGGACGCAACCCCATTTAATTACATACGCAAACGGGCAGCGTATTTTGATTGACTTTAAGAATGGAAAATTCTTTGCATAATTAAATTTAATCGGCGGTTGAAATATACCGCCATTAATAAAAGGTTTATTAAATGGCTAGTAAAATTAATATATCTATCCCTGATAGCAATTGTTTAGAAGATATTGCATGGTCTGAAATGTCTTTTGATAGTTGCGTTTATTTAGTTACCCGTGGAATGTACGCGGGAATGTATGCCGTATGGGATAGTAATATTGGCGAAAATATGATCTTTTTCGGAAAAGATTCTATTCAGGTAATGACAGGCGATTACCTTAATCAAGAAAGGAAAGACGGCGACGAAACATATTTTCGTAAAACTGATTTATCTTTAAATGTATTTTTTCAATAAATCCCGTAATTAAATAATCTGGAGTCATAAACATGAAAAACCAAAACATTCCTTTTGATCGTGCTACTTCTTCCATCGTTCTAGTTTATTCTAATGGCGAACGTTATCACGTCGAGGCGGGGCAAGTTAATTTCGATTTGCTGAATTATAACGACGCTTTGCAGGTTACTACCTTTGCTTATGATACCGGCAAAGTGGCAAGCCATACAAAAGCAAAAGGAGTTTACGCTGATACTTTGACAGTGCAAACAATTATTATTGATGCGCTTAAATCCGGTCTAGCGTTTGCAGTTGTTAAGCCGTGTCCGGTCTGCAATCCAGAAGCTAGCGCGATGGTTTACACTTGCGCGGGTATCCGTTCTAGCGTACATGGTGAAGATCTTTCCTTTATCGGTGACGCGCTAGCGTATGGCCTCACTAATTAATAGTTAATTGAACATTGCCAGCTAATCTAATATAATTAGCTGGCAAGATTGAATTAACCTAAATCCCTTAAATGGAGAATGTAACAAATGAATCAAGTCAATATGAATATTACCCGCACTTTCCCGCATATTTCCCGCGTTATGATTTGGGACTTAGACGGAACTATCATTAATTCTTTCCACCGTGTAGCGCCTTGCTTTGATGATAACGGGAATTTAGATTTAAATAAGTATTCCCGCGAAGCCTGTAAACATGATTTAATTATGCAGGATTCTTTATTGCCGTTGGTTGAATATATGCGCCAGTGCATGAATGATGCAAATACTTTAAATATTATTTGTACCGCTCGCTTAATGTCTAAAAGCGATTACTTTTATTTGCGTAAACAAGGCTTGCGAGGGCGCGGAAATAGTAATATTCGCGTATTTTCCCGCGATACATTACATAAATACT"
            }; 

    //printf("Conteúdo de fixed_text:\n%s\n", fixed_text);
    // printf("Primeiro caractere: %c\n", fixed_text[0]); 
  
    // array de char para evitar o '\0' implícito de string
    char pattern[MAX_PATTERN_SIZE] = {'A','T','G','C'}; // Padrão de tamanho fixo 
    //printf("Conteúdo de pattern:\n%s\n", pattern);

    int text_len = MAX_TEXT_SIZE;
    int pattern_len = MAX_PATTERN_SIZE;

  
    
    //("\n %zu\n",sizeof(int));
    // Ocupa MAX_OCCURRENCES * sizeof(int) = 40 bytes na pilha (stack) da main.
    // Array para armazenar as ocorrências.
    int occurrences[MAX_OCCURRENCES];

    // Chamada para a função de busca
    int count = z_search(fixed_text, pattern, text_len, pattern_len, occurrences);

    // Impressão dos resultados (via UART)
    printf("Z-Algorithm encontrou %d ocorrencias:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Posicao: %d\n", occurrences[i]);
    }
    printf("Execucao do Z-Algorithm concluida.\n");

   
    // while (1) {
    //    
    // }

    return 0; 
}