/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/list.h>
#include <optimizer/operation.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

list_value_t *list_set(uint64_t size)
{
    list_value_t *list = mr_alloc(sizeof(list_value_t));
    list->elements = mr_alloc(size * sizeof(value_t));
    list->size = size;
    list->ref = 0;
    return list;
}

void list_free(list_value_t *list)
{
    if (!list)
        return;

    if (list->ref)
    {
        list->ref--;
        return;
    }

    while (list->size)
        value_free(list->elements + --list->size);
    mr_free(list);
}

void list_print(const list_value_t *list)
{
    if (!list)
    {
        fputs("[]", stdout);
        return;
    }

    putchar('[');
    value_print(list->elements);

    for (uint64_t i = 1; i < list->size; i++)
    {
        fputs(", ", stdout);
        value_print(list->elements + i);
    }

    putchar(']');
}

list_value_t *list_append(list_value_t *list, value_t *value)
{
    if (!list)
    {
        list_value_t *res = list_set(1);
        *res->elements = *value;
        return res;
    }

    if (list->ref)
    {
        list->ref--;

        list_value_t *res = mr_alloc(sizeof(list_value_t));
        res->elements = mr_alloc((list->size + 1) * sizeof(value_t));
        res->ref = 0;

        for (res->size = 0; res->size < list->size; res->size++)
        {
            res->elements[res->size] = list->elements[res->size];
            value_addref(res->elements + res->size);
        }

        res->elements[res->size++] = *value;
        return res;
    }

    list->elements = mr_realloc(list->elements, (list->size + 1) * sizeof(value_t));
    list->elements[list->size++] = *value;
    return list;
}

list_value_t *list_concat(list_value_t *left, list_value_t *right)
{
    if (!left)
        return right;
    if (!right)
        return left;

    if (left->ref)
    {
        left->ref--;

        list_value_t *res = mr_alloc(sizeof(list_value_t));
        res->size = left->size + right->size;
        res->elements = mr_alloc(res->size * sizeof(value_t));
        res->ref = 0;

        uint64_t i, j;
        for (i = 0; i < left->size; i++)
        {
            res->elements[i] = left->elements[i];
            value_addref(res->elements + i);
        }

        if (right->ref)
        {
            right->ref--;

            for (i = left->size, j = 0; j < right->size; i++, j++)
            {
                res->elements[i] = right->elements[j];
                value_addref(res->elements + i);
            }
        }
        else
        {
            for (i = left->size, j = 0; j < right->size; i++, j++)
                res->elements[i] = right->elements[j];

            mr_free(right->elements);
            mr_free(right);
        }

        return res;
    }

    uint64_t size = left->size + right->size;
    left->elements = mr_realloc(left->elements, size * sizeof(value_t));

    uint64_t i, j;
    if (right->ref)
    {
        right->ref--;

        for (i = left->size, j = 0; j < right->size; i++, j++)
        {
            left->elements[i] = right->elements[j];
            value_addref(left->elements + i);
        }
    }
    else
    {
        for (i = left->size, j = 0; j < right->size; i++, j++)
            left->elements[i] = right->elements[j];

        mr_free(right->elements);
        mr_free(right);
    }

    left->size = size;
    return left;
}

list_value_t *list_remove(list_value_t *list, uint64_t index)
{
    if (list->size == 1)
    {
        value_free(list->elements);
        mr_free(list->elements);
        mr_free(list);
        return NULL;
    }

    if (list->ref)
    {
        list->ref--;

        list_value_t *res = mr_alloc(sizeof(list_value_t));
        res->size = list->size - 1;
        res->elements = mr_alloc(res->size * sizeof(value_t));
        res->ref = 0;

        uint64_t i;
        for (i = 0; i < index; i++)
        {
            res->elements[i] = list->elements[i];
            value_addref(res->elements + i);
        }
        for (index++; i < res->size; i++, index++)
        {
            res->elements[i] = list->elements[index];
            value_addref(res->elements + i);
        }

        return res;
    }

    list->size--;
    for (uint64_t i = index++; i < list->size; i++, index++)
        list->elements[i] = list->elements[index];

    list->elements = mr_realloc(list->elements, list->size * sizeof(value_t));
    return list;
}

list_value_t *list_repeat(list_value_t *list, uint64_t count)
{
    if (!list || count == 1)
        return list;
    if (!count)
    {
        list_free(list);
        return NULL;
    }

    if (list->ref)
    {
        list->ref--;

        list_value_t *res = list_set(list->size * count);

        uint64_t i, j;
        for (i = 0; i < res->size;)
            for (j = 0; j < list->size; i++, j++)
            {
                res->elements[i] = list->elements[j];
                value_addref(res->elements + i);
            }

        return res;
    }

    uint64_t size = list->size * count;
    list->elements = mr_realloc(list->elements, size * sizeof(value_t));

    uint64_t i, j;
    for (i = list->size; i < size;)
        for (j = 0; j < list->size; i++, j++)
        {
            list->elements[i] = list->elements[j];
            value_addref(list->elements + i);
        }

    list->size = size;
    return list;
}

uint8_t list_eq(const list_value_t *left, const list_value_t *right)
{
    if (!left)
        return !right;
    if (left->size != right->size)
        return 0;
    if (left->elements == right->elements)
        return 1;

    for (uint64_t i = 0; i < left->size; i++)
        if (compute_vneq(left->elements + i, right->elements + i))
            return 0;
    return 1;
}

uint8_t list_neq(const list_value_t *left, const list_value_t *right)
{
    if (!left)
        return right != NULL;
    if (left->size != right->size)
        return 1;
    if (left->elements == right->elements)
        return 0;

    for (uint64_t i = 0; i < left->size; i++)
        if (compute_vneq(left->elements + i, right->elements + i))
            return 1;
    return 0;
}

uint8_t list_isempty(const list_value_t *list)
{
    return !list->size;
}

uint8_t list_isnempty(const list_value_t *list)
{
    return list->size != 0;
}
