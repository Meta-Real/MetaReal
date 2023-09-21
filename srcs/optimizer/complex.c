/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/complex.h>
#include <alloc.h>
#include <string.h>

#define complex_binary(f)                                      \
    do                                                         \
    {                                                          \
        if (left->ref)                                         \
        {                                                      \
            left->ref--;                                       \
                                                               \
            if (right->ref)                                    \
            {                                                  \
                right->ref--;                                  \
                                                               \
                complex_value_t *res = complex_init();         \
                f(res->num, left->num, right->num, MPC_RNDNN); \
                return res;                                    \
            }                                                  \
                                                               \
            f(right->num, left->num, right->num, MPC_RNDNN);   \
            return right;                                      \
        }                                                      \
                                                               \
        f(left->num, left->num, right->num, MPC_RNDNN);        \
                                                               \
        complex_free(right);                                   \
        return left;                                           \
    } while (0)

#define complex_binary_float(f)                            \
    do                                                     \
    {                                                      \
        if (left->ref)                                     \
        {                                                  \
            left->ref--;                                   \
                                                           \
            complex_value_t *res = complex_init();         \
            f(res->num, left->num, right->num, MPC_RNDNN); \
                                                           \
            float_free(right);                             \
            return res;                                    \
        }                                                  \
                                                           \
        f(left->num, left->num, right->num, MPC_RNDNN);    \
                                                           \
        float_free(right);                                 \
        return left;                                       \
    } while (0)

#define complex_binary_float_rev(f)                        \
    do                                                     \
    {                                                      \
        if (right->ref)                                    \
        {                                                  \
            right->ref--;                                  \
                                                           \
            complex_value_t *res = complex_init();         \
            f(res->num, left->num, right->num, MPC_RNDNN); \
                                                           \
            float_free(left);                              \
            return res;                                    \
        }                                                  \
                                                           \
        f(right->num, left->num, right->num, MPC_RNDNN);   \
                                                           \
        float_free(left);                                  \
        return right;                                      \
    } while (0)

#define complex_binary_int(f)                          \
    do                                                 \
    {                                                  \
        mpfr_t rfloat;                                 \
        mpfr_init2(rfloat, 64);                        \
        mpfr_set_z(rfloat, right->num, MPFR_RNDN);     \
                                                       \
        if (left->ref)                                 \
        {                                              \
            left->ref--;                               \
                                                       \
            complex_value_t *res = complex_init();     \
            f(res->num, left->num, rfloat, MPC_RNDNN); \
                                                       \
            mpfr_clear(rfloat);                        \
            int_free(right);                           \
            return res;                                \
        }                                              \
                                                       \
        f(left->num, left->num, rfloat, MPC_RNDNN);    \
                                                       \
        mpfr_clear(rfloat);                            \
        int_free(right);                               \
        return left;                                   \
    } while (0)

#define complex_binary_int_rev(f)                       \
    do                                                  \
    {                                                   \
        mpfr_t lfloat;                                  \
        mpfr_init2(lfloat, 64);                         \
        mpfr_set_z(lfloat, left->num, MPFR_RNDN);       \
                                                        \
        if (right->ref)                                 \
        {                                               \
            right->ref--;                               \
                                                        \
            complex_value_t *res = complex_init();      \
            f(res->num, lfloat, right->num, MPC_RNDNN); \
                                                        \
            mpfr_clear(lfloat);                         \
            int_free(left);                             \
            return res;                                 \
        }                                               \
                                                        \
        f(right->num, lfloat, right->num, MPC_RNDNN);   \
                                                        \
        mpfr_clear(lfloat);                             \
        int_free(left);                                 \
        return right;                                   \
    } while (0)

#define complex_binary_ui(f)                          \
    do                                                \
    {                                                 \
        if (left->ref)                                \
        {                                             \
            left->ref--;                              \
                                                      \
            complex_value_t *res = complex_init();    \
            f(res->num, left->num, right, MPC_RNDNN); \
            return res;                               \
        }                                             \
                                                      \
        f(left->num, left->num, right, MPC_RNDNN);    \
        return left;                                  \
    } while (0)

