/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/value.h>
#include <optimizer/complex.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>

void print_value(const value_t *value);

const char *value_names[4] =
{
    "int", "float", "complex",
    "bool"
};

uint8_t value_name_lens[4] = {3, 5, 7, 4};

const char *value_labels[4] =
{
    "INT", "FLOAT", "COMPLEX",
    "BOOL"
};

void free_values(value_t *values, uint64_t size)
{
    if (!size)
    {
        mr_free(values);
        return;
    }

    do
        free_value(values + --size);
    while (size);
    mr_free(values);
}

void print_values(const value_t *values, uint64_t size)
{
    for (uint64_t i = 0; i < size; i++)
    {
        print_value(values + i);
        putchar('\n');
    }
}

void free_value(const value_t *value)
{
    switch (value->type)
    {
    case INT_V:
        int_free(value->value);
        break;
    case FLOAT_V:
        float_free(value->value);
        break;
    case COMPLEX_V:
        complex_free(value->value);
        break;
    case BOOL_V:
        break;
    }
}

uint8_t value_istrue(value_t *value)
{
    uint8_t res;

    switch (value->type)
    {
    case INT_V:
        res = !int_iszero(value->value);

        int_free(value->value);
        return res;
    case FLOAT_V:
        res = !float_iszero(value->value);

        float_free(value->value);
        return res;
    case COMPLEX_V:
        res = !complex_iszero(value->value);

        complex_free(value->value);
        return res;
    case BOOL_V:
        return (uint8_t)(uintptr_t)value->value;
    }

    fprintf(stderr, "Internal Error: Invalid value type #%hu (value_istrue function)\n", value->type);
    abort();
}

void print_value(const value_t *value)
{
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
    }

    putchar(')');
}
