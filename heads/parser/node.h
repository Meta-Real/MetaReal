/**
 * @file node.h
 * Definitions of the node data structure which is used by the parser to
 * create AST (abstract syntax tree) of the code. \n
 * All things defined in \a node.c and this file have the \a mr_node prefix.
*/

#ifndef __MR_NODE__
#define __MR_NODE__

#include <lexer/token.h>
#include <defs.h>

struct __MR_NODE_T
{
    mr_byte_t type;
    mr_ptr_t value;
};
typedef struct __MR_NODE_T mr_node_t;

enum __MR_NODE_ENUM
{
    MR_NODE_NULL,

    MR_NODE_INT,
    MR_NODE_FLOAT,
    MR_NODE_IMAG,

    MR_NODE_BINARY_OP,
    MR_NODE_UNARY_OP,

    MR_NODE_FUNC_CALL,
    MR_NODE_DOLLAR_METHOD
};

struct __MR_NODE_DATA_T
{
    mr_str_t data;
    short size;

    mr_pos_t poss;
    mr_pos_t pose;
};
typedef struct __MR_NODE_DATA_T mr_node_data_t;

struct __MR_NODE_BINARY_OP_T
{
    mr_byte_t op;

    mr_node_t left;
    mr_node_t right;
};
typedef struct __MR_NODE_BINARY_OP_T mr_node_binary_op_t;

struct __MR_NODE_UNARY_OP_T
{
    mr_byte_t op;
    mr_node_t operand;

    mr_pos_t poss;
};
typedef struct __MR_NODE_UNARY_OP_T mr_node_unary_op_t;

struct __MR_NODE_CALL_ARG_T
{
    mr_node_data_t name;
    mr_node_t value;
};
typedef struct __MR_NODE_CALL_ARG_T mr_node_call_arg_t;

struct __MR_NODE_FUNC_CALL_T
{
    mr_node_t func;

    mr_node_call_arg_t *args;
    mr_byte_t size;

    mr_pos_t pose;
};
typedef struct __MR_NODE_FUNC_CALL_T mr_node_func_call_t;

struct __MR_NODE_DOLLAR_METHOD_T
{
    mr_node_data_t name;

    mr_node_t *args;
    mr_byte_t size;

    mr_pos_t poss;
};
typedef struct __MR_NODE_DOLLAR_METHOD_T mr_node_dollar_method_t;

void mr_node_free(mr_node_t *node);

#endif
