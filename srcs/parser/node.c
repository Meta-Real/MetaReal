/*
    MetaReal Compiler version 1.0.0
*/

#include <parser/node.h>
#include <lexer/token.h>
#include <alloc.h>
#include <stdio.h>

void node_print(const node_t *node);

void list_node_print(const list_node_t *node);

void bin_operation_node_free(bin_operation_node_t *node);
void bin_operation_node_print(const bin_operation_node_t *node);

void unary_operation_node_free(unary_operation_node_t *node);
void unary_operation_node_print(const unary_operation_node_t *node);

void var_assign_node_free(var_assign_node_t *node);
void var_assign_node_print(const var_assign_node_t *node);

const char *node_labels[10] =
{
    "NONE", "INT", "FLOAT", "IMAG", "BOOL", "LIST",
    "BIN_OPERATION", "UNARY_OPERATION",
    "VAR_ASSIGN", "VAR_ACCESS"
};

void nodes_free(node_t *nodes, uint64_t size)
{
    if (!size)
    {
        mr_free(nodes);
        return;
    }

    do
        node_free(nodes + --size);
    while (size);
    mr_free(nodes);
}

void nodes_print(const node_t *nodes, uint64_t size)
{
    for (uint64_t i = 0; i < size; i++)
    {
        node_print(nodes + i);
        putchar('\n');
    }
}

void node_free(const node_t *node)
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
    case LIST_N:
        list_node_free(node->value);
        break;
    case BIN_OPERATION_N:
        bin_operation_node_free(node->value);
        break;
    case UNARY_OPERATION_N:
        unary_operation_node_free(node->value);
        break;
    case VAR_ASSIGN_N:
        var_assign_node_free(node->value);
        break;
    }
}

void node_print(const node_t *node)
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
        break;
    case LIST_N:
        list_node_print(node->value);
        break;
    case BIN_OPERATION_N:
        bin_operation_node_print(node->value);
        break;
    case UNARY_OPERATION_N:
        unary_operation_node_print(node->value);
        break;
    case VAR_ASSIGN_N:
        var_assign_node_print(node->value);
        break;
    }

    putchar(')');
}

void list_node_free(list_node_t *node)
{
    if (!node)
        return;

    while (node->size)
        node_free(node->elements + --node->size);
    mr_free(node->elements);
    mr_free(node);
}

void list_node_print(const list_node_t *node)
{
    if (!node)
    {
        fputs("NULL", stdout);
        return;
    }

    node_print(node->elements);

    for (uint64_t i = 1; i < node->size; i++)
    {
        fputs(", ", stdout);
        node_print(node->elements + i);
    }
}

void bin_operation_node_free(bin_operation_node_t *node)
{
    node_free(&node->right);
    node_free(&node->left);
    mr_free(node);
}

void bin_operation_node_print(const bin_operation_node_t *node)
{
    printf("%s, ", token_labels[node->operator]);
    node_print(&node->left);
    fputs(", ", stdout);
    node_print(&node->right);
}

void unary_operation_node_free(unary_operation_node_t *node)
{
    node_free(&node->operand);
    mr_free(node);
}

void unary_operation_node_print(const unary_operation_node_t *node)
{
    printf("%s, ", token_labels[node->operator]);
    node_print(&node->operand);
}

void var_assign_node_free(var_assign_node_t *node)
{
    node_free(&node->value);
    mr_free(node->name);
    mr_free(node);
}

void var_assign_node_print(const var_assign_node_t *node)
{
    printf("%s, ", node->name);
    node_print(&node->value);
}
