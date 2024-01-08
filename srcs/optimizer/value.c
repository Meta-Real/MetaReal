
#include <optimizer/value.h>
#include <stdlib.h>
#include <stdio.h>

mr_str_ct mr_value_label[MR_VALUE_COUNT] =
{
    "VALUE_CINT"
};

void mr_value_print(mr_node_t *value);

#ifndef __MR_DEBUG__
inline void mr_value_cint_print(mr_value_cint_t *value)
#else
void mr_value_cint_print(mr_value_cint_t *value)
#endif
{
    printf("%" PRIu64, value->value);
}

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

void mr_values_print(
    mr_node_t *values, mr_long_t size)
{
    for (mr_long_t i = 0; i != size; i++)
    {
        mr_value_print(values + i);
        putchar('\n');
    }
}

void mr_value_print(mr_node_t *value)
{
    if (value->type < MR_NODE_COUNT)
    {
        mr_node_print(value);
        return;
    }

    printf("(%s: ", mr_value_label[value->type - MR_NODE_COUNT]);

    switch (value->type)
    {
    case MR_VALUE_CINT:
        mr_value_cint_print(value->value);
        break;
    }

    putchar(')');
}
