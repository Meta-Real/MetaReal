/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/complex.h>
#include <alloc.h>
#include <string.h>

#define complex_binary(f)                                                               \
    do                                                                                  \
    {                                                                                   \
        if (left->ref)                                                                  \
        {                                                                               \
            left->ref--;                                                                \
                                                                                        \
            if (right->ref)                                                             \
            {                                                                           \
                right->ref--;                                                           \
                                                                                        \
                complex_value_t *value = complex_init();                                \
                f(value->num, COMPLEX_CAST(left), COMPLEX_CAST(right), MPC_RNDNN);      \
                                                                                        \
                value_t *res;                                                           \
                value_set(res, COMPLEX_V, value);                                       \
                return res;                                                             \
            }                                                                           \
                                                                                        \
            f(COMPLEX_CAST(right), COMPLEX_CAST(left), COMPLEX_CAST(right), MPC_RNDNN); \
            return right;                                                               \
        }                                                                               \
                                                                                        \
        f(COMPLEX_CAST(left), COMPLEX_CAST(left), COMPLEX_CAST(right), MPC_RNDNN);      \
                                                                                        \
        value_free_ts(right, complex_free);                                             \
        return left;                                                                    \
    } while (0)

#define complex_binary_int(f)                                           \
    do                                                                  \
    {                                                                   \
        if (left->ref)                                                  \
        {                                                               \
            left->ref--;                                                \
                                                                        \
            complex_value_t *value = complex_set_int(right->value);     \
            f(value->num, COMPLEX_CAST(left), value->num, MPC_RNDNN);   \
                                                                        \
            if (right->ref)                                             \
            {                                                           \
                right->ref--;                                           \
                                                                        \
                value_t *res;                                           \
                value_set(res, COMPLEX_V, value);                       \
                return res;                                             \
            }                                                           \
                                                                        \
            int_free(right->value);                                     \
            right->type = COMPLEX_V;                                    \
            right->value = value;                                       \
            return right;                                               \
        }                                                               \
                                                                        \
        mpc_t rcomplex;                                                 \
        mpc_init2(rcomplex, 64);                                        \
        mpc_set_z(rcomplex, INT_CAST(right), MPC_RNDNN);                \
                                                                        \
        f(COMPLEX_CAST(left), COMPLEX_CAST(left), rcomplex, MPC_RNDNN); \
                                                                        \
        mpc_clear(rcomplex);                                            \
        value_free_ts(right, int_free);                                 \
        return left;                                                    \
    } while (0)

#define complex_binary_int_rev(f)                                         \
    do                                                                    \
    {                                                                     \
        if (right->ref)                                                   \
        {                                                                 \
            right->ref--;                                                 \
                                                                          \
            complex_value_t *value = complex_set_int(left->value);        \
            f(value->num, value->num, COMPLEX_CAST(right), MPC_RNDNN);    \
                                                                          \
            if (left->ref)                                                \
            {                                                             \
                left->ref--;                                              \
                                                                          \
                value_t *res;                                             \
                value_set(res, COMPLEX_V, value);                         \
                return res;                                               \
            }                                                             \
                                                                          \
            int_free(left->value);                                        \
            left->type = COMPLEX_V;                                       \
            left->value = value;                                          \
            return left;                                                  \
        }                                                                 \
                                                                          \
        mpc_t lcomplex;                                                   \
        mpc_init2(lcomplex, 64);                                          \
        mpc_set_z(lcomplex, INT_CAST(left), MPC_RNDNN);                   \
                                                                          \
        f(COMPLEX_CAST(right), lcomplex, COMPLEX_CAST(right), MPC_RNDNN); \
                                                                          \
        mpc_clear(lcomplex);                                              \
        value_free_ts(left, int_free);                                    \
        return right;                                                     \
    } while (0)

#define complex_binary_float(f)                                                  \
    do                                                                           \
    {                                                                            \
        if (left->ref)                                                           \
        {                                                                        \
            left->ref--;                                                         \
                                                                                 \
            complex_value_t *value = complex_init();                             \
            f(value->num, COMPLEX_CAST(left), FLOAT_CAST(right), MPC_RNDNN);     \
                                                                                 \
            if (right->ref)                                                      \
            {                                                                    \
                right->ref--;                                                    \
                                                                                 \
                value_t *res;                                                    \
                value_set(res, COMPLEX_V, value);                                \
                return res;                                                      \
            }                                                                    \
                                                                                 \
            float_free(right->value);                                            \
            right->type = COMPLEX_V;                                             \
            right->value = value;                                                \
            return right;                                                        \
        }                                                                        \
                                                                                 \
        f(COMPLEX_CAST(left), COMPLEX_CAST(left), FLOAT_CAST(right), MPC_RNDNN); \
                                                                                 \
        value_free_ts(right, float_free);                                        \
        return left;                                                             \
    } while (0)

