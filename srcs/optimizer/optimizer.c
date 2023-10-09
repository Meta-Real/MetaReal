/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/context.h>
#include <optimizer/operation.h>
#include <optimizer/complex.h>
#include <optimizer/list.h>
#include <error/semantic.h>
#include <lexer/token.h>
#include <stdlib.h>
#include <consts.h>

#define PROP_SET(ptr, assign) 0b ## assign ## ptr 

#define PTR_MASK 1
#define ASSIGN_MASK 2

#define value_set_pos(t, v)          \
    do                               \
    {                                \
        value_set(res->value, t, v); \
        res->value->poss = *poss;    \
        res->value->pose = *pose;    \
    } while (0)

void visit_node(visit_res_t *res, node_t *node, context_t *context, uint8_t prop);

void visit_int(
    visit_res_t *res, char *node,
    pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_float(
    visit_res_t *res, char *node,
    pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_imag(
    visit_res_t *res, char *node,
    pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_bool(
    visit_res_t *res, void *node,
    pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_list(
    visit_res_t *res, list_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_tuple(
    visit_res_t *res, list_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_bin_operation(
    visit_res_t *res, bin_operation_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_unary_operation(
    visit_res_t *res, unary_operation_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_var_assign(
    visit_res_t *res, var_assign_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_var_modify(
    visit_res_t *res, bin_operation_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_var_fmodify(
    visit_res_t *res, unary_operation_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
);
void visit_var_access(
    visit_res_t *res, char *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
);

void map_array(visit_res_t *res, context_t *context, list_value_t *ptrs, value_t *values);
void link_array(visit_res_t *res, context_t *context, list_value_t *ptrs, value_t *values);

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
        visit_node(&visit_res, nodes + res.size, &context, 0);
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

void visit_node(visit_res_t *res, node_t *node, context_t *context, uint8_t prop)
{
    switch (node->type)
    {
    case INT_N:
        visit_int(res, node->value, &node->poss, &node->pose, prop);
        return;
    case FLOAT_N:
        visit_float(res, node->value, &node->poss, &node->pose, prop);
        return;
    case IMAG_N:
        visit_imag(res, node->value, &node->poss, &node->pose, prop);
        return;
    case BOOL_N:
        visit_bool(res, node->value, &node->poss, &node->pose, prop);
        return;
    case LIST_N:
        visit_list(res, node->value, context, &node->poss, &node->pose, prop);
        return;
    case TUPLE_N:
        visit_tuple(res, node->value, context, &node->poss, &node->pose, prop);
        return;
    case BIN_OPERATION_N:
        visit_bin_operation(res, node->value, context, &node->poss, &node->pose, prop);
        return;
    case UNARY_OPERATION_N:
        visit_unary_operation(res, node->value, context, &node->poss, &node->pose, prop);
        return;
    case VAR_ASSIGN_N:
        visit_var_assign(res, node->value, context, &node->poss, &node->pose, prop);
        return;
    case VAR_MODIFY_N:
        visit_var_modify(res, node->value, context, &node->poss, &node->pose, prop);
        return;
    case VAR_FMODIFY_N:
        visit_var_fmodify(res, node->value, context, &node->poss, &node->pose, prop);
        return;
    case VAR_ACCESS_N:
        visit_var_access(res, node->value, context, &node->poss, &node->pose, prop);
        return;
    }

    fprintf(stderr, "Internal Error: Invalid node type #%hu (visit function)\n", node->type);
    abort();
}

void visit_int(
    visit_res_t *res, char *node,
    pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (prop & PTR_MASK)
    {
        mr_free(node);
        access_datatype_error(INT_V, *poss, *pose);
    }

    value_set_pos(INT_V, int_set_str(node));
    mr_free(node);
}

void visit_float(
    visit_res_t *res, char *node,
    pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (prop & PTR_MASK)
    {
        mr_free(node);
        access_datatype_error(FLOAT_V, *poss, *pose);
    }

    value_set_pos(FLOAT_V, float_set_str(node));
    mr_free(node);
}

void visit_imag(
    visit_res_t *res, char *node,
    pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (prop & PTR_MASK)
    {
        mr_free(node);
        access_datatype_error(COMPLEX_V, *poss, *pose);
    }

    value_set_pos(COMPLEX_V, complex_set_str(node));
    mr_free(node);
}

void visit_bool(
    visit_res_t *res, void *node,
    pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (prop & PTR_MASK)
        access_datatype_error(BOOL_V, *poss, *pose);

    value_set_pos(BOOL_V, node);
}

void visit_list(
    visit_res_t *res, list_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (!node)
    {
        if (prop & PTR_MASK)
            value_set_pos(PTRS_V, NULL);
        else
            value_set_pos(LIST_V, NULL);
        return;
    }

    list_value_t *value = mr_alloc(sizeof(list_value_t));
    value->elements = mr_alloc(node->size * sizeof(value_t*));

    for (value->size = 0; value->size < node->size; value->size++)
    {
        visit_node(res, node->elements + value->size, context, prop);
        if (!res->value)
        {
            value->size++;
            while (node->size > value->size)
                node_free(node->elements + --node->size);
            value->size--;

            list_free(value);
            mr_free(node->elements);
            mr_free(node);
            return;
        }

        value->elements[value->size] = res->value;
    }

    if (prop & PTR_MASK)
        value_set_pos(PTRS_V, value);
    else
        value_set_pos(LIST_V, value);

    mr_free(node->elements);
    mr_free(node);
}

void visit_tuple(
    visit_res_t *res, list_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
)
{
    list_value_t *value = mr_alloc(sizeof(list_value_t));
    value->elements = mr_alloc(node->size * sizeof(value_t*));

    for (value->size = 0; value->size < node->size; value->size++)
    {
        visit_node(res, node->elements + value->size, context, prop);
        if (!res->value)
        {
            value->size++;
            while (node->size > value->size)
                node_free(node->elements + --node->size);
            value->size--;

            list_free(value);
            mr_free(node->elements);
            mr_free(node);
            return;
        }

        value->elements[value->size] = res->value;
    }

    if (prop & PTR_MASK)
        value_set_pos(PTRS_V, value);
    else
        value_set_pos(TUPLE_V, value);

    mr_free(node->elements);
    mr_free(node);
}

void visit_bin_operation(
    visit_res_t *res, bin_operation_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (prop & PTR_MASK)
    {
        node_free(&node->right);
        node_free(&node->left);
        mr_free(node);

        access_statement_error("Binary operation", 16, *poss, *pose);
    }

    visit_node(res, &node->left, context, 0);
    if (!res->value)
    {
        node_free(&node->right);
        mr_free(node);
        return;
    }

    if (node->operator == AND_KT)
    {
        if (value_isfalse(res->value))
        {
            value_set_pos(BOOL_V, NULL);

            node_free(&node->right);
            mr_free(node);
            return;
        }

        visit_node(res, &node->right, context, 0);
        if (!res->value)
        {
            mr_free(node);
            return;
        }

        value_set_pos(BOOL_V, (void*)(uintptr_t)value_istrue(res->value));

        mr_free(node);
        return;
    }
    if (node->operator == OR_KT)
    {
        if (value_istrue(res->value))
        {
            value_set_pos(BOOL_V, (void*)1);

            node_free(&node->right);
            mr_free(node);
            return;
        }

        visit_node(res, &node->right, context, 0);
        if (!res->value)
        {
            mr_free(node);
            return;
        }

        value_set_pos(BOOL_V, (void*)(uintptr_t)value_istrue(res->value));

        mr_free(node);
        return;
    }

    value_t *left = res->value;

    visit_node(res, &node->right, context, 0);
    if (!res->value)
    {
        value_free(left);
        mr_free(node);
        return;
    }

    switch (node->operator)
    {
    case ADD_T:
        compute_add(res, left, res->value);
        break;
    case SUB_T:
        compute_sub(res, left, res->value);
        break;
    case MUL_T:
        compute_mul(res, left, res->value);
        break;
    case DIV_T:
        compute_div(res, left, res->value);
        break;
    case MOD_T:
        compute_mod(res, left, res->value);
        break;
    case QUOT_T:
        compute_quot(res, left, res->value);
        break;
    case POW_T:
        compute_pow(res, left, res->value);
        break;
    case B_AND_T:
        compute_b_and(res, left, res->value);
        break;
    case B_OR_T:
        compute_b_or(res, left, res->value);
        break;
    case B_XOR_T:
        compute_b_xor(res, left, res->value);
        break;
    case LSHIFT_T:
        compute_lshift(res, left, res->value);
        break;
    case RSHIFT_T:
        compute_rshift(res, left, res->value);
        break;
    case EQ_T:
        compute_eq(res, left, res->value);
        break;
    case NEQ_T:
        compute_neq(res, left, res->value);
        break;
    case EX_EQ_T:
        compute_ex_eq(res, left, res->value);
        break;
    case EX_NEQ_T:
        compute_ex_neq(res, left, res->value);
        break;
    case LT_T:
        compute_lt(res, left, res->value);
        break;
    case GT_T:
        compute_gt(res, left, res->value);
        break;
    case LTE_T:
        compute_lte(res, left, res->value);
        break;
    case GTE_T:
        compute_gte(res, left, res->value);
        break;
    }

    if (!res->value)
    {
        mr_free(node);
        return;
    }

    res->value->poss = *poss;
    res->value->pose = *pose;

    mr_free(node);
}

void visit_unary_operation(
    visit_res_t *res, unary_operation_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (prop & PTR_MASK)
    {
        node_free(&node->operand);
        mr_free(node);

        access_statement_error("Unary operation", 15, *poss, *pose);
    }

    visit_node(res, &node->operand, context, 0);
    if (!res->value)
    {
        mr_free(node);
        return;
    }

    switch (node->operator)
    {
    case ADD_T:
        compute_pos(res, res->value, poss);
        break;
    case SUB_T:
        compute_neg(res, res->value, poss);
        break;
    case B_NOT_T:
        compute_b_not(res, res->value, poss);
        break;
    case NOT_KT:
        value_set(res->value, BOOL_V, (void*)(uintptr_t)value_isfalse(res->value));
        break;
    }

    if (!res->value)
    {
        mr_free(node);
        return;
    }

    res->value->poss = *poss;
    res->value->pose = *pose;

    mr_free(node);
}

void visit_var_assign(
    visit_res_t *res, var_assign_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (node->value.type)
    {
        visit_node(res, &node->value, context, 0);
        if (!res->value)
        {
            mr_free(node->name);
            mr_free(node);
            return;
        }
    }
    else
        value_set_vo(res->value, NONE_V);

    if (prop & PTR_MASK)
    {
        void* ptr = (void*)var_setp(context, node->name, res->value);
        value_set(res->value, PTR_V, ptr);
    }
    else
    {
        var_set(context, node->name, res->value);
        value_addref(res->value, 1);

        res->value->poss = *poss;
        res->value->pose = *pose;
    }

    mr_free(node);
}

void visit_var_modify(
    visit_res_t *res, bin_operation_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
)
{
}

void visit_var_fmodify(
    visit_res_t *res, unary_operation_node_t *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
)
{
}

void visit_var_access(
    visit_res_t *res, char *node,
    context_t *context, pos_t *poss, pos_t *pose, uint8_t prop
)
{
    if (prop & PTR_MASK)
    {
        uint8_t error = 0;
        void *ptr = (void*)var_getp(&error, context, node);

        if (error)
        {
            if (prop & ASSIGN_MASK)
            {
                ptr = (void*)var_add(context, node);
                value_set(res->value, PTR_V, ptr);
                return;
            }

            not_def_error(node, *poss, *pose);
        }

        value_set(res->value, PTR_V, ptr);
    }
    else
    {
        res->value = var_get(context, node);
        if (!res->value)
            not_def_error(node, *poss, *pose);

        res->value->poss = *poss;
        res->value->pose = *pose;
    }

    mr_free(node);
}

void map_array(visit_res_t *res, context_t *context, list_value_t *ptrs, value_t *values)
{
}

void link_array(visit_res_t *res, context_t *context, list_value_t *ptrs, value_t *values)
{
}
