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

    BOOL_V,

    LIST_V,
    TUPLE_V
};

struct __VALUE_T
{
    uint8_t type;
    void *value;

    pos_t poss;
    pos_t pose;
};
typedef struct __VALUE_T value_t;

extern const char *value_names[7];
extern uint8_t value_name_lens[7];

void values_free(value_t *values, uint64_t size);
void values_print(const value_t *values, uint64_t size);

void value_free(value_t *value);
void value_print(const value_t *value);

uint8_t value_istrue(value_t *value);
uint8_t value_isfalse(value_t *value);

void value_addref(value_t *value);

#endif /* __MR_VALUE__ */
