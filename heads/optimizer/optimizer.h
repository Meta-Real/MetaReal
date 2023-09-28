/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_OPTIMIZER__
#define __MR_OPTIMIZER__

#include <optimizer/value.h>
#include <parser/node.h>
#include <error/error.h>

struct __OPT_RES_T
{
    value_t **values;
    uint64_t size;

    invalid_semantic_t error;
};
typedef struct __OPT_RES_T opt_res_t;

struct __VISIT_RES_T
{
    value_t *value;
    invalid_semantic_t error;
};
typedef struct __VISIT_RES_T visit_res_t;

opt_res_t optimize(node_t *nodes, uint64_t size);

#endif /* __MR_OPTIMIZER__ */
