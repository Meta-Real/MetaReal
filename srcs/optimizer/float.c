/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/float.h>
#include <alloc.h>
#include <string.h>

float_value_t *float_init()
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    return num;
}

float_value_t *float_set_str(const char *str)
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    mpfr_set_str(num->num, str, 10, MPFR_RNDN);
    return num;
}

void float_free(float_value_t *num)
{
    mpfr_clear(num->num);
    mr_free(num);
}

char *float_get_str(const float_value_t *num)
{
    char* res = mr_alloc(100);
    mpfr_sprintf(res, "%.16Rg", num->num);

    res = mr_realloc(res, strlen(res) + 1);
    return res;
}

void float_print(const float_value_t *num)
{
    mpfr_printf("%.16Rg", num->num);
}

void float_add(float_value_t *left, const float_value_t *right)
{
    mpfr_add(left->num, left->num, right->num, MPFR_RNDN);
}

void float_sub(float_value_t *left, const float_value_t *right)
{
    mpfr_sub(left->num, left->num, right->num, MPFR_RNDN);
}

void float_mul(float_value_t *left, const float_value_t *right)
{
    mpfr_mul(left->num, left->num, right->num, MPFR_RNDN);
}

void float_div(float_value_t *left, const float_value_t *right)
{
    mpfr_div(left->num, left->num, right->num, MPFR_RNDN);
}

void float_mod(float_value_t *left, const float_value_t *right)
{
    mpfr_remainder(left->num, left->num, right->num, MPFR_RNDN);
}

int_value_t *float_quot(float_value_t *left, const float_value_t *right)
{
    int_value_t *res = mr_alloc(sizeof(int_value_t));
    mpz_init(res->num);

    mpfr_div(left->num, left->num, right->num, MPFR_RNDN);
    mpfr_get_z(res->num, left->num, MPFR_RNDD);
    return res;
}

void float_pow(float_value_t *left, const float_value_t *right)
{
    mpfr_pow(left->num, left->num, right->num, MPFR_RNDN);
}

void float_neg(float_value_t *num)
{
    mpfr_neg(num->num, num->num, MPFR_RNDN);
}

void float_add_int(float_value_t *left, const int_value_t *right)
{
    mpfr_add_z(left->num, left->num, right->num, MPFR_RNDN);
}

void float_sub_int(float_value_t *left, const int_value_t *right)
{
    mpfr_sub_z(left->num, left->num, right->num, MPFR_RNDN);
}

void float_int_sub(const int_value_t *left, float_value_t *right)
{
    mpfr_z_sub(right->num, left->num, right->num, MPFR_RNDN);
}

void float_mul_int(float_value_t *left, const int_value_t *right)
{
    mpfr_mul_z(left->num, left->num, right->num, MPFR_RNDN);
}

void float_div_int(float_value_t *left, const int_value_t *right)
{
    mpfr_div_z(left->num, left->num, right->num, MPFR_RNDN);
}

void float_int_div(const int_value_t *left, float_value_t *right)
{
    mpfr_t lint;
    mpfr_init2(lint, 64);
    mpfr_set_z(lint, left->num, MPFR_RNDN);

    mpfr_div(right->num, lint, right->num, MPFR_RNDN);

    mpfr_clear(lint);
}

float_value_t *float_int_div_int(const int_value_t *left, const int_value_t *right)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_z(res->num, left->num, MPFR_RNDN);

    mpfr_div_z(res->num, res->num, right->num, MPFR_RNDN);
    return res;
}

void float_mod_int(float_value_t *left, const int_value_t *right)
{
    mpfr_t rint;
    mpfr_init2(rint, 64);
    mpfr_set_z(rint, right->num, MPFR_RNDN);

    mpfr_remainder(left->num, left->num, rint, MPFR_RNDN);

    mpfr_clear(rint);
}

void float_int_mod(const int_value_t *left, float_value_t *right)
{
    mpfr_t lint;
    mpfr_init2(lint, 64);
    mpfr_set_z(lint, left->num, MPFR_RNDN);

    mpfr_remainder(right->num, lint, right->num, MPFR_RNDN);

    mpfr_clear(lint);
}

void float_quot_int(float_value_t *left, int_value_t *right)
{
    mpfr_div_z(left->num, left->num, right->num, MPFR_RNDN);
    mpfr_get_z(right->num, left->num, MPFR_RNDD);
}

void float_int_quot(int_value_t *left, const float_value_t *right)
{
    mpfr_t lint;
    mpfr_init2(lint, 64);
    mpfr_set_z(lint, left->num, MPFR_RNDN);

    mpfr_div(lint, lint, right->num, MPFR_RNDN);
    mpfr_get_z(left->num, lint, MPFR_RNDD);

    mpfr_clear(lint);
}

void float_pow_int(float_value_t *left, const int_value_t *right)
{
    mpfr_pow_z(left->num, left->num, right->num, MPFR_RNDN);
}

void float_int_pow(const int_value_t *left, float_value_t *right)
{
    mpfr_t lint;
    mpfr_init2(lint, 64);
    mpfr_set_z(lint, left->num, MPFR_RNDN);

    mpfr_pow(right->num, lint, right->num, MPFR_RNDN);

    mpfr_clear(lint);
}

float_value_t *float_int_pow_int(const int_value_t *left, const int_value_t *right)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_z(res->num, left->num, MPFR_RNDN);

    mpfr_pow_z(res->num, res->num, right->num, MPFR_RNDN);
    return res;
}

uint8_t float_iszero(const float_value_t *num)
{
    return mpfr_zero_p(num->num);
}

uint8_t float_isneg(const float_value_t *num)
{
    return mpfr_sgn(num->num) < 0;
}
