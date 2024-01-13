/**
 * @file node.c
 * This file contains definitions of the \a node.h file.
*/

#include <parser/node.h>
#include <config.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * List of node labels (used by the \a mr_node_print function).
*/
mr_str_ct mr_node_label[MR_NODE_COUNT] =
{
    "NODE_NULL",
    "NODE_INT", "NODE_FLOAT", "NODE_IMAGINARY",
    "NODE_BINARY_OP", "NODE_UNARY_OP",
    "NODE_VAR_ACCESS",
    "NODE_FUNC_CALL", "NODE_EX_FUNC_CALL",
    "NODE_DOLLAR_METHOD", "NODE_EX_DOLLAR_METHOD",
};

/**
 * It prints out a \a mr_node_data_t structure into <em>outstream</em>. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param node
 * Node value that needs to be printed.
*/
#ifndef __MR_DEBUG__
inline void mr_node_data_print(mr_node_data_t *node)
#else
void mr_node_data_print(mr_node_data_t *node)
#endif
{
    fwrite(_mr_config.code + node->sidx, sizeof(mr_chr_t), node->size, stdout);
}

/**
 * It deallocates a \a mr_node_binary_op_t structure from memory.
 * @param node
 * Node value that needs to be deallocated.
*/
#ifndef __MR_DEBUG__
inline void mr_node_binary_op_free(mr_node_binary_op_t *node)
#else
void mr_node_binary_op_free(mr_node_binary_op_t *node)
#endif
{
    mr_node_free(&node->right);
    mr_node_free(&node->left);
    free(node);
}

/**
 * It prints out a \a mr_node_binary_op_t structure into <em>outstream</em>. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param node
 * Node value that needs to be printed.
*/
void mr_node_binary_op_print(mr_node_binary_op_t *node);

/**
 * It deallocates a \a mr_node_unary_op_t structure from memory.
 * @param node
 * Node value that needs to be deallocated.
*/
#ifndef __MR_DEBUG__
inline void mr_node_unary_op_free(mr_node_unary_op_t *node)
#else
void mr_node_unary_op_free(mr_node_unary_op_t *node)
#endif
{
    mr_node_free(&node->operand);
    free(node);
}

/**
 * It prints out a \a mr_node_unary_op_t structure into <em>outstream</em>. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param node
 * Node value that needs to be printed.
*/
void mr_node_unary_op_print(mr_node_unary_op_t *node);

/**
 * It prints out a \a mr_node_func_call_t structure into <em>outstream</em>. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param node
 * Node value that needs to be printed.
*/
void mr_node_func_call_print(mr_node_func_call_t *node);

/**
 * It deallocates a \a mr_node_ex_func_call_t structure from memory.
 * @param node
 * Node value that needs to be deallocated.
*/
#ifndef __MR_DEBUG__
inline void mr_node_ex_func_call_free(mr_node_ex_func_call_t *node)
#else
void mr_node_ex_func_call_free(mr_node_ex_func_call_t *node)
#endif
{
    mr_node_free(&node->func);
    free(node);
}

/**
 * It prints out a \a mr_node_ex_func_call_t structure into <em>outstream</em>. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param node
 * Node value that needs to be printed.
*/
#ifndef __MR_DEBUG__
inline void mr_node_ex_func_call_print(mr_node_ex_func_call_t *node)
#else
void mr_node_ex_func_call_print(mr_node_ex_func_call_t *node)
#endif
{
    mr_node_print(&node->func);
}

/**
 * It deallocates a \a mr_node_dollar_method_t structure from memory.
 * @param node
 * Node value that needs to be deallocated.
*/
#ifndef __MR_DEBUG__
inline void mr_node_dollar_method_free(mr_node_dollar_method_t *node)
#else
void mr_node_dollar_method_free(mr_node_dollar_method_t *node)
#endif
{
    mr_nodes_free(node->params, node->size);
    free(node);
}

/**
 * It prints out a \a mr_node_dollar_method_t structure into <em>outstream</em>. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param node
 * Node value that needs to be printed.
*/
void mr_node_dollar_method_print(mr_node_dollar_method_t *node);

