// htab_find.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdio.h>
#include "htab_pvt.h"

htab_pair_t* htab_find(htab_t* t, htab_key_t key) {
    size_t hash_index = htab_hash_function(key) % htab_bucket_count(t);

    struct htab_item* ptr = t->arr_ptr[hash_index];
    htab_pair_t* record = NULL;

    while (ptr != NULL && strcmp(ptr->record->key, key)) {
        ptr = ptr->next;
    }
    
    if (ptr != NULL) {
        record = ptr->record;
    }

    return record;
}
