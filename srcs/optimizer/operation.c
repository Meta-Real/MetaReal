/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <optimizer/complex.h>
#include <optimizer/list.h>
#include <error/semantic.h>

#define bin_operation(l, r, f) \
    do                         \
    {                          \
        res->value = f(l, r);  \
        return;                \
    } while (0)

#define bin_operation_b(o)                           \
    do                                               \
    {                                                \
        int_value_t *value = int_set_ui(o);          \
                                                     \
        if (left->ref)                               \
        {                                            \
            left->ref--;                             \
                                                     \
            if (right->ref)                          \
            {                                        \
                right->ref--;                        \
                                                     \
                value_set(res->value, INT_V, value); \
                return;                              \
            }                                        \
                                                     \
            right->type = INT_V;                     \
            right->value = value;                    \
            res->value = right;                      \
            return;                                  \
        }                                            \
                                                     \
        left->type = INT_V;                          \
        left->value = value;                         \
        res->value = left;                           \
                                                     \
        value_free_vo(right);                        \
        return;                                      \
    } while (0)

#define bin_operation_b2(o)                                \
    do                                                     \
    {                                                      \
        if (left->ref)                                     \
        {                                                  \
            left->ref--;                                   \
                                                           \
            if (right->ref)                                \
            {                                              \
                right->ref--;                              \
                                                           \
                value_set(res->value, BOOL_V, (void*)(o)); \
                return;                                    \
            }                                              \
                                                           \
            right->value = (void*)(o);                     \
            res->value = right;                            \
            return;                                        \
        }                                                  \
                                                           \
        left->value = (void*)(o);                          \
        res->value = left;                                 \
                                                           \
        value_free_vo(right);                              \
        return;                                            \
    } while (0)

#define bin_operation_cmp(l, r, f, fl, fr)                     \
    do                                                         \
    {                                                          \
        void *value = (void*)(uintptr_t)f(l->value, r->value); \
                                                               \
        if (l->ref)                                            \
        {                                                      \
            l->ref--;                                          \
                                                               \
            if (r->ref)                                        \
            {                                                  \
                r->ref--;                                      \
                                                               \
                value_set(res->value, BOOL_V, value);          \
                return;                                        \
            }                                                  \
                                                               \
            fr(r->value);                                      \
            r->type = BOOL_V;                                  \
            r->value = value;                                  \
            res->value = r;                                    \
            return;                                            \
        }                                                      \
                                                               \
        fl(l->value);                                          \
        l->type = BOOL_V;                                      \
        l->value = value;                                      \
        res->value = l;                                        \
                                                               \
        value_free_ts(r, fr);                                  \
        return;                                                \
    } while (0)

#define bin_operation_cmpb(f, ff)                                                \
    do                                                                           \
    {                                                                            \
        void *value = (void*)(uintptr_t)f(left->value, (uintptr_t)right->value); \
                                                                                 \
        if (right->ref)                                                          \
        {                                                                        \
            right->ref--;                                                        \
                                                                                 \
            if (left->ref)                                                       \
            {                                                                    \
                left->ref--;                                                     \
                                                                                 \
                value_set(res->value, BOOL_V, value);                            \
                return;                                                          \
            }                                                                    \
                                                                                 \
            ff(left->value);                                                     \
            left->type = BOOL_V;                                                 \
            left->value = value;                                                 \
            res->value = left;                                                   \
            return;                                                              \
        }                                                                        \
                                                                                 \
        right->value = value;                                                    \
        res->value = right;                                                      \
                                                                                 \
        value_free_ts(left, ff);                                                 \
        return;                                                                  \
    } while (0)

#define bin_operation_cmpb_rev(f, ff)                                            \
    do                                                                           \
    {                                                                            \
        void *value = (void*)(uintptr_t)f(right->value, (uintptr_t)left->value); \
                                                                                 \
        if (left->ref)                                                           \
        {                                                                        \
            left->ref--;                                                         \
                                                                                 \
            if (right->ref)                                                      \
            {                                                                    \
                right->ref--;                                                    \
                                                                                 \
                value_set(res->value, BOOL_V, value);                            \
                return;                                                          \
            }                                                                    \
                                                                                 \
            ff(right->value);                                                    \
            right->type = BOOL_V;                                                \
            right->value = value;                                                \
            res->value = right;                                                  \
            return;                                                              \
        }                                                                        \
                                                                                 \
        left->value = value;                                                     \
        res->value = left;                                                       \
                                                                                 \
        value_free_ts(right, ff);                                                \
        return;                                                                  \
    } while (0)

