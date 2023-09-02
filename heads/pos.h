/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_POS__
#define __MR_POS__

#include <stdint.h>

struct __POS_T
{
    uint64_t idx;
    uint64_t ln;
};
typedef struct __POS_T pos_t;

#define set_pos(i, l) ((pos_t){i, l})

#endif /* __MR_POS__ */
