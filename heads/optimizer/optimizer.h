
#ifndef __MR_OPTIMIZER
#define __MR_OPTIMIZER

#include <parser/node.h>
#include <error/error.h>

struct __MR_OPTIMIZER_T
{
    mr_node_t *values;
    mr_long_t size;

    mr_invalid_semantic_t error;
};
typedef struct __MR_OPTIMIZER_T mr_optimizer_t;

mr_byte_t mr_optimizer(
    mr_optimizer_t *res, mr_node_t *nodes, mr_long_t size);

#endif
