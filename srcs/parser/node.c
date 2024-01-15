/**
 * @file node.c
 * This file contains definitions of the \a node.h file.
*/

#include <parser/node.h>
#include <lexer/token.h>
#include <config.h>
#include <stdlib.h>
#include <stdio.h>

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

void mr_node_free(mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_NONE:
    case MR_NODE_INT:
    case MR_NODE_FLOAT:
    case MR_NODE_IMAGINARY:
    case MR_NODE_VAR_ACCESS:
    case MR_NODE_EX_DOLLAR_METHOD:
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
