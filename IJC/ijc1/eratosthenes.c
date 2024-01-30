// eratosthenes.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include "eratosthenes.h"

void eratosthenes(bitset_t* bit_array) {
    bitset_setbit(bit_array, 0, 1);
    bitset_setbit(bit_array, 1, 1);

    for (unsigned long i = 2; i <= (unsigned long)sqrt(bitset_size(bit_array)); i++) {
        if (bitset_getbit(bit_array, i) == 0) {
            for (unsigned long m = i * i; m <= (unsigned long)bitset_size(bit_array) - 1; m += i) {
                bitset_setbit(bit_array, m, 1);
            }
        }
    }
}
