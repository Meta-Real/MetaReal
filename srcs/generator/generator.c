/*
MIT License

Copyright (c) 2023 MetaReal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#include <generator/generator.h>
#include <optimizer/value.h>
#include <lexer/token.h>
#include <config.h>
#include <stack.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <intrin.h>
#endif

mr_byte_t b10_guess[65] =
{
    1,  1,  1,  1,  2,  2,  2,  3,  3,  3 ,
    4,  4,  4,  4,  5,  5,  5,  6,  6,  6 ,
    7,  7,  7,  7,  8,  8,  8,  9,  9,  9 ,
    10, 10, 10, 10, 11, 11, 11, 12, 12, 12,
    13, 13, 13, 13, 14, 14, 14, 15, 15, 15,
    16, 16, 16, 16, 17, 17, 17, 18, 18, 18,
    19, 19, 19, 19, 20
};

mr_longlong_t pow10[20] =
{
    1, 10, 100, 1000,
    10000, 100000, 1000000, 10000000,
    100000000, 1000000000, 10000000000, 100000000000,
    1000000000000, 10000000000000, 100000000000000, 1000000000000000,
    10000000000000000, 100000000000000000, 1000000000000000000, 10000000000000000000
};

#pragma pack(push, 1)
struct __MR_GENERATOR_DATA_T
{
    mr_str_t data;
    mr_long_t size;
    mr_long_t alloc;
    mr_long_t exalloc;

    mr_byte_t error;
};
#pragma pack(pop)
typedef struct __MR_GENERATOR_DATA_T mr_generator_data_t;

void mr_generator_visit(
    mr_generator_data_t *data, mr_node_t *node);
void mr_generator_visit_binary_op(
    mr_generator_data_t *data, mr_node_t *node);
void mr_generator_visit_func_call(
    mr_generator_data_t *data, mr_node_t *node);
void mr_generator_visit_cint(
    mr_generator_data_t *data, mr_node_t *node);

void mr_generator_print(
    mr_generator_data_t *data, mr_long_t size, mr_str_ct format, ...);

mr_byte_t b2_digits(mr_longlong_t num);
mr_byte_t b10_digits(mr_longlong_t num);

mr_byte_t mr_generator(
    mr_generator_t *res,
    mr_node_t *nodes, mr_long_t size, mr_long_t alloc)
{
    mr_generator_data_t data = {malloc(alloc), 16, alloc, alloc, MR_NOERROR};
    if (!data.data)
        return MR_ERROR_NOT_ENOUGH_MEMORY;

    memcpy(data.data, ".code\nmain proc\n", 16 * sizeof(mr_chr_t));

    for (mr_long_t i = 0; i < size; i++)
    {
        mr_generator_visit(&data, nodes + i);
        if (data.error != MR_NOERROR)
        {
            free(nodes);
            free(data.data);
            return data.error;
        }
    }

    mr_generator_print(&data, 31,
        "\tmov rax, 0\n\tret\nmain endp\nend\n");

    *res = (mr_generator_t){data.data, data.size};
    free(nodes);
    return MR_NOERROR;
}

void mr_generator_visit(
    mr_generator_data_t *data, mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_NONE:
        break;
    case MR_NODE_BINARY_OP:
        mr_generator_visit_binary_op(data, node);
        break;
    case MR_NODE_FUNC_CALL:
        mr_generator_visit_func_call(data, node);
        break;
    case MR_VALUE_CINT:
        mr_generator_visit_cint(data, node);
        break;
    }
}

void mr_generator_visit_binary_op(
    mr_generator_data_t *data, mr_node_t *node)
{
    mr_node_binary_op_t *vnode = (mr_node_binary_op_t*)(_mr_stack.data + node->value);

    mr_generator_visit(data, &vnode->left);
    if (data->error != MR_NOERROR)
        return;

    mr_generator_visit(data, &vnode->right);
    if (data->error != MR_NOERROR || node->useless)
        return;
}

void mr_generator_visit_func_call(
    mr_generator_data_t *data, mr_node_t *node)
{
    mr_node_func_call_t *vnode = (mr_node_func_call_t*)(_mr_stack.data + node->value);

    mr_node_call_arg_t *args = _mr_stack.ptrs[vnode->args];
    for (mr_byte_t i = 0; i < vnode->size; i++)
    {
        mr_generator_visit(data, &args[i].value);
        if (data->error != MR_NOERROR)
            return;
    }

    switch (vnode->func.value)
    {
    case MR_VALUE_BIFUNC_PRINT:
        mr_generator_print(data, 23, "\tcall mr_print_digit10\n");
        break;
    }
}

void mr_generator_visit_cint(
    mr_generator_data_t *data, mr_node_t *node)
{
    if (node->useless)
        return;

    mr_value_cint_t *vnode = (mr_value_cint_t*)(_mr_stack.data + node->value);

    if (data->size != 0)
        vnode->value = 0;
    return;
}

void mr_generator_print(
    mr_generator_data_t *data, mr_long_t size, mr_str_ct format, ...)
{
    va_list params;
    va_start(params, format);

    if (data->size + size > data->alloc)
    {
        mr_str_t block = realloc(data->data,
            (data->alloc += data->exalloc + size) * sizeof(mr_chr_t));
        if (!block)
        {
            data->error = MR_ERROR_NOT_ENOUGH_MEMORY;
            return;
        }

        data->data = block;
    }

    vsprintf(data->data + data->size, format, params);
    data->size += size;

    va_end(params);
}

mr_byte_t b2_digits(mr_longlong_t num)
{
#if defined(__GNUC__) || defined(__clang__)
    return 64 - __builtin_clzll(x);
#elif defined(_MSC_VER)
    mr_byte_t idx;
    BitScanReverse64((DWORD*)&idx, num);
    return idx;
#endif
}

mr_byte_t b10_digits(mr_longlong_t num)
{
    if (!num)
        return 1;

    mr_byte_t digits = b10_guess[b2_digits(num)];
    return digits + (num >= pow10[digits]);
}
