/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_OPTIMIZER__
#define __MR_OPTIMIZER__

#include <optimizer/value.h>
#include <parser/node.h>

struct __OPT_RES_T
{
    value_t *values;
    uint64_t size;
};
typedef struct __OPT_RES_T opt_res_t;

struct __VISIT_RES_T
{
    value_t value;
};
typedef struct __VISIT_RES_T visit_res_t;

opt_res_t optimize(node_t *nodes, uint64_t size);

#endif /* __MR_OPTIMIZER__ */
