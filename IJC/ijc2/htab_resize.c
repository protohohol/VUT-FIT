// htab_resize.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdlib.h>
#include <stdio.h>
#include "htab_pvt.h"

void htab_resize(htab_t* t, size_t newn) {
    htab_t* n_t = htab_init(newn);

    for (size_t i = 0; i < htab_bucket_count(t); i++) {
        struct htab_item* cur_ptr = t->arr_ptr[i];

        while (cur_ptr != NULL) {
            htab_pair_t* new = htab_lookup_add(n_t, cur_ptr->record->key);

            cur_ptr = cur_ptr->next;
        }
    }

    htab_clear(t);
    t = n_t;
}