/**
 * It prints out a \a mr_node_ex_dollar_method_t structure into <em>outstream</em>. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param node
 * Node value that needs to be printed.
*/
#ifndef __MR_DEBUG__
inline void mr_node_ex_dollar_method_print(mr_node_ex_dollar_method_t *node)
#else
void mr_node_ex_dollar_method_print(mr_node_ex_dollar_method_t *node)
#endif
{
    mr_node_data_print(&node->name);
}

void mr_node_free(mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_NULL:
        return;
    case MR_NODE_INT:
    case MR_NODE_FLOAT:
    case MR_NODE_IMAGINARY:
    case MR_NODE_VAR_ACCESS:
    case MR_NODE_EX_DOLLAR_METHOD:
        free(node->value);
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
    case MR_NODE_EX_FUNC_CALL:
        mr_node_ex_func_call_free(node->value);
        return;
    case MR_NODE_DOLLAR_METHOD:
        mr_node_dollar_method_free(node->value);
        return;
    }
}

void mr_nodes_free(mr_node_t *nodes, mr_long_t size)
{
    while (size--)
        mr_node_free(nodes + size);
    free(nodes);
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
    case MR_NODE_IMAGINARY:
        mr_node_data_print(node->value);
        break;
    case MR_NODE_BINARY_OP:
        mr_node_binary_op_print(node->value);
        break;
    case MR_NODE_UNARY_OP:
        mr_node_unary_op_print(node->value);
        break;
    case MR_NODE_VAR_ACCESS:
        mr_node_data_print(node->value);
        break;
    case MR_NODE_FUNC_CALL:
        mr_node_func_call_print(node->value);
        break;
    case MR_NODE_EX_FUNC_CALL:
        mr_node_ex_func_call_print(node->value);
        break;
    case MR_NODE_DOLLAR_METHOD:
        mr_node_dollar_method_print(node->value);
        break;
    case MR_NODE_EX_DOLLAR_METHOD:
        mr_node_ex_dollar_method_print(node->value);
        break;
    }

    putchar(')');
}

void mr_nodes_print(mr_node_t *nodes, mr_long_t size)
{
    for (mr_long_t i = 0; i != size; i++)
    {
        mr_node_print(nodes + i);
        putchar('\n');
    }
}

void mr_node_func_call_free(mr_node_func_call_t *node)
{
    mr_node_call_arg_t *arg;
    for (; node->size--;)
    {
        arg = node->args + node->size;
        mr_node_free(&arg->value);
    }

    mr_node_free(&node->func);
    free(node);
}

void mr_node_binary_op_print(mr_node_binary_op_t *node)
{
    printf("%s, ", mr_token_label[node->op]);
    mr_node_print(&node->left);
    fputs(", ", stdout);
    mr_node_print(&node->right);
}

void mr_node_unary_op_print(mr_node_unary_op_t *node)
{
    printf("%s, ", mr_token_label[node->op]);
    mr_node_print(&node->operand);
}

void mr_node_func_call_print(mr_node_func_call_t *node)
{
    mr_node_print(&node->func);
    if (!node->size)
        return;

    printf(", [%" PRIu8 "]{", node->size);

    if (node->args->name.size)
    {
        putchar('"');
        mr_node_data_print(&node->args->name);
        fputs("\": ", stdout);
    }

    mr_node_print(&node->args->value);

    mr_node_call_arg_t *arg;
    for (mr_byte_t i = 1; i != node->size; i++)
    {
        fputs(", ", stdout);

        arg = node->args + i;
        if (arg->name.size)
        {
            putchar('"');
            mr_node_data_print(&arg->name);
            fputs("\": ", stdout);
        }

        mr_node_print(&arg->value);
    }

    putchar('}');
}

void mr_node_dollar_method_print(mr_node_dollar_method_t *node)
{
    mr_node_data_print(&node->name);

    printf(", [%" PRIu8 "]{", node->size);
    mr_node_print(node->params);

    for (mr_byte_t i = 1; i != node->size; i++)
    {
        fputs(", ", stdout);
        mr_node_print(node->params + i);
    }

    putchar('}');
}
