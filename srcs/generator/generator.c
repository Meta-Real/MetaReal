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

enum __MR_GENERATOR_REG_ENUM
{
    MR_GENERATOR_REG_RAX,
    MR_GENERATOR_REG_RBX,
    MR_GENERATOR_REG_RCX,
    MR_GENERATOR_REG_RDX,
    MR_GENERATOR_REG_RSI,
    MR_GENERATOR_REG_RDI,
    MR_GENERATOR_REG_RBP
};

#define MR_GENERATOR_REG_COUNT (MR_GENERATOR_REG_RBP + 1)

mr_str_ct mr_generator_reg_label[MR_GENERATOR_REG_COUNT] =
{
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp"
};

#pragma pack(push, 1)
struct __MR_GENERATOR_DATA_T
{
    mr_str_t data;
    mr_long_t size;
    mr_long_t alloc;
    mr_long_t exalloc;

    mr_byte_t error;

    mr_bool_t rax_free : 1;
    mr_bool_t rbx_free : 1;
    mr_bool_t rcx_free : 1;
    mr_bool_t rdx_free : 1;
    mr_bool_t rsi_free : 1;
    mr_bool_t rdi_free : 1;
    mr_bool_t rbp_free : 1;
    mr_byte_t reg;

    mr_longlong_t rsp;
};
#pragma pack(pop)
typedef struct __MR_GENERATOR_DATA_T mr_generator_data_t;

void mr_generator_visit(
    mr_generator_data_t *data, mr_node_t *node);
void mr_generator_visit_int(
    mr_generator_data_t *data, mr_node_t *node);
void mr_generator_visit_binary_op(
    mr_generator_data_t *data, mr_node_t *node);
void mr_generator_visit_cint(
    mr_generator_data_t *data, mr_node_t *node);

void mr_generator_print(
    mr_generator_data_t *data, mr_short_t size, mr_str_ct format, ...);

mr_byte_t b16_digits(mr_longlong_t num);

mr_byte_t mr_generator(
    mr_generator_t *res,
    mr_node_t *nodes, mr_long_t size, mr_long_t alloc)
{
    mr_generator_data_t data = {malloc(alloc), 16, alloc, alloc, MR_NOERROR,
        MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE,
        MR_GENERATOR_REG_RAX, 0};
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

    if (data.rsp)
        mr_generator_print(&data, 43 + b16_digits(data.rsp),
            "\tadd rsp, %" PRIx64 "h\n\tmov rax, 0\n\tret\nmain endp\nend\n",
            data.rsp);
    else
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
    case MR_NODE_INT:
        mr_generator_visit_int(data, node);
        break;
    case MR_NODE_BINARY_OP:
        mr_generator_visit_binary_op(data, node);
        break;
    case MR_VALUE_CINT:
        mr_generator_visit_cint(data, node);
        break;
    }
}

void mr_generator_visit_int(
    mr_generator_data_t *data, mr_node_t *node)
{
    if (node->useless)
        return;

    mr_long_t vsize = mr_token_getsize(MR_TOKEN_INT,
        (mr_long_t)(uintptr_t)node->value);

    if (data->rax_free)
    {
        data->reg = MR_GENERATOR_REG_RAX;
        data->rax_free = MR_FALSE;
    }
    else if (data->rbx_free)
    {
        data->reg = MR_GENERATOR_REG_RBX;
        data->rbx_free = MR_FALSE;
    }
    else if (data->rcx_free)
    {
        data->reg = MR_GENERATOR_REG_RCX;
        data->rcx_free = MR_FALSE;
    }
    else if (data->rdx_free)
    {
        data->reg = MR_GENERATOR_REG_RDX;
        data->rdx_free = MR_FALSE;
    }
    else if (data->rsi_free)
    {
        data->reg = MR_GENERATOR_REG_RSI;
        data->rsi_free = MR_FALSE;
    }
    else if (data->rdi_free)
    {
        data->reg = MR_GENERATOR_REG_RDI;
        data->rdi_free = MR_FALSE;
    }
    else if (data->rbp_free)
    {
        data->reg = MR_GENERATOR_REG_RBP;
        data->rbp_free = MR_FALSE;
    }
    else
    {
        mr_generator_print(data, 7 + (mr_short_t)vsize, "\tpush %.*s\n",
            vsize, _mr_config.code + node->value);
        data->rsp += 8;
        return;
    }

    mr_generator_print(data, 11 + (mr_short_t)vsize, "\tmov %s, %.*s\n",
        mr_generator_reg_label[data->reg], vsize,
        _mr_config.code + node->value);
}

