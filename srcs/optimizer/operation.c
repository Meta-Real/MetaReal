/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <optimizer/complex.h>
#include <optimizer/list.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

#define bin_operation(l, r, f) \
    do                         \
    {                          \
        res.value = f(l, r);   \
        return res;            \
    } while (0)

#define bin_operation_b1(l, r, f)              \
    do                                         \
    {                                          \
        res.value = f(l, (uintptr_t)r->value); \
                                               \
        value_free_vo(r);                      \
        return res;                            \
    } while (0)

#define bin_operation_b1_rev(l, r, f)          \
    do                                         \
    {                                          \
        res.value = f((uintptr_t)l->value, r); \
                                               \
        value_free_vo(l);                      \
        return res;                            \
    } while (0)

#define bin_operation_b2(l, r, f) \
    do                            \
    {                             \
        if (r->value)             \
        {                         \
            res.value = f(l, 1);  \
                                  \
            value_free_vo(r);     \
            return res;           \
        }                         \
                                  \
        res.value = l;            \
                                  \
        value_free_vo(r);         \
        return res;               \
    } while (0)

#define bin_operation_b3(o, t, f)              \
    do                                         \
    {                                          \
        if (left->ref)                         \
        {                                      \
            left->ref--;                       \
                                               \
            if (right->ref)                    \
            {                                  \
                right->ref--;                  \
                                               \
                value_set(res.value, t, f(o)); \
                return res;                    \
            }                                  \
                                               \
            right->type = t;                   \
            right->value = f(o);               \
            res.value = right;                 \
            return res;                        \
        }                                      \
                                               \
        left->type = t;                        \
        left->value = f(o);                    \
        res.value = left;                      \
                                               \
        value_free_vo(right);                  \
        return res;                            \
    } while (0)

#define bin_operation_b4(o)                      \
    do                                           \
    {                                            \
        if (left->ref)                           \
        {                                        \
            left->ref--;                         \
                                                 \
            if (right->ref)                      \
            {                                    \
                right->ref--;                    \
                                                 \
                value_set(res.value, BOOL_V, o); \
                return res;                      \
            }                                    \
                                                 \
            right->value = o;                    \
            res.value = right;                   \
            return res;                          \
        }                                        \
                                                 \
        left->value = o;                         \
        res.value = left;                        \
                                                 \
        value_free_vo(right);                    \
        return res;                              \
    } while (0)

#define bin_operation_bsub(f1, f2)    \
    do                                \
    {                                 \
        if (left->value)              \
            res.value = f1(1, right); \
        else                          \
            res.value = f2(right);    \
                                      \
        value_free_vo(left);          \
        return res;                   \
    } while (0)

#define bin_operation_bmul(l, r, f, ff)          \
    do                                           \
    {                                            \
        if (r->value)                            \
        {                                        \
            res.value = l;                       \
                                                 \
            value_free_vo(r);                    \
            return res;                          \
        }                                        \
                                                 \
        value_free_vo(r);                        \
                                                 \
        if (l->ref)                              \
        {                                        \
            l->ref--;                            \
                                                 \
            value_set(res.value, l->type, f(0)); \
            return res;                          \
        }                                        \
                                                 \
        ff(l->value);                            \
        l->value = f(0);                         \
        res.value = l;                           \
        return res;                              \
    } while (0)

#define bin_operation_bdiv(t, f1, f2, ff)    \
    do                                       \
    {                                        \
        if (left->value)                     \
            res.value = f1(1, right->value); \
        else                                 \
        {                                    \
            value_free_ts(right, ff);        \
            value_set(res.value, t, f2(0));  \
        }                                    \
                                             \
        value_free_vo(left);                 \
        return res;                          \
    } while (0)

#define bin_operation_cmp(l, r, f, fl, fr)                                     \
    do                                                                         \
    {                                                                          \
        value_set(res.value, BOOL_V, (void*)(uintptr_t)f(l->value, r->value)); \
                                                                               \
        value_free_ts(r, fr);                                                  \
        value_free_ts(l, fl);                                                  \
        return res;                                                            \
    } while (0)

#define bin_operation_cmp_rev(l, r, f, fl, fr)                                 \
    do                                                                         \
    {                                                                          \
        value_set(res.value, BOOL_V, (void*)(uintptr_t)f(r->value, l->value)); \
                                                                               \
        value_free_ts(r, fr);                                                  \
        value_free_ts(l, fl);                                                  \
        return res;                                                            \
    } while (0)

