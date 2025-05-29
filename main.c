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





