/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_VALUE__
#define __MR_VALUE__

#include <gmp.h>
#include <pos.h>

enum __VALUE_TYPES
{
    INT_V,
};

struct __VALUE_T
{
    uint8_t type;
    void *value;

    pos_t poss;
    pos_t pose;
};
typedef struct __VALUE_T value_t;

struct __INT_VALUE_T
{
    mpz_t num;
};
typedef struct __INT_VALUE_T int_value_t;

void free_values(value_t *values, uint64_t size);
void print_values(const value_t *values, uint64_t size);

void free_int_value(int_value_t *value);

#endif /* __MR_VALUE__ */