#define complex_binary_float_rev(f)                                               \
    do                                                                            \
    {                                                                             \
        if (right->ref)                                                           \
        {                                                                         \
            right->ref--;                                                         \
                                                                                  \
            complex_value_t *value = complex_init();                              \
            f(value->num, FLOAT_CAST(left), COMPLEX_CAST(right), MPC_RNDNN);      \
                                                                                  \
            if (left->ref)                                                        \
            {                                                                     \
                left->ref--;                                                      \
                                                                                  \
                value_t *res;                                                     \
                value_set(res, COMPLEX_V, value);                                 \
                return res;                                                       \
            }                                                                     \
                                                                                  \
            float_free(left->value);                                              \
            left->type = COMPLEX_V;                                               \
            left->value = value;                                                  \
            return left;                                                          \
        }                                                                         \
                                                                                  \
        f(COMPLEX_CAST(right), FLOAT_CAST(left), COMPLEX_CAST(right), MPC_RNDNN); \
                                                                                  \
        value_free_ts(left, float_free);                                          \
        return right;                                                             \
    } while (0)

#define complex_binary_ui(f)                                         \
    do                                                               \
    {                                                                \
        if (left->ref)                                               \
        {                                                            \
            left->ref--;                                             \
                                                                     \
            complex_value_t *value = complex_init();                 \
            f(value->num, COMPLEX_CAST(left), right, MPC_RNDNN);     \
                                                                     \
            value_t *res;                                            \
            value_set(res, COMPLEX_V, value);                        \
            return res;                                              \
        }                                                            \
                                                                     \
        f(COMPLEX_CAST(left), COMPLEX_CAST(left), right, MPC_RNDNN); \
        return left;                                                 \
    } while (0)

#define complex_binary_ui_rev(f)                                      \
    do                                                                \
    {                                                                 \
        if (right->ref)                                               \
        {                                                             \
            right->ref--;                                             \
                                                                      \
            complex_value_t *value = complex_init();                  \
            f(value->num, left, COMPLEX_CAST(right), MPC_RNDNN);      \
                                                                      \
            value_t *res;                                             \
            value_set(res, COMPLEX_V, value);                         \
            return res;                                               \
        }                                                             \
                                                                      \
        f(COMPLEX_CAST(right), left, COMPLEX_CAST(right), MPC_RNDNN); \
        return right;                                                 \
    } while (0)

complex_value_t *complex_init()
{
    complex_value_t *num = mr_alloc(sizeof(complex_value_t));
    mpc_init2(num->num, 64);
    return num;
}

complex_value_t *complex_set_str(const char *str)
{
    complex_value_t *num = mr_alloc(sizeof(complex_value_t));
    mpc_init2(num->num, 64);
    mpfr_set_zero(mpc_realref(num->num), 0);
    mpfr_set_str(mpc_imagref(num->num), str, 10, MPFR_RNDN);
    return num;
}

complex_value_t *complex_set_int(const int_value_t *num)
{
    complex_value_t *res = mr_alloc(sizeof(complex_value_t));
    mpc_init2(res->num, 64);
    mpc_set_z(res->num, num->num, MPC_RNDNN);
    return res;
}

complex_value_t *complex_set_float(const float_value_t *num)
{
    complex_value_t *res = mr_alloc(sizeof(complex_value_t));
    mpc_init2(res->num, 64);
    mpc_set_fr(res->num, num->num, MPC_RNDNN);
    return res;
}

complex_value_t *complex_set_ui(uint32_t ui)
{
    complex_value_t *num = mr_alloc(sizeof(complex_value_t));
    mpc_init2(num->num, 64);
    mpc_set_ui_ui(num->num, ui, 0, MPC_RNDNN);
    return num;
}

void complex_free(complex_value_t *num)
{
    mpc_clear(num->num);
    mr_free(num);
}

char *complex_get_str(const complex_value_t *num)
{
    if (!mpfr_zero_p(mpc_realref(num->num)))
    {
        char *res = mr_alloc(200);
        mpfr_sprintf(res, "(%.16Rg%+.16Rgi)", mpc_realref(num->num), mpc_imagref(num->num));

        res = mr_realloc(res, strlen(res) + 1);
        return res;
    }

    char *res = mr_alloc(100);
    mpfr_sprintf(res, "%.16Rgi", mpc_imagref(num->num));

    res = mr_realloc(res, strlen(res) + 1);
    return res;
}

