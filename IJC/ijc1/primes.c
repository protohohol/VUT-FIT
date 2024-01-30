// primes.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include "eratosthenes.h"
#include <time.h>

int main() {
    clock_t start = clock();

    bitset_alloc(bit_array, MAX_SIZE_A);
    eratosthenes(bit_array);

    unsigned long counter = 10;
    unsigned primes[9];

    for (unsigned long i = MAX_SIZE_A - 1; (counter > 0) && (i >= 2); i--) {
        if (bitset_getbit(bit_array, i) == 0) {
            primes[--counter] = i;
        }
    }

    if (counter) {
        warning_msg("There are only %lu primes!\n", 10UL - counter);
    }

    for (unsigned i = counter; i < 10; i++) {
        printf("%u\n", primes[i]);
    }

    bitset_free(bit_array);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);

    return 0;
}
