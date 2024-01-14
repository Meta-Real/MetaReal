
#include <optimizer/optimizer.h>
#include <optimizer/operation.h>
#include <lexer/token.h>
#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

mr_byte_t mr_visit(
    mr_invalid_semantic_t *error, mr_node_t *node);
mr_byte_t mr_visit_int(
    mr_node_t *node);
mr_byte_t mr_visit_binary_op(
    mr_invalid_semantic_t *error, mr_node_t *node);
mr_byte_t mr_visit_ex_dollar_method(
    mr_invalid_semantic_t *error, mr_node_t *node);

mr_byte_t mr_optimizer(
    mr_optimizer_t *res, mr_node_t *nodes, mr_long_t size)
{
    mr_byte_t retcode;
    for (res->size = 0; res->size != size; res->size++)
    {
        retcode = mr_visit(&res->error, nodes + res->size);
        if (retcode != MR_NOERROR)
        {
            mr_values_free(nodes, size);
            return retcode;
        }
    }

    res->values = nodes;
    return MR_NOERROR;
}

mr_byte_t mr_visit(
    mr_invalid_semantic_t *error, mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_INT:
        return mr_visit_int(node);
    case MR_NODE_BINARY_OP:
        return mr_visit_binary_op(error, node);
    case MR_NODE_EX_DOLLAR_METHOD:
        return mr_visit_ex_dollar_method(error, node);
    }

    return MR_NOERROR;
}

mr_byte_t mr_visit_int(
    mr_node_t *node)
{
    node->useless = _mr_config.opt_rem_useless;
    if (!_mr_config.opt_const_fold)
        return MR_NOERROR;

    mr_node_data_t data = node->value.data;
    mr_value_cint_t *value = malloc(sizeof(mr_value_cint_t));
    if (!value)
        return MR_ERROR_NOT_ENOUGH_MEMORY;

    // We can do better
    value->value = 0;
    for (mr_byte_t i = 0; i < data.size; i++)
        value->value = 10 * value->value + _mr_config.code[data.idx + i] - '0';

    value->idx = data.idx;
    value->size = data.size;

    node->type = MR_VALUE_CINT;
    node->value.ptr = value;
    return MR_NOERROR;
}

mr_byte_t mr_visit_binary_op(
    mr_invalid_semantic_t *error, mr_node_t *node)
{
    node->useless = _mr_config.opt_rem_useless;
    mr_node_binary_op_t *data = (mr_node_binary_op_t*)node->value.ptr;

    mr_byte_t retcode = mr_visit(error, &data->left);
    if (retcode != MR_NOERROR)
        return retcode;

    retcode = mr_visit(error, &data->right);
    if (retcode != MR_NOERROR)
        return retcode;

    if (!_mr_config.opt_const_fold)
        return MR_NOERROR;

    switch (data->op)
    {
    case MR_TOKEN_PLUS:
        mr_operation_add(&data->left, &data->right);
        break;
    }

    node->type = data->left.type;
    node->value = data->left.value;

    free(data);
    return MR_NOERROR;
}

mr_byte_t mr_visit_ex_dollar_method(
    mr_invalid_semantic_t *error, mr_node_t *node)
{
    mr_node_ex_dollar_method_t *data = (mr_node_ex_dollar_method_t*)node->value.ptr;

    if (data->name.size == 13 &&
        !memcmp(_mr_config.code + data->sidx, "od_const_fold", 13 * sizeof(mr_chr_t)))
        _mr_config.opt_const_fold = MR_FALSE;
    else if (data->name.size == 13 &&
        !memcmp(_mr_config.code + data->sidx, "oe_const_fold", 13 * sizeof(mr_chr_t)))
        _mr_config.opt_const_fold = MR_TRUE;
    else if (data->name.size == 14 &&
        !memcmp(_mr_config.code + data->sidx, "od_rem_useless", 14 * sizeof(mr_chr_t)))
        _mr_config.opt_rem_useless = MR_FALSE;
    else if (data->name.size == 14 &&
        !memcmp(_mr_config.code + data->sidx, "oe_rem_useless", 14 * sizeof(mr_chr_t)))
        _mr_config.opt_rem_useless = MR_TRUE;
    else
    {
        *error = (mr_invalid_semantic_t){malloc(25 + data->name.size), MR_FALSE,
            MR_INVALID_SEMANTIC_DOLLAR_METHOD,
            data->name.idx, (mr_byte_t)data->name.size};
        if (!error->detail)
            return MR_ERROR_NOT_ENOUGH_MEMORY;

        sprintf(error->detail, "Invalid dollar method \"%.*s\"",
            data->name.size, _mr_config.code + data->name.idx);
        return MR_ERROR_BAD_FORMAT;
    }

    node->type = MR_NODE_NONE;

    free(data);
    return MR_NOERROR;
}