void complex_print(const complex_value_t *num)
{
    if (!mpfr_zero_p(mpc_realref(num->num)))
        mpfr_printf("(%.16Rg%+.16Rgi)", mpc_realref(num->num), mpc_imagref(num->num));
    else
        mpfr_printf("%.16Rgi", mpc_imagref(num->num));
}

value_t *complex_add(value_t *left, value_t *right)
{
    complex_binary(mpc_add);
}

value_t *complex_sub(value_t *left, value_t *right)
{
    complex_binary(mpc_sub);
}

value_t *complex_mul(value_t *left, value_t *right)
{
    complex_binary(mpc_mul);
}

value_t *complex_div(value_t *left, value_t *right)
{
    complex_binary(mpc_div);
}

value_t *complex_pow(value_t *left, value_t *right)
{
    complex_binary(mpc_pow);
}

value_t *complex_neg(value_t *num)
{
    if (num->ref)
    {
        num->ref--;

        complex_value_t *value = complex_init();
        mpc_neg(value->num, COMPLEX_CAST(num), MPC_RNDNN);

        value_t *res;
        value_set(res, COMPLEX_V, value);
        return res;
    }

    mpc_neg(COMPLEX_CAST(num), COMPLEX_CAST(num), MPC_RNDNN);
    return num;
}

uint8_t complex_eq(const complex_value_t *left, const complex_value_t *right)
{
    return !mpc_cmp(left->num, right->num);
}

uint8_t complex_neq(const complex_value_t *left, const complex_value_t *right)
{
    return mpc_cmp(left->num, right->num) != 0;
}

value_t *complex_add_int(value_t *left, value_t *right)
{
    complex_binary_int(mpc_add);
}

value_t *complex_add_float(value_t *left, value_t *right)
{
    complex_binary_float(mpc_add_fr);
}

value_t *complex_sub_int(value_t *left, value_t *right)
{
    complex_binary_int(mpc_sub);
}

value_t *complex_int_sub(value_t *left, value_t *right)
{
    complex_binary_int_rev(mpc_sub);
}

value_t *complex_sub_float(value_t *left, value_t *right)
{
    complex_binary_float(mpc_sub_fr);
}

value_t *complex_float_sub(value_t *left, value_t *right)
{
    complex_binary_float_rev(mpc_fr_sub);
}

value_t *complex_mul_int(value_t *left, value_t *right)
{
    complex_binary_int(mpc_mul);
}

value_t *complex_mul_float(value_t *left, value_t *right)
{
    complex_binary_float(mpc_mul_fr);
}

value_t *complex_div_int(value_t *left, value_t *right)
{
    complex_binary_int(mpc_div);
}

value_t *complex_int_div(value_t *left, value_t *right)
{
    complex_binary_int_rev(mpc_div);
}

value_t *complex_div_float(value_t *left, value_t *right)
{
    complex_binary_float(mpc_div_fr);
}

value_t *complex_float_div(value_t *left, value_t *right)
{
    complex_binary_float_rev(mpc_fr_div);
}

value_t *complex_pow_int(value_t *left, value_t *right)
{
    if (left->ref)
    {
        left->ref--;

        complex_value_t *value = complex_init();
        mpc_pow_z(value->num, COMPLEX_CAST(left), INT_CAST(right), MPC_RNDNN);

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, COMPLEX_V, value);
            return res;
        }

        int_free(right->value);
        right->type = COMPLEX_V;
        right->value = value;
        return right;
    }

    mpc_pow_z(COMPLEX_CAST(left), COMPLEX_CAST(left), INT_CAST(right), MPC_RNDNN);

    value_free_ts(right, int_free);
    return left;
}

value_t *complex_int_pow(value_t *left, value_t *right)
{
    complex_binary_int_rev(mpc_pow);
}

value_t *complex_pow_float(value_t *left, value_t *right)
{
    complex_binary_float(mpc_pow_fr);
}

value_t *complex_float_pow(value_t *left, value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        complex_value_t *value = complex_set_float(left->value);
        mpc_pow(value->num, value->num, COMPLEX_CAST(right), MPC_RNDNN);

        if (left->ref)
        {
            left->ref--;

            value_t *res;
            value_set(res, COMPLEX_V, value);
            return res;
        }

        float_free(left->value);
        left->type = COMPLEX_V;
        left->value = value;
        return left;
    }

    mpc_t lcomplex;
    mpc_init2(lcomplex, 64);
    mpc_set_fr(lcomplex, FLOAT_CAST(left), MPC_RNDNN);

    mpc_pow(COMPLEX_CAST(right), lcomplex, COMPLEX_CAST(right), MPC_RNDNN);

    mpc_clear(lcomplex);
    value_free_ts(left, float_free);
    return right;
}

