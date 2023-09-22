/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <optimizer/complex.h>
#include <optimizer/list.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

#define bin_operation(t, l, r, f)                \
    do                                           \
    {                                            \
        res.value.type = t;                      \
        res.value.value = f(l->value, r->value); \
        return res;                              \
    } while (0)

#define bin_operation_b1(l, r, f)             \
    do                                        \
    {                                         \
        res.value.type = l->type;             \
                                              \
        if (r->value)                         \
            res.value.value = f(l->value, 1); \
        else                                  \
            res.value.value = l->value;       \
                                              \
        return res;                           \
    } while (0)

#define bin_operation_b2(t, l, r, f)                        \
    do                                                      \
    {                                                       \
        res.value.type = t;                                 \
        res.value.value = f(l->value, (uintptr_t)r->value); \
        return res;                                         \
    } while (0)

#define bin_operation_b2_rev(t, l, r, f)                    \
    do                                                      \
    {                                                       \
        res.value.type = t;                                 \
        res.value.value = f((uintptr_t)l->value, r->value); \
        return res;                                         \
    } while (0)

#define bin_operation_cmp(f, ff1, ff2)                                    \
    do                                                                    \
    {                                                                     \
        res.value.value = (void*)(uintptr_t)f(left->value, right->value); \
                                                                          \
        ff1(right->value);                                                \
        ff2(left->value);                                                 \
        return res;                                                       \
    } while (0)

#define bin_operation_cmp_rev(f, ff1, ff2)                                \
    do                                                                    \
    {                                                                     \
        res.value.value = (void*)(uintptr_t)f(right->value, left->value); \
                                                                          \
        ff1(right->value);                                                \
        ff2(left->value);                                                 \
        return res;                                                       \
    } while (0)

#define bin_operation_cmpb(f, ff)                                                    \
    do                                                                               \
    {                                                                                \
        res.value.value = (void*)(uintptr_t)f(left->value, (uintptr_t)right->value); \
                                                                                     \
        ff(left->value);                                                             \
        return res;                                                                  \
    } while (0)

#define bin_operation_cmpb_rev(f, ff)                                                \
    do                                                                               \
    {                                                                                \
        res.value.value = (void*)(uintptr_t)f(right->value, (uintptr_t)left->value); \
                                                                                     \
        ff(right->value);                                                            \
        return res;                                                                  \
    } while (0)

#define ill_op_error(o, ol)                                                                            \
    do                                                                                                 \
    {                                                                                                  \
        char *detail = mr_alloc(46 + ol + value_name_lens[left->type] + value_name_lens[right->type]); \
        sprintf(detail, "Invalid binary operation (" o ") between <%s> and <%s>",                      \
            value_names[left->type], value_names[right->type]);                                        \
                                                                                                       \
        res.has_error = 1;                                                                             \
        res.error = set_invalid_semantic(detail, ILLEGAL_OP_E, left->poss, right->pose);               \
        return res;                                                                                    \
    } while (0)

#define ill_unary_op_error(o, ol)                                                     \
    do                                                                                \
    {                                                                                 \
        char *detail = mr_alloc(34 + ol + value_name_lens[operand->type]);            \
        sprintf(detail, "Invalid unary operation (" o ") for <%s>",                   \
            value_names[operand->type]);                                              \
                                                                                      \
        res.has_error = 1;                                                            \
        res.error = set_invalid_semantic(detail, ILLEGAL_OP_E, *poss, operand->pose); \
        return res;                                                                   \
    } while (0)

#define index_out_error                                                              \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(19);                                                 \
        strcpy(detail, "Index out of range");                                        \
                                                                                     \
        res.has_error = 1;                                                           \
        res.error = set_invalid_semantic(detail, INDEX_E, right->poss, right->pose); \
        return res;                                                                  \
    } while (0)