#define bin_operation_cmpb(f, ff)                                                                    \
    do                                                                                               \
    {                                                                                                \
        if (right->ref)                                                                              \
        {                                                                                            \
            right->ref--;                                                                            \
                                                                                                     \
            value_set(res.value, BOOL_V, (void*)(uintptr_t)f(left->value, (uintptr_t)right->value)); \
                                                                                                     \
            value_free_ts(left, ff);                                                                 \
            return res;                                                                              \
        }                                                                                            \
                                                                                                     \
        right->value = (void*)(uintptr_t)f(left->value, (uintptr_t)right->value);                    \
        res.value = right;                                                                           \
                                                                                                     \
        value_free_ts(left, ff);                                                                     \
        return res;                                                                                  \
    } while (0)

#define bin_operation_cmpb_rev(f, ff)                                                                \
    do                                                                                               \
    {                                                                                                \
        if (left->ref)                                                                               \
        {                                                                                            \
            left->ref--;                                                                             \
                                                                                                     \
            value_set(res.value, BOOL_V, (void*)(uintptr_t)f(right->value, (uintptr_t)left->value)); \
                                                                                                     \
            value_free_ts(right, ff);                                                                \
            return res;                                                                              \
        }                                                                                            \
                                                                                                     \
        left->value = (void*)(uintptr_t)f(right->value, (uintptr_t)left->value);                     \
        res.value = left;                                                                            \
                                                                                                     \
        value_free_ts(right, ff);                                                                    \
        return res;                                                                                  \
    } while (0)

#define unary_operation(f)      \
    do                          \
    {                           \
        res.value = f(operand); \
        return res;             \
    } while (0)

#define ill_op_error(o, ol)                                                                            \
    do                                                                                                 \
    {                                                                                                  \
        char *detail = mr_alloc(46 + ol + value_name_lens[left->type] + value_name_lens[right->type]); \
        sprintf(detail, "Invalid binary operation (" o ") between <%s> and <%s>",                      \
            value_names[left->type], value_names[right->type]);                                        \
                                                                                                       \
        res.value = NULL;                                                                              \
        res.error = invalid_semantic_set(detail, ILLEGAL_OP_E, left->poss, right->pose);               \
        return res;                                                                                    \
    } while (0)

#define ill_unary_op_error(o, ol)                                                     \
    do                                                                                \
    {                                                                                 \
        char *detail = mr_alloc(34 + ol + value_name_lens[operand->type]);            \
        sprintf(detail, "Invalid unary operation (" o ") for <%s>",                   \
            value_names[operand->type]);                                              \
                                                                                      \
        res.value = NULL;                                                             \
        res.error = invalid_semantic_set(detail, ILLEGAL_OP_E, *poss, operand->pose); \
        return res;                                                                   \
    } while (0)

#define ill_inc_dec_op_error(o)                                               \
    do                                                                        \
    {                                                                         \
        char *detail = mr_alloc(26 + value_name_lens[operand->type]);         \
        sprintf(detail, "<%s> can not be " o, value_names[operand->type]);    \
                                                                              \
        res.value = NULL;                                                     \
        res.error = invalid_semantic_set(detail, ILLEGAL_OP_E, *poss, *pose); \
        return res;                                                           \
    } while (0)

#define index_out_error                                                              \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(19);                                                 \
        strcpy(detail, "Index out of range");                                        \
                                                                                     \
        res.value = NULL;                                                            \
        res.error = invalid_semantic_set(detail, INDEX_E, right->poss, right->pose); \
        return res;                                                                  \
    } while (0)

#define mem_overflow_error                                                                  \
    do                                                                                      \
    {                                                                                       \
        char *detail = mr_alloc(16);                                                        \
        strcpy(detail, "Memory overflow");                                                  \
                                                                                            \
        res.value = NULL;                                                                   \
        res.error = invalid_semantic_set(detail, MEM_OVERFLOW_E, right->poss, right->pose); \
        return res;                                                                         \
    } while (0)

#define div_by_zero_error                                                                  \
    do                                                                                     \
    {                                                                                      \
        char *detail = mr_alloc(17);                                                       \
        strcpy(detail, "Division by zero");                                                \
                                                                                           \
        res.value = NULL;                                                                  \
        res.error = invalid_semantic_set(detail, DIV_BY_ZERO_E, right->poss, right->pose); \
        return res;                                                                        \
    } while (0)

#define mod_by_zero_error                                                                  \
    do                                                                                     \
    {                                                                                      \
        char *detail = mr_alloc(15);                                                       \
        strcpy(detail, "Modulo by zero");                                                  \
                                                                                           \
        res.value = NULL;                                                                  \
        res.error = invalid_semantic_set(detail, DIV_BY_ZERO_E, right->poss, right->pose); \
        return res;                                                                        \
    } while (0)

