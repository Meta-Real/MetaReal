/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_NODE__
#define __MR_NODE__

#include <pos.h>

#define VAR_ASSIGN_LINK_MASK 1      /**< Variable properties link mask */
#define VAR_ASSIGN_CONST_MASK 2     /**< Variable properties constant mask */

enum __NODE_TYPES /** Node types */
{
    NONE_N,             /**< None node */
    INT_N,              /**< Integer node */
    FLOAT_N,            /**< Float node*/
    IMAG_N,             /**< Imaginary node */
    BOOL_N,             /**< Boolean node */
    LIST_N,             /**< List node */
    TUPLE_N,            /**< Tuple node */

    BIN_OPERATION_N,    /**< Binary operation node */
    UNARY_OPERATION_N,  /**< Unary operation node */

    VAR_ASSIGN_N,       /**< Variable assignment node */
    VAR_MODIFY_N,       /**< Variable modify node */
    VAR_FMODIFY_N,      /**< Variable fixed modify node */
    VAR_ACCESS_N        /**< Variable access node */
};

/**
 * Sentence equivalent in MetaReal translation process
*/
struct __NODE_T
{
    uint8_t type;           /**< Type of the node (from NODE_TYPES enum) */
    void *value;            /**< Value of the node */

    pos_t poss;             /**< Starting position of the node */
    pos_t pose;             /**< Ending position of the node */
};
typedef struct __NODE_T node_t; /**< Sentence equivalent in MetaReal translation process */

/**
 * List node structure
*/
struct __LIST_NODE_T
{
    node_t *elements;       /**< List of the elements */
    uint64_t size;          /**< Size of the elements list */
};
typedef struct __LIST_NODE_T list_node_t; /**< List node value structure */

/**
 * Binary operation node structure
*/
struct __BIN_OPERATION_NODE_T
{
    uint8_t operator;       /**< Binary operator (from TOKEN_TYPES enum) */

    node_t left;            /**< Left operand */
    node_t right;           /**< Right operand */
};
typedef struct __BIN_OPERATION_NODE_T bin_operation_node_t; /**< Binary operation node structure */

/**
 * Unary operation node structure
*/
struct __UNARY_OPERATION_NODE_T
{
    uint8_t operator;       /**< Unary operator (from TOKEN_TYPES enum) */

    node_t operand;         /**< Operand */
};
typedef struct __UNARY_OPERATION_NODE_T unary_operation_node_t; /**< Unary operation node structure */

/**
 * Variable assignment node structure
*/
struct __VAR_ASSIGN_NODE_T
{
    char *name;             /**< Name of the variable */
    node_t value;           /**< Value of the variable (possibly empty) */

    /**
     * Properties of the variable \n
     * 1st bit: Is the assignment linking (1) or not (0) \n
     * 2nd bit: Is the variable constant (1) or not (0)
    */
    uint8_t prop;
    uint8_t type;           /**< Type of the variable (0 if the type is not specified) */
};
typedef struct __VAR_ASSIGN_NODE_T var_assign_node_t; /**< Variable assignment node structure */

/**
 * \fn void nodes_free(node_t *nodes, uint64_t size)
 * Gets the \a nodes and deallocates all the nodes inside it ( \a nodes pointer included).
 * \param nodes The list of nodes
 * \param size Size of the nodes list
*/
void nodes_free(node_t *nodes, uint64_t size);

/**
 * \fn void nodes_print(const node_t *nodes, uint64_t size)
 * Prints the \a nodes in the stdout stream (for debugging). \n
 * Each node is printed in a separate line.
 * \param nodes The list of nodes
 * \param size Size of the nodes list
*/
void nodes_print(const node_t *nodes, uint64_t size);

/**
 * \fn void node_free(const node_t *node)
 * Gets the \a node and deallocates its content from memory.
 * \param node Node to be freed
*/
void node_free(const node_t *node);

/**
 * \fn void list_node_free(list_node_t *node)
 * Gets the \a node and deallocates its content from memory ( \a node pointer included).
 * \param node Node to be freed
*/
void list_node_free(list_node_t *node);

#endif /* __MR_NODE__ */
