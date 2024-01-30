#ifndef __HTAB_PVT_H__
#define __HTAB_PVT_H__

#include "htab.h"

#define AVG_LEN_MIN (size_t)100
#define AVG_LEN_MAX (size_t)200

struct htab_item {
    struct htab_item* next;
    htab_pair_t* record;
};

struct htab {
    size_t size;
    size_t arr_size;
    struct htab_item* arr_ptr[];
};

#endif // __HTAB_PVT_H__
