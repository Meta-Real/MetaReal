/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <lexer/token.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>

#define set_value(t, v)         \
    do                          \
    {                           \
        res.value.type = t;     \
        res.value.value = v;    \
        res.value.poss = *poss; \
        res.value.pose = *pose; \
    } while (0)

visit_res_t visit_node(node_t *node);

visit_res_t visit_int(char *node, pos_t *poss, pos_t *pose);
visit_res_t visit_bin_operation(bin_operation_node_t *node, pos_t *poss, pos_t *pose);
visit_res_t visit_unary_operation(unary_operation_node_t *node, pos_t *poss, pos_t *pose);

opt_res_t optimize(node_t *nodes, uint64_t size)
{
    opt_res_t res;
    res.values = mr_alloc(sizeof(value_t));
    res.size = 0;

    visit_res_t visit_res;
    do
    {
        visit_res = visit_node(nodes + res.size);
        res.values[res.size++] = visit_res.value;
    } while (0);

    mr_free(nodes);

    res.values = mr_realloc(res.values, res.size * sizeof(value_t));
    return res;
}

visit_res_t visit_node(node_t *node)
{
    switch (node->type)
    {
    case INT_N:
        return visit_int(node->value, &node->poss, &node->pose);
    case BIN_OPERATION_N:
        return visit_bin_operation(node->value, &node->poss, &node->pose);
    case UNARY_OPERATION_N:
        return visit_unary_operation(node->value, &node->poss, &node->pose);
    }

    fprintf(stderr, "Internal Error: Invalid node type #%hu (visit function)\n", node->type);
    abort();
}

visit_res_t visit_int(char *node, pos_t *poss, pos_t *pose)
{
    visit_res_t res;

    int_value_t *value = mr_alloc(sizeof(int_value_t));
    mpz_init_set_str(value->num, node, 10);

    set_value(INT_V, value);

    mr_free(node);
    return res;
}

visit_res_t visit_bin_operation(bin_operation_node_t *node, pos_t *poss, pos_t *pose)
{
    visit_res_t res = visit_node(&node->left);
    value_t left = res.value;

    res = visit_node(&node->right);
    value_t right = res.value;

    switch (node->operator)
    {
    case ADD_T:
        res = compute_add(&left, &right);
        break;
    case SUB_T:
        res = compute_sub(&left, &right);
        break;
    case MUL_T:
        res = compute_mul(&left, &right);
        break;
    }

    res.value.poss = *poss;
    res.value.pose = *pose;

    mr_free(node);
    return res;
}

visit_res_t visit_unary_operation(unary_operation_node_t *node, pos_t *poss, pos_t *pose)
{
    visit_res_t res = visit_node(&node->operand);

    switch (node->operator)
    {
    case ADD_T:
        res = compute_pos(&res.value);
        break;
    case SUB_T:
        res = compute_neg(&res.value);
        break;
    }

    res.value.poss = *poss;
    res.value.pose = *pose;

    mr_free(node);
    return res;
}
