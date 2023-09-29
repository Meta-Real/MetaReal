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
    list->elements = mr_alloc(size * sizeof(value_t*));
    list->size = size;
    return list;
}

void list_free(list_value_t *list)
{
    if (!list)
        return;

    while (list->size)
        value_free(list->elements[--list->size]);
    mr_free(list->elements);
    mr_free(list);
}

void list_print(const list_value_t *list, char lbrace, char rbrace)
{
    if (!list)
    {
        printf("%c%c", lbrace, rbrace);
        return;
    }

    putchar(lbrace);
    value_print(*list->elements);

    for (uint64_t i = 1; i < list->size; i++)
    {
        fputs(", ", stdout);
        value_print(list->elements[i]);
    }

    putchar(rbrace);
}

value_t *list_append(value_t *list, value_t *element)
{
    if (!list->value)
    {
        if (list->ref)
        {
            list->ref--;

            list_value_t *value = list_set(1);
            *value->elements = element;

            value_t *res;
            value_set(res, LIST_V, value);
            return res;
        }

        list->value = list_set(1);
        *LIST_CAST(list)->elements = element;
        return list;
    }

    if (list->ref)
    {
        list->ref--;

        list_value_t *value = list_set(LIST_CAST(list)->size + 1);

        for (value->size = 0; value->size < LIST_CAST(list)->size; value->size++)
        {
            value->elements[value->size] = LIST_CAST(list)->elements[value->size];
            value_addref(value->elements[value->size], 1);
        }

        value->elements[value->size++] = element;

        value_t *res;
        value_set(res, LIST_V, value);
        return res;
    }

    LIST_CAST(list)->elements = mr_realloc(LIST_CAST(list)->elements, (LIST_CAST(list)->size + 1) * sizeof(value_t));
    LIST_CAST(list)->elements[LIST_CAST(list)->size++] = element;
    return list;
}

value_t *list_concat(value_t *left, value_t *right)
{
    if (!left->value)
        return right;
    if (!right->value)
        return left;

    if (left->ref)
    {
        left->ref--;

        list_value_t *value = list_set(LIST_CAST(left)->size + LIST_CAST(right)->size);

        uint64_t i, j;
        for (i = 0; i < LIST_CAST(left)->size; i++)
        {
            value->elements[i] = LIST_CAST(left)->elements[i];
            value_addref(value->elements[i], 1);
        }

        if (right->ref)
        {
            right->ref--;

            for (j = 0; j < LIST_CAST(right)->size; i++, j++)
            {
                value->elements[i] = LIST_CAST(right)->elements[j];
                value_addref(value->elements[i], 1);
            }

            value_t *res;
            value_set(res, LIST_V, value);
            return res;
        }

        for (j = 0; j < LIST_CAST(right)->size; i++, j++)
            value->elements[i] = LIST_CAST(right)->elements[j];

        mr_free(LIST_CAST(right)->elements);
        mr_free(right->value);
        right->value = value;
        return right;
    }

    uint64_t size = LIST_CAST(left)->size + LIST_CAST(right)->size;
    LIST_CAST(left)->elements = mr_realloc(LIST_CAST(left)->elements, size * sizeof(value_t*));

    uint64_t i, j;
    if (right->ref)
    {
        right->ref--;

        for (i = LIST_CAST(left)->size, j = 0; j < LIST_CAST(right)->size; i++, j++)
        {
            LIST_CAST(left)->elements[i] = LIST_CAST(right)->elements[j];
            value_addref(LIST_CAST(left)->elements[i], 1);
        }
    }
    else
    {
        for (i = LIST_CAST(left)->size, j = 0; j < LIST_CAST(right)->size; i++, j++)
            LIST_CAST(left)->elements[i] = LIST_CAST(right)->elements[j];

        mr_free(LIST_CAST(right)->elements);
        mr_free(right->value);
        mr_free(right);
    }

    LIST_CAST(left)->size = size;
    return left;
}

