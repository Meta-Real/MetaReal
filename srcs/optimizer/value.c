/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/value.h>
#include <optimizer/complex.h>
#include <alloc.h>
#include <stdio.h>

void print_value(const value_t *value);

const char *value_names[3] =
{
    "int",
    "float",
    "complex"
};

uint8_t value_name_lens[3] = {3, 5, 7};

const char *value_labels[3] =
{
    "INT",
    "FLOAT",
    "COMPLEX"
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
    }
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
    }

    putchar(')');
}