#define ztn_power_error                                                                    \
    do                                                                                     \
    {                                                                                      \
        char *detail = mr_alloc(46);                                                       \
        strcpy(detail, "Zero raised to the power of a negative number");                   \
                                                                                           \
        res.value = NULL;                                                                  \
        res.error = invalid_semantic_set(detail, DIV_BY_ZERO_E, right->poss, right->pose); \
        return res;                                                                        \
    } while (0)

#define ztc_power_error                                                              \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(45);                                                 \
        strcpy(detail, "Zero raised to the power of a complex number");              \
                                                                                     \
        res.value = NULL;                                                            \
        res.error = invalid_semantic_set(detail, VALUE_E, right->poss, right->pose); \
        return res;                                                                  \
    } while (0)

#define neg_shift_error                                                              \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(21);                                                 \
        strcpy(detail, "Negative shift count");                                      \
                                                                                     \
        res.value = NULL;                                                            \
        res.error = invalid_semantic_set(detail, VALUE_E, right->poss, right->pose); \
        return res;                                                                  \
    } while (0)

#define neg_multiplier_error                                                         \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(20);                                                 \
        strcpy(detail, "Negative multiplier");                                       \
                                                                                     \
        res.value = NULL;                                                            \
        res.error = invalid_semantic_set(detail, VALUE_E, right->poss, right->pose); \
        return res;                                                                  \
    } while (0)

visit_res_t compute_add(value_t *left, value_t *right)
{
    visit_res_t res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_add);
        case FLOAT_V:
            bin_operation(right, left, float_add_int);
        case COMPLEX_V:
            bin_operation(right, left, complex_add_int);
        case BOOL_V:
            bin_operation_b2(left, right, int_add_ui);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_add_int);
        case FLOAT_V:
            bin_operation(left, right, float_add);
        case COMPLEX_V:
            bin_operation(right, left, complex_add_float);
        case BOOL_V:
            bin_operation_b2(left, right, float_add_ui);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, complex_add_int);
        case FLOAT_V:
            bin_operation(left, right, complex_add_float);
        case COMPLEX_V:
            bin_operation(left, right, complex_add);
        case BOOL_V:
            bin_operation_b2(left, right, complex_add_ui);
        }

        value_free(right);
        value_free_ts(left, complex_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_b2(right, left, int_add_ui);
        case FLOAT_V:
            bin_operation_b2(right, left, float_add_ui);
        case COMPLEX_V:
            bin_operation_b2(right, left, complex_add_ui);
        case BOOL_V:
            bin_operation_b3((uintptr_t)left->value + (uintptr_t)right->value, INT_V, int_set_ui);
        }

        value_free(right);
        value_free_vo(left);
        goto ret;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
        case TUPLE_V:
            bin_operation(left, right, list_concat);
        }

        bin_operation(left, right, list_append);
    }

    value_free(right);
    value_free(left);

ret:
    ill_op_error("+", 1);
}

