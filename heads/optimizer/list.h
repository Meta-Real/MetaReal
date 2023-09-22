/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_LIST__
#define __MR_LIST__

#include <optimizer/value.h>
#include <optimizer/int.h>

#define LIST_MAX_SIZE ((uint64_t)-1 / sizeof(value_t))

struct __LIST_VALUE_T
{
    value_t *elements;
    uint64_t size;

    uint64_t ref;
};
typedef struct __LIST_VALUE_T list_value_t;

#define list_size(l) ((list_value_t*)l)->size

list_value_t *list_set(uint64_t size);

void list_free(list_value_t *list);

void list_print(const list_value_t *list, char lbrace, char rbrace);

list_value_t *list_append(list_value_t *list, value_t *value);
list_value_t *list_concat(list_value_t *left, list_value_t *right);
list_value_t *list_remove(list_value_t *list, uint64_t index);
list_value_t *list_repeat(list_value_t *list, uint64_t count);

uint8_t list_eq(const list_value_t *left, const list_value_t *right);
uint8_t list_neq(const list_value_t *left, const list_value_t *right);

#endif /* __MR_LIST__ */
