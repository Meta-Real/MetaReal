/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_VALUE__
#define __MR_VALUE__

#include <pos.h>

enum __VALUE_TYPES
{
    INT_V,
    FLOAT_V
};

struct __VALUE_T
{
    uint8_t type;
    void *value;

    pos_t poss;
    pos_t pose;
};
typedef struct __VALUE_T value_t;

extern const char *value_names[2];
extern uint8_t value_name_lens[2];

void free_values(value_t *values, uint64_t size);
void print_values(const value_t *values, uint64_t size);

void free_value(const value_t *value);

#endif /* __MR_VALUE__ */
