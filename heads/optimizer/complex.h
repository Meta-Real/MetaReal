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
    uint64_t ref;
};
typedef struct __COMPLEX_VALUE_T complex_value_t;

complex_value_t *complex_init();

complex_value_t *complex_set_str(const char *str);
complex_value_t *complex_set_int(const int_value_t *num);
complex_value_t *complex_set_float(const float_value_t *num);
complex_value_t *complex_set_ui(uint32_t ui);

void complex_free(complex_value_t *num);

char *complex_get_str(const complex_value_t *num);

void complex_print(const complex_value_t *num);

complex_value_t *complex_add(complex_value_t *left, complex_value_t *right);
complex_value_t *complex_sub(complex_value_t *left, complex_value_t *right);
complex_value_t *complex_mul(complex_value_t *left, complex_value_t *right);
complex_value_t *complex_div(complex_value_t *left, complex_value_t *right);
complex_value_t *complex_pow(complex_value_t *left, complex_value_t *right);

complex_value_t *complex_neg(complex_value_t *num);

uint8_t complex_eq(const complex_value_t *left, const complex_value_t *right);
uint8_t complex_neq(const complex_value_t *left, const complex_value_t *right);

complex_value_t *complex_add_int(complex_value_t *left, int_value_t *right);
complex_value_t *complex_add_float(complex_value_t *left, float_value_t *right);
complex_value_t *complex_sub_int(complex_value_t *left, int_value_t *right);
complex_value_t *complex_int_sub(int_value_t *left, complex_value_t *right);
complex_value_t *complex_sub_float(complex_value_t *left, float_value_t *right);
complex_value_t *complex_float_sub(float_value_t *left, complex_value_t *right);
complex_value_t *complex_mul_int(complex_value_t *left, int_value_t *right);
complex_value_t *complex_mul_float(complex_value_t *left, float_value_t *right);
complex_value_t *complex_div_int(complex_value_t *left, int_value_t *right);
complex_value_t *complex_int_div(int_value_t *left, complex_value_t *right);
complex_value_t *complex_div_float(complex_value_t *left, float_value_t *right);
complex_value_t *complex_float_div(float_value_t *left, complex_value_t *right);
complex_value_t *complex_pow_int(complex_value_t *left, int_value_t *right);
complex_value_t *complex_int_pow(int_value_t *left, complex_value_t *right);
complex_value_t *complex_pow_float(complex_value_t *left, float_value_t *right);
complex_value_t *complex_float_pow(float_value_t *left, complex_value_t *right);
complex_value_t *complex_int_pow_float(int_value_t *left, float_value_t *right);
complex_value_t *complex_float_pow_float(float_value_t *left, float_value_t *right);

uint8_t complex_eq_int(const complex_value_t *left, const int_value_t *right);
uint8_t complex_neq_int(const complex_value_t *left, const int_value_t *right);
uint8_t complex_eq_float(const complex_value_t *left, const float_value_t *right);
uint8_t complex_neq_float(const complex_value_t *left, const float_value_t *right);

complex_value_t *complex_add_ui(complex_value_t *left, uint32_t right);
complex_value_t *complex_sub_ui(complex_value_t *left, uint32_t right);
complex_value_t *complex_ui_sub(uint32_t left, complex_value_t *right);
complex_value_t *complex_mul_ui(complex_value_t *left, uint32_t right);
complex_value_t *complex_div_ui(complex_value_t *left, uint32_t right);
complex_value_t *complex_ui_div(uint32_t left, complex_value_t *right);
complex_value_t *complex_pow_ui(complex_value_t *left, uint32_t right);
complex_value_t *complex_ui_pow(uint32_t left, complex_value_t *right);

uint8_t complex_eq_ui(const complex_value_t *left, uint32_t right);
uint8_t complex_neq_ui(const complex_value_t *left, uint32_t right);

uint8_t complex_iszero(const complex_value_t *num);
uint8_t complex_isnzero(const complex_value_t *num);

#endif /* __MR_COMPLEX__ */
