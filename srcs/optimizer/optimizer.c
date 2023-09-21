/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/context.h>
#include <optimizer/operation.h>
#include <optimizer/complex.h>
#include <optimizer/list.h>
#include <lexer/token.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <consts.h>

#define set_value(t, v)         \
    do                          \
    {                           \
        res.value.type = t;     \
        res.value.value = v;    \
        res.value.poss = *poss; \
        res.value.pose = *pose; \
    } while (0)

visit_res_t visit_node(node_t *node, context_t *context);

visit_res_t visit_int(char *node, pos_t *poss, pos_t *pose);
visit_res_t visit_float(char *node, pos_t *poss, pos_t *pose);
visit_res_t visit_imag(char *node, pos_t *poss, pos_t *pose);
visit_res_t visit_bool(void *node, pos_t *poss, pos_t *pose);
visit_res_t visit_list(list_node_t *node, context_t *context, pos_t *poss, pos_t *pose);
visit_res_t visit_bin_operation(bin_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose);
visit_res_t visit_unary_operation(unary_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose);
visit_res_t visit_var_assign(var_assign_node_t *node, context_t *context, pos_t *poss, pos_t *pose);
visit_res_t visit_var_access(char *node, context_t *context, pos_t *poss, pos_t *pose);

opt_res_t optimize(node_t *nodes, uint64_t size)
{
    opt_res_t res;
    res.values = mr_alloc(size * sizeof(value_t));

    context_t context;
    context.vars = mr_alloc(OPT_VARS_LIST_LEN * sizeof(var_t));
    context.size = 0;
    context.alloc = OPT_VARS_LIST_LEN;

    visit_res_t visit_res;
    for (res.size = 0; res.size < size; res.size++)
    {
        visit_res = visit_node(nodes + res.size, &context);
        if (visit_res.has_error)
        {
            context_free(&context);
            values_free(res.values, res.size);

            for (uint64_t i = res.size + 1; i < size; i++)
                node_free(nodes + i);
            mr_free(nodes);

            res.values = NULL;
            res.error = visit_res.error;
            return res;
        }

        res.values[res.size] = visit_res.value;
    }

    context_free(&context);

    if (res.size)
        res.values = mr_realloc(res.values, res.size * sizeof(value_t));
    else
        mr_free(res.values);

    mr_free(nodes);
    return res;
}

