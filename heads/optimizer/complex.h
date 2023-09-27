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

#define COMPLEX_CAST(v) ((complex_value_t*)v->value)->num

complex_value_t *complex_init();

complex_value_t *complex_set_str(const char *str);
complex_value_t *complex_set_int(const int_value_t *num);
complex_value_t *complex_set_float(const float_value_t *num);
complex_value_t *complex_set_ui(uint32_t ui);

void complex_free(complex_value_t *num);

char *complex_get_str(const complex_value_t *num);

void complex_print(const complex_value_t *num);

value_t *complex_add(value_t *left, value_t *right);
value_t *complex_sub(value_t *left, value_t *right);
value_t *complex_mul(value_t *left, value_t *right);
value_t *complex_div(value_t *left, value_t *right);
value_t *complex_pow(value_t *left, value_t *right);

value_t *complex_neg(value_t *num);

uint8_t complex_eq(const complex_value_t *left, const complex_value_t *right);
uint8_t complex_neq(const complex_value_t *left, const complex_value_t *right);

value_t *complex_add_int(value_t *left, value_t *right);
value_t *complex_add_float(value_t *left, value_t *right);
value_t *complex_sub_int(value_t *left, value_t *right);
value_t *complex_int_sub(value_t *left, value_t *right);
value_t *complex_sub_float(value_t *left, value_t *right);
value_t *complex_float_sub(value_t *left, value_t *right);
value_t *complex_mul_int(value_t *left, value_t *right);
value_t *complex_mul_float(value_t *left, value_t *right);
value_t *complex_div_int(value_t *left, value_t *right);
value_t *complex_int_div(value_t *left, value_t *right);
value_t *complex_div_float(value_t *left, value_t *right);
value_t *complex_float_div(value_t *left, value_t *right);
value_t *complex_pow_int(value_t *left, value_t *right);
value_t *complex_int_pow(value_t *left, value_t *right);
value_t *complex_pow_float(value_t *left, value_t *right);
value_t *complex_float_pow(value_t *left, value_t *right);
value_t *complex_int_pow_float(value_t *left, value_t *right);
value_t *complex_float_pow_float(value_t *left, value_t *right);

uint8_t complex_eq_int(const complex_value_t *left, const int_value_t *right);
uint8_t complex_neq_int(const complex_value_t *left, const int_value_t *right);
uint8_t complex_eq_float(const complex_value_t *left, const float_value_t *right);
uint8_t complex_neq_float(const complex_value_t *left, const float_value_t *right);

value_t *complex_add_ui(value_t *left, uint32_t right);
value_t *complex_sub_ui(value_t *left, uint32_t right);
value_t *complex_ui_sub(uint32_t left, value_t *right);
value_t *complex_mul_ui(value_t *left, uint32_t right);
value_t *complex_div_ui(value_t *left, uint32_t right);
value_t *complex_ui_div(uint32_t left, value_t *right);
value_t *complex_pow_ui(value_t *left, uint32_t right);
value_t *complex_ui_pow(uint32_t left, value_t *right);

uint8_t complex_eq_ui(const complex_value_t *left, uint32_t right);
uint8_t complex_neq_ui(const complex_value_t *left, uint32_t right);

uint8_t complex_iszero(const complex_value_t *num);
uint8_t complex_isnzero(const complex_value_t *num);

#endif /* __MR_COMPLEX__ */
