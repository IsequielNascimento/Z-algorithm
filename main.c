#include <stdio.h>
#include <string.h>

#define MAX_TEXT_LEN 1000
#define MAX_PATTERN_LEN 100

// Builds the Z array for the given string
void getZarr(const char *str, int Z[], int n) {
    int L = 0, R = 0, k;

    for (int i = 1; i < n; ++i) {
        if (i > R) {
            L = R = i;
            while (R < n && str[R - L] == str[R])
                R++;
            Z[i] = R - L;
            R--;
        } else {
            k = i - L;
            if (Z[k] < R - i + 1)
                Z[i] = Z[k];
            else {
                L = i;
                while (R < n && str[R - L] == str[R])
                    R++;
                Z[i] = R - L;
                R--;
            }
        }
    }
}



// Searches for the pattern in the text using Z algorithm
void search(const char *text, const char *pattern) {
    char concat[MAX_PATTERN_LEN + 1 + MAX_TEXT_LEN + 1]; // pattern + '$' + text + '\0'
    int Z[MAX_PATTERN_LEN + 1 + MAX_TEXT_LEN];           // Z array size matches concat length

    // Form the concatenated string
    int pat_len = strlen(pattern);
    int text_len = strlen(text);
    int concat_len = pat_len + 1 + text_len;

    // Constructing the concat string
    int i, j = 0;
    for (i = 0; i < pat_len; ++i)
        concat[j++] = pattern[i];
    concat[j++] = '$';
    for (i = 0; i < text_len; ++i)
        concat[j++] = text[i];
    concat[j] = '\0';

    // Get Z array for concat
    getZarr(concat, Z, concat_len);

    // Loop through Z array to find pattern matches
    for (i = 0; i < concat_len; ++i) {
        if (Z[i] == pat_len) {
            printf("Pattern found at index %d\n", i - pat_len - 1);
        }
    }
}

int main() {
    char text[MAX_TEXT_LEN] = "GEEKS FOR GEEKS";
    char pattern[MAX_PATTERN_LEN] = "GEEK";
    search(text, pattern);
    return 0;
}