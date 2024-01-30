// htab_lookup_add.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdlib.h>
#include <stdio.h>
#include "htab_pvt.h"


htab_pair_t* htab_lookup_add(htab_t* t, htab_key_t key) {
    htab_pair_t* record = htab_find(t, key);

    if (record == NULL) {
        size_t hash_index = htab_hash_function(key) % htab_bucket_count(t);
        struct htab_item* ptr = t->arr_ptr[hash_index];

        if (ptr == NULL) {
            ptr = malloc(sizeof(struct htab_item));
            if (ptr == NULL) {
                return NULL;
            }
            ptr->next = NULL;
            ptr->record = malloc(sizeof(htab_pair_t));
            if (ptr->record == NULL) {
                return NULL;
            }
            ptr->record->key = calloc(strlen(key) + 1, sizeof(htab_key_t));
            if (ptr->record->key == NULL) {
                return NULL;
            }
            strcpy((char*)ptr->record->key, key);
            ptr->record->value = 0;
            record = ptr->record;
            t->arr_ptr[hash_index] = ptr;
            (t->size)++;
        } else {
            struct htab_item* ptr_buf;
            while (ptr != NULL) {
                if (ptr->next == NULL) {
                    ptr_buf = ptr;
                }
                ptr = ptr->next;
            }

            ptr = malloc(sizeof(struct htab_item));
            if (ptr == NULL) {
                return NULL;
            }
            ptr->next = NULL;
            ptr->record = malloc(sizeof(htab_pair_t));
            if (ptr->record == NULL) {
                return NULL;
            }
            ptr->record->key = calloc(strlen(key) + 1, sizeof(htab_key_t));
            if (ptr->record->key == NULL) {
                return NULL;
            }
            strcpy((char*)ptr->record->key, key);
            ptr->record->value = 0;
            record = ptr->record;
            ptr_buf->next = ptr;
            (t->size)++;
        }
    }

    if (t->size/t->arr_size > AVG_LEN_MAX) {
        htab_resize(t, t->arr_size * 2);
    }

    return record;
}
