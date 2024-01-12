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
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum __MR_GENERATOR_REG_ENUM
{
    MR_GENERATOR_REG_RCX,
    MR_GENERATOR_REG_RDX,
    MR_GENERATOR_REG_RAX
};

#define MR_GENERATOR_REG_COUNT (MR_GENERATOR_REG_RAX + 1)

mr_str_ct mr_generator_reg_label[MR_GENERATOR_REG_COUNT] =
{"rcx", "rdx", "rax"};

#pragma pack(push, 1)
struct __MR_GENERATOR_DATA_T
{
    mr_str_t data;
    mr_long_t size;
    mr_long_t alloc;
    mr_long_t exalloc;

    mr_byte_t error;

    mr_bool_t rcx_free : 1;
    mr_bool_t rdx_free : 1;
    mr_bool_t rax_free : 1;
    mr_byte_t reg : 2;
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

mr_generator_t mr_generator(
    mr_node_t *nodes, mr_long_t size, mr_long_t alloc)
{
    mr_generator_data_t data = {malloc(alloc), 16, alloc, alloc, MR_NOERROR,
        MR_TRUE, MR_TRUE, MR_TRUE, MR_TRUE};
    if (!data.data)
    {
        mr_generator_t res;
        res.error = MR_ERROR_NOT_ENOUGH_MEMORY;
        return res;
    }

    memcpy(data.data, ".code\nmain proc\n", 16 * sizeof(mr_chr_t));

    for (mr_long_t i = 0; i < size; i++)
    {
        mr_generator_visit(&data, nodes + i);
        if (data.error != MR_NOERROR)
        {
            for (; i < size; i++)
                mr_node_free(nodes + i);
            free(nodes);
            free(data.data);

            mr_generator_t res;
            res.error = data.error;
            return res;
        }
    }

    size = data.size + 31;
    if (size > data.alloc)
    {
        mr_str_t block = realloc(data.data, size * sizeof(mr_chr_t));
        if (!block)
        {
            free(nodes);
            free(data.data);

            mr_generator_t res;
            res.error = MR_ERROR_NOT_ENOUGH_MEMORY;
            return res;
        }

        data.data = block;
    }

    memcpy(data.data + data.size,
        "\tmov\trax, 0\n\tret\nmain endp\nend\n", 31 * sizeof(mr_chr_t));

    mr_generator_t res = {data.data, size, MR_NOERROR};
    free(nodes);
    return res;
}

void mr_generator_visit(
    mr_generator_data_t *data, mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_NULL:
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
    mr_node_data_t *vnode = (mr_node_data_t*)node->value;

    if (node->useless)
    {
        free(vnode->data);
        free(vnode);
        return;
    }

    mr_short_t size = 11 + (mr_short_t)strlen(vnode->data);
    if (data->size + size > data->alloc)
    {
        mr_str_t block = realloc(data->data,
            (data->alloc += data->exalloc + size) * sizeof(mr_chr_t));
        if (!block)
        {
            free(vnode->data);
            free(vnode);

            data->error = MR_ERROR_NOT_ENOUGH_MEMORY;
            return;
        }

        data->data = block;
    }

    if (data->rcx_free)
    {
        data->reg = MR_GENERATOR_REG_RCX;
        data->rcx_free = MR_FALSE;
    }
    else if (data->rdx_free)
    {
        data->reg = MR_GENERATOR_REG_RDX;
        data->rdx_free = MR_FALSE;
    }
    else if (data->rax_free)
    {
        data->reg = MR_GENERATOR_REG_RAX;
        data->rax_free = MR_FALSE;
    }

    sprintf(data->data + data->size, "\tmov\t%s, %s\n",
        mr_generator_reg_label[data->reg], vnode->data);
    data->size += size;

    free(vnode->data);
    free(vnode);
}

void mr_generator_visit_binary_op(
    mr_generator_data_t *data, mr_node_t *node)
{
    mr_node_binary_op_t *vnode = (mr_node_binary_op_t*)node->value;

    mr_generator_visit(data, &vnode->left);
    if (data->error != MR_NOERROR)
    {
        mr_value_free(&vnode->right);
        free(vnode);
        return;
    }

    mr_byte_t reg = data->reg;

    mr_generator_visit(data, &vnode->right);
    if (data->error != MR_NOERROR || node->useless)
    {
        free(vnode);
        return;
    }

    if (data->size + 14 > data->alloc)
    {
        mr_str_t block = realloc(data->data,
            (data->alloc += data->exalloc + 14) * sizeof(mr_chr_t));
        if (!block)
        {
            free(vnode);

            data->error = MR_ERROR_NOT_ENOUGH_MEMORY;
            return;
        }

        data->data = block;
    }

    switch (vnode->op)
    {
    case MR_TOKEN_PLUS:
        sprintf(data->data + data->size, "\tadd\t%s, %s\n",
            mr_generator_reg_label[reg],
            mr_generator_reg_label[data->reg]);
        data->size += 14;
        break;
    case MR_TOKEN_MINUS:
        sprintf(data->data + data->size, "\tsub\t%s, %s\n",
            mr_generator_reg_label[reg],
            mr_generator_reg_label[data->reg]);
        data->size += 14;
        break;
    case MR_TOKEN_MULTIPLY:
        sprintf(data->data + data->size, "\tmul\t%s, %s\n",
            mr_generator_reg_label[reg],
            mr_generator_reg_label[data->reg]);
        data->size += 14;
        break;
    }

    switch (data->reg)
    {
    case MR_GENERATOR_REG_RCX:
        data->rcx_free = MR_TRUE;
        break;
    case MR_GENERATOR_REG_RDX:
        data->rdx_free = MR_TRUE;
        break;
    case MR_GENERATOR_REG_RAX:
        data->rax_free = MR_TRUE;
        break;
    }

    data->reg = reg;
    free(vnode);
}

void mr_generator_visit_cint(
    mr_generator_data_t *data, mr_node_t *node)
{
    if (node->useless)
    {
        free(node->value);
        return;
    }

    mr_value_cint_t *vnode = (mr_value_cint_t*)node->value;

    // We can do better
    mr_short_t size = 1;
    mr_longlong_t copy = vnode->value;
    while (copy /= 10)
        size++;

    size += 11;
    if (data->size + size > data->alloc)
    {
        mr_str_t block = realloc(data->data,
            (data->alloc += data->exalloc + size) * sizeof(mr_chr_t));
        if (!block)
        {
            free(vnode);

            data->error = MR_ERROR_NOT_ENOUGH_MEMORY;
            return;
        }

        data->data = block;
    }

    if (data->rcx_free)
    {
        data->reg = MR_GENERATOR_REG_RCX;
        data->rcx_free = MR_FALSE;
    }
    else if (data->rdx_free)
    {
        data->reg = MR_GENERATOR_REG_RDX;
        data->rdx_free = MR_FALSE;
    }
    else if (data->rax_free)
    {
        data->reg = MR_GENERATOR_REG_RAX;
        data->rax_free = MR_FALSE;
    }

    sprintf(data->data + data->size, "\tmov\t%s, %" PRIu64 "\n",
        mr_generator_reg_label[data->reg], vnode->value);
    data->size += size;

    free(vnode);
}
