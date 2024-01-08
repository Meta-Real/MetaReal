
#ifndef __MR_VALUE__
#define __MR_VALUE__

#include <parser/node.h>

enum __MR_VALUE_ENUM
{
    MR_VALUE_CINT = MR_NODE_COUNT,
};

#define MR_VALUE_COUNT (MR_VALUE_CINT - MR_NODE_COUNT + 1)

#pragma pack(push, 1)
struct __MR_VALUE_CINT_T
{
    mr_longlong_t value;

    mr_long_t sidx;
    mr_short_t size;
};
#pragma pack(pop)
typedef struct __MR_VALUE_CINT_T mr_value_cint_t;

void mr_value_free(
    mr_node_t *value);

void mr_values_free(
    mr_node_t *values, mr_long_t size);

void mr_values_print(
    mr_node_t *values, mr_long_t size);

#endif
