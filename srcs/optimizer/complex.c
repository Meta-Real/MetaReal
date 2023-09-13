/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/complex.h>
#include <alloc.h>
#include <string.h>

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

void complex_add(complex_value_t *left, const complex_value_t *right)
{
    mpc_add(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_sub(complex_value_t *left, const complex_value_t *right)
{
    mpc_sub(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_mul(complex_value_t *left, const complex_value_t *right)
{
    mpc_mul(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_div(complex_value_t *left, const complex_value_t *right)
{
    mpc_div(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_pow(complex_value_t *left, const complex_value_t *right)
{
    mpc_pow(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_neg(complex_value_t *num)
{
    mpc_neg(num->num, num->num, MPC_RNDNN);
}

void complex_add_int(complex_value_t *left, const int_value_t *right)
{
    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_z(rfloat, right->num, MPFR_RNDN);

    mpc_add_fr(left->num, left->num, rfloat, MPC_RNDNN);

    mpfr_clear(rfloat);
}

void complex_add_float(complex_value_t *left, const float_value_t *right)
{
    mpc_add_fr(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_sub_int(complex_value_t *left, const int_value_t *right)
{
    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_z(rfloat, right->num, MPFR_RNDN);

    mpc_sub_fr(left->num, left->num, rfloat, MPC_RNDNN);

    mpfr_clear(rfloat);
}

void complex_int_sub(const int_value_t *left, complex_value_t *right)
{
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_z(lfloat, left->num, MPFR_RNDN);

    mpc_fr_sub(right->num, lfloat, right->num, MPC_RNDNN);

    mpfr_clear(lfloat);
}

void complex_sub_float(complex_value_t *left, const float_value_t *right)
{
    mpc_sub_fr(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_float_sub(const float_value_t *left, complex_value_t *right)
{
    mpc_fr_sub(right->num, left->num, right->num, MPC_RNDNN);
}

void complex_mul_int(complex_value_t *left, const int_value_t *right)
{
    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_z(rfloat, right->num, MPFR_RNDN);

    mpc_mul_fr(left->num, left->num, rfloat, MPC_RNDNN);

    mpfr_clear(rfloat);
}

void complex_mul_float(complex_value_t *left, const float_value_t *right)
{
    mpc_mul_fr(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_div_int(complex_value_t *left, const int_value_t *right)
{
    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_z(rfloat, right->num, MPFR_RNDN);

    mpc_div_fr(left->num, left->num, rfloat, MPC_RNDNN);

    mpfr_clear(rfloat);
}

void complex_int_div(const int_value_t *left, complex_value_t *right)
{
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_z(lfloat, left->num, MPFR_RNDN);

    mpc_fr_div(right->num, lfloat, right->num, MPC_RNDNN);

    mpfr_clear(lfloat);
}

void complex_div_float(complex_value_t *left, const float_value_t *right)
{
    mpc_div_fr(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_float_div(const float_value_t *left, complex_value_t *right)
{
    mpc_fr_div(right->num, left->num, right->num, MPC_RNDNN);
}

void complex_pow_int(complex_value_t *left, const int_value_t *right)
{
    mpc_pow_z(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_int_pow(const int_value_t *left, complex_value_t *right)
{
    mpc_t lcomplex;
    mpc_init2(lcomplex, 64);
    mpc_set_z(lcomplex, left->num, MPC_RNDNN);

    mpc_pow(right->num, lcomplex, right->num, MPC_RNDNN);

    mpc_clear(lcomplex);
}

void complex_pow_float(complex_value_t *left, const float_value_t *right)
{
    mpc_pow_fr(left->num, left->num, right->num, MPC_RNDNN);
}

void complex_float_pow(const float_value_t *left, complex_value_t *right)
{
    mpc_t lcomplex;
    mpc_init2(lcomplex, 64);
    mpc_set_fr(lcomplex, left->num, MPC_RNDNN);

    mpc_pow(right->num, lcomplex, right->num, MPC_RNDNN);

    mpc_clear(lcomplex);
}

complex_value_t *complex_int_pow_float(const int_value_t *left, const float_value_t *right)
{
    complex_value_t *res = mr_alloc(sizeof(complex_value_t));
    mpc_init2(res->num, 64);
    mpc_set_z(res->num, left->num, MPC_RNDNN);

    mpc_pow_fr(res->num, res->num, right->num, MPC_RNDNN);
    return res;
}

complex_value_t *complex_float_pow_float(const float_value_t *left, const float_value_t *right)
{
    complex_value_t *res = mr_alloc(sizeof(complex_value_t));
    mpc_init2(res->num, 64);
    mpc_set_fr(res->num, left->num, MPC_RNDNN);

    mpc_pow_fr(res->num, res->num, right->num, MPC_RNDNN);
    return res;
}

uint8_t complex_iszero(const complex_value_t *num)
{
    return !(mpfr_zero_p(mpc_realref(num->num)) || mpfr_zero_p(mpc_imagref(num->num)));
}
