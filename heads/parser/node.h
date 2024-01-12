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

/** 
 * @struct __MR_NODE_T
 * Sentence equivalent in the compilation process. \n
 * The node contains all the information needed about
 * an statement and the grammar of that statement.
 * @var mr_byte_t __MR_NODE_T::type
 * Type of the <em>node</em>.
 * @var mr_ptr_t __MR_NODE_T::value
 * Value of the <em>node</em>.
*/
#pragma pack(push, 1)
struct __MR_NODE_T
{
    mr_byte_t type;
    mr_ptr_t value;
};
#pragma pack(pop)
typedef struct __MR_NODE_T mr_node_t;

/**
 * @enum __MR_NODE_ENUM
 * Complete list of valid node types.
 * @var __MR_NODE_ENUM::MR_NODE_NULL
 * \a NULL node type.
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
    MR_NODE_NULL,

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
 * @struct __MR_NODE_DATA_T
 * Data structure that holds information about a single string. \n
 * Constant version of this structure is used for identifiers.
 * @var mr_str_t __MR_NODE_DATA_T::data
 * Data of the structure (can be constant or allocated).
 * @var mr_long_t __MR_NODE_DATA_T::sidx
 * Index of the start of data.
 * @var mr_short_t __MR_NODE_DATA_T::size
 * Size of the \a data (this parameter is also used for debugging).
*/
#pragma pack(push, 1)
struct __MR_NODE_DATA_T
{
    mr_str_t data;

    mr_long_t sidx;
    mr_short_t size;
};
#pragma pack(pop)
typedef struct __MR_NODE_DATA_T mr_node_data_t;

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
 * @var mr_long_t __MR_NODE_UNARY_OP_T::sidx
 * Index of the start of the operation.
*/
#pragma pack(push, 1)
struct __MR_NODE_UNARY_OP_T
{
    mr_byte_t op;
    mr_node_t operand;

    mr_long_t sidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_UNARY_OP_T mr_node_unary_op_t;

/**
 * @struct __MR_NODE_CALL_ARG_T
 * Data structure that holds information about a single function call argument. \n
 * This structure is used by the \a __MR_NODE_FUNC_CALL_T data structure.
 * @var mr_node_data_t __MR_NODE_CALL_ARG_T::name
 * Name of the argument. \n
 * If the name is not specified in the code, the \a data field of this parameter will be NULL.
 * @var mr_node_t __MR_NODE_CALL_ARG_T::value
 * Value of the argument.
*/
#pragma pack(push, 1)
struct __MR_NODE_CALL_ARG_T
{
    mr_node_data_t name;
    mr_node_t value;
};
#pragma pack(pop)
typedef struct __MR_NODE_CALL_ARG_T mr_node_call_arg_t;

/**
 * @struct __MR_NODE_FUNC_CALL_T
 * Data structure that holds information about a function call (with arguments).
 * @var mr_node_t __MR_NODE_FUNC_CALL_T::func
 * Function that needs to be called.
 * @var mr_node_call_arg_t* __MR_NODE_FUNC_CALL_T::args
 * List of function call arguments.
 * @var mr_byte_t __MR_NODE_FUNC_CALL_T::size
 * Size of the \a args list.
 * @var mr_long_t __MR_NODE_FUNC_CALL_T::eidx
 * Index of the end of the function call.
*/
#pragma pack(push, 1)
struct __MR_NODE_FUNC_CALL_T
{
    mr_node_t func;

    mr_node_call_arg_t *args;
    mr_byte_t size;

    mr_long_t eidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_FUNC_CALL_T mr_node_func_call_t;

/**
 * @struct __MR_NODE_EX_FUNC_CALL_T
 * Data structure that holds information about a function call (without argument).
 * @var mr_node_t __MR_NODE_EX_FUNC_CALL_T::func
 * Function that needs to be called.
 * @var mr_long_t __MR_NODE_EX_FUNC_CALL_T::eidx
 * Index of the end of the function call.
*/
#pragma pack(push, 1)
struct __MR_NODE_EX_FUNC_CALL_T
{
    mr_node_t func;
    mr_long_t eidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_EX_FUNC_CALL_T mr_node_ex_func_call_t;

/**
 * @struct __MR_NODE_DOLLAR_METHOD_T
 * Data structure that holds information about a dollar method call (with arguments).
 * @var mr_node_data_t __MR_NODE_DOLLAR_METHOD_T::name
 * Name of the dollar method.
 * @var mr_node_t* __MR_NODE_DOLLAR_METHOD_T::params
 * List of parameters.
 * @var mr_byte_t __MR_NODE_DOLLAR_METHOD_T::size
 * Size of the \a params list.
 * @var mr_long_t __MR_NODE_DOLLAR_METHOD_T::sidx
 * Index of the start of the dollar method call.
*/
#pragma pack(push, 1)
struct __MR_NODE_DOLLAR_METHOD_T
{
    mr_node_data_t name;

    mr_node_t *params;
    mr_byte_t size;

    mr_long_t sidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_DOLLAR_METHOD_T mr_node_dollar_method_t;

/**
 * @struct __MR_NODE_EX_DOLLAR_METHOD_T
 * Data structure that holds information about a dollar method call (without argument).
 * @var mr_node_data_t __MR_NODE_EX_DOLLAR_METHOD_T::name
 * Name of the dollar method.
 * @var mr_long_t __MR_NODE_EX_DOLLAR_METHOD_T::sidx
 * Index of the start of the dollar method call.
*/
#pragma pack(push, 1)
struct __MR_NODE_EX_DOLLAR_METHOD_T
{
    mr_node_data_t name;
    mr_long_t sidx;
};
#pragma pack(pop)
typedef struct __MR_NODE_EX_DOLLAR_METHOD_T mr_node_ex_dollar_method_t;

/**
 * It deallocates a single node passed as a pointer.
 * @param node
 * Node that needs to be deallocated.
*/
void mr_node_free(mr_node_t *node);

/**
 * It deallocates the nodes list and its elements from memory according to its size.
 * @param nodes
 * List of nodes.
 * @param size
 * Size of the \a nodes list.
*/
void mr_nodes_free(mr_node_t *nodes, mr_long_t size);

/**
 * It prints out a single node passed as a pointer into <em>outstream</em>. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param node
 * A node that needs to be printed.
*/
void mr_node_print(mr_node_t *node);

/**
 * It prints out the nodes list into \a outstream according to its size. \n
 * \a outstream is \a stdout by default and
 * can be changed with the \a $set_outstream dollar method.
 * @param nodes
 * List of nodes.
 * @param size
 * Size of the \a nodes list.
*/
void mr_nodes_print(mr_node_t *nodes, mr_long_t size);

/**
 * It deallocates a \a mr_node_func_call_t structure from memory.
 * @param node
 * Node value that needs to be deallocated.
*/
void mr_node_func_call_free(mr_node_func_call_t *node);

#endif
