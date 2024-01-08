
#include <optimizer/operation.h>
#include <stdlib.h>

void mr_operation_add(
    mr_node_t *left, mr_node_t *right)
{
    switch (left->type)
    {
    case MR_VALUE_CINT:
    {
        mr_value_cint_t *lvalue = left->value;
        switch (right->type)
        {
        case MR_VALUE_CINT:
        {
            mr_value_cint_t *rvalue = right->value;
            lvalue->value += rvalue->value;

            lvalue->size = (mr_short_t)(rvalue->sidx - lvalue->sidx) + rvalue->size;
            free(rvalue);
            return;
        }
        }
    }
    }
}
