/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <optimizer/complex.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

#define bin_operation(l, r, f, ff)  \
    do                              \
    {                               \
        f(l->value, r->value);      \
        res.value.type = l->type;   \
        res.value.value = l->value; \
                                    \
        ff(r->value);               \
        return res;                 \
    } while (0)

#define bin_operation_rev(l, r, f, ff) \
    do                                 \
    {                                  \
        f(l->value, r->value);         \
        res.value.type = r->type;      \
        res.value.value = r->value;    \
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

#define ztn_power_error                                                                  \
    do                                                                                   \
    {                                                                                    \
        char *detail = mr_alloc(46);                                                     \
        strcpy(detail, "Zero raised to the power of a negative number");                 \
                                                                                         \
        res.has_error = 1;                                                               \
        res.error = set_invalid_semantic(detail, DIV_BY_ZERO, right->poss, right->pose); \
        return res;                                                                      \
    } while (0)

#define ztc_power_error                                                                  \
    do                                                                                   \
    {                                                                                    \
        char *detail = mr_alloc(45);                                                     \
        strcpy(detail, "Zero raised to the power of a complex number");                  \
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
        case BOOL_V:
            if (right->value)
                int_add_ui(left->value, 1);

            res.value.type = INT_V;
            res.value.value = left->value;
            return res;
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
        case BOOL_V:
            if (right->value)
                float_add_ui(left->value, 1);

            res.value.type = FLOAT_V;
            res.value.value = left->value;
            return res;
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
        case BOOL_V:
            if (right->value)
                complex_add_ui(left->value, 1);

            res.value.type = COMPLEX_V;
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (left->value)
                int_add_ui(right->value, 1);

            res.value.type = INT_V;
            res.value.value = right->value;
            return res;
        case FLOAT_V:
            if (left->value)
                float_add_ui(right->value, 1);

            res.value.type = FLOAT_V;
            res.value.value = right->value;
            return res;
        case COMPLEX_V:
            if (left->value)
                complex_add_ui(right->value, 1);

            res.value.type = COMPLEX_V;
            res.value.value = right->value;
            return res;
        case BOOL_V:
            res.value.type = BOOL_V;
            res.value.value = int_set_ui((uintptr_t)left->value + (uintptr_t)right->value);
            return res;
        }

        free_value(right);
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
        case BOOL_V:
            if (right->value)
                int_sub_ui(left->value, 1);

            res.value.type = INT_V;
            res.value.value = left->value;
            return res;
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
        case BOOL_V:
            if (right->value)
                float_sub_ui(left->value, 1);

            res.value.type = FLOAT_V;
            res.value.value = left->value;
            return res;
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
        case BOOL_V:
            if (right->value)
                complex_sub_ui(left->value, 1);

            res.value.type = COMPLEX_V;
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (left->value)
                int_ui_sub(1, right->value);
            else
                int_neg(right->value);

            res.value.type = INT_V;
            res.value.value = right->value;
            return res;
        case FLOAT_V:
            if (left->value)
                float_ui_sub(1, right->value);
            else
                float_neg(right->value);

            res.value.type = FLOAT_V;
            res.value.value = right->value;
            return res;
        case COMPLEX_V:
            if (left->value)
                complex_ui_sub(1, right->value);
            else
                complex_neg(right->value);

            res.value.type = COMPLEX_V;
            res.value.value = right->value;
            return res;
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value - (uintptr_t)right->value);
            return res;
        }

        free_value(right);
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
        case BOOL_V:
            if (right->value)
                res.value.value = left->value;
            else
            {
                int_free(left->value);
                res.value.value = int_set_ui(0);
            }

            res.value.type = INT_V;
            return res;
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
        case BOOL_V:
            if (right->value)
                res.value.value = left->value;
            else
            {
                float_free(left->value);
                res.value.value = float_set_ui(0);
            }

            res.value.type = FLOAT_V;
            return res;
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
        case BOOL_V:
            if (right->value)
                res.value.value = left->value;
            else
            {
                complex_free(left->value);
                res.value.value = complex_set_ui(0);
            }

            res.value.type = COMPLEX_V;
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (left->value)
                res.value.value = right->value;
            else
            {
                int_free(right->value);
                res.value.value = int_set_ui(0);
            }

            res.value.type = INT_V;
            return res;
        case FLOAT_V:
            if (left->value)
                res.value.value = right->value;
            else
            {
                float_free(right->value);
                res.value.value = float_set_ui(0);
            }

            res.value.type = FLOAT_V;
            return res;
        case COMPLEX_V:
            if (left->value)
                res.value.value = right->value;
            else
            {
                complex_free(right->value);
                res.value.value = complex_set_ui(0);
            }

            res.value.type = COMPLEX_V;
            return res;
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value & (uintptr_t)right->value);
            return res;
        }

        free_value(right);
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
        case BOOL_V:
            if (!right->value)
            {
                int_free(left->value);
                div_by_zero_error;
            }

            res.value.type = INT_V;
            res.value.value = left->value;
            return res;
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
        case BOOL_V:
            if (!right->value)
            {
                float_free(left->value);
                div_by_zero_error;
            }

            res.value.type = FLOAT_V;
            res.value.value = left->value;
            return res;
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
        case BOOL_V:
            if (!right->value)
            {
                complex_free(left->value);
                div_by_zero_error;
            }

            res.value.type = COMPLEX_V;
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                int_free(right->value);
                div_by_zero_error;
            }

            if (left->value)
                res.value.value = float_ui_div_int(1, right->value);
            else
                res.value.value = float_set_ui(0);

            res.value.type = FLOAT_V;

            int_free(right->value);
            return res;
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                div_by_zero_error;
            }

            if (left->value)
            {
                float_ui_div(1, right->value);
                res.value.value = right->value;
            }
            else
            {
                res.value.value = float_set_ui(0);
                float_free(right->value);
            }

            res.value.type = FLOAT_V;
            return res;
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                complex_free(right->value);
                div_by_zero_error;
            }

            if (left->value)
            {
                complex_ui_div(1, right->value);
                res.value.value = right->value;
            }
            else
            {
                res.value.value = complex_set_ui(0);
                complex_free(right->value);
            }

            res.value.type = COMPLEX_V;
            return res;
        case BOOL_V:
            if (!right->value)
                div_by_zero_error;

            res.value.type = FLOAT_V;
            res.value.value = float_set_ui((uintptr_t)left->value);
            return res;
        }

        free_value(right);
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
        case BOOL_V:
            if (!right->value)
            {
                int_free(left->value);
                mod_by_zero_error;
            }

            res.value.type = INT_V;
            res.value.value = int_set_ui(0);

            int_free(left->value);
            return res;
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
        case BOOL_V:
            if (!right->value)
            {
                float_free(left->value);
                mod_by_zero_error;
            }

            res.value.type = FLOAT_V;
            float_mod_ui(left->value, (uintptr_t)right->value);
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                int_free(right->value);
                mod_by_zero_error;
            }

            res.value.type = INT_V;
            res.value.value = int_set_ui(0);

            int_free(right->value);
            return res;
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                mod_by_zero_error;
            }

            res.value.type = FLOAT_V;
            float_ui_mod((uintptr_t)left->value, right->value);
            res.value.value = right->value;
            return res;
        case BOOL_V:
            if (!right->value)
                mod_by_zero_error;

            res.value.type = INT_V;
            res.value.value = int_set_ui(0);
            return res;
        }

        free_value(right);
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
        case BOOL_V:
            if (!right->value)
            {
                int_free(left->value);
                div_by_zero_error;
            }

            res.value.type = INT_V;
            res.value.value = left->value;
            return res;
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
        case BOOL_V:
            if (!right->value)
            {
                float_free(left->value);
                div_by_zero_error;
            }

            res.value.type = INT_V;
            res.value.value = float_get_int(left->value);

            float_free(left->value);
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                int_free(right->value);
                div_by_zero_error;
            }

            res.value.type = INT_V;
            int_ui_quot((uintptr_t)left->value, right->value);
            res.value.value = right->value;
            return res;
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                div_by_zero_error;
            }

            res.value.type = INT_V;
            res.value.value = float_ui_quot((uintptr_t)left->value, right->value);

            float_free(right->value);
            return res;
        case BOOL_V:
            if (!right->value)
                div_by_zero_error;

            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value);
            return res;
        }

        free_value(right);
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
                if (int_iszero(left->value))
                {
                    int_free(right->value);
                    int_free(left->value);
                    ztn_power_error;
                }

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
                if (int_iszero(left->value))
                {
                    float_free(right->value);
                    int_free(left->value);
                    ztn_power_error;
                }

                res.value.type = COMPLEX_V;
                res.value.value = complex_int_pow_float(left->value, right->value);

                float_free(right->value);
                int_free(left->value);
                return res;
            }

            bin_operation_rev(left, right, float_int_pow, int_free);
        case COMPLEX_V:
            if (int_iszero(left->value))
            {
                complex_free(right->value);
                int_free(left->value);
                ztc_power_error;
            }

            bin_operation_rev(left, right, complex_int_pow, int_free);
        case BOOL_V:
            if (right->value)
                res.value.value = left->value;
            else
            {
                res.value.value = int_set_ui(int_iszero(left->value));
                int_free(left->value);
            }

            res.value.type = INT_V;
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (float_iszero(left->value) && int_isneg(right->value))
            {
                int_free(right->value);
                float_free(left->value);
                ztn_power_error;
            }

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
            if (float_iszero(left->value) && float_isneg(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                ztn_power_error;
            }

            bin_operation(left, right, float_pow, float_free);
        case COMPLEX_V:
            if (float_iszero(left->value))
            {
                complex_free(right->value);
                float_free(left->value);
                ztc_power_error;
            }

            bin_operation_rev(left, right, complex_float_pow, float_free);
        case BOOL_V:
            if (right->value)
                res.value.value = left->value;
            else
            {
                res.value.value = float_set_ui(float_iszero(left->value));
                float_free(left->value);
            }

            res.value.type = FLOAT_V;
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            if (complex_iszero(left->value) && int_isneg(right->value))
            {
                int_free(right->value);
                complex_free(left->value);
                ztn_power_error;
            }

            bin_operation(left, right, complex_pow_int, int_free);
        case FLOAT_V:
            if (complex_iszero(left->value) && float_isneg(right->value))
            {
                float_free(right->value);
                complex_free(left->value);
                ztn_power_error;
            }

            bin_operation(left, right, complex_pow_float, float_free);
        case COMPLEX_V:
            if (complex_iszero(left->value))
            {
                complex_free(right->value);
                complex_free(left->value);
                ztc_power_error;
            }

            bin_operation(left, right, complex_pow, complex_free);
        case BOOL_V:
            if (right->value)
                res.value.value = left->value;
            else
            {
                res.value.value = complex_set_ui(complex_iszero(left->value));
                float_free(left->value);
            }

            res.value.type = COMPLEX_V;
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!left->value && int_isneg(right->value))
            {
                int_free(right->value);
                ztn_power_error;
            }

            res.value.type = INT_V;
            int_ui_pow((uintptr_t)left->value, right->value);
            res.value.value = right->value;
            return res;
        case FLOAT_V:
            if (!left->value && float_isneg(right->value))
            {
                float_free(right->value);
                ztn_power_error;
            }

            res.value.type = FLOAT_V;
            float_ui_pow((uintptr_t)left->value, right->value);
            res.value.value = right->value;
            return res;
        case COMPLEX_V:
            if (!left->value)
            {
                complex_free(right->value);
                ztc_power_error;
            }

            res.value.type = COMPLEX_V;
            complex_ui_pow((uintptr_t)left->value, right->value);
            res.value.value = right->value;
            return res;
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value | !(uintptr_t)right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("**", 2);
}