visit_res_t compute_sub(value_t *left, value_t *right)
{
    visit_res_t res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_sub);
        case FLOAT_V:
            bin_operation(left, right, float_int_sub);
        case COMPLEX_V:
            bin_operation(left, right, complex_int_sub);
        case BOOL_V:
            bin_operation_b2(right, left, int_sub_ui);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_sub_int);
        case FLOAT_V:
            bin_operation(left, right, float_sub);
        case COMPLEX_V:
            bin_operation(left, right, complex_float_sub);
        case BOOL_V:
            bin_operation_b2(right, left, float_sub_ui);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, complex_sub_int);
        case FLOAT_V:
            bin_operation(left, right, complex_sub_float);
        case COMPLEX_V:
            bin_operation(left, right, complex_sub);
        case BOOL_V:
            bin_operation_b2(right, left, complex_sub_ui);
        }

        value_free(right);
        value_free_ts(left, complex_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_bsub(int_ui_sub, int_neg);
        case FLOAT_V:
            bin_operation_bsub(float_ui_sub, float_neg);
        case COMPLEX_V:
            bin_operation_bsub(complex_ui_sub, complex_neg);
        case BOOL_V:
            bin_operation_b3((uintptr_t)left->value - (uintptr_t)right->value, INT_V, int_set_ui);
        }

        value_free(right);
        value_free_vo(left);
        goto ret;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!left->value || int_nfit_ull(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, list_free);
                index_out_error;
            }

            uint64_t index = int_get_ull(right->value);
            if (int_isneg(right->value))
                index = LIST_CAST(left)->size - index;

            value_free_ts(right, int_free);
            if (index >= LIST_CAST(left)->size)
            {
                value_free_ts(left, list_free);
                index_out_error;
            }

            bin_operation(left, index, list_remove);
        case BOOL_V:
            if (!left->value || (uintptr_t)right->value >= LIST_CAST(left)->size)
            {
                value_free_vo(right);
                value_free_ts(left, list_free);
                index_out_error;
            }

            res.value = list_remove(left, (uintptr_t)right->value);

            value_free_vo(right);
            return res;
        };

        value_free(right);
        value_free_ts(left, list_free);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_mul);
        case FLOAT_V:
            bin_operation(right, left, float_mul_int);
        case COMPLEX_V:
            bin_operation(right, left, complex_mul_int);
        case BOOL_V:
            bin_operation_bmul(left, right, int_set_ui, int_free);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, float_mul_int);
        case FLOAT_V:
            bin_operation(left, right, float_mul);
        case COMPLEX_V:
            bin_operation(right, left, complex_mul_float);
        case BOOL_V:
            bin_operation_bmul(left, right, float_set_ui, float_free);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, complex_mul_int);
        case FLOAT_V:
            bin_operation(left, right, complex_mul_float);
        case COMPLEX_V:
            bin_operation(left, right, complex_mul);
        case BOOL_V:
            bin_operation_bmul(left, right, complex_set_ui, complex_free);
        }

        value_free(right);
        value_free_ts(left, complex_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_bmul(right, left, int_set_ui, int_free);
        case FLOAT_V:
            bin_operation_bmul(right, left, float_set_ui, float_free);
        case COMPLEX_V:
            bin_operation_bmul(right, left, complex_set_ui, complex_free);
        case BOOL_V:
            bin_operation_b3((uintptr_t)left->value & (uintptr_t)right->value, INT_V, int_set_ui);
        }

        value_free(right);
        value_free_vo(left);
        goto ret;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!left->value)
            {
                res.value = left;

                value_free_ts(right, int_free);
                return res;
            }

            if (int_gt_ull(right->value, LIST_MAX_SIZE / LIST_CAST(left)->size))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, list_free);
                mem_overflow_error;
            }
            if (int_isneg(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, list_free);
                neg_multiplier_error;
            }

            res.value = list_repeat(left, int_get_ull(right->value));

            value_free_ts(right, int_free);
            return res;
        case BOOL_V:
            if (!right->value)
            {
                if (left->ref)
                {
                    left->ref--;

                    value_set(res.value, LIST_V, NULL);
                    return res;
                }

                list_free(left->value);
                left->value = NULL;
            }

            res.value = left;

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, list_free);
    }

    value_free(right);
    value_free(left);

ret:
    ill_op_error("*", 1);
}

