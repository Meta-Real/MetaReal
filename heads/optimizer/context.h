/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_CONTEXT__
#define __MR_CONTEXT__

#include <optimizer/value.h>

struct __VAR_T
{
    char *name;
    value_t value;
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

value_t var_get(uint8_t *error, const context_t *context, const char *name);
void var_set(context_t *context, char *name, value_t *value);

uint64_t var_getp(uint8_t *error, const context_t *context, const char *name);
uint64_t var_setp(context_t *context, char *name, value_t *value);
uint64_t var_add(context_t *context, char *name);

#endif /* __MR_CONTEXT__ */