visit_res_t compute_b_and(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_and, int_free);
        case BOOL_V:
            res.value.type = INT_V;
            int_and_ui(left->value, (uintptr_t)right->value);
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.type = INT_V;
            int_and_ui(right->value, (uintptr_t)left->value);
            res.value.value = right->value;
            return res;
        case BOOL_V:
            res.value.type = BOOL_V;
            res.value.value = (void*)((uintptr_t)left->value & (uintptr_t)right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("&", 1);
}

visit_res_t compute_b_or(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_or, int_free);
        case BOOL_V:
            res.value.type = INT_V;
            int_or_ui(left->value, (uintptr_t)right->value);
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.type = INT_V;
            int_or_ui(right->value, (uintptr_t)left->value);
            res.value.value = right->value;
            return res;
        case BOOL_V:
            res.value.type = BOOL_V;
            res.value.value = (void*)((uintptr_t)left->value | (uintptr_t)right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("|", 1);
}

visit_res_t compute_b_xor(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_xor, int_free);
        case BOOL_V:
            res.value.type = INT_V;
            int_xor_ui(left->value, (uintptr_t)right->value);
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.type = INT_V;
            int_xor_ui(right->value, (uintptr_t)left->value);
            res.value.value = right->value;
            return res;
        case BOOL_V:
            res.value.type = BOOL_V;
            res.value.value = (void*)((uintptr_t)left->value ^ (uintptr_t)right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("^", 1);
}

