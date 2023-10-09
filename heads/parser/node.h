/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_NODE__
#define __MR_NODE__

#include <pos.h>

#define VAR_ASSIGN_LINK_MASK 1
#define VAR_ASSIGN_CONST_MASK 2

enum __NODE_TYPES
{
    NONE_N,
    INT_N,
    FLOAT_N,
    IMAG_N,
    BOOL_N,
    LIST_N,
    TUPLE_N,

    BIN_OPERATION_N,
    UNARY_OPERATION_N,

    VAR_ASSIGN_N,
    VAR_MODIFY_N,
    VAR_FMODIFY_N,
    VAR_ACCESS_N
};

struct __NODE_T
{
    uint8_t type;
    void *value;

    pos_t poss;
    pos_t pose;
};
typedef struct __NODE_T node_t;

struct __LIST_NODE_T
{
    node_t *elements;
    uint64_t size;
};
typedef struct __LIST_NODE_T list_node_t;

struct __BIN_OPERATION_NODE_T
{
    uint8_t operator;

    node_t left;
    node_t right;
};
typedef struct __BIN_OPERATION_NODE_T bin_operation_node_t;

struct __UNARY_OPERATION_NODE_T
{
    uint8_t operator;

    node_t operand;
};
typedef struct __UNARY_OPERATION_NODE_T unary_operation_node_t;

struct __VAR_ASSIGN_NODE_T
{
    char *name;
    node_t value;

    uint8_t prop;
};
typedef struct __VAR_ASSIGN_NODE_T var_assign_node_t;

void nodes_free(node_t *nodes, uint64_t size);
void nodes_print(const node_t *nodes, uint64_t size);

void node_free(const node_t *node);

void list_node_free(list_node_t *node);

#endif /* __MR_NODE__ */
