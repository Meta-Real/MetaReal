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

/**
 * @file node.h
 * Definitions of the node data structure which is used by the parser to
 * create AST (abstract syntax tree) of the code. \n
 * All things defined in this file have the \a mr_node prefix.
*/

#ifndef __MR_NODE__
#define __MR_NODE__

#include <defs.h>

/** 
 * @struct __MR_NODE_T
 * Sentence equivalent in the compilation process. \n
 * The node contains all the information needed about an statement
 * and the grammar of that statement.
 * @var mr_byte_t __MR_NODE_T::type
 * Type of the <em>node</em>.
 * @var mr_long_t __MR_NODE_T::value
 * Value of the <em>node</em>.
 * @var mr_bool_t __MR_NODE_T::useless
 * Determines if the node is useless or not (used for optimization purposes).
*/
#pragma pack(push, 1)
struct __MR_NODE_T
{
    mr_byte_t type;
    mr_long_t value;

    mr_bool_t useless : 1;

    mr_byte_t reg : 4;
    mr_long_t rsp;
};
#pragma pack(pop)
typedef struct __MR_NODE_T mr_node_t;

/**
 * @enum __MR_NODE_ENUM
 * List of valid node types.
 * @var __MR_NODE_ENUM::MR_NODE_NONE
 * \a None node type.
 * @var __MR_NODE_ENUM::MR_NODE_INT
 * \a Integer node type.
 * @var __MR_NODE_ENUM::MR_NODE_FLOAT
 * \a Float node type.
 * @var __MR_NODE_ENUM::MR_NODE_IMAGINARY
 * \a Imaginary node type.
 * @var __MR_NODE_ENUM::MR_NODE_BINARY_OP
 * <em>Binary operation</em> node type.
 * @var __MR_NODE_ENUM::MR_NODE_UNARY_OP
 * <em>Unary operation</em> node type.
 * @var __MR_NODE_ENUM::MR_NODE_VAR_ACCESS
 * <em>Variable access</em> node type.
 * @var __MR_NODE_ENUM::MR_NODE_FUNC_CALL
 * <em>Function call</em> node type.
 * @var __MR_NODE_ENUM::MR_NODE_EX_FUNC_CALL
 * <em>Exclusive function call</em> node type with an empty list of parameters.
 * @var __MR_NODE_ENUM::MR_NODE_DOLLAR_METHOD
 * <em>Dollar method call</em> node type.
 * @var __MR_NODE_ENUM::MR_NODE_EX_DOLLAR_METHOD
 * <em>Exclusive dollar method call</em> node type with an empty list of parameters.
*/
enum __MR_NODE_ENUM
{
    MR_NODE_NONE,

    MR_NODE_INT,
    MR_NODE_FLOAT,
    MR_NODE_IMAGINARY,

    MR_NODE_BINARY_OP,
    MR_NODE_UNARY_OP,

    MR_NODE_VAR_ACCESS,

    MR_NODE_FUNC_CALL,
    MR_NODE_EX_FUNC_CALL,

    MR_NODE_DOLLAR_METHOD,
    MR_NODE_EX_DOLLAR_METHOD
};

/**
 * Number of valid nodes.
*/
#define MR_NODE_COUNT (MR_NODE_EX_DOLLAR_METHOD + 1)

/**
 * @struct __MR_NODE_BINARY_OP_T
 * Data structure that holds information about a binary operation.
 * @var mr_byte_t __MR_NODE_BINARY_OP_T::op
 * Operator of the operation.
 * @var mr_node_t __MR_NODE_BINARY_OP_T::left
 * Left operand of the operation.
 * @var mr_node_t __MR_NODE_BINARY_OP_T::right
 * Right operand of the operation.
*/
#pragma pack(push, 1)
struct __MR_NODE_BINARY_OP_T
{
    mr_byte_t op;

    mr_node_t left;
    mr_node_t right;
};
#pragma pack(pop)
typedef struct __MR_NODE_BINARY_OP_T mr_node_binary_op_t;

