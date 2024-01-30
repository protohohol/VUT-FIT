// htab_clear.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdlib.h>
#include <stdio.h>
#include "htab_pvt.h"

void htab_clear(htab_t * t) {
    for (size_t i = 0; i < htab_bucket_count(t); i++) {
        struct htab_item* buf_ptr;

        while (t->arr_ptr[i] != NULL) {
            buf_ptr = t->arr_ptr[i]->next;

            free((char*)t->arr_ptr[i]->record->key);
            free(t->arr_ptr[i]->record);
            free(t->arr_ptr[i]);

            t->arr_ptr[i] = buf_ptr;
        }
    }

    t->size = 0;
}