value_t *list_remove_int(value_t *list, value_t *ptr, uint64_t index)
{
    if (LIST_CAST(list)->size == 1)
    {
        if (list->ref)
        {
            list->ref--;

            if (ptr->ref)
            {
                ptr->ref--;

                value_t *res;
                value_set(res, LIST_V, NULL);
                return res;
            }

            int_free(ptr->value);
            ptr->type = LIST_V;
            ptr->value = NULL;
            return ptr;
        }

        value_free(*LIST_CAST(list)->elements);
        mr_free(LIST_CAST(list)->elements);
        mr_free(list->value);
        list->value = NULL;

        value_free_ts(ptr, int_free);
        return list;
    }

    if (list->ref)
    {
        list->ref--;

        list_value_t *value = list_set(LIST_CAST(list)->size - 1);

        uint64_t i;
        for (i = 0; i < index; i++)
        {
            value->elements[i] = LIST_CAST(list)->elements[i];
            value_addref(value->elements[i], 1);
        }
        for (index++; i < value->size; i++, index++)
        {
            value->elements[i] = LIST_CAST(list)->elements[index];
            value_addref(value->elements[i], 1);
        }

        if (ptr->ref)
        {
            ptr->ref--;

            value_t *res;
            value_set(res, LIST_V, value);
            return res;
        }

        int_free(ptr->value);
        ptr->type = LIST_V;
        ptr->value = value;
        return ptr;
    }

    value_free(LIST_CAST(list)->elements[index]);

    LIST_CAST(list)->size--;
    for (uint64_t i = index++; i < LIST_CAST(list)->size; i++, index++)
        LIST_CAST(list)->elements[i] = LIST_CAST(list)->elements[index];

    LIST_CAST(list)->elements = mr_realloc(LIST_CAST(list)->elements, LIST_CAST(list)->size * sizeof(value_t));

    value_free_ts(ptr, int_free);
    return list;
}

value_t *list_remove_ui(value_t *list, value_t *index)
{
    if (LIST_CAST(list)->size == 1)
    {
        if (list->ref)
        {
            list->ref--;

            if (index->ref)
            {
                index->ref--;

                value_t *res;
                value_set(res, LIST_V, NULL);
                return res;
            }

            index->type = LIST_V;
            index->value = NULL;
            return index;
        }

        value_free(*LIST_CAST(list)->elements);
        mr_free(LIST_CAST(list)->elements);
        mr_free(list->value);
        list->value = NULL;

        value_free_vo(index);
        return list;
    }

    if (list->ref)
    {
        list->ref--;

        list_value_t *value = list_set(LIST_CAST(list)->size - 1);

        uint64_t i;
        for (i = 0; i < (uintptr_t)index->value; i++)
        {
            value->elements[i] = LIST_CAST(list)->elements[i];
            value_addref(value->elements[i], 1);
        }
        for (; i < value->size; i++)
        {
            value->elements[i] = LIST_CAST(list)->elements[i + 1];
            value_addref(value->elements[i], 1);
        }

        if (index->ref)
        {
            index->ref--;

            value_t *res;
            value_set(res, LIST_V, value);
            return res;
        }

        index->type = LIST_V;
        index->value = value;
        return index;
    }

    value_free(LIST_CAST(list)->elements[(uintptr_t)index->value]);
    LIST_CAST(list)->size--;

    for (uint64_t i = (uintptr_t)index->value; i < LIST_CAST(list)->size; i++)
        LIST_CAST(list)->elements[i] = LIST_CAST(list)->elements[i + 1];

    LIST_CAST(list)->elements = mr_realloc(LIST_CAST(list)->elements, LIST_CAST(list)->size * sizeof(value_t));

    value_free_vo(index);
    return list;
}

