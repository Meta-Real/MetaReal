
#ifndef __MR_OPTIMIZER
#define __MR_OPTIMIZER

#include <optimizer/value.h>
#include <error/error.h>

#pragma pack(push, 1)
struct __MR_OPTIMIZER_T
{
    mr_node_t *values;
    mr_long_t size;

    mr_invalid_semantic_t error;
};
#pragma pack(pop)
typedef struct __MR_OPTIMIZER_T mr_optimizer_t;

mr_byte_t mr_optimizer(
    mr_optimizer_t *res, mr_node_t *values, mr_long_t size);

#endif
