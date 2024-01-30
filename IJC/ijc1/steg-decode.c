// steg-decode.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include "ppm.h"
#include "eratosthenes.h"
#include "bitset.h"

int main (int argc, char* argv[]) {
    if (argc != 2) {
        error_exit("Wrong number of arguments!\n");
    }

    const char* filename = argv[1];
    struct ppm* image = ppm_read(filename);
    if (image == NULL) {
        return 1;
    }

    bitset_alloc(bit_array, (image->xsize * image->ysize * 3));
    eratosthenes(bit_array);

    char word = 0;
    unsigned counter = 0;

    for (unsigned long i = 29; i < bitset_size(bit_array); i++) {
        if (bitset_getbit(bit_array, i) == 0) {
            if ((image->data[i+1] & 1) == 1) {
                word |= (1 << counter);
            } else {
                word &= ~(1 << counter);
            }

            if (counter < 7) {
                counter++;
            } else {
                if (word == '\0') {
                    printf("\n");
                    break;
                }
                printf("%c", word);
                counter = 0;
                word = 0;
            }
        }
    }

    bitset_free(bit_array);
    ppm_free(image);

    return 0;
}
