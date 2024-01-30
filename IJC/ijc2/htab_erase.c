// htab_erase.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdlib.h>
#include <stdio.h>
#include "htab_pvt.h"

bool htab_erase(htab_t * t, htab_key_t key) {
    htab_pair_t* record = htab_find(t, key);
    if (record == NULL) {
        return false;
    }

    size_t hash_index = htab_hash_function(key) % htab_bucket_count(t);
    struct htab_item* ptr = t->arr_ptr[hash_index];
    if (strcmp(ptr->record->key, key)) {
        while (strcmp(ptr->next->record->key, key)) {
            ptr = ptr->next;
        }
        struct htab_item* ptr_buf = ptr;
        ptr = ptr->next;
        ptr_buf->next = ptr->next;
        (t->size)--;

        free((char*)ptr->record->key);
        free(ptr->record);
        free(ptr);
    } else {
        struct htab_item* ptr_buf = ptr->next;
        (t->size)--;

        free((char*)ptr->record->key);
        free(ptr->record);
        free(ptr);

        t->arr_ptr[hash_index] = ptr_buf;
    }

    if (t->size/t->arr_size < AVG_LEN_MIN) {
        htab_resize(t, t->arr_size / 2);
    }

    return true;
}
