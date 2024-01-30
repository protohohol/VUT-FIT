/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
    int result = 1;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
    result += key[i];
    }
    return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    if (table != NULL) {
        for (int i = 0; i < HT_SIZE; i++) {
            (*table)[i] = NULL;
        }
    } else {
        return;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    if (table != NULL && key != NULL) {
        int hash = get_hash(key);
        ht_item_t *cur_item = (*table)[hash];
        if (cur_item != NULL) {
            while (cur_item != NULL) {
                if (!strcmp(cur_item->key, key)) {
                    return cur_item;
                }
                cur_item = cur_item->next;
            }
        }
    }
    return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    if (table != NULL && key != NULL) {
        ht_item_t *f_item = ht_search(table, key);
        if (f_item != NULL) {
            f_item->value = value;
        } else {
            int hash = get_hash(key);
            ht_item_t *cur_item = (*table)[hash];
            ht_item_t *new_item = (ht_item_t*) malloc(sizeof(ht_item_t));
            if (new_item == NULL) {
                return;
            }
            new_item->key = key;
            new_item->value = value;
            new_item->next = NULL;
            if(cur_item != NULL) {
                new_item->next = cur_item;
            }
            (*table)[hash] = new_item;
        }
    } else {
        return;
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    if (table != NULL && key != NULL) {
        ht_item_t *f_item = ht_search(table, key);
        if(f_item != NULL) {
            return &(f_item->value);
        }
    }
    return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    if (table != NULL && key != NULL) {
        int hash = get_hash(key);
        ht_item_t *cur_item = (*table)[hash];
        ht_item_t *next_item = cur_item->next;
        if (cur_item != NULL) {
            if (!strcmp(cur_item->key, key)) {
                (*table)[hash] = next_item;
            } else {
                ht_item_t *prev_item;
                while (strcmp(cur_item->key, key)) {
                    prev_item = cur_item;
                    cur_item = next_item;
                    if (cur_item != NULL) {
                        next_item = cur_item->next;
                        if (!strcmp(cur_item->key, key)) {
                            prev_item->next = next_item;
                            free(cur_item);
                            return;
                        }
                    }
                }
            }
        }
    } else {
        return;
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
    if (table != NULL) {
        for (int i = 0; i < HT_SIZE; i++) {
            ht_item_t *cur_item = (*table)[i];
            ht_item_t *next_item;
            while (cur_item != NULL) {
                next_item = cur_item->next;
                free(cur_item);
                cur_item = next_item;
            }
            (*table)[i] = NULL;
        }
    } else {
        return;
    }
}
