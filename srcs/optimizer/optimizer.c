
#include <optimizer/optimizer.h>
#include <optimizer/operation.h>
#include <lexer/token.h>
#include <config.h>
#include <stack.h>
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
            return retcode;
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

    mr_long_t ptr;
    mr_byte_t retcode = mr_stack_push(&ptr, sizeof(mr_value_cint_t));
    if (retcode != MR_NOERROR)
        return retcode;

    mr_value_cint_t *value = (mr_value_cint_t*)(_mr_stack.data + ptr);
    value->value = 0;
    value->storage = MR_VALUE_REG_ALL;

    mr_str_ct num = _mr_config.code + node->value;
    do
    {
        if (*num == '_')
        {
            num++;
            continue;
        }

        value->value = 10 * value->value + *num++ - '0';
    } while ((*num >= '0' && *num <= '9') || *num == '_');

    MR_IDX_DECOMPOSE(value->idx, node->value);

    node->type = MR_VALUE_CINT;
    node->value = ptr;
    return MR_NOERROR;
}

mr_byte_t mr_visit_binary_op(
    mr_invalid_semantic_t *error, mr_node_t *node)
{
    node->useless = _mr_config.opt_rem_useless;
    mr_node_binary_op_t *data = (mr_node_binary_op_t*)(_mr_stack.data + node->value);

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
    return MR_NOERROR;
}

mr_byte_t mr_visit_ex_dollar_method(
    mr_invalid_semantic_t *error, mr_node_t *node)
{
    mr_node_ex_dollar_method_t *data =
        (mr_node_ex_dollar_method_t*)(_mr_stack.data + node->value);

    mr_long_t idx = MR_IDX_EXTRACT(data->idx);
    mr_long_t size = mr_token_getsize(MR_TOKEN_IDENTIFIER, idx);
    mr_str_ct name = _mr_config.code + idx;

    if (size == 13 &&
        !memcmp(name, "od_const_fold", 13 * sizeof(mr_chr_t)))
        _mr_config.opt_const_fold = MR_FALSE;
    else if (size == 13 &&
        !memcmp(name, "oe_const_fold", 13 * sizeof(mr_chr_t)))
        _mr_config.opt_const_fold = MR_TRUE;
    else if (size == 14 &&
        !memcmp(name, "od_rem_useless", 14 * sizeof(mr_chr_t)))
        _mr_config.opt_rem_useless = MR_FALSE;
    else if (size == 14 &&
        !memcmp(name, "oe_rem_useless", 14 * sizeof(mr_chr_t)))
        _mr_config.opt_rem_useless = MR_TRUE;
    else
    {
        *error = (mr_invalid_semantic_t){
            malloc(25 + mr_token_getsize(MR_TOKEN_IDENTIFIER, MR_IDX_EXTRACT(data->idx))),
            MR_FALSE, MR_INVALID_SEMANTIC_DOLLAR_METHOD,
            data->idx, MR_TOKEN_IDENTIFIER, 0};
        if (!error->detail)
            return MR_ERROR_NOT_ENOUGH_MEMORY;

        sprintf(error->detail, "Invalid dollar method \"%.*s\"",
            size, name);
        return MR_ERROR_BAD_FORMAT;
    }

    node->type = MR_NODE_NONE;
    return MR_NOERROR;
}
