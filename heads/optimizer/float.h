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

#define FLOAT_CAST(v) ((float_value_t*)v->value)->num

float_value_t *float_init();

float_value_t *float_set_str(const char *str);
float_value_t *float_set_int(const int_value_t *num);
float_value_t *float_set_ui(uint32_t ui);

void float_free(float_value_t *num);

char *float_get_str(const float_value_t *num);
int_value_t *float_get_int(const float_value_t *num);

void float_print(const float_value_t *num);

value_t *float_add(value_t *left, value_t *right);
value_t *float_sub(value_t *left, value_t *right);
value_t *float_mul(value_t *left, value_t *right);
value_t *float_div(value_t *left, value_t *right);
value_t *float_mod(value_t *left, value_t *right);
value_t *float_quot(value_t *left, value_t *right);
value_t *float_pow(value_t *left, value_t *right);

value_t *float_neg(value_t *num);

uint8_t float_eq(const float_value_t *left, const float_value_t *right);
uint8_t float_neq(const float_value_t *left, const float_value_t *right);
uint8_t float_lt(const float_value_t *left, const float_value_t *right);
uint8_t float_gt(const float_value_t *left, const float_value_t *right);
uint8_t float_lte(const float_value_t *left, const float_value_t *right);
uint8_t float_gte(const float_value_t *left, const float_value_t *right);

value_t *float_add_int(value_t *left, value_t *right);
value_t *float_sub_int(value_t *left, value_t *right);
value_t *float_int_sub(value_t *left, value_t *right);
value_t *float_mul_int(value_t *left, value_t *right);
value_t *float_div_int(value_t *left, value_t *right);
value_t *float_int_div(value_t *left, value_t *right);
value_t *float_int_div_int(value_t *left, value_t *right);
value_t *float_mod_int(value_t *left, value_t *right);
value_t *float_int_mod(value_t *left, value_t *right);
value_t *float_quot_int(value_t *left, value_t *right);
value_t *float_int_quot(value_t *left, value_t *right);
value_t *float_pow_int(value_t *left, value_t *right);
value_t *float_int_pow(value_t *left, value_t *right);
value_t *float_int_pow_int(value_t *left, value_t *right);

uint8_t float_eq_int(const float_value_t *left, const int_value_t *right);
uint8_t float_neq_int(const float_value_t *left, const int_value_t *right);
uint8_t float_lt_int(const float_value_t *left, const int_value_t *right);
uint8_t float_gt_int(const float_value_t *left, const int_value_t *right);
uint8_t float_lte_int(const float_value_t *left, const int_value_t *right);
uint8_t float_gte_int(const float_value_t *left, const int_value_t *right);

value_t *float_add_ui(value_t *left, uint32_t right);
value_t *float_sub_ui(value_t *left, uint32_t right);
value_t *float_ui_sub(uint32_t left, value_t *right);
value_t *float_mul_ui(value_t *left, uint32_t right);
value_t *float_div_ui(value_t *left, uint32_t right);
value_t *float_ui_div(uint32_t left, value_t *right);
value_t *float_int_div_ui(value_t *left, uint32_t right);
value_t *float_ui_div_int(uint32_t left, value_t *right);
value_t *float_ui_div_ui(uint32_t left, uint32_t right);
value_t *float_mod_ui(value_t *left, uint32_t right);
value_t *float_ui_mod(uint32_t left, value_t *right);
value_t *float_quot_ui(value_t *left, uint32_t right);
value_t *float_ui_quot(uint32_t left, value_t *right);
value_t *float_pow_ui(value_t *left, uint32_t right);
value_t *float_ui_pow(uint32_t left, value_t *right);
value_t *float_ui_pow_int(uint32_t left, value_t *right);

uint8_t float_eq_ui(const float_value_t *left, uint32_t right);
uint8_t float_neq_ui(const float_value_t *left, uint32_t right);
uint8_t float_lt_ui(const float_value_t *left, uint32_t right);
uint8_t float_gt_ui(const float_value_t *left, uint32_t right);
uint8_t float_lte_ui(const float_value_t *left, uint32_t right);
uint8_t float_gte_ui(const float_value_t *left, uint32_t right);

uint8_t float_iszero(const float_value_t *num);
uint8_t float_isnzero(const float_value_t *num);
uint8_t float_isneg(const float_value_t *num);

#endif /* __MR_FLOAT__ */
