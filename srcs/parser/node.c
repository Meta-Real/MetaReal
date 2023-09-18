/*
    MetaReal Compiler version 1.0.0
*/

#include <parser/node.h>
#include <lexer/token.h>
#include <alloc.h>
#include <stdio.h>

void print_node(const node_t *node);

void free_bin_operation_node(bin_operation_node_t *node);
void print_bin_operation_node(const bin_operation_node_t *node);

void free_unary_operation_node(unary_operation_node_t *node);
void print_unary_operation_node(const unary_operation_node_t *node);

void free_var_assign_node(var_assign_node_t *node);
void print_var_assign_node(const var_assign_node_t *node);

const char *node_labels[9] =
{
    "NONE", "INT", "FLOAT", "IMAG", "BOOL",
    "BIN_OPERATION", "UNARY_OPERATION",
    "VAR_ASSIGN", "VAR_ACCESS"
};

void free_nodes(node_t *nodes, uint64_t size)
{
    if (!size)
    {
        mr_free(nodes);
        return;
    }

    do
        free_node(nodes + --size);
    while (size);
    mr_free(nodes);
}

void print_nodes(const node_t *nodes, uint64_t size)
{
    for (uint64_t i = 0; i < size; i++)
    {
        print_node(nodes + i);
        putchar('\n');
    }
}

void free_node(const node_t *node)
{
    switch (node->type)
    {
    case NONE_N:
    case BOOL_N:
        break;
    case INT_N:
    case FLOAT_N:
    case IMAG_N:
    case VAR_ACCESS_N:
        mr_free(node->value);
        break;
    case BIN_OPERATION_N:
        free_bin_operation_node(node->value);
        break;
    case UNARY_OPERATION_N:
        free_unary_operation_node(node->value);
        break;
    case VAR_ASSIGN_N:
        free_var_assign_node(node->value);
        break;
    }
}

void print_node(const node_t *node)
{
    if (!node->type)
    {
        fputs("(NONE)", stdout);
        return;
    }

    printf("(%s: ", node_labels[node->type]);

    switch (node->type)
    {
    case INT_N:
    case FLOAT_N:
    case IMAG_N:
    case VAR_ACCESS_N:
        fputs(node->value, stdout);
        break;
    case BOOL_N:
        printf("%hu", (uint8_t)(uintptr_t)node->value);
    case BIN_OPERATION_N:
        print_bin_operation_node(node->value);
        break;
    case UNARY_OPERATION_N:
        print_unary_operation_node(node->value);
        break;
    case VAR_ASSIGN_N:
        print_var_assign_node(node->value);
        break;
    }

    putchar(')');
}

void free_bin_operation_node(bin_operation_node_t *node)
{
    free_node(&node->right);
    free_node(&node->left);
    mr_free(node);
}

void print_bin_operation_node(const bin_operation_node_t *node)
{
    printf("%s, ", token_labels[node->operator]);
    print_node(&node->left);
    fputs(", ", stdout);
    print_node(&node->right);
}

void free_unary_operation_node(unary_operation_node_t *node)
{
    free_node(&node->operand);
    mr_free(node);
}

void print_unary_operation_node(const unary_operation_node_t *node)
{
    printf("%s, ", token_labels[node->operator]);
    print_node(&node->operand);
}

void free_var_assign_node(var_assign_node_t *node)
{
    free_node(&node->value);
    mr_free(node->name);
    mr_free(node);
}

void print_var_assign_node(const var_assign_node_t *node)
{
    printf("%s, ", node->name);
    print_node(&node->value);
}
