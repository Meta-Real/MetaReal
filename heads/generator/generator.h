/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_GENERATOR__
#define __MR_GENERATOR__

#include <optimizer/optimizer.h>

struct __GEN_RES_T
{
    char *consts;
    char *main;
};
typedef struct __GEN_RES_T gen_res_t;

gen_res_t generate(value_t *values, uint64_t size);

#endif /* __MR_GENERATOR__ */