visit_res_t visit_node(node_t *node, context_t *context)
{
    switch (node->type)
    {
    case INT_N:
        return visit_int(node->value, &node->poss, &node->pose);
    case FLOAT_N:
        return visit_float(node->value, &node->poss, &node->pose);
    case IMAG_N:
        return visit_imag(node->value, &node->poss, &node->pose);
    case BOOL_N:
        return visit_bool(node->value, &node->poss, &node->pose);
    case LIST_N:
        return visit_list(node->value, context, &node->poss, &node->pose);
    case BIN_OPERATION_N:
        return visit_bin_operation(node->value, context, &node->poss, &node->pose);
    case UNARY_OPERATION_N:
        return visit_unary_operation(node->value, context, &node->poss, &node->pose);
    case VAR_ASSIGN_N:
        return visit_var_assign(node->value, context, &node->poss, &node->pose);
    case VAR_ACCESS_N:
        return visit_var_access(node->value, context, &node->poss, &node->pose);
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

visit_res_t visit_imag(char *node, pos_t *poss, pos_t *pose)
{
    visit_res_t res;
    res.has_error = 0;
    set_value(COMPLEX_V, complex_set_str(node));

    mr_free(node);
    return res;
}

visit_res_t visit_bool(void *node, pos_t *poss, pos_t *pose)
{
    visit_res_t res;
    res.has_error = 0;
    set_value(BOOL_V, node);

    return res;
}

visit_res_t visit_list(list_node_t *node, context_t *context, pos_t *poss, pos_t *pose)
{
    visit_res_t res;
    res.has_error = 0;

    if (!node)
    {
        set_value(LIST_V, NULL);
        return res;
    }

    list_value_t *value = list_set(node->size);
    for (uint64_t i = 0; i < node->size; i++)
    {
        res = visit_node(node->elements + i, context);
        if (res.has_error)
        {
            i++;
            while (node->size > i)
                node_free(node->elements + node->size);
            i--;

            while (i)
                value_free(value->elements + --i);
            mr_free(value->elements);
            mr_free(value);

            goto ret;
        }

        value->elements[i] = res.value;
    }

    set_value(LIST_V, value);

ret:
    mr_free(node->elements);
    mr_free(node);
    return res;
}

visit_res_t visit_bin_operation(bin_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose)
{
    visit_res_t res = visit_node(&node->left, context);
    if (res.has_error)
    {
        node_free(&node->right);
        goto ret;
    }

    if (node->operator == AND_T || node->operator == AND_KT)
    {
        if (value_isfalse(&res.value))
        {
            res.value.type = BOOL_V;
            res.value.value = NULL;

            node_free(&node->right);
            goto rets;
        }

        res = visit_node(&node->right, context);
        if (res.has_error)
            goto ret;

        res.value.value = (void*)(uintptr_t)value_istrue(&res.value);
        res.value.type = BOOL_V;
        goto rets;
    }
    if (node->operator == OR_T || node->operator == OR_KT)
    {
        if (value_istrue(&res.value))
        {
            res.value.type = BOOL_V;
            res.value.value = (void*)1;

            node_free(&node->right);
            goto rets;
        }

        res = visit_node(&node->right, context);
        if (res.has_error)
            goto ret;

        res.value.value = (void*)(uintptr_t)value_istrue(&res.value);
        res.value.type = BOOL_V;
        goto rets;
    }

    value_t left = res.value;

    res = visit_node(&node->right, context);
    if (res.has_error)
    {
        value_free(&left);
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
    case MOD_T:
        res = compute_mod(&left, &right);
        break;
    case QUOT_T:
        res = compute_quot(&left, &right);
        break;
    case POW_T:
        res = compute_pow(&left, &right);
        break;
    case B_AND_T:
        res = compute_b_and(&left, &right);
        break;
    case B_OR_T:
        res = compute_b_or(&left, &right);
        break;
    case B_XOR_T:
        res = compute_b_xor(&left, &right);
        break;
    case LSHIFT_T:
        res = compute_lshift(&left, &right);
        break;
    case RSHIFT_T:
        res = compute_rshift(&left, &right);
        break;
    case EQ_T:
        res = compute_eq(&left, &right);
        break;
    case NEQ_T:
        res = compute_neq(&left, &right);
        break;
    case EX_EQ_T:
        res = compute_ex_eq(&left, &right);
        break;
    case EX_NEQ_T:
        res = compute_ex_neq(&left, &right);
        break;
    case LT_T:
        res = compute_lt(&left, &right);
        break;
    case GT_T:
        res = compute_gt(&left, &right);
        break;
    case LTE_T:
        res = compute_lte(&left, &right);
        break;
    case GTE_T:
        res = compute_gte(&left, &right);
        break;
    }

    if (res.has_error)
        goto ret;

rets:
    res.value.poss = *poss;
    res.value.pose = *pose;

ret:
    mr_free(node);
    return res;
}

visit_res_t visit_unary_operation(unary_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose)
{
    visit_res_t res = visit_node(&node->operand, context);
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
    case B_NOT_T:
        res = compute_b_not(&res.value, poss);
        break;
    case NOT_T:
    case NOT_KT:
        res = compute_not(&res.value, poss);
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

visit_res_t visit_var_assign(var_assign_node_t *node, context_t *context, pos_t *poss, pos_t *pose)
{
    visit_res_t res;
    res.has_error = 0;

    if (node->value.type)
    {
        res = visit_node(&node->value, context);
        if (res.has_error)
        {
            mr_free(node->name);
            goto ret;
        }

        value_addref(&res.value);
    }
    else
        res.value.type = NONE_V;

    var_set(context, node->name, &res.value);

    res.value.poss = *poss;
    res.value.pose = *pose;

ret:
    mr_free(node);
    return res;
}

visit_res_t visit_var_access(char *node, context_t *context, pos_t *poss, pos_t *pose)
{
    visit_res_t res;
    res.has_error = 0;
    res.value = var_get(&res.has_error, context, node);

    if (res.has_error)
    {
        char *detail = mr_alloc(18 + strlen(node));
        sprintf(detail, "'%s' is not defined", node);

        res.error = set_invalid_semantic(detail, NOT_DEF_E, *poss, *pose);
        goto ret;
    }

    res.value.poss = *poss;
    res.value.pose = *pose;

ret:
    mr_free(node);
    return res;
}