visit_res_t compute_div(value_t *left, value_t *right)
{
    visit_res_t res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                div_by_zero_error;
            }

            bin_operation(left, right, float_int_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, int_free);
                div_by_zero_error;
            }

            bin_operation(left, right, float_int_div);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                value_free_ts(right, complex_free);
                value_free_ts(left, int_free);
                div_by_zero_error;
            }

            bin_operation(left, right, complex_int_div);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_ts(left, int_free);
                div_by_zero_error;
            }

            if (left->ref)
            {
                left->ref--;

                value_set(res.value, FLOAT_V, float_set_int(left->value));

                value_free_vo(right);
                return res;
            }

            float_value_t *value = float_set_int(left->value);
            int_free(left->value);

            left->type = FLOAT_V;
            left->value = value;
            res.value = left;

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, float_free);
                div_by_zero_error;
            }

            bin_operation(left, right, float_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, float_free);
                div_by_zero_error;
            }

            bin_operation(left, right, float_div);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                value_free_ts(right, complex_free);
                value_free_ts(left, float_free);
                div_by_zero_error;
            }

            bin_operation(left, right, complex_float_div);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_ts(left, float_free);
                div_by_zero_error;
            }

            res.value = left;

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, complex_free);
                div_by_zero_error;
            }

            bin_operation(left, right, complex_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, complex_free);
                div_by_zero_error;
            }

            bin_operation(left, right, complex_div_float);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                value_free_ts(right, complex_free);
                value_free_ts(left, complex_free);
                div_by_zero_error;
            }

            bin_operation(left, right, complex_div);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_ts(left, complex_free);
                div_by_zero_error;
            }

            res.value = left;

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, complex_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_vo(left);
                div_by_zero_error;
            }

            bin_operation_bdiv(FLOAT_V, float_ui_div_int, float_set_ui, int_free);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_vo(left);
                div_by_zero_error;
            }

            bin_operation_bdiv(FLOAT_V, float_ui_div, float_set_ui, float_free);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                value_free_ts(right, complex_free);
                value_free_vo(left);
                div_by_zero_error;
            }

            bin_operation_bdiv(COMPLEX_V, complex_ui_div, complex_set_ui, complex_free);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_vo(left);
                div_by_zero_error;
            }

            bin_operation_b3((uintptr_t)left->value, FLOAT_V, float_set_ui);
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                mod_by_zero_error;
            }

            bin_operation(left, right, int_mod);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, int_free);
                mod_by_zero_error;
            }

            bin_operation(left, right, float_int_mod);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_ts(left, int_free);
                mod_by_zero_error;
            }

            value_set(res.value, INT_V, int_set_ui(0));

            value_free_vo(right);
            value_free_ts(left, int_free);
            return res;
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, float_free);
                mod_by_zero_error;
            }

            bin_operation(left, right, float_mod_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, float_free);
                mod_by_zero_error;
            }

            bin_operation(left, right, float_mod);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_ts(left, float_free);
                mod_by_zero_error;
            }

            bin_operation_b1(left, right, float_mod_ui);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_vo(left);
                mod_by_zero_error;
            }

            bin_operation_b1_rev(left, right, int_ui_mod);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_vo(left);
                mod_by_zero_error;
            }

            bin_operation_b1_rev(left, right, float_ui_mod);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_vo(left);
                mod_by_zero_error;
            }

            bin_operation_b3(0, INT_V, int_set_ui);
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                div_by_zero_error;
            }

            bin_operation(left, right, int_quot);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, int_free);
                div_by_zero_error;
            }

            bin_operation(left, right, float_int_quot);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_ts(left, int_free);
                div_by_zero_error;
            }

            res.value = left;

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, float_free);
                div_by_zero_error;
            }

            bin_operation(left, right, float_quot_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, float_free);
                div_by_zero_error;
            }

            bin_operation(left, right, float_quot);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_ts(left, float_free);
                div_by_zero_error;
            }

            if (left->ref)
            {
                left->ref--;

                if (right->ref)
                {
                    right->ref--;

                    value_set(res.value, INT_V, float_get_int(left->value));
                    return res;
                }

                right->type = INT_V;
                right->value = float_get_int(left->value);
                res.value = right;
                return res;
            }

            int_value_t *value = float_get_int(left->value);
            float_free(left->value);

            left->type = INT_V;
            left->value = value;
            res.value = left;

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                value_free_ts(right, int_free);
                value_free_vo(left);
                div_by_zero_error;
            }

            bin_operation_b1_rev(left, right, int_ui_quot);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                value_free_ts(right, float_free);
                value_free_vo(left);
                div_by_zero_error;
            }

            bin_operation_b1_rev(left, right, float_ui_quot);
        case BOOL_V:
            if (!right->value)
            {
                value_free_vo(right);
                value_free_vo(left);
                div_by_zero_error;
            }

            bin_operation_b3((uintptr_t)left->value, INT_V, int_set_ui);
        }

        value_free(right);
        value_free_vo(left);
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
                    value_free_ts(right, int_free);
                    value_free_ts(left, int_free);
                    ztn_power_error;
                }

                bin_operation(left, right, float_int_pow_int);
            }

            bin_operation(left, right, int_pow);
        case FLOAT_V:
            if (int_isneg(left->value))
                bin_operation(left, right, complex_int_pow_float);

            if (int_iszero(left->value) && float_isneg(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, int_free);
                ztn_power_error;
            }

            bin_operation(left, right, float_int_pow);
        case COMPLEX_V:
            if (int_iszero(left->value))
            {
                value_free_ts(right, complex_free);
                value_free_ts(left, int_free);
                ztc_power_error;
            }

            bin_operation(left, right, complex_int_pow);
        case BOOL_V:
            if (!right->value)
            {
                if (left->ref)
                {
                    left->ref--;

                    value_set(res.value, INT_V, int_set_ui(1));

                    value_free_vo(right);
                    return res;
                }

                int_free(left->value);
                left->value = int_set_ui(1);
                res.value = left;

                value_free_vo(right);
                return res;
            }

            res.value = left;

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (float_iszero(left->value) && int_isneg(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, float_free);
                ztn_power_error;
            }

            bin_operation(left, right, float_pow_int);
        case FLOAT_V:
            if (float_isneg(left->value))
                bin_operation(left, right, complex_float_pow_float);

            if (float_iszero(left->value) && float_isneg(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, float_free);
                ztn_power_error;
            }

            bin_operation(left, right, float_pow);
        case COMPLEX_V:
            if (float_iszero(left->value))
            {
                value_free_ts(right, complex_free);
                value_free_ts(left, float_free);
                ztc_power_error;
            }

            bin_operation(left, right, complex_float_pow);
        case BOOL_V:
            if (right->value)
                res.value = left;
            else
            {
                value_free_ts(left, float_free);
                value_set(res.value, FLOAT_V, float_set_ui(0));
            }

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            if (complex_iszero(left->value) && int_isneg(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, complex_free);
                ztn_power_error;
            }

            bin_operation(left, right, complex_pow_int);
        case FLOAT_V:
            if (complex_iszero(left->value) && float_isneg(right->value))
            {
                value_free_ts(right, float_free);
                value_free_ts(left, complex_free);
                ztn_power_error;
            }

            bin_operation(left, right, complex_pow_float);
        case COMPLEX_V:
            if (complex_iszero(left->value))
            {
                value_free_ts(right, complex_free);
                value_free_ts(left, complex_free);
                ztc_power_error;
            }

            bin_operation(left, right, complex_pow);
        case BOOL_V:
            if (right->value)
                res.value = left;
            else
            {
                value_free_ts(left, complex_free);
                value_set(res.value, COMPLEX_V, complex_set_ui(1));
            }

            value_free_vo(right);
            return res;
        }

        value_free(right);
        value_free_ts(left, complex_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!left->value && int_isneg(right->value))
            {
                value_free_ts(right, int_free);
                value_free_vo(left);
                ztn_power_error;
            }

            bin_operation_b1_rev(left, right, int_ui_pow);
        case FLOAT_V:
            if (!left->value && float_isneg(right->value))
            {
                value_free_ts(right, float_free);
                value_free_vo(left);
                ztn_power_error;
            }

            bin_operation_b1_rev(left, right, float_ui_pow);
        case COMPLEX_V:
            if (!left->value)
            {
                value_free_ts(right, complex_free);
                value_free_vo(left);
                ztc_power_error;
            }

            bin_operation_b1_rev(left, right, complex_ui_pow);
        case BOOL_V:
            bin_operation_b3((uintptr_t)left->value | !right->value, INT_V, int_set_ui);
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_and);
        case BOOL_V:
            bin_operation_b1(left, right, int_and_ui);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_b1(right, left, int_and_ui);
        case BOOL_V:
            bin_operation_b4((void*)((uintptr_t)left->value & (uintptr_t)right->value));
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_or);
        case BOOL_V:
            bin_operation_b1(left, right, int_or_ui);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_b1(right, left, int_or_ui);
        case BOOL_V:
            bin_operation_b4((void*)((uintptr_t)left->value | (uintptr_t)right->value));
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_xor);
        case BOOL_V:
            bin_operation_b1(left, right, int_xor_ui);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_b1(right, left, int_xor_ui);
        case BOOL_V:
            bin_operation_b4((void*)((uintptr_t)left->value ^ (uintptr_t)right->value));
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_isnzero(left->value) && int_nfit_ull(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                mem_overflow_error;
            }

            if (int_isneg(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                neg_shift_error;
            }

            bin_operation(left, right, int_lshift);
        case BOOL_V:
            bin_operation_b1(left, right, int_lshift_ui);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (left->value && int_nfit_ull(right->value))
            {
                value_free_ts(right, int_free);
                value_free_vo(left);
                mem_overflow_error;
            }

            if (int_isneg(right->value))
            {
                value_free_ts(right, int_free);
                value_free_vo(left);
                neg_shift_error;
            }

            bin_operation_b1_rev(left, right, int_ui_lshift);
        case BOOL_V:
            bin_operation_b3((uintptr_t)left->value << (uintptr_t)right->value, INT_V, int_set_ui);
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_nfit_ull(right->value))
            {
                if (left->ref)
                {
                    left->ref--;

                    if (right->ref)
                    {
                        right->ref--;

                        value_set(res.value, INT_V, int_set_ui(0));
                        return res;
                    }

                    int_free(right->value);
                    right->value = int_set_ui(0);
                    res.value = right;
                    return res;
                }

                int_free(left->value);
                left->value = int_set_ui(0);
                res.value = left;

                value_free_ts(right, int_free);
                return res;
            }

            if (int_isneg(right->value))
            {
                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                neg_shift_error;
            }

            bin_operation(left, right, int_rshift);
        case BOOL_V:
            bin_operation_b2(left, right, int_rshift_ui);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_isneg(right->value))
            {
                value_free_ts(right, int_free);
                value_free_vo(left);
                neg_shift_error;
            }

            if (int_isnzero(right->value))
                value_set(res.value, INT_V, int_set_ui(0));
            else
                value_set(res.value, INT_V, int_set_ui((uintptr_t)left->value));

            value_free_ts(right, int_free);
            value_free_vo(left);
            return res;
        case BOOL_V:
            bin_operation_b3((uintptr_t)left->value & !right->value, INT_V, int_set_ui);
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, int_eq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(right, left, float_eq_int, float_free, int_free);
        case COMPLEX_V:
            bin_operation_cmp(right, left, complex_eq_int, complex_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_eq_ui, int_free);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, float_eq_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_eq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp(right, left, complex_eq_float, complex_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_eq_ui, float_free);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, complex_eq_int, int_free, complex_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, complex_eq_float, float_free, complex_free);
        case COMPLEX_V:
            bin_operation_cmp(left, right, complex_eq, complex_free, complex_free);
        case BOOL_V:
            bin_operation_cmpb(complex_eq_ui, complex_free);
        }

        value_free(right);
        value_free_ts(left, complex_free);
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
            bin_operation_b4((void*)(uintptr_t)(left->value == right->value));
        }

        value_free(right);
        value_free_vo(left);
        goto ret;
    case LIST_V:
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(left, right, list_eq, list_free, list_free);
        }

        value_free(right);
        value_free_ts(left, list_free);
        goto ret;
    }

    value_free(right);
    value_free(left);