visit_res_t compute_lshift(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_lshift, int_free);
        case BOOL_V:
            res.value.type = INT_V;
            int_lshift_ui(left->value, (uintptr_t)right->value);
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.type = INT_V;
            int_ui_lshift((uintptr_t)left->value, right->value);
            res.value.value = right->value;
            return res;
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value << (uintptr_t)right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("<<", 2);
}

visit_res_t compute_rshift(value_t *left, value_t *right)
{
    visit_res_t res;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_rshift, int_free);
        case BOOL_V:
            res.value.type = INT_V;
            int_rshift_ui(left->value, (uintptr_t)right->value);
            res.value.value = left->value;
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.type = INT_V;
            int_ui_rshift((uintptr_t)left->value, right->value);
            res.value.value = right->value;
            return res;
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value >> (uintptr_t)right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error(">>", 2);
}

visit_res_t compute_eq(value_t *left, value_t *right)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)int_eq(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_eq_int(right->value, left->value);

            float_free(right->value);
            int_free(left->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)complex_eq_int(right->value, left->value);

            complex_free(right->value);
            int_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)int_eq_ui(left->value, (uintptr_t)right->value);

            int_free(left->value);
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)float_eq_int(left->value, right->value);

            int_free(right->value);
            float_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_eq(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)complex_eq_float(right->value, left->value);

            complex_free(right->value);
            float_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)float_eq_ui(left->value, (uintptr_t)right->value);

            float_free(left->value);
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)complex_eq_int(left->value, right->value);

            int_free(right->value);
            complex_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)complex_eq_float(left->value, right->value);

            float_free(right->value);
            complex_free(left->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)complex_eq(left->value, right->value);

            complex_free(right->value);
            complex_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)complex_eq_ui(left->value, (uintptr_t)right->value);

            complex_free(left->value);
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)int_eq_ui(right->value, (uintptr_t)left->value);

            int_free(right->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_eq_ui(right->value, (uintptr_t)left->value);

            float_free(right->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)complex_eq_ui(right->value, (uintptr_t)left->value);

            complex_free(right->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value == right->value);
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    res.value.value = NULL;
    return res;
}

