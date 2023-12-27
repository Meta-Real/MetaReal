/**
 * @file token.c
 * This file contains definitions of the \a node.h file.
*/

#include <parser/node.h>
#include <stdio.h>
#include <alloc.h>

#define MR_NODE_COUNT (MR_NODE_DOLLAR_METHOD + 1)

mr_str_ct mr_node_label[MR_NODE_COUNT] =
{
    "NODE_NULL",
    "NODE_INT", "NODE_FLOAT", "NODE_IMAG",
    "NODE_BINARY_OP", "NODE_UNARY_OP",
    "NODE_FUNC_CALL", "NODE_DOLLAR_METHOD"
};

void mr_node_print(mr_node_t *node);

static inline void mr_node_data_free(mr_node_data_t *node)
{
    mr_free(node->data);
    mr_free(node);
}

static inline void mr_node_data_print(mr_node_data_t *node)
{
    fwrite(node->data, sizeof(mr_chr_t), node->size, stdout);
}

static inline void mr_node_binary_op_free(mr_node_binary_op_t *node)
{
    mr_node_free(&node->right);
    mr_node_free(&node->left);
    mr_free(node);
}

static inline void mr_node_binary_op_print(mr_node_binary_op_t *node)
{
    printf("%s, ", mr_token_label[node->op]);
    mr_node_print(&node->left);
    fputs(", ", stdout);
    mr_node_print(&node->right);
}

static inline void mr_node_unary_op_free(mr_node_unary_op_t *node)
{
    mr_node_free(&node->operand);
    mr_free(node);
}

static inline void mr_node_unary_op_print(mr_node_unary_op_t *node)
{
    printf("%s, ", mr_token_label[node->op]);
    mr_node_print(&node->operand);
}

static inline void mr_node_func_call_free(mr_node_func_call_t *node)
{
    mr_node_call_arg_t *arg;
    for (; node->size--;)
    {
        arg = node->args + node->size;
        mr_node_free(&arg->value);
        mr_node_data_free(&arg->name);
    }

    mr_node_free(&node->func);
}

static inline void mr_node_func_call_print(mr_node_func_call_t *node)
{
    mr_node_print(&node->func);
    if (!node->size)
        return;

    printf(", [%hu]{", node->size);

    if (node->args->name.data)
    {
        putchar('"');
        mr_node_data_print(&node->args->name);
        fputs("\": ", stdout);
    }

    mr_node_print(&node->args->value);

    mr_node_call_arg_t *arg;
    for (mr_byte_t i = 1; i < node->size; i++)
    {
        fputs(", ", stdout);

        arg = node->args + i;
        if (arg->name.data)
        {
            putchar('"');
            mr_node_data_print(&arg->name);
            fputs("\": ", stdout);
        }

        mr_node_print(&arg->value);
    }

    putchar('}');
}

static inline void mr_node_dollar_method_free(mr_node_dollar_method_t *node)
{
    mr_nodes_free(node->args, node->size);
    mr_node_data_free(&node->name);
}

static inline void mr_node_dollar_method_print(mr_node_dollar_method_t *node)
{
    mr_node_data_print(&node->name);
    if (!node->size)
        return;

    printf(", [%hu]{", node->size);
    mr_node_print(node->args);

    for (mr_byte_t i = 1; i < node->size; i++)
    {
        fputs(", ", stdout);
        mr_node_print(node->args + i);
    }

    putchar('}');
}

void mr_node_free(mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_NULL:
        return;
    case MR_NODE_INT:
    case MR_NODE_FLOAT:
    case MR_NODE_IMAG:
        mr_node_data_free(node->value);
        return;
    case MR_NODE_BINARY_OP:
        mr_node_binary_op_free(node->value);
        return;
    case MR_NODE_UNARY_OP:
        mr_node_unary_op_free(node->value);
        return;
    case MR_NODE_FUNC_CALL:
        mr_node_func_call_free(node->value);
        return;
    case MR_NODE_DOLLAR_METHOD:
        mr_node_dollar_method_free(node->value);
        return;
    }
}

void mr_nodes_free(mr_node_t *nodes, mr_long_t size)
{
    for (; size--;)
        mr_node_free(nodes + size);
    mr_free(nodes);
}

void mr_nodes_print(mr_node_t *nodes, mr_long_t size)
{
    for (mr_long_t i = 0; i < size; i++)
    {
        mr_node_print(nodes + i);
        putchar('\n');
    }
}

void mr_node_print(mr_node_t *node)
{
    printf("(%s", mr_node_label[node->type]);
    if (node->type == MR_NODE_NULL)
    {
        putchar(')');
        return;
    }

    fputs(": ", stdout);
    switch (node->type)
    {
    case MR_NODE_INT:
    case MR_NODE_FLOAT:
    case MR_NODE_IMAG:
        mr_node_data_print(node->value);
        break;
    case MR_NODE_BINARY_OP:
        mr_node_binary_op_print(node->value);
        break;
    case MR_NODE_UNARY_OP:
        mr_node_unary_op_print(node->value);
        break;
    case MR_NODE_FUNC_CALL:
        mr_node_func_call_print(node->value);
        break;
    case MR_NODE_DOLLAR_METHOD:
        mr_node_dollar_method_print(node->value);
        break;
    }

    putchar(')');
}
