/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_COMPLEX__
#define __MR_COMPLEX__

#include <mpc.h>
#include <optimizer/float.h>

struct __COMPLEX_VALUE_T
{
    mpc_t num;
};
typedef struct __COMPLEX_VALUE_T complex_value_t;

complex_value_t *complex_init();
complex_value_t *complex_set_str(const char *str);
void complex_free(complex_value_t *num);

char *complex_get_str(const complex_value_t *num);

void complex_print(const complex_value_t *num);

void complex_add(complex_value_t *left, const complex_value_t *right);
void complex_sub(complex_value_t *left, const complex_value_t *right);
void complex_mul(complex_value_t *left, const complex_value_t *right);
void complex_div(complex_value_t *left, const complex_value_t *right);
void complex_pow(complex_value_t *left, const complex_value_t *right);

void complex_neg(complex_value_t *num);

void complex_add_int(complex_value_t *left, const int_value_t *right);
void complex_add_float(complex_value_t *left, const float_value_t *right);
void complex_sub_int(complex_value_t *left, const int_value_t *right);
void complex_int_sub(const int_value_t *left, complex_value_t *right);
void complex_sub_float(complex_value_t *left, const float_value_t *right);
void complex_float_sub(const float_value_t *left, complex_value_t *right);
void complex_mul_int(complex_value_t *left, const int_value_t *right);
void complex_mul_float(complex_value_t *left, const float_value_t *right);
void complex_div_int(complex_value_t *left, const int_value_t *right);
void complex_int_div(const int_value_t *left, complex_value_t *right);
void complex_div_float(complex_value_t *left, const float_value_t *right);
void complex_float_div(const float_value_t *left, complex_value_t *right);
void complex_pow_int(complex_value_t *left, const int_value_t *right);
void complex_int_pow(const int_value_t *left, complex_value_t *right);
void complex_pow_float(complex_value_t *left, const float_value_t *right);
void complex_float_pow(const float_value_t *left, complex_value_t *right);
complex_value_t *complex_int_pow_float(const int_value_t *left, const float_value_t *right);
complex_value_t *complex_float_pow_float(const float_value_t *left, const float_value_t *right);

uint8_t complex_iszero(const complex_value_t *num);

#endif /* __MR_COMPLEX__ */
