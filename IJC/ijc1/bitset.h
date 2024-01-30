// bitset.h
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Vsevolod Tiemnohorov, FIT
// Přeloženo: gcc 7.5.0

#ifndef BITSET_H
#define BITSET_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "error.h"

#define MAX_SIZE_A 300000000UL

typedef unsigned long bitset_t;
typedef unsigned long bitset_index_t;

#define size_calc(size) (unsigned long)size / (CHAR_BIT * sizeof(unsigned long)) + \
    ((unsigned long)size % (CHAR_BIT * sizeof(unsigned long)) ? 1 : 0) + 1

#define index_calc(index) (unsigned long)index / (CHAR_BIT * sizeof(unsigned long)) + 1

#define bitset_create(name, size) \
    static_assert((unsigned long)size <= MAX_SIZE_A, "Wrong size of array!\n"); \
    bitset_t name[size_calc(size)] = {(unsigned long)size};

#define bitset_alloc(name, size) \
    assert((unsigned long)size <= MAX_SIZE_A); \
    bitset_t* name = calloc(size_calc(size), sizeof(unsigned long)); \
    if (name == NULL) error_exit("bitset_alloc : memory allocation error!\n"); \
    name[0] = size;

#ifdef USE_INLINE

inline void bitset_free(bitset_t* name) {
    free(name);
}

inline unsigned long bitset_size(bitset_t* name) {
    return name[0];
}

inline void bitset_setbit(bitset_t* name, bitset_index_t index, int expression) {
    if (index > bitset_size(name)) { 
        printf("Index out of range!\n"); 
        exit(1); 
    }
    expression == 0 ? (name[index_calc(index)] &= ~(1UL << (unsigned long)index % (CHAR_BIT * sizeof(unsigned long))))
    : (name[index_calc(index)] |= (1UL << (unsigned long)index % (CHAR_BIT * sizeof(unsigned long))));
}

inline unsigned bitset_getbit(bitset_t* name, bitset_index_t index) {
    return (unsigned long)index > bitset_size(name) ? (error_exit("bitset_getbit: Index %lu out of range 0..%lu",
            (unsigned long)index, (unsigned long)bitset_size(name)), 1) : (name[index_calc(index)]
    & (1UL << ((unsigned long)index % (CHAR_BIT * sizeof(unsigned long)))) ? 1 : 0);
}

#else

#define bitset_free(name) free(name)

#define bitset_size(name) name[0]

#define bitset_setbit(name, index, expression) \
    (unsigned long)index > bitset_size(name) ? (error_exit("bitset_getbit: Index %lu out of range 0..%lu\n", \
    (unsigned long)index, bitset_size(name)), 1) : \
    (expression == 0 ? (name[index_calc(index)] &= ~(1UL << (unsigned long)index % (CHAR_BIT * sizeof(unsigned long)))) \
    : (name[index_calc(index)] |= (1UL << (unsigned long)index % (CHAR_BIT * sizeof(unsigned long)))))

#define bitset_getbit(name, index) \
    ((unsigned long)index > bitset_size(name)) ? (error_exit("bitset_getbit: Index %lu out of range 0..%lu\n", \
    (unsigned long)index, bitset_size(name)), 1) : (name[index_calc(index)] \
    & (1UL << ((unsigned long)index % (CHAR_BIT * sizeof(unsigned long)))) ? 1 : 0)

#endif

#endif
