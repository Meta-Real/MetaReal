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
    MR_GENERATOR_REG_RBP,
    MR_GENERATOR_REG_R8,
    MR_GENERATOR_REG_R9,
    MR_GENERATOR_REG_R10,
    MR_GENERATOR_REG_R11,
    MR_GENERATOR_REG_R12,
    MR_GENERATOR_REG_R13,
    MR_GENERATOR_REG_R14,
    MR_GENERATOR_REG_R15,
    MR_GENERATOR_REG_RSP
};

#define MR_GENERATOR_REG_COUNT (MR_GENERATOR_REG_R15 + 1)
mr_str_ct mr_generator_reg_label[MR_GENERATOR_REG_COUNT] =
{
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp",
    " r8", " r9", "r10", "r11", "r12", "r13", "r14", "r15"
};

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

mr_longlong_t pow10[20] = {
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

    mr_bool_t rax_free : 1;
    mr_bool_t rbx_free : 1;
    mr_bool_t rcx_free : 1;
    mr_bool_t rdx_free : 1;
    mr_bool_t rsi_free : 1;
    mr_bool_t rdi_free : 1;
    mr_bool_t rbp_free : 1;
    mr_bool_t r8_free  : 1;
    mr_bool_t r9_free  : 1;
    mr_bool_t r10_free : 1;
    mr_bool_t r11_free : 1;
    mr_bool_t r12_free : 1;
    mr_bool_t r13_free : 1;
    mr_bool_t r14_free : 1;
    mr_bool_t r15_free : 1;
    mr_byte_t storage;

    mr_long_t stack;
};
#pragma pack(pop)
typedef struct __MR_GENERATOR_DATA_T mr_generator_data_t;

