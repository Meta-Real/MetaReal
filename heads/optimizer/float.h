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
float_value_t *float_set_ui(uint32_t ui);
void float_free(float_value_t *num);

char *float_get_str(const float_value_t *num);
int_value_t *float_get_int(const float_value_t *num);

void float_print(const float_value_t *num);

void float_add(float_value_t *left, const float_value_t *right);
void float_sub(float_value_t *left, const float_value_t *right);
void float_mul(float_value_t *left, const float_value_t *right);
void float_div(float_value_t *left, const float_value_t *right);
void float_mod(float_value_t *left, const float_value_t *right);
int_value_t *float_quot(float_value_t *left, const float_value_t *right);
void float_pow(float_value_t *left, const float_value_t *right);

void float_neg(float_value_t *num);

uint8_t float_eq(const float_value_t *left, const float_value_t *right);
uint8_t float_lt(const float_value_t *left, const float_value_t *right);
uint8_t float_gt(const float_value_t *left, const float_value_t *right);

void float_add_int(float_value_t *left, const int_value_t *right);
void float_sub_int(float_value_t *left, const int_value_t *right);
void float_int_sub(const int_value_t *left, float_value_t *right);
void float_mul_int(float_value_t *left, const int_value_t *right);
void float_div_int(float_value_t *left, const int_value_t *right);
void float_int_div(const int_value_t *left, float_value_t *right);
float_value_t *float_int_div_int(const int_value_t *left, const int_value_t *right);
void float_mod_int(float_value_t *left, const int_value_t *right);
void float_int_mod(const int_value_t *left, float_value_t *right);
void float_quot_int(float_value_t *left, int_value_t *right);
void float_int_quot(int_value_t *left, const float_value_t *right);
void float_pow_int(float_value_t *left, const int_value_t *right);
void float_int_pow(const int_value_t *left, float_value_t *right);
float_value_t *float_int_pow_int(const int_value_t *left, const int_value_t *right);

uint8_t float_eq_int(const float_value_t *left, const int_value_t *right);
uint8_t float_lt_int(const float_value_t *left, const int_value_t *right);
uint8_t float_gt_int(const float_value_t *left, const int_value_t *right);

void float_add_ui(float_value_t *left, uint32_t right);
void float_sub_ui(float_value_t *left, uint32_t right);
void float_ui_sub(uint32_t left, float_value_t *right);
void float_mul_ui(float_value_t *left, uint32_t right);
void float_div_ui(float_value_t *left, uint32_t right);
void float_ui_div(uint32_t left, float_value_t *right);
float_value_t *float_int_div_ui(const int_value_t *left, uint32_t right);
float_value_t *float_ui_div_int(uint32_t left, const int_value_t *right);
float_value_t *float_ui_div_ui(uint32_t left, uint32_t right);
void float_mod_ui(float_value_t *left, uint32_t right);
void float_ui_mod(uint32_t left, float_value_t *right);
int_value_t *float_quot_ui(float_value_t *left, uint32_t right);
int_value_t *float_ui_quot(uint32_t left, float_value_t *right);
void float_pow_ui(float_value_t *left, uint32_t right);
void float_ui_pow(uint32_t left, float_value_t *right);
float_value_t *float_ui_pow_int(uint32_t left, const int_value_t *right);

uint8_t float_eq_ui(const float_value_t *left, uint32_t right);
uint8_t float_lt_ui(const float_value_t *left, uint32_t right);
uint8_t float_gt_ui(const float_value_t *left, uint32_t right);

uint8_t float_iszero(const float_value_t *num);
uint8_t float_isneg(const float_value_t *num);

#endif /* __MR_FLOAT__ */
