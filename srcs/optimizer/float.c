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

float_value_t *float_set_ui(uint32_t ui)
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    mpfr_set_ui(num->num, ui, MPFR_RNDN);
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

int_value_t *float_get_int(const float_value_t *num)
{
    int_value_t *res = mr_alloc(sizeof(int_value_t));
    mpz_init(res->num);
    mpfr_get_z(res->num, num->num, MPFR_RNDZ);
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
    mpfr_get_z(res->num, left->num, MPFR_RNDZ);
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

uint8_t float_eq(const float_value_t *left, const float_value_t *right)
{
    return mpfr_equal_p(left->num, right->num) != 0;
}

uint8_t float_lt(const float_value_t *left, const float_value_t *right)
{
    return mpfr_less_p(left->num, right->num) != 0;
}

uint8_t float_gt(const float_value_t *left, const float_value_t *right)
{
    return mpfr_greater_p(left->num, right->num) != 0;
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
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_z(lfloat, left->num, MPFR_RNDN);

    mpfr_div(right->num, lfloat, right->num, MPFR_RNDN);

    mpfr_clear(lfloat);
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
    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_z(rfloat, right->num, MPFR_RNDN);

    mpfr_remainder(left->num, left->num, rfloat, MPFR_RNDN);

    mpfr_clear(rfloat);
}

void float_int_mod(const int_value_t *left, float_value_t *right)
{
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_z(lfloat, left->num, MPFR_RNDN);

    mpfr_remainder(right->num, lfloat, right->num, MPFR_RNDN);

    mpfr_clear(lfloat);
}

void float_quot_int(float_value_t *left, int_value_t *right)
{
    mpfr_div_z(left->num, left->num, right->num, MPFR_RNDN);
    mpfr_get_z(right->num, left->num, MPFR_RNDZ);
}

void float_int_quot(int_value_t *left, const float_value_t *right)
{
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_z(lfloat, left->num, MPFR_RNDN);

    mpfr_div(lfloat, lfloat, right->num, MPFR_RNDN);
    mpfr_get_z(left->num, lfloat, MPFR_RNDZ);

    mpfr_clear(lfloat);
}

void float_pow_int(float_value_t *left, const int_value_t *right)
{
    mpfr_pow_z(left->num, left->num, right->num, MPFR_RNDN);
}

void float_int_pow(const int_value_t *left, float_value_t *right)
{
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_z(lfloat, left->num, MPFR_RNDN);

    mpfr_pow(right->num, lfloat, right->num, MPFR_RNDN);

    mpfr_clear(lfloat);
}

float_value_t *float_int_pow_int(const int_value_t *left, const int_value_t *right)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_z(res->num, left->num, MPFR_RNDN);

    mpfr_pow_z(res->num, res->num, right->num, MPFR_RNDN);
    return res;
}

uint8_t float_eq_int(const float_value_t *left, const int_value_t *right)
{
    return !mpfr_cmp_z(left->num, right->num);
}

uint8_t float_lt_int(const float_value_t *left, const int_value_t *right)
{
    return mpfr_cmp_z(left->num, right->num) < 0;
}

uint8_t float_gt_int(const float_value_t *left, const int_value_t *right)
{
    return mpfr_cmp_z(left->num, right->num) > 0;
}

void float_add_ui(float_value_t *left, uint32_t right)
{
    mpfr_add_ui(left->num, left->num, right, MPFR_RNDN);
}

void float_sub_ui(float_value_t *left, uint32_t right)
{
    mpfr_sub_ui(left->num, left->num, right, MPFR_RNDN);
}

void float_ui_sub(uint32_t left, float_value_t *right)
{
    mpfr_ui_sub(right->num, left, right->num, MPFR_RNDN);
}

void float_mul_ui(float_value_t *left, uint32_t right)
{
    mpfr_mul_ui(left->num, left->num, right, MPFR_RNDN);
}

void float_div_ui(float_value_t *left, uint32_t right)
{
    mpfr_div_ui(left->num, left->num, right, MPFR_RNDN);
}

void float_ui_div(uint32_t left, float_value_t *right)
{
    mpfr_ui_div(right->num, left, right->num, MPFR_RNDN);
}

float_value_t *float_int_div_ui(const int_value_t *left, uint32_t right)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_z(res->num, left->num, MPFR_RNDN);

    mpfr_div_ui(res->num, res->num, right, MPFR_RNDN);
    return res;
}

float_value_t *float_ui_div_int(uint32_t left, const int_value_t *right)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_ui(res->num, left, MPFR_RNDN);

    mpfr_div_z(res->num, res->num, right->num, MPFR_RNDN);
    return res;
}

float_value_t *float_ui_div_ui(uint32_t left, uint32_t right)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_ui(res->num, left, MPFR_RNDN);

    mpfr_div_ui(res->num, res->num, right, MPFR_RNDN);
    return res;
}

void float_mod_ui(float_value_t *left, uint32_t right)
{
    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_ui(rfloat, right, MPFR_RNDN);

    mpfr_remainder(left->num, left->num, rfloat, MPFR_RNDN);

    mpfr_clear(rfloat);
}

void float_ui_mod(uint32_t left, float_value_t *right)
{
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_ui(lfloat, left, MPFR_RNDN);

    mpfr_remainder(right->num, lfloat, right->num, MPFR_RNDN);

    mpfr_clear(lfloat);
}

int_value_t *float_quot_ui(float_value_t *left, uint32_t right)
{
    int_value_t *res = mr_alloc(sizeof(int_value_t));
    mpz_init(res->num);

    mpfr_div_ui(left->num, left->num, right, MPFR_RNDN);
    mpfr_get_z(res->num, left->num, MPFR_RNDZ);
    return res;
}

int_value_t *float_ui_quot(uint32_t left, float_value_t *right)
{
    int_value_t *res = mr_alloc(sizeof(int_value_t));
    mpz_init(res->num);

    mpfr_ui_div(right->num, left, right->num, MPFR_RNDN);
    mpfr_get_z(res->num, right->num, MPFR_RNDZ);
    return res;
}

void float_pow_ui(float_value_t *left, uint32_t right)
{
    mpfr_pow_ui(left->num, left->num, right, MPFR_RNDN);
}

void float_ui_pow(uint32_t left, float_value_t *right)
{
    mpfr_ui_pow(right->num, left, right->num, MPFR_RNDN);
}

float_value_t *float_ui_pow_int(uint32_t left, const int_value_t *right)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_ui(res->num, left, MPFR_RNDN);

    mpfr_pow_z(res->num, res->num, right->num, MPFR_RNDN);
    return res;
}

uint8_t float_eq_ui(const float_value_t *left, uint32_t right)
{
    return !mpfr_cmp_ui(left->num, right);
}

uint8_t float_lt_ui(const float_value_t *left, uint32_t right)
{
    return mpfr_cmp_ui(left->num, right) < 0;
}

uint8_t float_gt_ui(const float_value_t *left, uint32_t right)
{
    return mpfr_cmp_ui(left->num, right) > 0;
}

uint8_t float_iszero(const float_value_t *num)
{
    return mpfr_zero_p(num->num);
}

uint8_t float_isneg(const float_value_t *num)
{
    return mpfr_sgn(num->num) < 0;
}
