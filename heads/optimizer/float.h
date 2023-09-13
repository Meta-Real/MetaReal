/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_FLOAT__
#define __MR_FLOAT__

#include <mpfr.h>
#include <optimizer/int.h>

struct __FLOAT_VALUE_T
{
    mpfr_t num;
};
typedef struct __FLOAT_VALUE_T float_value_t;

float_value_t *float_init();
float_value_t *float_set_str(const char *str);
void float_free(float_value_t *num);

char *float_get_str(const float_value_t *num);

void float_print(const float_value_t *num);

void float_add(float_value_t *res, const float_value_t *left, const float_value_t *right);
void float_sub(float_value_t *res, const float_value_t *left, const float_value_t *right);
void float_mul(float_value_t *res, const float_value_t *left, const float_value_t *right);
void float_div(float_value_t *res, const float_value_t *left, const float_value_t *right);

void float_neg(float_value_t *res, const float_value_t *num);

void float_add_int(float_value_t *res, const float_value_t *left, const int_value_t *right);
void float_sub_int(float_value_t *res, const float_value_t *left, const int_value_t *right);
void float_int_sub(float_value_t *res, const int_value_t *left, const float_value_t *right);
void float_mul_int(float_value_t *res, const float_value_t *left, const int_value_t *right);
void float_div_int(float_value_t *res, const float_value_t *left, const int_value_t *right);
void float_int_div(float_value_t *res, const int_value_t *left, const float_value_t *right);
float_value_t *float_int_div_int(const int_value_t *left, const int_value_t *right);

uint8_t float_iszero(const float_value_t *num);

#endif /* __MR_FLOAT__ */