value_t *list_repeat_int(value_t *list, value_t *ptr)
{
    uint64_t count = int_get_ull(ptr->value);

    if (count == 1)
    {
        value_free_ts(ptr, int_free);
        return list;
    }
    if (!count)
    {
        if (list->ref)
        {
            list->ref--;

            if (ptr->ref)
            {
                ptr->ref--;

                value_t *res;
                value_set(res, LIST_V, NULL);
                return res;
            }

            int_free(ptr->value);
            ptr->type = LIST_V;
            ptr->value = NULL;
            return ptr;
        }

        while (LIST_CAST(list)->size)
            value_free(LIST_CAST(list)->elements[--LIST_CAST(list)->size]);
        mr_free(LIST_CAST(list)->elements);
        mr_free(list->value);
        list->value = NULL;

        value_free_ts(ptr, int_free);
        return list;
    }

    if (list->ref)
    {
        list->ref--;

        list_value_t *value = list_set(LIST_CAST(list)->size * count);

        uint64_t i, j;
        for (i = 0; i < LIST_CAST(list)->size; i++)
        {
            value->elements[i] = LIST_CAST(list)->elements[i];
            value_addref(value->elements[i], count);
        }
        for (; i < value->size;)
            for (j = 0; j < LIST_CAST(list)->size; i++, j++)
                value->elements[i] = value->elements[j];

        if (ptr->ref)
        {
            ptr->ref--;

            value_t *res;
            value_set(res, LIST_V, value);
            return res;
        }

        int_free(ptr->value);
        ptr->type = LIST_V;
        ptr->value = value;
        return ptr;
    }

    uint64_t size = LIST_CAST(list)->size * count;
    LIST_CAST(list)->elements = mr_realloc(LIST_CAST(list)->elements, size * sizeof(value_t*));

    uint64_t i, j;
    for (i = 0; i < LIST_CAST(list)->size; i++)
        value_addref(LIST_CAST(list)->elements[i], count - 1);
    for (; i < size;)
        for (j = 0; j < LIST_CAST(list)->size; i++, j++)
            LIST_CAST(list)->elements[i] = LIST_CAST(list)->elements[j];

    LIST_CAST(list)->size = size;

    value_free_ts(ptr, int_free);
    return list;
}

value_t *list_repeat_ui(value_t *list, value_t *count)
{
    if ((uintptr_t)count->value == 1)
    {
        value_free_vo(count);
        return list;
    }
    if (!(uintptr_t)count->value)
    {
        if (list->ref)
        {
            list->ref--;

            if (count->ref)
            {
                count->ref--;

                value_t *res;
                value_set(res, LIST_V, NULL);
                return res;
            }

            count->type = LIST_V;
            count->value = NULL;
            return count;
        }

        while (LIST_CAST(list)->size)
            value_free(LIST_CAST(list)->elements[--LIST_CAST(list)->size]);
        mr_free(LIST_CAST(list)->elements);
        mr_free(list->value);
        list->value = NULL;

        value_free_vo(count);
        return list;
    }

    if (list->ref)
    {
        list->ref--;

        list_value_t *value = list_set(LIST_CAST(list)->size * (uintptr_t)count->value);

        uint64_t i, j;
        for (i = 0; i < LIST_CAST(list)->size; i++)
        {
            value->elements[i] = LIST_CAST(list)->elements[i];
            value_addref(value->elements[i], (uintptr_t)count->value);
        }
        for (; i < value->size;)
            for (j = 0; j < LIST_CAST(list)->size; i++, j++)
                value->elements[i] = value->elements[j];

        if (count->ref)
        {
            count->ref--;

            value_t *res;
            value_set(res, LIST_V, value);
            return res;
        }

        count->type = LIST_V;
        count->value = value;
        return count;
    }

    uint64_t size = LIST_CAST(list)->size * (uintptr_t)count->value;
    LIST_CAST(list)->elements = mr_realloc(LIST_CAST(list)->elements, size * sizeof(value_t*));

    uint64_t i, j;
    for (i = 0; i < LIST_CAST(list)->size; i++)
        value_addref(LIST_CAST(list)->elements[i], (uintptr_t)count->value - 1);
    for (; i < size;)
        for (j = 0; j < LIST_CAST(list)->size; i++, j++)
            LIST_CAST(list)->elements[i] = LIST_CAST(list)->elements[j];

    LIST_CAST(list)->size = size;

    value_free_vo(count);
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
        if (compute_vneq(left->elements[i], right->elements[i]))
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
        if (compute_vneq(left->elements[i], right->elements[i]))
            return 1;
    return 0;
}
