/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/value.h>
#include <gmp.h>
#include <alloc.h>
#include <stdio.h>

void free_value(const value_t *value);
void print_value(const value_t *value);

void print_int_value(const int_value_t *value);

const char *value_labels[INT_V + 1] =
{
    "INT"
};

void free_values(value_t *values, uint64_t size)
{
    if (!size)
        return;

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
        free_int_value(value->value);
        break;
    }
}

void print_value(const value_t *value)
{
    printf("(%s: ", value_labels[value->type]);

    switch (value->type)
    {
    case INT_V:
        print_int_value(value->value);
        break;
    }

    putchar(')');
}

void free_int_value(int_value_t *value)
{
    mpz_clear(value->num);
    mr_free(value);
}

void print_int_value(const int_value_t *value)
{
    gmp_printf("%Zd", value->num);
}
