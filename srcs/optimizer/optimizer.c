/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <optimizer/float.h>
#include <lexer/token.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <consts.h>

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
visit_res_t visit_float(char *node, pos_t *poss, pos_t *pose);
visit_res_t visit_bin_operation(bin_operation_node_t *node, pos_t *poss, pos_t *pose);
visit_res_t visit_unary_operation(unary_operation_node_t *node, pos_t *poss, pos_t *pose);

opt_res_t optimize(node_t *nodes, uint64_t size)
{
    opt_res_t res;
    res.values = mr_alloc(size * sizeof(value_t));

    visit_res_t visit_res;
    for (res.size = 0; res.size < size; res.size++)
    {
        visit_res = visit_node(nodes + res.size);
        if (visit_res.has_error)
        {
            free_values(res.values, res.size);

            for (uint64_t i = res.size + 1; i < size; i++)
                free_node(nodes + i);
            mr_free(nodes);

            res.values = NULL;
            res.error = visit_res.error;
            return res;
        }

        res.values[res.size] = visit_res.value;
    }

    mr_free(nodes);
    return res;
}

visit_res_t visit_node(node_t *node)
{
    switch (node->type)
    {
    case INT_N:
        return visit_int(node->value, &node->poss, &node->pose);
    case FLOAT_N:
        return visit_float(node->value, &node->poss, &node->pose);
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
    res.has_error = 0;
    set_value(INT_V, int_set_str(node));

    mr_free(node);
    return res;
}

visit_res_t visit_float(char *node, pos_t *poss, pos_t *pose)
{
    visit_res_t res;
    res.has_error = 0;
    set_value(FLOAT_V, float_set_str(node));

    mr_free(node);
    return res;
}

visit_res_t visit_bin_operation(bin_operation_node_t *node, pos_t *poss, pos_t *pose)
{
    visit_res_t res = visit_node(&node->left);
    if (res.has_error)
    {
        free_node(&node->right);
        goto ret;
    }

    value_t left = res.value;

    res = visit_node(&node->right);
    if (res.has_error)
    {
        free_value(&left);
        goto ret;
    }

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
    case DIV_T:
        res = compute_div(&left, &right);
        break;
    }

    if (res.has_error)
        goto ret;

    res.value.poss = *poss;
    res.value.pose = *pose;

ret:
    mr_free(node);
    return res;
}

visit_res_t visit_unary_operation(unary_operation_node_t *node, pos_t *poss, pos_t *pose)
{
    visit_res_t res = visit_node(&node->operand);
    if (res.has_error)
        goto ret;

    switch (node->operator)
    {
    case ADD_T:
        res = compute_pos(&res.value, poss);
        break;
    case SUB_T:
        res = compute_neg(&res.value, poss);
        break;
    }

    if (res.has_error)
        goto ret;

    res.value.poss = *poss;
    res.value.pose = *pose;

ret:
    mr_free(node);
    return res;
}