#define complex_binary_ui_rev(f)                      \
    do                                                \
    {                                                 \
        if (right->ref)                               \
        {                                             \
            right->ref--;                             \
                                                      \
            complex_value_t *res = complex_init();    \
            f(res->num, left, right->num, MPC_RNDNN); \
            return res;                               \
        }                                             \
                                                      \
        f(right->num, left, right->num, MPC_RNDNN);   \
        return right;                                 \
    } while (0)

complex_value_t *complex_init()
{
    complex_value_t *num = mr_alloc(sizeof(complex_value_t));
    mpc_init2(num->num, 64);
    num->ref = 0;
    return num;
}

complex_value_t *complex_set_str(const char *str)
{
    complex_value_t *num = mr_alloc(sizeof(complex_value_t));
    mpc_init2(num->num, 64);
    mpfr_set_zero(mpc_realref(num->num), 0);
    mpfr_set_str(mpc_imagref(num->num), str, 10, MPFR_RNDN);
    num->ref = 0;
    return num;
}

complex_value_t *complex_set_int(const int_value_t *num)
{
    complex_value_t *res = mr_alloc(sizeof(complex_value_t));
    mpc_init2(res->num, 64);
    mpc_set_z(res->num, num->num, MPC_RNDNN);
    res->ref = 0;
    return res;
}

complex_value_t *complex_set_float(const float_value_t *num)
{
    complex_value_t *res = mr_alloc(sizeof(complex_value_t));
    mpc_init2(res->num, 64);
    mpc_set_fr(res->num, num->num, MPC_RNDNN);
    res->ref = 0;
    return res;
}

complex_value_t *complex_set_ui(uint32_t ui)
{
    complex_value_t *num = mr_alloc(sizeof(complex_value_t));
    mpc_init2(num->num, 64);
    mpc_set_ui_ui(num->num, ui, 0, MPC_RNDNN);
    num->ref = 0;
    return num;
}

