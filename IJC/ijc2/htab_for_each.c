// htab_for_each.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdio.h>
#include "htab_pvt.h"

void htab_for_each(const htab_t* t, void (*f)(htab_pair_t *data)) {
    for (size_t i = 0; i < htab_bucket_count(t); i++) {
        for (struct htab_item* ptr = t->arr_ptr[i]; ptr != NULL; ptr = ptr->next) {
            (*f)(ptr->record);
        }
    }
}
