// htab_free.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdlib.h>
#include <stdio.h>
#include "htab_pvt.h"

void htab_free(htab_t * t) {
    htab_clear(t);
    free(t);
}