ret:
    value_set(res.value, BOOL_V, NULL);
    return res;
}

visit_res_t compute_neq(value_t *left, value_t *right)
{
    visit_res_t res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, int_neq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(right, left, float_neq_int, float_free, int_free);
        case COMPLEX_V:
            bin_operation_cmp(right, left, complex_neq_int, complex_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_neq_ui, int_free);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, float_neq_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_neq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp(right, left, complex_neq_float, complex_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_neq_ui, float_free);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, complex_neq_int, int_free, complex_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, complex_neq_float, float_free, complex_free);
        case COMPLEX_V:
            bin_operation_cmp(left, right, complex_neq, complex_free, complex_free);
        case BOOL_V:
            bin_operation_cmpb(complex_neq_ui, complex_free);
        }

        value_free(right);
        value_free_ts(left, complex_free);
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
            bin_operation_b4((void*)(uintptr_t)(left->value != right->value));
        }

        value_free(right);
        value_free_vo(left);
        goto ret;
    case LIST_V:
    case TUPLE_V:
        switch (right->type)
        {
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(left, right, list_neq, list_free, list_free);
        }

        value_free(right);
        value_free_ts(left, list_free);
        goto ret;
    }

    value_free(right);
    value_free(left);

ret:
    value_set(res.value, BOOL_V, (void*)1);
    return res;
}

