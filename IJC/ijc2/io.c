// io.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <ctype.h>
#include "io.h"

int read_word(char *s, int max, FILE *f) {

    if (f == NULL) {
        fprintf(stderr, "Cannot open the file!\n");
        return -1;
    }

    int c = 0;
    int length = 0;

    while ((c = getc(f)) != EOF) {
        if (isspace(c)) {
            break;
        } else {
            length++;
            if (length < max) {
                s[length - 1] = c;
            }
        }
    }

    s[length] = '\0';
    
    if (c == EOF) {
        length = c;
    }

    return length;
}