#define unary_operation(f)       \
    do                           \
    {                            \
        res->value = f(operand); \
        return;                  \
    } while (0)

#define setup_error          \
    do                       \
    {                        \
        type1 = left->type;  \
        type2 = right->type; \
        poss = left->poss;   \
        pose = right->pose;  \
    } while (0)

#define setup_error2        \
    do                      \
    {                       \
        poss = left->poss;  \
        pose = right->pose; \
    } while (0)

#define setup_error3        \
    do                      \
    {                       \
        poss = right->poss; \
        pose = right->pose; \
    } while (0)

void compute_add(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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
            bin_operation(left, right, int_add_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
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
            bin_operation(left, right, float_add_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
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
            bin_operation(left, right, complex_add_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, complex_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(right, left, int_add_ui);
        case FLOAT_V:
            bin_operation(right, left, float_add_ui);
        case COMPLEX_V:
            bin_operation(right, left, complex_add_ui);
        case BOOL_V:
            bin_operation_b((uintptr_t)left->value + (uintptr_t)right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    case LIST_V:
        switch (right->type)
        {
        case LIST_V:
        case TUPLE_V:
            bin_operation(left, right, list_concat);
        }

        bin_operation(left, right, list_append);
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("+", 1, type1, type2, poss, pose);
}

void compute_sub(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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
            bin_operation(left, right, int_sub_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
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
            bin_operation(left, right, float_sub_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
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
            bin_operation(left, right, complex_sub_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, complex_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_ui_sub);
        case FLOAT_V:
            bin_operation(left, right, float_ui_sub);
        case COMPLEX_V:
            bin_operation(left, right, complex_ui_sub);
        case BOOL_V:
            bin_operation_b((uintptr_t)left->value - (uintptr_t)right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!left->value || int_nfit_ull(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, list_free);
                index_out_error(poss, pose);
            }

            uint64_t index = int_get_ull(right->value);
            if (int_isneg(right->value))
                index = LIST_CAST(left)->size - index;

            if (index >= LIST_CAST(left)->size)
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, list_free);
                index_out_error(poss, pose);
            }

            res->value = list_remove_int(left, right, index);

            value_free_ts(right, int_free);
            return;
        case BOOL_V:
            if (!left->value || (uintptr_t)right->value == LIST_CAST(left)->size)
            {
                setup_error3;

                value_free_vo(right);
                value_free_ts(left, list_free);
                index_out_error(poss, pose);
            }

            bin_operation(left, right, list_remove_ui);
        };

        setup_error;

        value_free(right);
        value_free_ts(left, list_free);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("-", 1, type1, type2, poss, pose);
}

void compute_mul(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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
            bin_operation(left, right, int_mul_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
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
            bin_operation(left, right, float_mul_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
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
            bin_operation(left, right, complex_mul_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, complex_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(right, left, int_mul_ui);
        case FLOAT_V:
            bin_operation(right, left, float_mul_ui);
        case COMPLEX_V:
            bin_operation(right, left, complex_mul_ui);
        case BOOL_V:
            bin_operation_b((uintptr_t)left->value & (uintptr_t)right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    case LIST_V:
        switch (right->type)
        {
        case INT_V:
            if (!left->value)
            {
                res->value = left;

                value_free_ts(right, int_free);
                return;
            }

            if (int_gt_ull(right->value, LIST_MAX_SIZE / LIST_CAST(left)->size))
            {
                setup_error2;

                value_free_ts(right, int_free);
                value_free_ts(left, list_free);
                mem_overflow_error(poss, pose);
            }
            if (int_isneg(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, list_free);
                neg_multiplier_error(poss, pose);
            }

            bin_operation(left, right, list_repeat_int);
        case BOOL_V:
            if (!left->value)
            {
                res->value = left;

                value_free_vo(right);
                return;
            }

            bin_operation(left, right, list_repeat_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, list_free);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
    }

    ill_op_error("*", 1, type1, type2, poss, pose);
}

void compute_div(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_int_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_ts(left, int_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_int_div);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, complex_free);
                value_free_ts(left, int_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, complex_int_div);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_ts(left, int_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_int_div_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, float_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_ts(left, float_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_div);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, complex_free);
                value_free_ts(left, float_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, complex_float_div);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_ts(left, float_free);
                div_by_zero_error(poss, pose);
            }

            res->value = left;

            value_free_vo(right);
            return;
        }

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, complex_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, complex_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_ts(left, complex_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, complex_div_float);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, complex_free);
                value_free_ts(left, complex_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, complex_div);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_ts(left, complex_free);
                div_by_zero_error(poss, pose);
            }

            res->value = left;

            value_free_vo(right);
            return;
        }

        setup_error;

        value_free(right);
        value_free_ts(left, complex_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_vo(left);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_ui_div_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_vo(left);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_ui_div);
        case COMPLEX_V:
            if (complex_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, complex_free);
                value_free_vo(left);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, complex_ui_div);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_vo(left);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_ui_div_ui);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("/", 1, type1, type2, poss, pose);
}

