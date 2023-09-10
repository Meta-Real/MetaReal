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

void float_add(float_value_t *res, const float_value_t *left, const float_value_t *right)
{
    mpfr_add(res->num, left->num, right->num, MPFR_RNDN);
}

void float_sub(float_value_t *res, const float_value_t *left, const float_value_t *right)
{
    mpfr_sub(res->num, left->num, right->num, MPFR_RNDN);
}

void float_mul(float_value_t *res, const float_value_t *left, const float_value_t *right)
{
    mpfr_mul(res->num, left->num, right->num, MPFR_RNDN);
}

void float_div(float_value_t *res, const float_value_t *left, const float_value_t *right)
{
    mpfr_div(res->num, left->num, right->num, MPFR_RNDN);
}

void float_neg(float_value_t *res, const float_value_t *num)
{
    mpfr_neg(res->num, num->num, MPFR_RNDN);
}

void float_add_int(float_value_t *res, const float_value_t *left, const int_value_t *right)
{
    mpfr_add_z(res->num, left->num, right->num, MPFR_RNDN);
}

void float_sub_int(float_value_t *res, const float_value_t *left, const int_value_t *right)
{
    mpfr_sub_z(res->num, left->num, right->num, MPFR_RNDN);
}

void float_int_sub(float_value_t *res, const int_value_t *left, const float_value_t *right)
{
    mpfr_z_sub(res->num, left->num, right->num, MPFR_RNDN);
}

void float_mul_int(float_value_t *res, const float_value_t *left, const int_value_t *right)
{
    mpfr_mul_z(res->num, left->num, right->num, MPFR_RNDN);
}

void float_div_int(float_value_t *res, const float_value_t *left, const int_value_t *right)
{
    mpfr_div_z(res->num, left->num, right->num, MPFR_RNDN);
}

void float_int_div(float_value_t *res, const int_value_t *left, const float_value_t *right)
{
    mpfr_set_z(res->num, left->num, MPFR_RNDN);
    mpfr_div(res->num, res->num, right->num, MPFR_RNDN);
}

float_value_t *float_int_div_int(const int_value_t *left, const int_value_t *right)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);

    mpfr_set_z(res->num, left->num, MPFR_RNDN);
    mpfr_div_z(res->num, res->num, right->num, MPFR_RNDN);
    return res;
}