#define mem_overflow_error                                                                  \
    do                                                                                      \
    {                                                                                       \
        char *detail = mr_alloc(16);                                                        \
        strcpy(detail, "Memory overflow");                                                  \
                                                                                            \
        res.has_error = 1;                                                                  \
        res.error = set_invalid_semantic(detail, MEM_OVERFLOW_E, right->poss, right->pose); \
        return res;                                                                         \
    } while (0)

#define div_by_zero_error                                                                  \
    do                                                                                     \
    {                                                                                      \
        char *detail = mr_alloc(17);                                                       \
        strcpy(detail, "Division by zero");                                                \
                                                                                           \
        res.has_error = 1;                                                                 \
        res.error = set_invalid_semantic(detail, DIV_BY_ZERO_E, right->poss, right->pose); \
        return res;                                                                        \
    } while (0)

#define mod_by_zero_error                                                                  \
    do                                                                                     \
    {                                                                                      \
        char *detail = mr_alloc(15);                                                       \
        strcpy(detail, "Modulo by zero");                                                  \
                                                                                           \
        res.has_error = 1;                                                                 \
        res.error = set_invalid_semantic(detail, DIV_BY_ZERO_E, right->poss, right->pose); \
        return res;                                                                        \
    } while (0)

#define ztn_power_error                                                                    \
    do                                                                                     \
    {                                                                                      \
        char *detail = mr_alloc(46);                                                       \
        strcpy(detail, "Zero raised to the power of a negative number");                   \
                                                                                           \
        res.has_error = 1;                                                                 \
        res.error = set_invalid_semantic(detail, DIV_BY_ZERO_E, right->poss, right->pose); \
        return res;                                                                        \
    } while (0)

#define ztc_power_error                                                              \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(45);                                                 \
        strcpy(detail, "Zero raised to the power of a complex number");              \
                                                                                     \
        res.has_error = 1;                                                           \
        res.error = set_invalid_semantic(detail, VALUE_E, right->poss, right->pose); \
        return res;                                                                  \
    } while (0)

#define neg_shift_error                                                              \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(21);                                                 \
        strcpy(detail, "Negative shift count");                                      \
                                                                                     \
        res.has_error = 1;                                                           \
        res.error = set_invalid_semantic(detail, VALUE_E, right->poss, right->pose); \
        return res;                                                                  \
    } while (0)