void complex_free(complex_value_t *num)
{
    if (num->ref)
    {
        num->ref--;
        return;
    }

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

complex_value_t *complex_add(complex_value_t *left, complex_value_t *right)
{
    complex_binary(mpc_add);
}

complex_value_t *complex_sub(complex_value_t *left, complex_value_t *right)
{
    complex_binary(mpc_sub);
}

complex_value_t *complex_mul(complex_value_t *left, complex_value_t *right)
{
    complex_binary(mpc_mul);
}

complex_value_t *complex_div(complex_value_t *left, complex_value_t *right)
{
    complex_binary(mpc_div);
}

complex_value_t *complex_pow(complex_value_t *left, complex_value_t *right)
{
    complex_binary(mpc_pow);
}

complex_value_t *complex_neg(complex_value_t *num)
{
    if (num->ref)
    {
        num->ref--;

        complex_value_t *res = complex_init();
        mpc_neg(res->num, num->num, MPC_RNDNN);
        return res;
    }

    mpc_neg(num->num, num->num, MPC_RNDNN);
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

complex_value_t *complex_add_int(complex_value_t *left, int_value_t *right)
{
    complex_binary_int(mpc_add_fr);
}

complex_value_t *complex_add_float(complex_value_t *left, float_value_t *right)
{
    complex_binary_float(mpc_add_fr);
}

complex_value_t *complex_sub_int(complex_value_t *left, int_value_t *right)
{
    complex_binary_int(mpc_sub_fr);
}

complex_value_t *complex_int_sub(int_value_t *left, complex_value_t *right)
{
    complex_binary_int_rev(mpc_fr_sub);
}

complex_value_t *complex_sub_float(complex_value_t *left, float_value_t *right)
{
    complex_binary_float(mpc_sub_fr);
}

complex_value_t *complex_float_sub(float_value_t *left, complex_value_t *right)
{
    complex_binary_float_rev(mpc_fr_sub);
}

complex_value_t *complex_mul_int(complex_value_t *left, int_value_t *right)
{
    complex_binary_int(mpc_mul_fr);
}

complex_value_t *complex_mul_float(complex_value_t *left, float_value_t *right)
{
    complex_binary_float(mpc_mul_fr);
}

complex_value_t *complex_div_int(complex_value_t *left, int_value_t *right)
{
    complex_binary_int(mpc_div_fr);
}

complex_value_t *complex_int_div(int_value_t *left, complex_value_t *right)
{
    complex_binary_int_rev(mpc_fr_div);
}

complex_value_t *complex_div_float(complex_value_t *left, float_value_t *right)
{
    complex_binary_float(mpc_div_fr);
}

complex_value_t *complex_float_div(float_value_t *left, complex_value_t *right)
{
    complex_binary_float_rev(mpc_fr_div);
}

complex_value_t *complex_pow_int(complex_value_t *left, int_value_t *right)
{
    if (left->ref)
    {
        left->ref--;

        complex_value_t *res = complex_init();
        mpc_pow_z(res->num, left->num, right->num, MPC_RNDNN);

        int_free(right);
        return res;
    }

    mpc_pow_z(left->num, left->num, right->num, MPC_RNDNN);

    int_free(right);
    return left;
}

complex_value_t *complex_int_pow(int_value_t *left, complex_value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        complex_value_t *res = complex_set_int(left);
        mpc_pow(res->num, res->num, right->num, MPC_RNDNN);

        int_free(left);
        return res;
    }

    mpc_t lcomplex;
    mpc_init2(lcomplex, 64);
    mpc_set_z(lcomplex, left->num, MPC_RNDNN);

    mpc_pow(right->num, lcomplex, right->num, MPC_RNDNN);

    mpc_clear(lcomplex);
    int_free(left);
    return right;
}

complex_value_t *complex_pow_float(complex_value_t *left, float_value_t *right)
{
    complex_binary_float(mpc_pow_fr);
}

complex_value_t *complex_float_pow(float_value_t *left, complex_value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        complex_value_t *res = complex_set_float(left);
        mpc_pow(res->num, res->num, right->num, MPC_RNDNN);

        float_free(left);
        return res;
    }

    mpc_t lcomplex;
    mpc_init2(lcomplex, 64);
    mpc_set_fr(lcomplex, left->num, MPC_RNDNN);

    mpc_pow(right->num, lcomplex, right->num, MPC_RNDNN);

    mpc_clear(lcomplex);
    float_free(left);
    return right;
}

complex_value_t *complex_int_pow_float(int_value_t *left, float_value_t *right)
{
    complex_value_t *res = complex_set_int(left);
    mpc_pow_fr(res->num, res->num, right->num, MPC_RNDNN);

    float_free(right);
    int_free(left);
    return res;
}

complex_value_t *complex_float_pow_float(float_value_t *left, float_value_t *right)
{
    complex_value_t *res = complex_set_float(left);
    mpc_pow_fr(res->num, res->num, right->num, MPC_RNDNN);

    float_free(right);
    float_free(left);
    return res;
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

complex_value_t *complex_add_ui(complex_value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_add_ui);
}

complex_value_t *complex_sub_ui(complex_value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_sub_ui);
}

complex_value_t *complex_ui_sub(uint32_t left, complex_value_t *right)
{
    complex_binary_ui_rev(mpc_ui_sub);
}

complex_value_t *complex_mul_ui(complex_value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_mul_ui);
}

complex_value_t *complex_div_ui(complex_value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_div_ui);
}

complex_value_t *complex_ui_div(uint32_t left, complex_value_t *right)
{
    complex_binary_ui_rev(mpc_ui_div);
}

complex_value_t *complex_pow_ui(complex_value_t *left, uint32_t right)
{
    complex_binary_ui(mpc_pow_ui);
}

complex_value_t *complex_ui_pow(uint32_t left, complex_value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        complex_value_t *res = complex_set_ui(left);
        mpc_pow(res->num, res->num, right->num, MPC_RNDNN);
        return res;
    }

    mpc_t lcomplex;
    mpc_init2(lcomplex, 64);
    mpc_set_ui(lcomplex, left, MPC_RNDNN);

    mpc_pow(right->num, lcomplex, right->num, MPC_RNDNN);

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
