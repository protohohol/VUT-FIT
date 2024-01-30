// bitset.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#include "bitset.h"

#ifdef USE_INLINE

extern void bitset_free(bitset_t* name);
extern unsigned long bitset_size(bitset_t* name);
extern void bitset_setbit(bitset_t* name, bitset_index_t index, int expression);
extern unsigned bitset_getbit(bitset_t* name, bitset_index_t index);

#endif
