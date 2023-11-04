/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_CONTEXT__
#define __MR_CONTEXT__

#include <optimizer/value.h>

struct __VAR_T
{
    char *name;
    value_t *value;

    uint8_t prop;
    uint8_t type;
};
typedef struct __VAR_T var_t;

struct __CONTEXT_T
{
    var_t *vars;
    uint64_t size;
    uint64_t alloc;
};
typedef struct __CONTEXT_T context_t;

void context_free(context_t *context);

value_t *var_get(const context_t *context, const char *name);
uint8_t var_set(uint8_t *flag, context_t *context, char *name, value_t *value, uint8_t prop, uint8_t type);

uint64_t var_getp(uint8_t *error, const context_t *context, const char *name);
uint64_t var_setp(uint8_t *error, context_t *context, char *name, value_t *value, uint8_t prop, uint8_t type);
uint64_t var_add(context_t *context, char *name);

#endif /* __MR_CONTEXT__ */
