/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_VALUE__
#define __MR_VALUE__

#include <pos.h>

enum __VALUE_TYPES
{
    NONE_V,

    INT_V,
    FLOAT_V,
    COMPLEX_V,

    BOOL_V
};

struct __VALUE_T
{
    uint8_t type;
    void *value;

    pos_t poss;
    pos_t pose;
};
typedef struct __VALUE_T value_t;

extern const char *value_names[5];
extern uint8_t value_name_lens[5];

void free_values(value_t *values, uint64_t size);
void print_values(const value_t *values, uint64_t size);

void free_value(value_t *value);

uint8_t value_istrue(value_t *value);
void value_addref(value_t *value);

#endif /* __MR_VALUE__ */
