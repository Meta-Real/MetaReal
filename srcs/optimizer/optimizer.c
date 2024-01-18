
#include <optimizer/optimizer.h>
#include <optimizer/operation.h>
#include <lexer/token.h>
#include <config.h>
#include <stack.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct __MR_OPTIMIZER_DATA_T
{
    mr_byte_t props : 1;

    mr_byte_t flag;
    mr_invalid_semantic_t *error;
};
typedef struct __MR_OPTIMIZER_DATA_T mr_optimizer_data_t;

#define MR_OPTIMIZER_USEFUL 1

void mr_visit(
    mr_optimizer_data_t *data, mr_node_t *node);
void mr_visit_int(
    mr_optimizer_data_t *data, mr_node_t *node);
void mr_visit_binary_op(
    mr_optimizer_data_t *data, mr_node_t *node);
void mr_visit_var_access(
    mr_optimizer_data_t *data, mr_node_t *node);
void mr_visit_func_call(
    mr_optimizer_data_t *data, mr_node_t *node);
void mr_visit_ex_dollar_method(
    mr_optimizer_data_t *data, mr_node_t *node);

mr_byte_t mr_optimizer(
    mr_optimizer_t *res, mr_node_t *nodes, mr_long_t size)
{
    mr_optimizer_data_t data = {0, MR_NOERROR, &res->error};

    for (res->size = 0; res->size != size; res->size++)
    {
        mr_visit(&data, nodes + res->size);
        if (data.flag != MR_NOERROR)
            return data.flag;
    }

    res->values = nodes;
    return MR_NOERROR;
}

void mr_visit(
    mr_optimizer_data_t *data, mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_INT:
        mr_visit_int(data, node);
        return;
    case MR_NODE_BINARY_OP:
        mr_visit_binary_op(data, node);
        return;
    case MR_NODE_VAR_ACCESS:
        mr_visit_var_access(data, node);
        return;
    case MR_NODE_FUNC_CALL:
        mr_visit_func_call(data, node);
        return;
    case MR_NODE_EX_DOLLAR_METHOD:
        mr_visit_ex_dollar_method(data, node);
        return;
    }
}

void mr_visit_int(
    mr_optimizer_data_t *data, mr_node_t *node)
{
    node->useless = !(data->props & MR_OPTIMIZER_USEFUL) && _mr_config.opt_rem_useless;
    if (!node->useless)
        return;

    mr_long_t ptr;
    mr_byte_t retcode = mr_stack_push(&ptr, sizeof(mr_value_cint_t));
    if (retcode != MR_NOERROR)
    {
        data->flag = retcode;
        return;
    }

    mr_value_cint_t *vnode = (mr_value_cint_t*)(_mr_stack.data + ptr);
    vnode->value = 0;

    mr_str_ct num = _mr_config.code + node->value;
    do
    {
        if (*num == '_')
        {
            num++;
            continue;
        }

        vnode->value = 10 * vnode->value + *num++ - '0';
    } while ((*num >= '0' && *num <= '9') || *num == '_');

    MR_IDX_DECOMPOSE(vnode->idx, node->value);

    node->type = MR_VALUE_CINT;
    node->value = ptr;
}

void mr_visit_binary_op(
    mr_optimizer_data_t *data, mr_node_t *node)
{
    node->useless = !(data->props & MR_OPTIMIZER_USEFUL) && _mr_config.opt_rem_useless;
    if (!node->useless)
        return;

    mr_node_binary_op_t *vnode = (mr_node_binary_op_t*)(_mr_stack.data + node->value);

    mr_visit(data, &vnode->left);
    if (data->flag != MR_NOERROR)
        return;

    mr_visit(data, &vnode->right);
    if (data->flag != MR_NOERROR)
        return;

    if (!_mr_config.opt_const_fold)
        return;

    switch (vnode->op)
    {
    case MR_TOKEN_PLUS:
        mr_operation_add(&vnode->left, &vnode->right);
        break;
    }

    node->type = vnode->left.type;
    node->value = vnode->left.value;
}

void mr_visit_var_access(
    mr_optimizer_data_t *data, mr_node_t *node)
{
    mr_str_t name = (mr_str_t)(_mr_config.code + node->value);
    mr_short_t size = (mr_short_t)mr_token_getsize(MR_TOKEN_IDENTIFIER, node->value);

    if (size == 5 && !memcmp(name, "print", 5 * sizeof(mr_chr_t)))
    {
        node->type = MR_VALUE_BIFUNC;
        node->value = MR_VALUE_BIFUNC_PRINT;

        data->props |= MR_OPTIMIZER_USEFUL;
    }
}

void mr_visit_func_call(
    mr_optimizer_data_t *data, mr_node_t *node)
{
    mr_node_func_call_t *vnode = (mr_node_func_call_t*)(_mr_stack.data + node->value);
    mr_byte_t props = data->props;

    mr_visit(data, &vnode->func);
    if (data->flag != MR_NOERROR)
        return;

    mr_node_call_arg_t *args = _mr_stack.ptrs[vnode->args];
    for (mr_byte_t i = 0; i < vnode->size; i++)
    {
        mr_visit(data, &args[i].value);
        if (data->flag != MR_NOERROR)
            return;
    }

    data->props = props;
}

void mr_visit_ex_dollar_method(
    mr_optimizer_data_t *data, mr_node_t *node)
{
    mr_node_ex_dollar_method_t *vnode =
        (mr_node_ex_dollar_method_t*)(_mr_stack.data + node->value);

    mr_long_t idx = MR_IDX_EXTRACT(vnode->idx);
    mr_long_t size = mr_token_getsize(MR_TOKEN_IDENTIFIER, idx);
    mr_str_ct name = _mr_config.code + idx;

    if (size == 13 && !memcmp(name, "od_const_fold", 13 * sizeof(mr_chr_t)))
        _mr_config.opt_const_fold = MR_FALSE;
    else if (size == 13 && !memcmp(name, "oe_const_fold", 13 * sizeof(mr_chr_t)))
        _mr_config.opt_const_fold = MR_TRUE;
    else if (size == 14 && !memcmp(name, "od_rem_useless", 14 * sizeof(mr_chr_t)))
        _mr_config.opt_rem_useless = MR_FALSE;
    else if (size == 14 && !memcmp(name, "oe_rem_useless", 14 * sizeof(mr_chr_t)))
        _mr_config.opt_rem_useless = MR_TRUE;
    else
    {
        mr_invalid_semantic_t *error = data->error;

        *error = (mr_invalid_semantic_t){
            malloc(25 + mr_token_getsize(MR_TOKEN_IDENTIFIER, MR_IDX_EXTRACT(vnode->idx))),
            MR_FALSE, MR_INVALID_SEMANTIC_DOLLAR_METHOD,
            vnode->idx, MR_TOKEN_IDENTIFIER, 0};
        if (!error->detail)
        {
            data->flag = MR_ERROR_NOT_ENOUGH_MEMORY;
            return;
        }

        sprintf(error->detail, "Invalid dollar method \"%.*s\"", size, name);
        data->flag = MR_ERROR_BAD_FORMAT;
        return;
    }

    node->type = MR_NODE_NONE;
}
