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
};
typedef struct __INT_VALUE_T int_value_t;

int_value_t *int_init();
int_value_t *int_set_str(const char *str);
void int_free(int_value_t *num);

char *int_get_str(const int_value_t *num);

void int_print(const int_value_t *num);

void int_add(int_value_t *left, const int_value_t *right);
void int_sub(int_value_t *left, const int_value_t *right);
void int_mul(int_value_t *left, const int_value_t *right);
void int_mod(int_value_t *left, const int_value_t *right);
void int_quot(int_value_t *left, const int_value_t *right);
void int_pow(int_value_t *left, const int_value_t *right);

void int_neg(int_value_t *num);

uint8_t int_iszero(const int_value_t *num);
uint8_t int_isneg(const int_value_t *num);

#endif /* __MR_INT__ */