visit_res_t compute_neq(value_t *left, value_t *right)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!int_eq(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_eq_int(right->value, left->value);

            float_free(right->value);
            int_free(left->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)!complex_eq_int(right->value, left->value);

            complex_free(right->value);
            int_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)!int_eq_ui(left->value, (uintptr_t)right->value);

            int_free(left->value);
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!float_eq_int(left->value, right->value);

            int_free(right->value);
            float_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_eq(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)!complex_eq_float(right->value, left->value);

            complex_free(right->value);
            float_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)!float_eq_ui(left->value, (uintptr_t)right->value);

            float_free(left->value);
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!complex_eq_int(left->value, right->value);

            int_free(right->value);
            complex_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!complex_eq_float(left->value, right->value);

            float_free(right->value);
            complex_free(left->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)!complex_eq(left->value, right->value);

            complex_free(right->value);
            complex_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)!complex_eq_ui(left->value, (uintptr_t)right->value);

            complex_free(left->value);
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!int_eq_ui(right->value, (uintptr_t)left->value);

            int_free(right->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_eq_ui(right->value, (uintptr_t)left->value);

            float_free(right->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)!complex_eq_ui(right->value, (uintptr_t)left->value);

            complex_free(right->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value != right->value);
            return res;
        }

        free_value(right);
        complex_free(left->value);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    res.value.value = (void*)1;
    return res;
}

visit_res_t compute_ex_eq(value_t *left, value_t *right)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.has_error = 0;

    if (left->type == right->type)
        switch (left->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)int_eq(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_eq(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)complex_eq(left->value, right->value);

            complex_free(right->value);
            complex_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value == right->value);
            return res;
        }

    free_value(right);
    free_value(left);

    res.value.value = NULL;
    return res;
}

visit_res_t compute_ex_neq(value_t *left, value_t *right)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.has_error = 0;

    if (left->type == right->type)
        switch (left->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!int_eq(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_eq(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        case COMPLEX_V:
            res.value.value = (void*)(uintptr_t)!complex_eq(left->value, right->value);

            complex_free(right->value);
            complex_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value != right->value);
            return res;
        }

    free_value(right);
    free_value(left);

    res.value.value = (void*)1;
    return res;
}