#define neg_multiplier_error                                                         \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(20);                                                 \
        strcpy(detail, "Negative multiplier");                                       \
                                                                                     \
        res.has_error = 1;                                                           \
        res.error = set_invalid_semantic(detail, VALUE_E, right->poss, right->pose); \
        return res;                                                                  \
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
            bin_operation(INT_V, left, right, int_add);
        case FLOAT_V:
            bin_operation(FLOAT_V, right, left, float_add_int);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, right, left, complex_add_int);
        case BOOL_V:
            bin_operation_b1(left, right, int_add_ui);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(FLOAT_V, left, right, float_add_int);
        case FLOAT_V:
            bin_operation(FLOAT_V, left, right, float_add);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, right, left, complex_add_float);
        case BOOL_V:
            bin_operation_b1(left, right, float_add_ui);
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(COMPLEX_V, left, right, complex_add_int);
        case FLOAT_V:
            bin_operation(COMPLEX_V, left, right, complex_add_float);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, left, right, complex_add);
        case BOOL_V:
            bin_operation_b1(left, right, complex_add_ui);
        }

        value_free(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_b1(right, left, int_add_ui);
        case FLOAT_V:
            bin_operation_b1(right, left, float_add_ui);
        case COMPLEX_V:
            bin_operation_b1(right, left, complex_add_ui);
        case BOOL_V:
            res.value.type = BOOL_V;
            res.value.value = int_set_ui((uintptr_t)left->value + (uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
            bin_operation(LIST_V, left, right, list_concat);
        }

        res.value.type = LIST_V;
        res.value.value = list_append(left->value, right);
        return res;
    }

    value_free(right);
    value_free(left);

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
            bin_operation(INT_V, left, right, int_sub);
        case FLOAT_V:
            bin_operation(FLOAT_V, left, right, float_int_sub);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, left, right, complex_int_sub);
        case BOOL_V:
            bin_operation_b1(right, left, int_sub_ui);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(FLOAT_V, left, right, float_sub_int);
        case FLOAT_V:
            bin_operation(FLOAT_V, left, right, float_sub);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, left, right, complex_float_sub);
        case BOOL_V:
            bin_operation_b1(right, left, float_sub_ui);
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(COMPLEX_V, left, right, complex_sub_int);
        case FLOAT_V:
            bin_operation(COMPLEX_V, left, right, complex_sub_float);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, left, right, complex_sub);
        case BOOL_V:
            bin_operation_b1(right, left, complex_sub_ui);
        }

        value_free(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.type = INT_V;

            if (left->value)
                res.value.value = int_ui_sub(1, right->value);
            else
                res.value.value = int_neg(right->value);

            return res;
        case FLOAT_V:
            res.value.type = FLOAT_V;

            if (left->value)
                res.value.value = float_ui_sub(1, right->value);
            else
                res.value.value = float_neg(right->value);

            return res;
        case COMPLEX_V:
            res.value.type = COMPLEX_V;

            if (left->value)
                res.value.value = complex_ui_sub(1, right->value);
            else
                res.value.value = complex_neg(right->value);

            return res;
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value - (uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (int_nfit_ull(right->value))
            {
                int_free(right->value);
                list_free(left->value);
                index_out_error;
            }

            uint64_t index = int_get_ull(right->value);
            if (int_isneg(right->value))
                index = list_size(left->value) - index;

            int_free(right->value);
            if (index >= list_size(left->value))
            {
                list_free(left->value);
                index_out_error;
            }

            res.value.type = LIST_V;
            res.value.value = list_remove(left->value, index);
            return res;
        case BOOL_V:
            if ((uintptr_t)right->value >= list_size(left->value))
            {
                list_free(left->value);
                index_out_error;
            }

            res.value.type = LIST_V;
            res.value.value = list_remove(left->value, (uintptr_t)right->value);
            return res;
        };

        value_free(right);
        list_free(left->value);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation(INT_V, left, right, int_mul);
        case FLOAT_V:
            bin_operation(FLOAT_V, right, left, float_mul_int);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, right, left, complex_mul_int);
        case BOOL_V:
            res.value.type = INT_V;

            if (right->value)
                res.value.value = left->value;
            else
            {
                int_free(left->value);
                res.value.value = int_set_ui(0);
            }

            return res;
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(FLOAT_V, left, right, float_mul_int);
        case FLOAT_V:
            bin_operation(FLOAT_V, left, right, float_mul);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, right, left, complex_mul_float);
        case BOOL_V:
            res.value.type = FLOAT_V;

            if (right->value)
                res.value.value = left->value;
            else
            {
                float_free(left->value);
                res.value.value = float_set_ui(0);
            }

            return res;
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(COMPLEX_V, left, right, complex_mul_int);
        case FLOAT_V:
            bin_operation(COMPLEX_V, left, right, complex_mul_float);
        case COMPLEX_V:
            bin_operation(COMPLEX_V, left, right, complex_mul);
        case BOOL_V:
            res.value.type = COMPLEX_V;

            if (right->value)
                res.value.value = left->value;
            else
            {
                complex_free(left->value);
                res.value.value = complex_set_ui(0);
            }

            return res;
        }

        value_free(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            res.value.type = INT_V;

            if (left->value)
                res.value.value = right->value;
            else
            {
                int_free(right->value);
                res.value.value = int_set_ui(0);
            }

            return res;
        case FLOAT_V:
            res.value.type = FLOAT_V;

            if (left->value)
                res.value.value = right->value;
            else
            {
                float_free(right->value);
                res.value.value = float_set_ui(0);
            }

            return res;
        case COMPLEX_V:
            res.value.type = COMPLEX_V;

            if (left->value)
                res.value.value = right->value;
            else
            {
                complex_free(right->value);
                res.value.value = complex_set_ui(0);
            }

            return res;
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value & (uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (int_gt_ull(right->value, LIST_MAX_SIZE / list_size(left->value)))
            {
                int_free(right->value);
                list_free(left->value);
                mem_overflow_error;
            }
            if (int_isneg(right->value))
            {
                int_free(right->value);
                list_free(left->value);
                neg_multiplier_error;
            }

            res.value.type = LIST_V;
            res.value.value = list_repeat(left->value, int_get_ull(right->value));

            int_free(right->value);
            return res;
        case BOOL_V:
            res.value.type = LIST_V;

            if (right->value)
                res.value.value = left->value;
            else
            {
                list_free(left->value);
                res.value.value = NULL;
            }

            return res;
        }

        value_free(right);
        list_free(left->value);
    }

    value_free(right);
    value_free(left);

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
            return res;
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                int_free(left->value);
                div_by_zero_error;
            }

            bin_operation(FLOAT_V, left, right, float_int_div);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                complex_free(right->value);
                int_free(left->value);
                div_by_zero_error;
            }

            bin_operation(COMPLEX_V, left, right, complex_int_div);
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

        value_free(right);
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

            bin_operation(FLOAT_V, left, right, float_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(FLOAT_V, left, right, float_div);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                complex_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(COMPLEX_V, left, right, complex_float_div);
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

        value_free(right);
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

            bin_operation(COMPLEX_V, left, right, complex_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(COMPLEX_V, left, right, complex_div_float);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                complex_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            bin_operation(COMPLEX_V, left, right, complex_div);
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

        value_free(right);
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

            res.value.type = FLOAT_V;

            if (left->value)
                res.value.value = float_ui_div_int(1, right->value);
            else
            {
                res.value.value = float_set_ui(0);
                int_free(right->value);
            }

            return res;
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                div_by_zero_error;
            }

            res.value.type = FLOAT_V;

            if (left->value)
                res.value.value = float_ui_div(1, right->value);
            else
            {
                float_free(right->value);
                res.value.value = float_set_ui(0);
            }

            return res;
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                complex_free(right->value);
                div_by_zero_error;
            }

            res.value.type = COMPLEX_V;

            if (left->value)
                res.value.value = complex_ui_div(1, right->value);
            else
            {
                complex_free(right->value);
                res.value.value = complex_set_ui(0);
            }

            return res;
        case BOOL_V:
            if (!right->value)
                div_by_zero_error;

            res.value.type = FLOAT_V;
            res.value.value = float_set_ui((uintptr_t)left->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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

            bin_operation(INT_V, left, right, int_mod);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                int_free(left->value);
                mod_by_zero_error;
            }

            bin_operation(INT_V, left, right, float_int_mod);
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

        value_free(right);
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

            bin_operation(FLOAT_V, left, right, float_mod_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                mod_by_zero_error;
            }

            bin_operation(FLOAT_V, left, right, float_mod);
        case BOOL_V:
            if (!right->value)
            {
                float_free(left->value);
                mod_by_zero_error;
            }

            bin_operation_b2(FLOAT_V, left, right, float_mod_ui);
        }

        value_free(right);
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

            bin_operation_b2_rev(INT_V, left, right, int_ui_mod);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                mod_by_zero_error;
            }

            bin_operation_b2_rev(FLOAT_V, left, right, float_ui_mod);
        case BOOL_V:
            if (!right->value)
                mod_by_zero_error;

            res.value.type = INT_V;
            res.value.value = int_set_ui(0);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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

            bin_operation(INT_V, left, right, int_quot);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                int_free(left->value);
                div_by_zero_error;
            }

            bin_operation(INT_V, left, right, float_int_quot);
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

        value_free(right);
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

            bin_operation(INT_V, left, right, float_quot_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                div_by_zero_error;
            }

            res.value.type = INT_V;
            res.value.value = float_quot(left->value, right->value);
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

        value_free(right);
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

            bin_operation_b2_rev(INT_V, left, right, int_ui_quot);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                float_free(right->value);
                div_by_zero_error;
            }

            bin_operation_b2_rev(INT_V, left, right, float_ui_quot);
        case BOOL_V:
            if (!right->value)
                div_by_zero_error;

            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
                return res;
            }

            bin_operation(INT_V, left, right, int_pow);
        case FLOAT_V:
            if (int_isneg(left->value))
            {
                res.value.type = COMPLEX_V;
                res.value.value = complex_int_pow_float(left->value, right->value);
                return res;
            }

            if (int_iszero(left->value) && float_isneg(right->value))
            {
                float_free(right->value);
                int_free(left->value);
                ztn_power_error;
            }

            bin_operation(FLOAT_V, left, right, float_int_pow);
        case COMPLEX_V:
            if (int_iszero(left->value))
            {
                complex_free(right->value);
                int_free(left->value);
                ztc_power_error;
            }

            bin_operation(COMPLEX_V, left, right, complex_int_pow);
        case BOOL_V:
            res.value.type = INT_V;

            if (right->value)
                res.value.value = left->value;
            else
            {
                int_free(left->value);
                res.value.value = int_set_ui(1);
            }

            return res;
        }

        value_free(right);
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

            bin_operation(FLOAT_V, left, right, float_pow_int);
        case FLOAT_V:
            if (float_isneg(left->value))
            {
                res.value.type = COMPLEX_V;
                res.value.value = complex_float_pow_float(left->value, right->value);
                return res;
            }

            if (float_iszero(left->value) && float_isneg(right->value))
            {
                float_free(right->value);
                float_free(left->value);
                ztn_power_error;
            }

            bin_operation(FLOAT_V, left, right, float_pow);
        case COMPLEX_V:
            if (float_iszero(left->value))
            {
                complex_free(right->value);
                float_free(left->value);
                ztc_power_error;
            }

            bin_operation(COMPLEX_V, left, right, complex_float_pow);
        case BOOL_V:
            res.value.type = FLOAT_V;

            if (right->value)
                res.value.value = left->value;
            else
            {
                float_free(left->value);
                res.value.value = float_set_ui(1);
            }

            return res;
        }

        value_free(right);
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

            bin_operation(COMPLEX_V, left, right, complex_pow_int);
        case FLOAT_V:
            if (complex_iszero(left->value) && float_isneg(right->value))
            {
                float_free(right->value);
                complex_free(left->value);
                ztn_power_error;
            }

            bin_operation(COMPLEX_V, left, right, complex_pow_float);
        case COMPLEX_V:
            if (complex_iszero(left->value))
            {
                complex_free(right->value);
                complex_free(left->value);
                ztc_power_error;
            }

            bin_operation(COMPLEX_V, left, right, complex_pow);
        case BOOL_V:
            res.value.type = COMPLEX_V;

            if (right->value)
                res.value.value = left->value;
            else
            {
                float_free(left->value);
                res.value.value = complex_set_ui(1);
            }

            return res;
        }

        value_free(right);
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

            bin_operation_b2_rev(INT_V, left, right, int_ui_pow);
        case FLOAT_V:
            if (!left->value && float_isneg(right->value))
            {
                float_free(right->value);
                ztn_power_error;
            }

            bin_operation_b2_rev(FLOAT_V, left, right, float_ui_pow);
        case COMPLEX_V:
            if (!left->value)
            {
                complex_free(right->value);
                ztc_power_error;
            }

            bin_operation_b2_rev(COMPLEX_V, left, right, complex_ui_pow);
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value | !(uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation(INT_V, left, right, int_and);
        case BOOL_V:
            bin_operation_b2(INT_V, left, right, int_and_ui);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_b2(INT_V, right, left, int_and_ui);
        case BOOL_V:
            res.value.type = BOOL_V;
            res.value.value = (void*)((uintptr_t)left->value & (uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation(INT_V, left, right, int_or);
        case BOOL_V:
            bin_operation_b2(INT_V, left, right, int_or_ui);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_b2(INT_V, right, left, int_or_ui);
        case BOOL_V:
            res.value.type = BOOL_V;
            res.value.value = (void*)((uintptr_t)left->value | (uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation(INT_V, left, right, int_xor);
        case BOOL_V:
            bin_operation_b2(INT_V, left, right, int_xor_ui);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_b2(INT_V, right, left, int_xor_ui);
        case BOOL_V:
            res.value.type = BOOL_V;
            res.value.value = (void*)((uintptr_t)left->value ^ (uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            if (int_isneg(right->value))
            {
                int_free(right->value);
                int_free(left->value);
                neg_shift_error;
            }

            bin_operation(INT_V, left, right, int_lshift);
        case BOOL_V:
            bin_operation_b2(INT_V, left, right, int_lshift_ui);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_isneg(right->value))
            {
                int_free(right->value);
                neg_shift_error;
            }

            bin_operation_b2_rev(INT_V, left, right, int_ui_lshift);
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value << (uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            if (int_isneg(right->value))
            {
                int_free(right->value);
                int_free(left->value);
                neg_shift_error;
            }

            bin_operation(INT_V, left, right, int_rshift);
        case BOOL_V:
            bin_operation_b2(INT_V, left, right, int_rshift_ui);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_isneg(right->value))
            {
                int_free(right->value);
                neg_shift_error;
            }

            bin_operation_b2_rev(INT_V, left, right, int_ui_rshift);
        case BOOL_V:
            res.value.type = INT_V;
            res.value.value = int_set_ui((uintptr_t)left->value >> (uintptr_t)right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation_cmp(int_eq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp_rev(float_eq_int, float_free, int_free);
        case COMPLEX_V:
            bin_operation_cmp_rev(complex_eq_int, complex_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_eq_ui, int_free);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(float_eq_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(float_eq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp_rev(complex_eq_float, complex_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_eq_ui, float_free);
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(complex_eq_int, int_free, complex_free);
        case FLOAT_V:
            bin_operation_cmp(complex_eq_float, float_free, complex_free);
        case COMPLEX_V:
            bin_operation_cmp(complex_eq, complex_free, complex_free);
        case BOOL_V:
            bin_operation_cmpb(complex_eq_ui, complex_free);
        }

        value_free(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_eq_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_eq_ui, float_free);
        case COMPLEX_V:
            bin_operation_cmpb_rev(complex_eq_ui, complex_free);
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value == right->value);
            return res;
        }

        value_free(right);
        complex_free(left->value);
        goto ret;
    case LIST_V:
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(list_eq, list_free, list_free);
        }

        value_free(right);
        list_free(left->value);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation_cmp(int_neq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp_rev(float_neq_int, float_free, int_free);
        case COMPLEX_V:
            bin_operation_cmp_rev(complex_neq_int, complex_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_neq_ui, int_free);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(float_neq_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(float_neq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp_rev(complex_neq_float, complex_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_neq_ui, float_free);
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(complex_neq_int, int_free, complex_free);
        case FLOAT_V:
            bin_operation_cmp(complex_neq_float, float_free, complex_free);
        case COMPLEX_V:
            bin_operation_cmp(complex_neq, complex_free, complex_free);
        case BOOL_V:
            bin_operation_cmpb(complex_neq_ui, complex_free);
        }

        value_free(right);
        complex_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_neq_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_neq_ui, float_free);
        case COMPLEX_V:
            bin_operation_cmpb_rev(complex_neq_ui, complex_free);
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value != right->value);
            return res;
        }

        value_free(right);
        complex_free(left->value);
        goto ret;
    case LIST_V:
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(list_neq, list_free, list_free);
        }

        value_free(right);
        list_free(left->value);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation_cmp(int_eq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(float_eq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp(complex_eq, complex_free, complex_free);
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value == right->value);
            return res;
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(list_eq, list_free, list_free);
        }

    value_free(right);
    value_free(left);

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
            bin_operation_cmp(int_neq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(float_neq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp(complex_neq, complex_free, complex_free);
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value == right->value);
            return res;
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(list_eq, list_free, list_free);
        }

    value_free(right);
    value_free(left);

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
            bin_operation_cmp(int_lt, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp_rev(float_gt_int, float_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_lt_ui, int_free);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(float_lt_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(float_lt, float_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_lt_ui, float_free);
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_gt_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_gt_ui, float_free);
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value < right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation_cmp(int_gt, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp_rev(float_lt_int, float_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_gt_ui, int_free);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(float_gt_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(float_gt, float_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_gt_ui, float_free);
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_lt_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_lt_ui, float_free);
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value > right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation_cmp(int_lte, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp_rev(float_gte_int, float_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_lte_ui, int_free);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(float_lte_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(float_lte, float_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_lte_ui, float_free);
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_gte_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_gte_ui, float_free);
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value <= right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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
            bin_operation_cmp(int_gte, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp_rev(float_lte_int, float_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_gte_ui, int_free);
        }

        value_free(right);
        int_free(left->value);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(float_gte_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(float_gte, float_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_gte_ui, float_free);
        }

        value_free(right);
        float_free(left->value);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_lte_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_lte_ui, float_free);
        case BOOL_V:
            res.value.value = (void*)(uintptr_t)(left->value >= right->value);
            return res;
        }

        value_free(right);
        goto ret;
    }

    value_free(right);
    value_free(left);

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

    value_free(operand);
    ill_unary_op_error("+", 1);
}

visit_res_t compute_neg(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.has_error = 0;

    switch (operand->type)
    {
    case INT_V:
        res.value.type = INT_V;
        res.value.value = int_neg(operand->value);
        return res;
    case FLOAT_V:
        res.value.type = FLOAT_V;
        res.value.value = float_neg(operand->value);
        return res;
    case COMPLEX_V:
        res.value.type = COMPLEX_V;
        res.value.value = complex_neg(operand->value);
        return res;
    case BOOL_V:
        res.value.type = INT_V;
        res.value.value = int_set_ui(!operand->value);
        return res;
    }

    value_free(operand);
    ill_unary_op_error("-", 1);
}

visit_res_t compute_b_not(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.has_error = 0;

    switch (operand->type)
    {
    case INT_V:
        res.value.type = INT_V;
        res.value.value = int_not(operand->value);
        return res;
    case BOOL_V:
        res.value.type = BOOL_V;
        res.value.value = (void*)(uintptr_t)!operand->value;
        return res;
    }

    value_free(operand);
    ill_unary_op_error("~", 1);
}

visit_res_t compute_not(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    res.value.type = BOOL_V;
    res.value.value = (void*)(uintptr_t)value_isfalse(operand);
    res.has_error = 0;
    return res;
}

uint8_t compute_vneq(const value_t *left, const value_t *right)
{
    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            return int_neq(left->value, right->value);
        case FLOAT_V:
            return float_neq_int(right->value, left->value);
        case COMPLEX_V:
            return complex_neq_int(right->value, left->value);
        case BOOL_V:
            return int_neq_ui(left->value, (uintptr_t)right->value);
        }

        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            return float_neq_int(left->value, right->value);
        case FLOAT_V:
            return float_neq(left->value, right->value);
        case COMPLEX_V:
            return complex_neq_float(right->value, left->value);
        case BOOL_V:
            return float_neq_ui(left->value, (uintptr_t)right->value);
        }

        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            return complex_neq_int(left->value, right->value);
        case FLOAT_V:
            return complex_neq_float(left->value, right->value);
        case COMPLEX_V:
            return complex_neq(left->value, right->value);
        case BOOL_V:
            return complex_neq_ui(left->value, (uintptr_t)right->value);
        }

        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            return int_neq_ui(right->value, (uintptr_t)left->value);
        case FLOAT_V:
            return float_neq_ui(right->value, (uintptr_t)left->value);
        case COMPLEX_V:
            return complex_neq_ui(right->value, (uintptr_t)left->value);
        case BOOL_V:
            return right->value != left->value;
        }

        break;
    case LIST_V:
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
        case TUPLE_V:
            return list_neq(left->value, right->value);
        }

        break;
    }

    return 0;
}