void compute_mod(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                mod_by_zero_error(poss, pose);
            }

            bin_operation(left, right, int_mod);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_ts(left, int_free);
                mod_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_int_mod);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_ts(left, int_free);
                mod_by_zero_error(poss, pose);
            }

            bin_operation(left, right, int_mod_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, float_free);
                mod_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_mod_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_ts(left, float_free);
                mod_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_mod);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_ts(left, float_free);
                mod_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_mod_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_vo(left);
                mod_by_zero_error(poss, pose);
            }

            bin_operation(left, right, int_ui_mod);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_vo(left);
                mod_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_ui_mod);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_vo(left);
                mod_by_zero_error(poss, pose);
            }

            bin_operation_b(0);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("%%", 1, type1, type2, poss, pose);
}

void compute_quot(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, int_quot);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_ts(left, int_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_int_quot);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_ts(left, int_free);
                div_by_zero_error(poss, pose);
            }

            res->value = left;

            value_free_vo(right);
            return;
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, float_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_quot_int);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_ts(left, float_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_quot);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_ts(left, float_free);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_quot_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_vo(left);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, int_ui_quot);
        case FLOAT_V:
            if (float_iszero(right->value))
            {
                setup_error3;

                value_free_ts(right, float_free);
                value_free_vo(left);
                div_by_zero_error(poss, pose);
            }

            bin_operation(left, right, float_ui_quot);
        case BOOL_V:
            if (!right->value)
            {
                setup_error3;

                value_free_vo(right);
                value_free_vo(left);
                div_by_zero_error(poss, pose);
            }

            bin_operation_b((uintptr_t)left->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
    }

    ill_op_error("//", 2, type1, type2, poss, pose);
}

