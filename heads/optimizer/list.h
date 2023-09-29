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
    value_t **elements;
    uint64_t size;
};
typedef struct __LIST_VALUE_T list_value_t;

#define LIST_CAST(v) ((list_value_t*)v->value)

list_value_t *list_set(uint64_t size);

void list_free(list_value_t *list);

void list_print(const list_value_t *list, char lbrace, char rbrace);

value_t *list_append(value_t *list, value_t *element);
value_t *list_concat(value_t *left, value_t *right);
value_t *list_remove_int(value_t *list, value_t *ptr, uint64_t index);
value_t *list_remove_ui(value_t *list, value_t *index);
value_t *list_repeat_int(value_t *list, value_t *ptr);
value_t *list_repeat_ui(value_t *list, value_t *count);

uint8_t list_eq(const list_value_t *left, const list_value_t *right);
uint8_t list_neq(const list_value_t *left, const list_value_t *right);

#endif /* __MR_LIST__ */
