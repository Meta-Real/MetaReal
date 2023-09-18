/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_INT__
#define __MR_INT__

#include <gmp.h>
#include <stdint.h>

struct __INT_VALUE_T
{
    mpz_t num;
    uint64_t ref;
};
typedef struct __INT_VALUE_T int_value_t;

int_value_t *int_init();

int_value_t *int_set_str(const char *str);
int_value_t *int_set_ui(uint32_t ui);

void int_free(int_value_t *num);

char *int_get_str(const int_value_t *num);

void int_print(const int_value_t *num);

int_value_t *int_add(int_value_t *left, int_value_t *right);
int_value_t *int_sub(int_value_t *left, int_value_t *right);
int_value_t *int_mul(int_value_t *left, int_value_t *right);
int_value_t *int_mod(int_value_t *left, int_value_t *right);
int_value_t *int_quot(int_value_t *left, int_value_t *right);
int_value_t *int_pow(int_value_t *left, int_value_t *right);
int_value_t *int_and(int_value_t *left, int_value_t *right);
int_value_t *int_or(int_value_t *left, int_value_t *right);
int_value_t *int_xor(int_value_t *left, int_value_t *right);
int_value_t *int_lshift(int_value_t *left, int_value_t *right);
int_value_t *int_rshift(int_value_t *left, int_value_t *right);

int_value_t *int_neg(int_value_t *num);
int_value_t *int_not(int_value_t *num);

uint8_t int_eq(const int_value_t *left, const int_value_t *right);
uint8_t int_neq(const int_value_t *left, const int_value_t *right);
uint8_t int_lt(const int_value_t *left, const int_value_t *right);
uint8_t int_gt(const int_value_t *left, const int_value_t *right);
uint8_t int_lte(const int_value_t *left, const int_value_t *right);
uint8_t int_gte(const int_value_t *left, const int_value_t *right);

int_value_t *int_add_ui(int_value_t *left, uint32_t right);
int_value_t *int_sub_ui(int_value_t *left, uint32_t right);
int_value_t *int_ui_sub(uint32_t left, int_value_t *right);
int_value_t *int_mul_ui(int_value_t *left, uint32_t right);
int_value_t *int_mod_ui(int_value_t *left, uint32_t right);
int_value_t *int_ui_mod(uint32_t left, int_value_t *right);
int_value_t *int_quot_ui(int_value_t *left, uint32_t right);
int_value_t *int_ui_quot(uint32_t left, int_value_t *right);
int_value_t *int_pow_ui(int_value_t *left, uint32_t right);
int_value_t *int_ui_pow(uint32_t left, int_value_t *right);
int_value_t *int_and_ui(int_value_t *left, uint32_t right);
int_value_t *int_or_ui(int_value_t *left, uint32_t right);
int_value_t *int_xor_ui(int_value_t *left, uint32_t right);
int_value_t *int_lshift_ui(int_value_t *left, uint32_t right);
int_value_t *int_ui_lshift(uint32_t left, int_value_t *right);
int_value_t *int_rshift_ui(int_value_t *left, uint32_t right);
int_value_t *int_ui_rshift(uint32_t left, int_value_t *right);

uint8_t int_eq_ui(const int_value_t *left, uint32_t right);
uint8_t int_neq_ui(const int_value_t *left, uint32_t right);
uint8_t int_lt_ui(const int_value_t *left, uint32_t right);
uint8_t int_gt_ui(const int_value_t *left, uint32_t right);
uint8_t int_lte_ui(const int_value_t *left, uint32_t right);
uint8_t int_gte_ui(const int_value_t *left, uint32_t right);

uint8_t int_iszero(const int_value_t *num);
uint8_t int_isneg(const int_value_t *num);

#endif /* __MR_INT__ */