void compute_pow(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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
                    setup_error2;

                    value_free_ts(right, int_free);
                    value_free_ts(left, int_free);
                    ztn_power_error(poss, pose);
                }

                bin_operation(left, right, float_int_pow_int);
            }

            bin_operation(left, right, int_pow);
        case FLOAT_V:
            if (int_isneg(left->value))
                bin_operation(left, right, complex_int_pow_float);

            if (int_iszero(left->value) && float_isneg(right->value))
            {
                setup_error2;

                value_free_ts(right, float_free);
                value_free_ts(left, int_free);
                ztn_power_error(poss, pose);
            }

            bin_operation(left, right, float_int_pow);
        case COMPLEX_V:
            if (int_iszero(left->value))
            {
                setup_error2;

                value_free_ts(right, complex_free);
                value_free_ts(left, int_free);
                ztc_power_error(poss, pose);
            }

            bin_operation(left, right, complex_int_pow);
        case BOOL_V:
            bin_operation(left, right, int_pow_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case FLOAT_V:
        switch (right->type)
        {
        case INT_V:
            if (float_iszero(left->value) && int_isneg(right->value))
            {
                setup_error2;

                value_free_ts(right, int_free);
                value_free_ts(left, float_free);
                ztn_power_error(poss, pose);
            }

            bin_operation(left, right, float_pow_int);
        case FLOAT_V:
            if (float_isneg(left->value))
                bin_operation(left, right, complex_float_pow_float);

            if (float_iszero(left->value) && float_isneg(right->value))
            {
                setup_error2;

                value_free_ts(right, float_free);
                value_free_ts(left, float_free);
                ztn_power_error(poss, pose);
            }

            bin_operation(left, right, float_pow);
        case COMPLEX_V:
            if (float_iszero(left->value))
            {
                setup_error2;

                value_free_ts(right, complex_free);
                value_free_ts(left, float_free);
                ztc_power_error(poss, pose);
            }

            bin_operation(left, right, complex_float_pow);
        case BOOL_V:
            bin_operation(left, right, float_pow_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
    case COMPLEX_V:
        switch (right->type)
        {
        case INT_V:
            if (complex_iszero(left->value) && int_isneg(right->value))
            {
                setup_error2;

                value_free_ts(right, int_free);
                value_free_ts(left, complex_free);
                ztn_power_error(poss, pose);
            }

            bin_operation(left, right, complex_pow_int);
        case FLOAT_V:
            if (complex_iszero(left->value) && float_isneg(right->value))
            {
                setup_error2;

                value_free_ts(right, float_free);
                value_free_ts(left, complex_free);
                ztn_power_error(poss, pose);
            }

            bin_operation(left, right, complex_pow_float);
        case COMPLEX_V:
            if (complex_iszero(left->value))
            {
                setup_error2;

                value_free_ts(right, complex_free);
                value_free_ts(left, complex_free);
                ztc_power_error(poss, pose);
            }

            bin_operation(left, right, complex_pow);
        case BOOL_V:
            bin_operation(left, right, complex_pow_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, complex_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (!left->value && int_isneg(right->value))
            {
                setup_error2;

                value_free_ts(right, int_free);
                value_free_vo(left);
                ztn_power_error(poss, pose);
            }

            bin_operation(left, right, int_ui_pow);
        case FLOAT_V:
            if (!left->value && float_isneg(right->value))
            {
                setup_error2;

                value_free_ts(right, float_free);
                value_free_vo(left);
                ztn_power_error(poss, pose);
            }

            bin_operation(left, right, float_ui_pow);
        case COMPLEX_V:
            if (!left->value)
            {
                setup_error2;

                value_free_ts(right, complex_free);
                value_free_vo(left);
                ztc_power_error(poss, pose);
            }

            bin_operation(left, right, complex_ui_pow);
        case BOOL_V:
            bin_operation_b((uintptr_t)left->value | !right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("**", 2, type1, type2, poss, pose);
}

void compute_b_and(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_and);
        case BOOL_V:
            bin_operation(left, right, int_and_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(right, left, int_and_ui);
        case BOOL_V:
            bin_operation_b2((uintptr_t)left->value & (uintptr_t)right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("&", 1, type1, type2, poss, pose);
}

void compute_b_or(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_or);
        case BOOL_V:
            bin_operation(left, right, int_or_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(right, left, int_or_ui);
        case BOOL_V:
            bin_operation_b2((uintptr_t)left->value | (uintptr_t)right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("|", 1, type1, type2, poss, pose);
}

void compute_b_xor(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(left, right, int_xor);
        case BOOL_V:
            bin_operation(left, right, int_xor_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation(right, left, int_xor_ui);
        case BOOL_V:
            bin_operation_b2((uintptr_t)left->value ^ (uintptr_t)right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("^", 1, type1, type2, poss, pose);
}

void compute_lshift(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

    switch (left->type)
    {
    case INT_V:
        switch (right->type)
        {
        case INT_V:
            if (int_isnzero(left->value) && int_nfit_ull(right->value))
            {
                setup_error2;

                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                mem_overflow_error(poss, pose);
            }

            if (int_isneg(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                neg_shift_error(poss, pose);
            }

            bin_operation(left, right, int_lshift);
        case BOOL_V:
            bin_operation(left, right, int_lshift_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (left->value && int_nfit_ull(right->value))
            {
                setup_error2;

                value_free_ts(right, int_free);
                value_free_vo(left);
                mem_overflow_error(poss, pose);
            }

            if (int_isneg(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_vo(left);
                neg_shift_error(poss, pose);
            }

            bin_operation(left, right, int_ui_lshift);
        case BOOL_V:
            bin_operation_b((uintptr_t)left->value << (uintptr_t)right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("<<", 2, type1, type2, poss, pose);
}

void compute_rshift(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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

                        value_set(res->value, INT_V, int_set_ui(0));
                        return;
                    }

                    int_free(right->value);
                    right->value = int_set_ui(0);
                    res->value = right;
                    return;
                }

                int_free(left->value);
                left->value = int_set_ui(0);
                res->value = left;

                value_free_ts(right, int_free);
                return;
            }

            if (int_isneg(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_ts(left, int_free);
                neg_shift_error(poss, pose);
            }

            bin_operation(left, right, int_rshift);
        case BOOL_V:
            bin_operation(left, right, int_rshift_ui);
        }

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            if (int_isneg(right->value))
            {
                setup_error3;

                value_free_ts(right, int_free);
                value_free_vo(left);
                neg_shift_error(poss, pose);
            }

            bin_operation(left, right, int_ui_rshift);
        case BOOL_V:
            bin_operation_b((uintptr_t)left->value & !right->value);
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error(">>", 2, type1, type2, poss, pose);
}

void compute_eq(visit_res_t *res, value_t *left, value_t *right)
{
    switch (left->type)
    {
    case NONE_V:
        if (right->type == NONE_V)
        {
            if (left->ref)
            {
                left->ref--;

                if (right->ref)
                {
                    right->ref--;

                    value_set(res->value, BOOL_V, (void*)1);
                    return;
                }

                right->type = BOOL_V;
                right->value = (void*)1;
                res->value = right;
                return;
            }

            left->type = BOOL_V;
            left->value = (void*)1;
            res->value = left;

            value_free_vo(right);
            return;
        }

        value_free(right);
        value_free_vo(left);
        goto ret;
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
            bin_operation_b2((uintptr_t)(left->value == right->value));
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
    value_set(res->value, BOOL_V, NULL);
}

void compute_neq(visit_res_t *res, value_t *left, value_t *right)
{
    switch (left->type)
    {
    case NONE_V:
        if (right->type == NONE_V)
        {
            if (left->ref)
            {
                left->ref--;

                if (right->ref)
                {
                    right->ref--;

                    value_set(res->value, BOOL_V, NULL);
                    return;
                }

                right->type = BOOL_V;
                right->value = NULL;
                res->value = right;
                return;
            }

            left->type = BOOL_V;
            left->value = NULL;
            res->value = left;

            value_free_vo(right);
            return;
        }

        value_free(right);
        value_free_vo(left);
        goto ret;
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
            bin_operation_b2((uintptr_t)(left->value != right->value));
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
    value_set(res->value, BOOL_V, (void*)1);
}

void compute_ex_eq(visit_res_t *res, value_t *left, value_t *right)
{
    if (left->type == right->type)
        switch (left->type)
        {
        case NONE_V:
            value_set(res->value, BOOL_V, (void*)1);

            value_free_vo(right);
            value_free_vo(left);
            return;
        case INT_V:
            bin_operation_cmp(left, right, int_eq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_eq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp(left, right, complex_eq, complex_free, complex_free);
        case BOOL_V:
            bin_operation_b2((uintptr_t)(left->value == right->value));
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(left, right, list_eq, list_free, list_free);
        }

    value_free(right);
    value_free(left);

    value_set(res->value, BOOL_V, NULL);
}

void compute_ex_neq(visit_res_t *res, value_t *left, value_t *right)
{
    if (left->type == right->type)
        switch (left->type)
        {
        case NONE_V:
            value_set(res->value, BOOL_V, NULL);

            value_free_vo(right);
            value_free_vo(left);
            return;
        case INT_V:
            bin_operation_cmp(left, right, int_neq, int_free, int_free);
        case FLOAT_V:
            bin_operation_cmp(left, right, float_neq, float_free, float_free);
        case COMPLEX_V:
            bin_operation_cmp(left, right, complex_neq, complex_free, complex_free);
        case BOOL_V:
            bin_operation_b2((uintptr_t)(left->value != right->value));
        case LIST_V:
        case TUPLE_V:
            bin_operation_cmp(left, right, list_neq, list_free, list_free);
        }

    value_free(right);
    value_free(left);

    value_set(res->value, BOOL_V, (void*)1);
}

void compute_lt(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
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

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_gt_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_gt_ui, float_free);
        case BOOL_V:
            bin_operation_b2((uintptr_t)(left->value < right->value));
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("<", 1, type1, type2, poss, pose);
}

void compute_gt(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
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

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_lt_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_lt_ui, float_free);
        case BOOL_V:
            bin_operation_b2((uintptr_t)(left->value > right->value));
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error(">", 1, type1, type2, poss, pose);
}

void compute_lte(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
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

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_gte_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_gte_ui, float_free);
        case BOOL_V:
            bin_operation_b2((uintptr_t)(left->value <= right->value));
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error("<=", 2, type1, type2, poss, pose);
}

void compute_gte(visit_res_t *res, value_t *left, value_t *right)
{
    uint8_t type1, type2;
    pos_t poss, pose;

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

        setup_error;

        value_free(right);
        value_free_ts(left, int_free);
        break;
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

        setup_error;

        value_free(right);
        value_free_ts(left, float_free);
        break;
    case BOOL_V:
        switch (right->type)
        {
        case INT_V:
            bin_operation_cmpb_rev(int_lte_ui, int_free);
        case FLOAT_V:
            bin_operation_cmpb_rev(float_lte_ui, float_free);
        case BOOL_V:
            bin_operation_b2((uintptr_t)(left->value >= right->value));
        }

        setup_error;

        value_free(right);
        value_free_vo(left);
        break;
    default:
        setup_error;

        value_free(right);
        value_free(left);
        break;
    }

    ill_op_error(">=", 2, type1, type2, poss, pose);
}

void compute_pos(visit_res_t *res, value_t *operand, pos_t *poss)
{
    switch (operand->type)
    {
    case INT_V:
    case FLOAT_V:
    case COMPLEX_V:
        res->value = operand;
        return;
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res->value, INT_V, int_set_ui((uintptr_t)operand->value));
            return;
        }

        operand->type = INT_V;
        operand->value = int_set_ui((uintptr_t)operand->value);

        res->value = operand;
        return;
    }

    uint8_t type = operand->type;
    pos_t pose = operand->pose;

    value_free(operand);
    ill_unary_op_error("+", 1, type, *poss, pose);
}

void compute_neg(visit_res_t *res, value_t *operand, pos_t *poss)
{
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

            value_set(res->value, INT_V, int_set_ui(-(uintptr_t)operand->value));
            return;
        }

        operand->type = INT_V;
        operand->value = int_set_ui(-(uintptr_t)operand->value);
        res->value = operand;
        return;
    }

    uint8_t type = operand->type;
    pos_t pose = operand->pose;

    value_free(operand);
    ill_unary_op_error("-", 1, type, *poss, pose);
}

void compute_b_not(visit_res_t *res, value_t *operand, pos_t *poss)
{
    switch (operand->type)
    {
    case INT_V:
        unary_operation(int_not);
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res->value, BOOL_V, (void*)(uintptr_t)!operand->value);
            return;
        }

        operand->value = (void*)(uintptr_t)!operand->value;
        res->value = operand;
        return;
    }

    uint8_t type = operand->type;
    pos_t pose = operand->pose;

    value_free(operand);
    ill_unary_op_error("~", 1, type, *poss, pose);
}

void compute_inc(visit_res_t *res, value_t *operand, pos_t *poss, pos_t *pose)
{
    switch (operand->type)
    {
    case INT_V:
        unary_operation(int_inc);
    case FLOAT_V:
        unary_operation(float_inc);
    case COMPLEX_V:
        unary_operation(complex_inc);
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res->value, INT_V, int_set_ui((uintptr_t)operand->value + 1));
            return;
        }

        operand->type = INT_V;
        operand->value = int_set_ui((uintptr_t)operand->value + 1);

        res->value = operand;
        return;
    }

    ill_inc_dec_op_error("incremented", operand->type, *poss, *pose);
}

void compute_dec(visit_res_t *res, value_t *operand, pos_t *poss, pos_t *pose)
{
    switch (operand->type)
    {
    case INT_V:
        unary_operation(int_dec);
    case FLOAT_V:
        unary_operation(float_dec);
    case COMPLEX_V:
        unary_operation(complex_dec);
    case BOOL_V:
        if (operand->ref)
        {
            operand->ref--;

            value_set(res->value, INT_V, int_set_ui((uintptr_t)operand->value - 1));
            return;
        }

        operand->type = INT_V;
        operand->value = int_set_ui((uintptr_t)operand->value - 1);

        res->value = operand;
        return;
    }

    ill_inc_dec_op_error("decremented", operand->type, *poss, *pose);
}

uint8_t compute_vneq(const value_t *left, const value_t *right)
{
    switch (left->type)
    {
    case NONE_V:
        return right->type != NONE_V;
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