visit_res_t compute_lt(value_t *left, value_t *right)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)int_lt(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_gt_int(right->value, left->value);

            float_free(right->value);
            int_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)int_lt_ui(left->value, (uintptr_t)right->value);

            int_free(left->value);
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)float_lt_int(left->value, right->value);

            int_free(right->value);
            float_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_lt(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)float_lt_ui(left->value, (uintptr_t)right->value);

            float_free(left->value);
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)int_gt_ui(right->value, (uintptr_t)left->value);

            int_free(right->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_gt_ui(right->value, (uintptr_t)left->value);

            float_free(right->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value < right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("<", 1);
}

visit_res_t compute_gt(value_t *left, value_t *right)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)int_gt(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_lt_int(right->value, left->value);

            float_free(right->value);
            int_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)int_gt_ui(left->value, (uintptr_t)right->value);

            int_free(left->value);
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)float_gt_int(left->value, right->value);

            int_free(right->value);
            float_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_gt(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)float_gt_ui(left->value, (uintptr_t)right->value);

            float_free(left->value);
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)int_lt_ui(right->value, (uintptr_t)left->value);

            int_free(right->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)float_lt_ui(right->value, (uintptr_t)left->value);

            float_free(right->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value > right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error(">", 1);
}

visit_res_t compute_lte(value_t *left, value_t *right)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!int_gt(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_lt_int(right->value, left->value);

            float_free(right->value);
            int_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)!int_gt_ui(left->value, (uintptr_t)right->value);

            int_free(left->value);
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!float_gt_int(left->value, right->value);

            int_free(right->value);
            float_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_gt(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)!float_gt_ui(left->value, (uintptr_t)right->value);

            float_free(left->value);
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!int_lt_ui(right->value, (uintptr_t)left->value);

            int_free(right->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_lt_ui(right->value, (uintptr_t)left->value);

            float_free(right->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value <= right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error("<=", 2);
}

visit_res_t compute_gte(value_t *left, value_t *right)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.has_error = 0;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!int_lt(left->value, right->value);

            int_free(right->value);
            int_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_gt_int(right->value, left->value);

            float_free(right->value);
            int_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)!int_lt_ui(left->value, (uintptr_t)right->value);

            int_free(left->value);
            return res;
        }

        free_value(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!float_lt_int(left->value, right->value);

            int_free(right->value);
            float_free(left->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_lt(left->value, right->value);

            float_free(right->value);
            float_free(left->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)!float_lt_ui(left->value, (uintptr_t)right->value);

            float_free(left->value);
            return res;
        }

        free_value(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.value = (void*)(uintptr_t)!int_gt_ui(right->value, (uintptr_t)left->value);

            int_free(right->value);
            return res;
        case FLOAT_V:
            res.value.value = (void*)(uintptr_t)!float_gt_ui(right->value, (uintptr_t)left->value);

            float_free(right->value);
            return res;
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value >= right->value);
            return res;
        }

        free_value(right);
        goto ret;
    }

    free_value(right);
    free_value(left);

ret:
    ill_op_error(">=", 2);
}

visit_res_t compute_pos(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.has_error = 0;

    switch (operand->type)
    {
    case INT_V:
    case FLOAT_V:
    case COMPLEX_V:
        res.value.type = operand->type;
        res.value.value = operand->value;
        return res;
    case BOOL_V:
        res.value.type = INT_V;
        res.value.value = int_set_ui((uintptr_t)operand->value);
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
        res.value.type = INT_V;
        res.value.value = operand->value;
        return res;
    case FLOAT_V:
        float_neg(operand->value);
        res.value.type = FLOAT_V;
        res.value.value = operand->value;
        return res;
    case COMPLEX_V:
        complex_neg(operand->value);
        res.value.type = COMPLEX_V;
        res.value.value = operand->value;
        return res;
    case BOOL_V:
        res.value.type = INT_V;
        res.value.value = int_set_ui(!operand->value);
        return res;
    }

    free_value(operand);
    ill_unary_op_error("-", 1);
}

visit_res_t compute_b_not(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.has_error = 0;

    switch (operand->type)
    {
    case INT_V:
        int_not(operand->value);
        res.value.type = INT_V;
        res.value.value = operand->value;
        return res;
    case BOOL_V:
        res.value.type = BOOL_V;
        res.value.value = (void*)(uintptr_t)!operand->value;
        return res;
    }

    free_value(operand);
    ill_unary_op_error("~", 1);
}

visit_res_t compute_not(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.value.value = (void*)(uintptr_t)!value_istrue(operand);
    res.has_error = 0;
    return res;
}
