// htab_bucket_count.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include "htab_pvt.h"

size_t htab_bucket_count(const htab_t* t) {
    return t->arr_size;
}
