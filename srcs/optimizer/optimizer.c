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

#define value_set_pos(t, v)         \
    do                              \
    {                               \
        value_set(res.value, t, v); \
        res.value->poss = *poss;    \
        res.value->pose = *pose;    \
    } while (0)

#define access_datatype(t)                                                           \
    do                                                                               \
    {                                                                                \
        char *detail = mr_alloc(39 + value_name_lens[t]);                            \
        sprintf(detail, "<%s> can not be accessed like a variable", value_names[t]); \
                                                                                     \
        res.value = NULL;                                                           \
        res.error = invalid_semantic_set(detail, ACCESS_E, *poss, *pose);            \
        return res;                                                                  \
    } while (0)

#define access_statement(s, sl)                                           \
    do                                                                    \
    {                                                                     \
        char *detail = mr_alloc(37 + sl);                                 \
        sprintf(detail, "%s can not be accessed like a variable", s);     \
                                                                          \
        res.value = NULL;                                                \
        res.error = invalid_semantic_set(detail, ACCESS_E, *poss, *pose); \
        return res;                                                       \
    } while (0)

visit_res_t visit_node(node_t *node, context_t *context, uint8_t prop);

visit_res_t visit_int(char *node, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_float(char *node, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_imag(char *node, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_bool(void *node, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_list(list_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_tuple(list_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_bin_operation(bin_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_unary_operation(unary_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_var_assign(var_assign_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_var_modify(bin_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_var_fmodify(unary_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop);
visit_res_t visit_var_access(char *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop);

opt_res_t optimize(node_t *nodes, uint64_t size)
{
    opt_res_t res;
    res.values = mr_alloc(size * sizeof(value_t*));

    context_t context;
    context.vars = mr_alloc(OPT_VARS_LIST_LEN * sizeof(var_t));
    context.size = 0;
    context.alloc = OPT_VARS_LIST_LEN;

    visit_res_t visit_res;
    for (res.size = 0; res.size < size; res.size++)
    {
        visit_res = visit_node(nodes + res.size, &context, 0);
        if (!visit_res.value)
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
        res.values = mr_realloc(res.values, res.size * sizeof(value_t*));
    else
        mr_free(res.values);

    mr_free(nodes);
    return res;
}

visit_res_t visit_node(node_t *node, context_t *context, uint8_t prop)
{
    switch (node->type)
    {
    case INT_N:
        return visit_int(node->value, &node->poss, &node->pose, prop);
    case FLOAT_N:
        return visit_float(node->value, &node->poss, &node->pose, prop);
    case IMAG_N:
        return visit_imag(node->value, &node->poss, &node->pose, prop);
    case BOOL_N:
        return visit_bool(node->value, &node->poss, &node->pose, prop);
    case LIST_N:
        return visit_list(node->value, context, &node->poss, &node->pose, prop);
    case TUPLE_N:
        return visit_tuple(node->value, context, &node->poss, &node->pose, prop);
    case BIN_OPERATION_N:
        return visit_bin_operation(node->value, context, &node->poss, &node->pose, prop);
    case UNARY_OPERATION_N:
        return visit_unary_operation(node->value, context, &node->poss, &node->pose, prop);
    case VAR_ASSIGN_N:
        return visit_var_assign(node->value, context, &node->poss, &node->pose, prop);
    case VAR_MODIFY_N:
        return visit_var_modify(node->value, context, &node->poss, &node->pose, prop);
    case VAR_FMODIFY_N:
        return visit_var_fmodify(node->value, context, &node->poss, &node->pose, prop);
    case VAR_ACCESS_N:
        return visit_var_access(node->value, context, &node->poss, &node->pose, prop);
    }

    fprintf(stderr, "Internal Error: Invalid node type #%hu (visit function)\n", node->type);
    abort();
}

visit_res_t visit_int(char *node, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    if (prop)
    {
        mr_free(node);
        access_datatype(INT_V);
    }

    value_set_pos(INT_V, int_set_str(node));

    mr_free(node);
    return res;
}

visit_res_t visit_float(char *node, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    if (prop)
    {
        mr_free(node);
        access_datatype(FLOAT_V);
    }

    value_set_pos(FLOAT_V, float_set_str(node));

    mr_free(node);
    return res;
}

visit_res_t visit_imag(char *node, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    if (prop)
    {
        mr_free(node);
        access_datatype(COMPLEX_V);
    }

    value_set_pos(COMPLEX_V, complex_set_str(node));

    mr_free(node);
    return res;
}

visit_res_t visit_bool(void *node, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    if (prop)
        access_datatype(BOOL_V);

    value_set_pos(BOOL_V, node);
    return res;
}

visit_res_t visit_list(list_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    if (prop)
    {
        list_node_free(node);
        access_datatype(LIST_V);
    }

    if (!node)
    {
        value_set_pos(LIST_V, NULL);
        return res;
    }

    list_value_t *value = list_set(node->size);
    for (uint64_t i = 0; i < node->size; i++)
    {
        res = visit_node(node->elements + i, context, 0);
        if (!res.value)
        {
            i++;
            while (node->size > i)
                node_free(node->elements + node->size);
            i--;

            while (i)
                value_free(value->elements[--i]);
            mr_free(value->elements);
            mr_free(value);

            mr_free(node->elements);
            mr_free(node);
            return res;
        }

        value->elements[i] = res.value;
    }

    value_set_pos(LIST_V, value);

    mr_free(node->elements);
    mr_free(node);
    return res;
}

visit_res_t visit_tuple(list_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    if (prop)
    {
        list_node_free(node);
        access_datatype(TUPLE_V);
    }

    list_value_t *value = list_set(node->size);
    for (uint64_t i = 0; i < node->size; i++)
    {
        res = visit_node(node->elements + i, context, 0);
        if (!res.value)
        {
            i++;
            while (node->size > i)
                node_free(node->elements + node->size);
            i--;

            while (i)
                value_free(value->elements[--i]);
            mr_free(value->elements);
            mr_free(value);

            mr_free(node->elements);
            mr_free(node);
            return res;
        }

        value->elements[i] = res.value;
    }

    value_set_pos(TUPLE_V, value);

    mr_free(node->elements);
    mr_free(node);
    return res;
}

visit_res_t visit_bin_operation(bin_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    if (prop)
        access_statement("Binary operation", 16);

    res = visit_node(&node->left, context, 0);
    if (!res.value)
    {
        node_free(&node->right);
        mr_free(node);
        return res;
    }

    if (node->operator == AND_KT)
    {
        if (value_isfalse(res.value))
        {
            value_set_pos(BOOL_V, NULL);

            node_free(&node->right);
            mr_free(node);
            return res;
        }

        res = visit_node(&node->right, context, 0);
        if (!res.value)
        {
            mr_free(node);
            return res;
        }

        value_set_pos(BOOL_V, (void*)(uintptr_t)value_istrue(res.value));

        mr_free(node);
        return res;
    }
    if (node->operator == OR_KT)
    {
        if (value_istrue(res.value))
        {
            value_set_pos(BOOL_V, (void*)1);

            node_free(&node->right);
            mr_free(node);
            return res;
        }

        res = visit_node(&node->right, context, 0);
        if (!res.value)
        {
            mr_free(node);
            return res;
        }

        value_set_pos(BOOL_V, (void*)(uintptr_t)value_istrue(res.value));

        mr_free(node);
        return res;
    }

    value_t *left = res.value;

    res = visit_node(&node->right, context, 0);
    if (!res.value)
    {
        value_free(left);
        mr_free(node);
        return res;
    }

    value_t *right = res.value;

    switch (node->operator)
    {
    case ADD_T:
        res = compute_add(left, right);
        break;
    case SUB_T:
        res = compute_sub(left, right);
        break;
    case MUL_T:
        res = compute_mul(left, right);
        break;
    case DIV_T:
        res = compute_div(left, right);
        break;
    case MOD_T:
        res = compute_mod(left, right);
        break;
    case QUOT_T:
        res = compute_quot(left, right);
        break;
    case POW_T:
        res = compute_pow(left, right);
        break;
    case B_AND_T:
        res = compute_b_and(left, right);
        break;
    case B_OR_T:
        res = compute_b_or(left, right);
        break;
    case B_XOR_T:
        res = compute_b_xor(left, right);
        break;
    case LSHIFT_T:
        res = compute_lshift(left, right);
        break;
    case RSHIFT_T:
        res = compute_rshift(left, right);
        break;
    case EQ_T:
        res = compute_eq(left, right);
        break;
    case NEQ_T:
        res = compute_neq(left, right);
        break;
    case EX_EQ_T:
        res = compute_ex_eq(left, right);
        break;
    case EX_NEQ_T:
        res = compute_ex_neq(left, right);
        break;
    case LT_T:
        res = compute_lt(left, right);
        break;
    case GT_T:
        res = compute_gt(left, right);
        break;
    case LTE_T:
        res = compute_lte(left, right);
        break;
    case GTE_T:
        res = compute_gte(left, right);
        break;
    }

    if (!res.value)
    {
        mr_free(node);
        return res;
    }

    res.value->poss = *poss;
    res.value->pose = *pose;

    mr_free(node);
    return res;
}

visit_res_t visit_unary_operation(unary_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    if (prop)
        access_statement("Unary operation", 15);

    res = visit_node(&node->operand, context, 0);
    if (!res.value)
    {
        mr_free(node);
        return res;
    }

    switch (node->operator)
    {
    case ADD_T:
        res = compute_pos(res.value, poss);
        break;
    case SUB_T:
        res = compute_neg(res.value, poss);
        break;
    case B_NOT_T:
        res = compute_b_not(res.value, poss);
        break;
    case NOT_KT:
        res = compute_not(res.value, poss);
        break;
    }

    if (!res.value)
    {
        mr_free(node);
        return res;
    }

    res.value->poss = *poss;
    res.value->pose = *pose;

    mr_free(node);
    return res;
}

visit_res_t visit_var_assign(var_assign_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;

    if (node->value.type)
    {
        res = visit_node(&node->value, context, 0);
        if (!res.value)
        {
            mr_free(node->name);
            mr_free(node);
            return res;
        }

        value_addref(res.value, 1);
    }
    else
        value_set_vo(res.value, NONE_V);

    if (prop)
    {
        void* ptr = (void*)var_setp(context, node->name, res.value);
        value_set(res.value, PTR_V, ptr);
    }
    else
    {
        var_set(context, node->name, res.value);
        value_addref(res.value, 1);

        res.value->poss = *poss;
        res.value->pose = *pose;
    }

    mr_free(node);
    return res;
}

visit_res_t visit_var_modify(bin_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;
    uint64_t ptr;

    if (node->operator == ASSIGN_T)
    {
        res = visit_node(&node->left, context, 3);
        if (!res.value)
        {
            node_free(&node->right);
            mr_free(node);
            return res;
        }

        ptr = (uintptr_t)res.value->value;

        mr_free(res.value);
        res = visit_node(&node->right, context, 0);
        if (!res.value)
        {
            mr_free(node);
            return res;
        }

        if (context->vars[ptr].value)
            value_free(context->vars[ptr].value);

        context->vars[ptr].value = res.value;
        if (prop)
            value_set(res.value, PTR_V, (void*)ptr);
        else
        {
            value_addref(res.value, 1);
            res.value->poss = *poss;
            res.value->pose = *pose;
        }

        mr_free(node);
        return res;
    }
    if (node->operator == LINK_T)
    {
        res = visit_node(&node->left, context, 3);
        if (!res.value)
        {
            node_free(&node->right);
            mr_free(node);
            return res;
        }

        ptr = (uintptr_t)res.value->value;

        mr_free(res.value);
        res = visit_node(&node->right, context, 1);
        if (!res.value)
        {
            mr_free(node);
            return res;
        }

        if (context->vars[ptr].value)
            value_free(context->vars[ptr].value);

        context->vars[ptr].value = res.value;
        if (!prop)
        {
            res.value = context->vars[(uintptr_t)res.value->value].value;
            value_addref(res.value, 1);

            res.value->poss = *poss;
            res.value->pose = *pose;
        }

        mr_free(node);
        return res;
    }

    res = visit_node(&node->left, context, 1);
    if (!res.value)
    {
        node_free(&node->right);
        mr_free(node);
        return res;
    }

    ptr = (uintptr_t)res.value->value;

    mr_free(res.value);
    res = visit_node(&node->right, context, 0);
    if (!res.value)
    {
        mr_free(node);
        return res;
    }

    switch (node->operator)
    {
    case ADD_EQ_T:
        res = compute_add(context->vars[ptr].value, res.value);
        break;
    case SUB_EQ_T:
        res = compute_sub(context->vars[ptr].value, res.value);
        break;
    case MUL_EQ_T:
        res = compute_mul(context->vars[ptr].value, res.value);
        break;
    case DIV_EQ_T:
        res = compute_div(context->vars[ptr].value, res.value);
        break;
    case MOD_EQ_T:
        res = compute_mod(context->vars[ptr].value, res.value);
        break;
    case QUOT_EQ_T:
        res = compute_quot(context->vars[ptr].value, res.value);
        break;
    case POW_EQ_T:
        res = compute_pow(context->vars[ptr].value, res.value);
        break;
    case B_AND_EQ_T:
        res = compute_b_and(context->vars[ptr].value, res.value);
        break;
    case B_OR_EQ_T:
        res = compute_b_or(context->vars[ptr].value, res.value);
        break;
    case B_XOR_EQ_T:
        res = compute_b_xor(context->vars[ptr].value, res.value);
        break;
    case LSHIFT_EQ_T:
        res = compute_lshift(context->vars[ptr].value, res.value);
        break;
    case RSHIFT_EQ_T:
        res = compute_rshift(context->vars[ptr].value, res.value);
        break;
    }

    if (!res.value)
    {
        context->vars[ptr].value = NULL;

        mr_free(node);
        return res;
    }

    context->vars[ptr].value = res.value;

    if (prop)
        value_set(res.value, PTR_V, (void*)ptr);
    else
    {
        value_addref(res.value, 1);
        res.value->poss = *poss;
        res.value->pose = *pose;
    }

    mr_free(node);
    return res;
}

visit_res_t visit_var_fmodify(unary_operation_node_t *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res = visit_node(&node->operand, context, 1);
    if (!res.value)
    {
        mr_free(node);
        return res;
    }

    uint64_t ptr = (uintptr_t)res.value->value;
    mr_free(res.value);

    if (node->operator == ADD_T)
    {
        if (prop)
        {
            res = compute_inc(context->vars[ptr].value, poss, pose);
            if (!res.value)
            {
                mr_free(node);
                return res;
            }

            context->vars[ptr].value = res.value;
            value_set(res.value, PTR_V, (void*)ptr);

            mr_free(node);
            return res;
        }

        value_t *old = context->vars[ptr].value;
        value_addref(old, 1);

        res = compute_inc(old, poss, pose);
        if (!res.value)
        {
            old->ref--;
            mr_free(node);
            return res;
        }

        context->vars[ptr].value = res.value;

        old->poss = *poss;
        old->pose = *pose;
        res.value = old;

        mr_free(node);
        return res;
    }
    if (node->operator == SUB_T)
    {
        if (prop)
        {
            res = compute_dec(context->vars[ptr].value, poss, pose);
            if (!res.value)
            {
                mr_free(node);
                return res;
            }

            context->vars[ptr].value = res.value;
            value_set(res.value, PTR_V, (void*)ptr);

            mr_free(node);
            return res;
        }

        value_t *old = context->vars[ptr].value;
        value_addref(old, 1);

        res = compute_dec(old, poss, pose);
        if (!res.value)
        {
            old->ref--;
            mr_free(node);
            return res;
        }

        context->vars[ptr].value = res.value;

        old->poss = *poss;
        old->pose = *pose;
        res.value = old;

        mr_free(node);
        return res;
    }

    if (node->operator == INC_T)
        res = compute_inc(context->vars[ptr].value, poss, pose);
    else if (node->operator == DEC_T)
        res = compute_dec(context->vars[ptr].value, poss, pose);

    if (!res.value)
    {
        mr_free(node);
        return res;
    }

    context->vars[ptr].value = res.value;
    if (prop)
        value_set(res.value, PTR_V, (void*)ptr);
    else
    {
        value_addref(res.value, 1);
        res.value->poss = *poss;
        res.value->pose = *pose;
    }

    mr_free(node);
    return res;
}

visit_res_t visit_var_access(char *node, context_t *context, pos_t *poss, pos_t *pose, uint8_t prop)
{
    visit_res_t res;

    if (prop)
    {
        uint8_t error = 0;
        void *ptr = (void*)var_getp(&error, context, node);

        if (error)
        {
            if (prop & 2)
            {
                ptr = (void*)var_add(context, node);
                value_set(res.value, PTR_V, ptr);
                return res;
            }

            char *detail = mr_alloc(18 + strlen(node));
            sprintf(detail, "'%s' is not defined", node);

            res.value = NULL;
            res.error = invalid_semantic_set(detail, NOT_DEF_E, *poss, *pose);

            mr_free(node);
            return res;
        }

        value_set(res.value, PTR_V, ptr);
    }
    else
    {
        res.value = var_get(context, node);
        if (!res.value)
        {
            char *detail = mr_alloc(18 + strlen(node));
            sprintf(detail, "'%s' is not defined", node);

            res.value = NULL;
            res.error = invalid_semantic_set(detail, NOT_DEF_E, *poss, *pose);

            mr_free(node);
            return res;
        }

        res.value->poss = *poss;
        res.value->pose = *pose;
    }

    mr_free(node);
    return res;
}