void mr_generator_visit_binary_op(
    mr_generator_data_t *data, mr_node_t *node)
{
    mr_node_binary_op_t *vnode = (mr_node_binary_op_t*)(_mr_stack.data + node->value);

    mr_generator_visit(data, &vnode->left);
    if (data->error != MR_NOERROR)
        return;

    mr_byte_t reg = data->reg;

    mr_generator_visit(data, &vnode->right);
    if (data->error != MR_NOERROR || node->useless)
        return;

    switch (vnode->op)
    {
    case MR_TOKEN_PLUS:
        mr_generator_print(data, 14, "\tadd %s, %s\n",
            mr_generator_reg_label[reg], mr_generator_reg_label[data->reg]);
        break;
    case MR_TOKEN_MINUS:
        mr_generator_print(data, 14, "\tsub %s, %s\n",
            mr_generator_reg_label[reg], mr_generator_reg_label[data->reg]);
        break;
    case MR_TOKEN_MULTIPLY:
        mr_generator_print(data, 14, "\tmul %s, %s\n",
            mr_generator_reg_label[reg], mr_generator_reg_label[data->reg]);
        break;
    }

    if (data->error != MR_NOERROR)
        return;

    switch (data->reg)
    {
    case MR_GENERATOR_REG_RAX:
        data->rax_free = MR_TRUE;
        break;
    case MR_GENERATOR_REG_RBX:
        data->rdx_free = MR_TRUE;
        break;
    case MR_GENERATOR_REG_RCX:
        data->rcx_free = MR_TRUE;
        break;
    case MR_GENERATOR_REG_RDX:
        data->rdx_free = MR_TRUE;
        break;
    case MR_GENERATOR_REG_RSI:
        data->rsi_free = MR_TRUE;
        break;
    case MR_GENERATOR_REG_RDI:
        data->rdi_free = MR_TRUE;
        break;
    case MR_GENERATOR_REG_RBP:
        data->rbp_free = MR_TRUE;
        break;
    }

    data->reg = reg;
}

void mr_generator_visit_cint(
    mr_generator_data_t *data, mr_node_t *node)
{
    if (node->useless)
        return;

    if (data->rax_free)
    {
        data->reg = MR_GENERATOR_REG_RAX;
        data->rax_free = MR_FALSE;
    }
    else if (data->rbx_free)
    {
        data->reg = MR_GENERATOR_REG_RBX;
        data->rbx_free = MR_FALSE;
    }
    else if (data->rcx_free)
    {
        data->reg = MR_GENERATOR_REG_RCX;
        data->rcx_free = MR_FALSE;
    }
    else if (data->rdx_free)
    {
        data->reg = MR_GENERATOR_REG_RDX;
        data->rdx_free = MR_FALSE;
    }
    else if (data->rsi_free)
    {
        data->reg = MR_GENERATOR_REG_RSI;
        data->rsi_free = MR_FALSE;
    }
    else if (data->rdi_free)
    {
        data->reg = MR_GENERATOR_REG_RDI;
        data->rdi_free = MR_FALSE;
    }
    else if (data->rbp_free)
    {
        data->reg = MR_GENERATOR_REG_RBP;
        data->rbp_free = MR_FALSE;
    }
    else
    {
        mr_value_cint_t *vnode = (mr_value_cint_t*)(_mr_stack.data + node->value);

        mr_generator_print(data, 7 + b16_digits(vnode->value),
            "\tpush %" PRIx64 "\n", vnode->value);
        data->rsp += 8;
        return;
    }

    mr_value_cint_t *vnode = (mr_value_cint_t*)(_mr_stack.data + node->value);

    mr_generator_print(data, b16_digits(vnode->value) + 12,
        "\tmov %s, %" PRIx64 "h\n",
        mr_generator_reg_label[data->reg], vnode->value);
}

void mr_generator_print(
    mr_generator_data_t *data, mr_short_t size, mr_str_ct format, ...)
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

mr_byte_t b16_digits(mr_longlong_t num)
{
    if (!num)
        return 1;

#if defined(__GNUC__) || defined(__clang__)
    return (67 - __builtin_clzll(x)) / 4;
#elif defined(_MSC_VER)
    if (num == 1)
        return 1;

    mr_byte_t idx;
    BitScanReverse64((DWORD*)&idx, num);
    return (idx + 3) / 4;
#endif
}
