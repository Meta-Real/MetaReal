/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <optimizer/complex.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

#define bin_operation(l, r, f, ff) \
    do                             \
    {                              \
        f(l->value, r->value);     \
        res.value = *l;            \
                                   \
        ff(r->value);              \
        return res;                \
    } while (0)

#define bin_operation_rev(l, r, f, ff) \
    do                                 \
    {                                  \
        f(l->value, r->value);         \
        res.value = *r;                \
                                       \
        ff(l->value);                  \
        return res;                    \
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

#define mod_by_zero_error                                                                \
    do                                                                                   \
    {                                                                                    \
        char *detail = mr_alloc(15);                                                     \
        strcpy(detail, "Modulo by zero");                                                \
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
        case COMPLEX_V:
            bin_operation(right, left, complex_add_int, int_free);
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_add_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, float_add, float_free);
        case COMPLEX_V:
            bin_operation(right, left, complex_add_float, float_free);
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, complex_add_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, complex_add_float, float_free);
        case COMPLEX_V:
            bin_operation(left, right, complex_add, complex_free);
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
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
        case COMPLEX_V:
            bin_operation_rev(left, right, complex_int_sub, int_free);
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_sub_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, float_sub, float_free);
        case COMPLEX_V:
            bin_operation_rev(left, right, complex_float_sub, float_free);
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, complex_sub_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, complex_sub_float, float_free);
        case COMPLEX_V:
            bin_operation(left, right, complex_sub, complex_free);
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
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
        case COMPLEX_V:
            bin_operation(right, left, complex_mul_int, int_free);
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_mul_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, float_mul, float_free);
        case COMPLEX_V:
            bin_operation(right, left, complex_mul_float, float_free);
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, complex_mul_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, complex_mul_float, float_free);
        case COMPLEX_V:
            bin_operation(left, right, complex_mul, complex_free);
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
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
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                complex_free(right->value);
                int_free(left->value);
                div_by_zero_error;
            }

            bin_operation_rev(left, right, complex_int_div, int_free);
        }

        free_value(right);
        int_free(left->value);
        goto ret;
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
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                complex_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation_rev(left, right, complex_float_div, float_free);
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                int_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(left, right, complex_div_int, int_free);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(left, right, complex_div_float, float_free);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                complex_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(left, right, complex_div, complex_free);
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("/", 1);
}

visit_res_t compute_mod(value_t *left, value_t *right)
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
                mod_by_zero_error;
            }

            bin_operation(left, right, int_mod, int_free);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                int_free(left->value);
                mod_by_zero_error;
            }

            bin_operation_rev(left, right, float_int_mod, int_free);
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                int_free(right->value);
                float_free(left->value);
                mod_by_zero_error;
            }

            bin_operation(left, right, float_mod_int, int_free);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                mod_by_zero_error;
            }

            bin_operation(left, right, float_mod, float_free);
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("%%", 1);
}

visit_res_t compute_quot(value_t *left, value_t *right)
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

            bin_operation(left, right, int_quot, int_free);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                int_free(left->value);
                div_by_zero_error;
            }

            bin_operation(left, right, float_int_quot, float_free);
        }

        free_value(right);
        int_free(left->value);
        goto ret;
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

            bin_operation_rev(left, right, float_quot_int, float_free);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            res.value.type = INT_V;
            res.value.value = float_quot(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("//", 2);
}

visit_res_t compute_pow(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_isneg(right->value))
            {
                res.value.type = FLOAT_V;
                res.value.value = float_int_pow_int(left->value, right->value);

                int_free(right->value);
                int_free(left->value);
                return res;
            }

            bin_operation(left, right, int_pow, int_free);
        case FLOAT_V:
            if (int_isneg(left->value))
            {
                res.value.type = COMPLEX_V;
                res.value.value = complex_int_pow_float(left->value, right->value);

                float_free(right->value);
                int_free(left->value);
                return res;
            }

            bin_operation_rev(left, right, float_int_pow, int_free);
        case COMPLEX_V:
            bin_operation_rev(left, right, complex_int_pow, int_free);
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_pow_int, int_free);
        case FLOAT_V:
            if (float_isneg(left->value))
            {
                res.value.type = COMPLEX_V;
                res.value.value = complex_float_pow_float(left->value, right->value);

                float_free(right->value);
                float_free(left->value);
                return res;
            }

            bin_operation(left, right, float_pow, float_free);
        case COMPLEX_V:
            bin_operation_rev(left, right, complex_float_pow, float_free);
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, complex_pow_int, int_free);
        case FLOAT_V:
            bin_operation(left, right, complex_pow_float, float_free);
        case COMPLEX_V:
            bin_operation(left, right, complex_pow, complex_free);
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("**", 2);
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

    free_value(operand);
    ill_unary_op_error("+", 1);
}

visit_res_t compute_neg(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.has_error = 0;

    switch (operand->type)
    {
    case INT_V:
        int_neg(operand->value);
        res.value = *operand;
        return res;
    case FLOAT_V:
        float_neg(operand->value);
        res.value = *operand;
        return res;
    }

    free_value(operand);
    ill_unary_op_error("-", 1);
}