/**
 * @struct __MR_NODE_UNARY_OP_T
 * Data structure that holds information about a unary operation.
 * @var mr_byte_t __MR_NODE_UNARY_OP_T::op
 * Operator of the operation.
 * @var mr_node_t __MR_NODE_UNARY_OP_T::operand
 * Operand of the operation.
 * @var mr_idx_t __MR_NODE_UNARY_OP_T::sidx
 * Starting index of operation.
*/
#pragma pack(push, 1)
struct __MR_NODE_UNARY_OP_T
{
    mr_byte_t op;
    mr_node_t operand;

    mr_idx_t sidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_UNARY_OP_T mr_node_unary_op_t;

/**
 * @struct __MR_NODE_CALL_ARG_T
 * Data structure that holds information about a single function call argument. \n
 * This structure is used by the \a __MR_NODE_FUNC_CALL_T data structure.
 * @var mr_idx_t __MR_NODE_CALL_ARG_T::idx
 * Starting index of the name.
 * If the name is not specified in the code, the \a size field of this parameter will be zero.
 * @var mr_node_t __MR_NODE_CALL_ARG_T::value
 * Value of the argument.
*/
#pragma pack(push, 1)
struct __MR_NODE_CALL_ARG_T
{
    mr_idx_t idx;
    mr_node_t value;
};
#pragma pack(pop)
typedef struct __MR_NODE_CALL_ARG_T mr_node_call_arg_t;

/**
 * @struct __MR_NODE_FUNC_CALL_T
 * Data structure that holds information about a function call (with arguments).
 * @var mr_long_t __MR_NODE_FUNC_CALL_T::args
 * List of function call arguments.
 * @var mr_byte_t __MR_NODE_FUNC_CALL_T::size
 * Size of the \a args list.
 * @var mr_node_t __MR_NODE_FUNC_CALL_T::func
 * Function that needs to be called.
 * @var mr_idx_t __MR_NODE_FUNC_CALL_T::eidx
 * Ending index of the call.
*/
#pragma pack(push, 1)
struct __MR_NODE_FUNC_CALL_T
{
    mr_long_t args;
    mr_byte_t size;

    mr_node_t func;
    mr_idx_t eidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_FUNC_CALL_T mr_node_func_call_t;

/**
 * @struct __MR_NODE_EX_FUNC_CALL_T
 * Data structure that holds information about a function call (without argument).
 * @var mr_node_t __MR_NODE_EX_FUNC_CALL_T::func
 * Function that needs to be called.
 * @var mr_idx_t __MR_NODE_EX_FUNC_CALL_T::eidx
 * Ending index of the call.
*/
#pragma pack(push, 1)
struct __MR_NODE_EX_FUNC_CALL_T
{
    mr_node_t func;
    mr_idx_t eidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_EX_FUNC_CALL_T mr_node_ex_func_call_t;

/**
 * @struct __MR_NODE_DOLLAR_METHOD_T
 * Data structure that holds information about a dollar method call (with arguments).
 * @var mr_long_t __MR_NODE_DOLLAR_METHOD_T::params
 * List of parameters.
 * @var mr_byte_t __MR_NODE_DOLLAR_METHOD_T::size
 * Size of the \a params list.
 * @var mr_idx_t __MR_NODE_DOLLAR_METHOD_T::idx
 * Index of the name of dollar method.
 * @var mr_short_t __MR_NODE_DOLLAR_METHOD_T::sidx
 * Starting index of dollar method.
*/
#pragma pack(push, 1)
struct __MR_NODE_DOLLAR_METHOD_T
{
    mr_long_t params;
    mr_byte_t size;

    mr_idx_t idx;
    mr_idx_t sidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_DOLLAR_METHOD_T mr_node_dollar_method_t;

/**
 * @struct __MR_NODE_EX_DOLLAR_METHOD_T
 * Data structure that holds information about a dollar method call (without argument).
 * @var mr_idx_t __MR_NODE_EX_DOLLAR_METHOD_T::idx
 * Index of the name of dollar method.
 * @var mr_idx_t __MR_NODE_EX_DOLLAR_METHOD_T::sidx
 * Starting index of dollar method call.
*/
#pragma pack(push, 1)
struct __MR_NODE_EX_DOLLAR_METHOD_T
{
    mr_idx_t idx;
    mr_idx_t sidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_EX_DOLLAR_METHOD_T mr_node_ex_dollar_method_t;

#endif
