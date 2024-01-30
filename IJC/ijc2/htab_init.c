// htab_init.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdlib.h>
#include "htab_pvt.h"
#include "htab.h"

htab_t* htab_init(size_t n) {
    htab_t* t = malloc(sizeof(size_t) + (n * sizeof(htab_t)));

    if (t == NULL) {
        return NULL;
    }

    t->size = 0;
    t->arr_size = n;
    for (size_t i = 0; i < n; i++) {
        t->arr_ptr[i] = NULL;
    }

    return t;
}
