
#include <optimizer/value.h>
#include <stdlib.h>
#include <stdio.h>

void mr_value_free(
    mr_node_t *value)
{
    if (value->type < MR_NODE_COUNT)
    {
        mr_node_free(value);
        return;
    }

    switch (value->type)
    {
    case MR_VALUE_CINT:
        free(value->value);
        break;
    }
}

void mr_values_free(
    mr_node_t *values, mr_long_t size)
{
    while (size--)
        mr_value_free(values + size);
    free(values);
}
