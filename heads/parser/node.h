/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_NODE__
#define __MR_NODE__

#include <pos.h>

enum __NODE_TYPES
{
    INT_N,

    BIN_OPERATION_N,
    UNARY_OPERATION_N
};

struct __NODE_T
{
    uint8_t type;
    void *value;

    pos_t poss;
    pos_t pose;
};
typedef struct __NODE_T node_t;

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

void free_nodes(node_t *nodes, uint64_t size);
void print_nodes(const node_t *nodes, uint64_t size);

void free_node(const node_t *node);

#endif /* __MR_NODE__ */