value_t *complex_int_pow_float(value_t *left, value_t *right)
{
    complex_value_t *value = complex_set_int(left->value);
    mpc_pow_fr(value->num, value->num, FLOAT_CAST(right), MPC_RNDNN);

    if (left->ref)
    {
        left->ref--;

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, COMPLEX_V, value);
            return res;
        }

        float_free(right->value);
        right->type = COMPLEX_V;
        right->value = value;
        return right;
    }

    value_free_ts(right, float_free);
    int_free(left->value);

    left->type = COMPLEX_V;
    left->value = value;
    return left;
}

value_t *complex_float_pow_float(value_t *left, value_t *right)
{
    complex_value_t *value = complex_set_float(left->value);
    mpc_pow_fr(value->num, value->num, FLOAT_CAST(right), MPC_RNDNN);

    if (left->ref)
    {
        left->ref--;

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, COMPLEX_V, value);
            return res;
        }

        float_free(right->value);
        right->type = COMPLEX_V;
        right->value = value;
        return right;
    }

    value_free_ts(right, float_free);
    float_free(left->value);

    left->type = COMPLEX_V;
    left->value = value;
    return left;
}

uint8_t complex_eq_int(const complex_value_t *left, const int_value_t *right)
{
    if (!mpfr_zero_p(mpc_imagref(left->num)))
        return 0;

    return !mpfr_cmp_z(mpc_realref(left->num), right->num);
}

uint8_t complex_neq_int(const complex_value_t *left, const int_value_t *right)
{
    if (!mpfr_zero_p(mpc_imagref(left->num)))
        return 1;

    return mpfr_cmp_z(mpc_realref(left->num), right->num) != 0;
}

uint8_t complex_eq_float(const complex_value_t *left, const float_value_t *right)
{
    if (!mpfr_zero_p(mpc_imagref(left->num)))
        return 0;

    return mpfr_equal_p(mpc_realref(left->num), right->num) != 0;
}

uint8_t complex_neq_float(const complex_value_t *left, const float_value_t *right)
{
    if (!mpfr_zero_p(mpc_imagref(left->num)))
        return 1;

    return !mpfr_equal_p(mpc_realref(left->num), right->num);
}

value_t *complex_add_ui(value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_add_ui);
}

value_t *complex_sub_ui(value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_sub_ui);
}

value_t *complex_ui_sub(uint32_t left, value_t *right)
{
    complex_binary_ui_rev(mpc_ui_sub);
}

value_t *complex_mul_ui(value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_mul_ui);
}

value_t *complex_div_ui(value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_div_ui);
}

value_t *complex_ui_div(uint32_t left, value_t *right)
{
    complex_binary_ui_rev(mpc_ui_div);
}

value_t *complex_pow_ui(value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_pow_ui);
}

value_t *complex_ui_pow(uint32_t left, value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        complex_value_t *value = complex_set_ui(left);
        mpc_pow(value->num, value->num, COMPLEX_CAST(right), MPC_RNDNN);

        value_t *res;
        value_set(res, COMPLEX_V, value);
        return res;
    }

    mpc_t lcomplex;
    mpc_init2(lcomplex, 64);
    mpc_set_ui(lcomplex, left, MPC_RNDNN);

    mpc_pow(COMPLEX_CAST(right), lcomplex, COMPLEX_CAST(right), MPC_RNDNN);

    mpc_clear(lcomplex);
    return right;
}

uint8_t complex_eq_ui(const complex_value_t *left, uint32_t right)
{
    if (!mpfr_zero_p(mpc_imagref(left->num)))
        return 0;

    return !mpfr_cmp_ui(mpc_realref(left->num), right);
}

uint8_t complex_neq_ui(const complex_value_t *left, uint32_t right)
{
    if (!mpfr_zero_p(mpc_imagref(left->num)))
        return 1;

    return mpfr_cmp_ui(mpc_realref(left->num), right) != 0;
}

uint8_t complex_iszero(const complex_value_t *num)
{
    return mpfr_zero_p(mpc_realref(num->num)) && mpfr_zero_p(mpc_imagref(num->num));
}

uint8_t complex_isnzero(const complex_value_t *num)
{
    return !(mpfr_zero_p(mpc_realref(num->num)) && mpfr_zero_p(mpc_imagref(num->num)));
}
