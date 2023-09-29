/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_INT__
#define __MR_INT__

#include <optimizer/value.h>
#include <gmp.h>
#include <stdint.h>

struct __INT_VALUE_T
{
    mpz_t num;
};
typedef struct __INT_VALUE_T int_value_t;

#define INT_CAST(v) ((int_value_t*)v->value)->num

int_value_t *int_init();

int_value_t *int_set_str(const char *str);
int_value_t *int_set_ui(uint32_t ui);

void int_free(int_value_t *num);

char *int_get_str(const int_value_t *num);
uint64_t int_get_ull(const int_value_t *num);

void int_print(const int_value_t *num);

value_t *int_add(value_t *left, value_t *right);
value_t *int_sub(value_t *left, value_t *right);
value_t *int_mul(value_t *left, value_t *right);
value_t *int_mod(value_t *left, value_t *right);
value_t *int_quot(value_t *left, value_t *right);
value_t *int_pow(value_t *left, value_t *right);
value_t *int_and(value_t *left, value_t *right);
value_t *int_or(value_t *left, value_t *right);
value_t *int_xor(value_t *left, value_t *right);
value_t *int_lshift(value_t *left, value_t *right);
value_t *int_rshift(value_t *left, value_t *right);

value_t *int_neg(value_t *num);
value_t *int_not(value_t *num);
value_t *int_inc(value_t *num);
value_t *int_dec(value_t *num);

uint8_t int_eq(const int_value_t *left, const int_value_t *right);
uint8_t int_neq(const int_value_t *left, const int_value_t *right);
uint8_t int_lt(const int_value_t *left, const int_value_t *right);
uint8_t int_gt(const int_value_t *left, const int_value_t *right);
uint8_t int_lte(const int_value_t *left, const int_value_t *right);
uint8_t int_gte(const int_value_t *left, const int_value_t *right);

value_t *int_add_ui(value_t *left, value_t *right);
value_t *int_sub_ui(value_t *left, value_t *right);
value_t *int_ui_sub(value_t *left, value_t *right);
value_t *int_mul_ui(value_t *left, value_t *right);
value_t *int_mod_ui(value_t *left, value_t *right);
value_t *int_ui_mod(value_t *left, value_t *right);
value_t *int_quot_ui(value_t *left, value_t *right);
value_t *int_ui_quot(value_t *left, value_t *right);
value_t *int_pow_ui(value_t *left, value_t *right);
value_t *int_ui_pow(value_t *left, value_t *right);
value_t *int_and_ui(value_t *left, value_t *right);
value_t *int_or_ui(value_t *left, value_t *right);
value_t *int_xor_ui(value_t *left, value_t *right);
value_t *int_lshift_ui(value_t *left, value_t *right);
value_t *int_ui_lshift(value_t *left, value_t *right);
value_t *int_rshift_ui(value_t *left, value_t *right);
value_t *int_ui_rshift(value_t *left, value_t *right);

uint8_t int_eq_ui(const int_value_t *left, uint32_t right);
uint8_t int_neq_ui(const int_value_t *left, uint32_t right);
uint8_t int_lt_ui(const int_value_t *left, uint32_t right);
uint8_t int_gt_ui(const int_value_t *left, uint32_t right);
uint8_t int_lte_ui(const int_value_t *left, uint32_t right);
uint8_t int_gte_ui(const int_value_t *left, uint32_t right);

uint8_t int_iszero(const int_value_t *num);
uint8_t int_isnzero(const int_value_t *num);
uint8_t int_isneg(const int_value_t *num);

uint8_t int_gt_ull(const int_value_t *left, uint64_t right);
uint8_t int_nfit_ull(const int_value_t *num);

#endif /* __MR_INT__ */