visit_res_t compute_ex_eq(value_t *left, value_t *right)
{
    visit_res_t res;

    if (left->type == right->type)
        switch (left->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, int_eq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_eq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp(left, right, complex_eq, complex_free, complex_free);
        case BOOL_V:
            bin_operation_b4((void*)(uintptr_t)(left->value == right->value));
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(left, right, list_eq, list_free, list_free);
        }

    value_free(right);
    value_free(left);

    value_set(res.value, BOOL_V, NULL);
    return res;
}

visit_res_t compute_ex_neq(value_t *left, value_t *right)
{
    visit_res_t res;

    if (left->type == right->type)
        switch (left->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, int_neq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_neq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp(left, right, complex_neq, complex_free, complex_free);
        case BOOL_V:
            bin_operation_b4((void*)(uintptr_t)(left->value != right->value));
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(left, right, list_neq, list_free, list_free);
        }

    value_free(right);
    value_free(left);

    value_set(res.value, BOOL_V, (void*)1);
    return res;
}

visit_res_t compute_lt(value_t *left, value_t *right)
{
    visit_res_t res;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, int_lt, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(right, left, float_gt_int, float_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_lt_ui, int_free);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, float_lt_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_lt, float_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_lt_ui, float_free);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_gt_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_gt_ui, float_free);
        case BOOL_V:
            bin_operation_b4((void*)(uintptr_t)(left->value < right->value));
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, int_gt, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(right, left, float_lt_int, float_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_gt_ui, int_free);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, float_gt_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_gt, float_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_gt_ui, float_free);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_lt_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_lt_ui, float_free);
        case BOOL_V:
            bin_operation_b4((void*)(uintptr_t)(left->value > right->value));
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, int_lte, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(right, left, float_gte_int, float_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_lte_ui, int_free);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, float_lte_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_lte, float_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_lte_ui, float_free);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_gte_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_gte_ui, float_free);
        case BOOL_V:
            bin_operation_b4((void*)(uintptr_t)(left->value <= right->value));
        }

        value_free(right);
        value_free_vo(left);
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

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, int_gte, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(right, left, float_lte_int, float_free, int_free);
        case BOOL_V:
            bin_operation_cmpb(int_gte_ui, int_free);
        }

        value_free(right);
        value_free_ts(left, int_free);
        goto ret;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmp(left, right, float_gte_int, int_free, float_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_gte, float_free, float_free);
        case BOOL_V:
            bin_operation_cmpb(float_gte_ui, float_free);
        }

        value_free(right);
        value_free_ts(left, float_free);
        goto ret;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_lte_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_lte_ui, float_free);
        case BOOL_V:
            bin_operation_b4((void*)(uintptr_t)(left->value >= right->value));
        }

        value_free(right);
        value_free_vo(left);
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

    switch (operand->type)
    {
    case INT_V:
    case FLOAT_V:
    case COMPLEX_V:
        res.value = operand;
        return res;
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res.value, INT_V, int_set_ui((uintptr_t)operand->value));
            return res;
        }

        operand->type = INT_V;
        operand->value = int_set_ui((uintptr_t)operand->value);

        res.value = operand;
        return res;
    }

    value_free(operand);
    ill_unary_op_error("+", 1);
}

