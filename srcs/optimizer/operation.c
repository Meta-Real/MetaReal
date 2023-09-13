/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <optimizer/float.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

#define bin_operation(l, r, f1, f2)       \
    do                                    \
    {                                     \
        f1(l->value, l->value, r->value); \
        res.value = *l;                   \
                                          \
        f2(r->value);                     \
        return res;                       \
    } while (0)

#define bin_operation_rev(l, r, f1, f2)   \
    do                                    \
    {                                     \
        f1(r->value, l->value, r->value); \
        res.value = *r;                   \
                                          \
        f2(l->value);                     \
        return res;                       \
    } while (0)

#define ill_op_error(o, ol)                                                                            \
    do                                                                                                 \
    {                                                                                                  \
        char *detail = mr_alloc(46 + ol + value_name_lens[left->type] + value_name_lens[right->type]); \
        sprintf(detail, "Invalid binary operation (" o ") between <%s> and <%s>",                      \
            value_names[left->type], value_names[right->type]);                                        \
                                                                                                       \
        res.has_error = 1;                                                                             \
        res.error = set_invalid_semantic(detail, ILLEGAL_OP, left->poss, right->pose);                 \
        return res;                                                                                    \
    } while (0)

#define ill_unary_op_error(o, ol)                                                   \
    do                                                                              \
    {                                                                               \
        char *detail = mr_alloc(34 + ol + value_name_lens[operand->type]);          \
        sprintf(detail, "Invalid unary operation (" o ") for <%s>",                 \
            value_names[operand->type]);                                            \
                                                                                    \
        res.has_error = 1;                                                          \
        res.error = set_invalid_semantic(detail, ILLEGAL_OP, *poss, operand->pose); \
        return res;                                                                 \
    } while (0)

#define div_by_zero_error                                                                \
    do                                                                                   \
    {                                                                                    \
        char *detail = mr_alloc(17);                                                     \
        strcpy(detail, "Division by zero");                                              \
                                                                                         \
        res.has_error = 1;                                                               \
        res.error = set_invalid_semantic(detail, DIV_BY_ZERO, right->poss, right->pose); \
        return res;                                                                      \
    } while (0)

visit_res_t compute_add(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_add, int_free);
        case FLOAT_V:
            bin_operation(right, left, float_add_int, int_free);
        }

        free_value(right);
        int_free(left->value);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_add_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, float_add, float_free);
        }

        free_value(right);
        float_free(left->value);
        break;
    }

    ill_op_error("+", 1);
}

visit_res_t compute_sub(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_sub, int_free);
        case FLOAT_V:
            bin_operation_rev(left, right, float_int_sub, int_free);
        }
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_sub_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, float_sub, float_free);
        }
    }

    ill_op_error("-", 1);
}

visit_res_t compute_mul(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_mul, int_free);
        case FLOAT_V:
            bin_operation(right, left, float_mul_int, int_free);
        }
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_mul_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, float_mul, float_free);
        }
    }

    ill_op_error("*", 1);
}

visit_res_t compute_div(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                int_free(right->value);
                int_free(left->value);
                div_by_zero_error;
            }

            res.value.type = FLOAT_V;
            res.value.value = float_int_div_int(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                int_free(left->value);
                div_by_zero_error;
            }

            bin_operation_rev(left, right, float_int_div, int_free);
        }
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                int_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(left, right, float_div_int, int_free);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(left, right, float_div, float_free);
        }
    }

    ill_op_error("/", 1);
}

visit_res_t compute_pos(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.has_error = 0;

    switch (operand->type)
    {
    case INT_V:
    case FLOAT_V:
        res.value = *operand;
        return res;
    }

    ill_unary_op_error("+", 1);
}

visit_res_t compute_neg(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.has_error = 0;

    switch (operand->type)
    {
    case INT_V:
        int_neg(operand->value, operand->value);
        res.value = *operand;
        return res;
    case FLOAT_V:
        float_neg(operand->value, operand->value);
        res.value = *operand;
        return res;
    }

    ill_unary_op_error("-", 1);
}
