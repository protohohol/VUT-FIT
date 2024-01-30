// wordcount.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "htab_pvt.h"
#include "io.h"

#define MAX_LEN 127
#define SIZE 2100

void print_table(htab_t* t) {
  for (size_t arr = 0; arr < t->arr_size; arr++) {
    while (t->arr_ptr[arr] != NULL) {
      printf("[%s, %d] --> ", t->arr_ptr[arr]->record->key, t->arr_ptr[arr]->record->value);
      t->arr_ptr[arr] = t->arr_ptr[arr]->next;
    }
    printf("\n");
  }
}

void print_data(htab_pair_t *data)
{
    printf("%s\t%d\n", data->key, data->value);
}

int main() {
    htab_t* t = htab_init(SIZE);

    if (t == NULL) {
        fprintf(stderr, "Cannot allocate memory for table!\n");
        return 1;
    }

    char inp[MAX_LEN + 1];
    int length = 0;
    bool len_f = 0; // overlength flag

    while ((length = read_word(inp, MAX_LEN, stdin)) != -1) {
        if (length == 0) {
            continue;
        }

        if (length > MAX_LEN - 1 && !len_f) {
            fprintf(stderr, "Your word is longer than allowed!\n");
            len_f = 1;
        }

        htab_pair_t* buf = htab_lookup_add(t, inp);
        if (buf == NULL) {
            fprintf(stderr, "htab_lookup_add doesn't work!\n");
            htab_free(t);
            return 1;
        }
        (buf->value)++;
    }

    void (*f)(htab_pair_t *data) = &print_data;
    htab_for_each(t, f);

    htab_free(t);

    return 0;
}