#define mr_generator_reg_check(small, capital)                     \
    do                                                             \
    {                                                              \
        if (data->small ## _free && id & MR_VALUE_REG_ ## capital) \
        {                                                          \
            data->small ## _free = MR_FALSE;                       \
            data->storage = MR_GENERATOR_REG_ ## capital;          \
            return;                                                \
        }                                                          \
    } while (0)

#define mr_generator_regfree(storage) \
    do                                \
    {                                 \
        switch (storage)              \
        {                             \
        case MR_GENERATOR_REG_RAX:    \
            data->rax_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_RBX:    \
            data->rbx_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_RCX:    \
            data->rcx_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_RDX:    \
            data->rdx_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_RSI:    \
            data->rsi_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_RDI:    \
            data->rdi_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_RBP:    \
            data->rbp_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_R8:     \
            data->r8_free = MR_TRUE;  \
            break;                    \
        case MR_GENERATOR_REG_R9:     \
            data->r9_free = MR_TRUE;  \
            break;                    \
        case MR_GENERATOR_REG_R10:    \
            data->r10_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_R11:    \
            data->r11_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_R12:    \
            data->r12_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_R13:    \
            data->r13_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_R14:    \
            data->r14_free = MR_TRUE; \
            break;                    \
        case MR_GENERATOR_REG_R15:    \
            data->r15_free = MR_TRUE; \
            break;                    \
        }                             \
    } while (0)

void mr_generator_visit(
    mr_generator_data_t *data, mr_node_t *node);
void mr_generator_visit_binary_op(
    mr_generator_data_t *data, mr_node_t *node);
void mr_generator_visit_cint(
    mr_generator_data_t *data, mr_node_t *node);

void mr_generator_regres(
    mr_generator_data_t *data, mr_long_t id);

void mr_generator_print(
    mr_generator_data_t *data, mr_long_t size, mr_str_ct format, ...);

mr_long_t mr_generator_getstack(
    mr_node_t *node);

mr_byte_t b2_digits(mr_longlong_t num);
mr_byte_t b10_digits(mr_longlong_t num);

mr_byte_t mr_generator(
    mr_generator_t *res,
    mr_node_t *nodes, mr_long_t size, mr_long_t alloc)
{
    mr_generator_data_t data = {malloc(alloc), 16, alloc, alloc, MR_NOERROR,
        MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE,
        MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE,
        0, 0};
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

    if (data.stack)
        mr_generator_print(&data, b10_digits(data.stack) + 42,
            "\tadd rsp, %" PRIu32 "\n\tmov rax, 0\n\tret\nmain endp\nend\n",
            data.stack);
    else
        mr_generator_print(&data, 31,
            "\tmov rax, 0\n\tret\nmain endp\nend\n");

    *res = (mr_generator_t){data.data, data.size, data.stack, b10_digits(data.stack)};
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

    mr_byte_t lstorage = data->storage;

    mr_generator_visit(data, &vnode->right);
    if (data->error != MR_NOERROR || node->useless)
        return;

    switch (vnode->op)
    {
    case MR_TOKEN_PLUS:
        if (lstorage != MR_GENERATOR_REG_RSP)
        {
            if (data->storage != MR_GENERATOR_REG_RSP)
            {
                mr_generator_print(data, 14, "\tadd %s, %s\n",
                    mr_generator_reg_label[lstorage],
                    mr_generator_reg_label[data->storage]);
                break;
            }

            mr_long_t rstack = data->stack - mr_generator_getstack(&vnode->right);
            mr_generator_print(data, 17 + b10_digits(rstack),
                "\tadd %s, [rsp-%" PRIu32 "]\n",
                mr_generator_reg_label[lstorage], rstack);
            break;
        }

        if (data->storage != MR_GENERATOR_REG_RSP)
        {
            mr_long_t lstack = data->stack - mr_generator_getstack(&vnode->left);
            mr_generator_print(data, 17 + b10_digits(lstack),
                "\tadd [rsp-%" PRIu32 "], %s\n",
                lstack, mr_generator_reg_label[data->storage]);
            break;
        }

        /*mr_long_t rrsp = data->rsp - vnode->right.rsp;
        mr_generator_print(data, 51 + 2 * b10_digits(lrsp) + b10_digits(rrsp),
            "\tmov r15, [rsp-%" PRIu32 "]\n"
            "\tadd r15, [rsp-%" PRIu32 "]\n"
            "\tmov [rsp-%" PRIu32 "], r15\n",
            lrsp, rrsp, lrsp);*/
        break;
    }

    if (data->error != MR_NOERROR)
        return;

    mr_generator_regfree(data->storage);
    data->storage = lstorage;
}

void mr_generator_visit_cint(
    mr_generator_data_t *data, mr_node_t *node)
{
    if (node->useless)
        return;

    mr_value_cint_t *vnode = (mr_value_cint_t*)(_mr_stack.data + node->value);

    mr_generator_regres(data, vnode->storage);
    if (data->storage != MR_GENERATOR_REG_RSP)
    {
        mr_generator_print(data, 11 + b10_digits(vnode->value),
            "\tmov %s, %" PRIu64 "\n",
            mr_generator_reg_label[data->storage], vnode->value);

        vnode->storage = data->storage;
        return;
    }

    mr_generator_print(data, 7 + b10_digits(vnode->value),
        "\tpush %" PRIu64 "\n", vnode->value);

    data->stack += 8;
    vnode->storage = data->stack;
    vnode->storage |= MR_VALUE_REG_RSP;
}

void mr_generator_regres(
    mr_generator_data_t *data, mr_long_t id)
{
    mr_generator_reg_check(rax, RAX);
    mr_generator_reg_check(rbx, RBX);
    mr_generator_reg_check(rcx, RCX);
    mr_generator_reg_check(rdx, RDX);
    mr_generator_reg_check(rsi, RSI);
    mr_generator_reg_check(rdi, RDI);
    mr_generator_reg_check(rbp, RBP);
    mr_generator_reg_check(r8 , R8 );
    mr_generator_reg_check(r9 , R9 );
    mr_generator_reg_check(r10, R10);
    mr_generator_reg_check(r11, R11);
    mr_generator_reg_check(r12, R12);
    mr_generator_reg_check(r13, R13);
    mr_generator_reg_check(r14, R14);
    mr_generator_reg_check(r15, R15);
    data->storage = MR_GENERATOR_REG_RSP;
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

mr_long_t mr_generator_getstack(
    mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_BINARY_OP:
    {
        mr_node_binary_op_t *vnode = (mr_node_binary_op_t*)(_mr_stack.data + node->value);
        return mr_generator_getstack(&vnode->left);
    }
    case MR_VALUE_CINT:
    {
        mr_value_cint_t *vnode = (mr_value_cint_t*)(_mr_stack.data + node->value);
        return vnode->storage ^ MR_VALUE_REG_RSP;
    }
    }

    return 0;
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