visit_res_t compute_neg(value_t *operand, pos_t *poss)
{
    visit_res_t res;

    switch (operand->type)
    {
    case INT_V:
        unary_operation(int_neg);
    case FLOAT_V:
        unary_operation(float_neg);
    case COMPLEX_V:
        unary_operation(complex_neg);
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res.value, INT_V, int_set_ui(!operand->value));
            return res;
        }

        operand->type = INT_V;
        operand->value = int_set_ui(!operand->value);

        res.value = operand;
        return res;
    }

    value_free(operand);
    ill_unary_op_error("-", 1);
}

visit_res_t compute_b_not(value_t *operand, pos_t *poss)
{
    visit_res_t res;

    switch (operand->type)
    {
    case INT_V:
        unary_operation(int_not);
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res.value, BOOL_V, (void*)(uintptr_t)!operand->value);
            return res;
        }

        operand->value = (void*)(uintptr_t)!operand->value;

        res.value = operand;
        return res;
    }

    value_free(operand);
    ill_unary_op_error("~", 1);
}

visit_res_t compute_not(value_t *operand, pos_t *poss)
{
    visit_res_t res;
    value_set(res.value, BOOL_V, (void*)(uintptr_t)value_isfalse(operand));
    return res;
}

visit_res_t compute_inc(value_t *operand, pos_t *poss, pos_t *pose)
{
    visit_res_t res;

    switch (operand->type)
    {
    case INT_V:
        bin_operation(operand, 1, int_add_ui);
    case FLOAT_V:
        bin_operation(operand, 1, float_add_ui);
    case COMPLEX_V:
        bin_operation(operand, 1, complex_add_ui);
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res.value, INT_V, int_set_ui((uintptr_t)operand->value + 1));
            return res;
        }

        operand->type = INT_V;
        operand->value = int_set_ui((uintptr_t)operand->value + 1);

        res.value = operand;
        return res;
    }

    ill_inc_dec_op_error("incremented");
}

visit_res_t compute_dec(value_t *operand, pos_t *poss, pos_t *pose)
{
    visit_res_t res;

    switch (operand->type)
    {
    case INT_V:
        bin_operation(operand, 1, int_sub_ui);
    case FLOAT_V:
        bin_operation(operand, 1, float_sub_ui);
    case COMPLEX_V:
        bin_operation(operand, 1, complex_sub_ui);
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res.value, INT_V, int_set_ui((uintptr_t)operand->value - 1));
            return res;
        }

        operand->type = INT_V;
        operand->value = int_set_ui((uintptr_t)operand->value - 1);

        res.value = operand;
        return res;
    }

    ill_inc_dec_op_error("decremented");
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
