/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/value.h>
#include <optimizer/complex.h>
#include <optimizer/list.h>
#include <lexer/token.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>

const char *value_names[7] =
{
    "none",
    "int", "float", "complex",
    "bool",
    "list", "tuple"
};

uint8_t value_name_lens[7] = {4, 3, 5, 7, 4, 4, 5};

const char *value_labels[7] =
{
    "NONE",
    "INT", "FLOAT", "COMPLEX",
    "BOOL",
    "LIST", "TUPLE"
};

void values_free(value_t **values, uint64_t size)
{
    if (!size)
    {
        mr_free(values);
        return;
    }

    do
        value_free(values[--size]);
    while (size);
    mr_free(values);
}

void values_print(const value_t **values, uint64_t size)
{
    for (uint64_t i = 0; i < size; i++)
    {
        value_print(values[i]);
        putchar('\n');
    }
}

void value_free(value_t *value)
{
    if (!value)
        return;

    switch (value->type)
    {
    case NONE_V:
    case BOOL_V:
    case PTR_V:
        value_free_vo(value);
        break;
    case INT_V:
        value_free_ts(value, int_free);
        break;
    case FLOAT_V:
        value_free_ts(value, float_free);
        break;
    case COMPLEX_V:
        value_free_ts(value, complex_free);
        break;
    case LIST_V:
    case TUPLE_V:
        value_free_ts(value, list_free);
        break;
    }
}

uint8_t value_istrue(value_t *value)
{
    uint8_t res;

    switch (value->type)
    {
    case NONE_V:
        value_free_vo(value);
        return 0;
    case INT_V:
        res = int_isnzero(value->value);

        value_free_ts(value, int_free);
        return res;
    case FLOAT_V:
        res = float_isnzero(value->value);

        value_free_ts(value, float_free);
        return res;
    case COMPLEX_V:
        res = complex_isnzero(value->value);

        value_free_ts(value, complex_free);
        return res;
    case BOOL_V:
        res = (uint8_t)(uintptr_t)value->value;

        value_free_vo(value);
        return res;
    case LIST_V:
    case TUPLE_V:
        res = value->value != NULL;

        value_free_ts(value, list_free);
        return res;
    }

    fprintf(stderr, "Internal Error: Invalid value type #%hu (value_istrue function)\n", value->type);
    abort();
}

uint8_t value_isfalse(value_t *value)
{
    uint8_t res;

    switch (value->type)
    {
    case NONE_V:
        value_free_vo(value);
        return 1;
    case INT_V:
        res = int_iszero(value->value);

        value_free_ts(value, int_free);
        return res;
    case FLOAT_V:
        res = float_iszero(value->value);

        value_free_ts(value, float_free);
        return res;
    case COMPLEX_V:
        res = complex_iszero(value->value);

        value_free_ts(value, complex_free);
        return res;
    case BOOL_V:
        res = (uint8_t)(uintptr_t)!value->value;

        value_free_vo(value);
        return res;
    case LIST_V:
    case TUPLE_V:
        res = !value->value;

        value_free_ts(value, list_free);
        return res;
    }

    fprintf(stderr, "Internal Error: Invalid value type #%hu (value_isfalse function)\n", value->type);
    abort();
}

void value_print(const value_t *value)
{
    if (!value->type)
    {
        fputs("(NONE)", stdout);
        return;
    }

    printf("(%s: ", value_labels[value->type]);

    switch (value->type)
    {
    case INT_V:
        int_print(value->value);
        break;
    case FLOAT_V:
        float_print(value->value);
        break;
    case COMPLEX_V:
        complex_print(value->value);
        break;
    case BOOL_V:
        printf(value->value ? "true" : "false");
        break;
    case LIST_V:
        list_print(value->value, '[', ']');
        break;
    case TUPLE_V:
        list_print(value->value, '(', ')');
        break;
    }

    putchar(')');
}
