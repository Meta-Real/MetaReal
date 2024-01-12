
#include <optimizer/optimizer.h>
#include <optimizer/operation.h>
#include <config.h>
#include <stdlib.h>

mr_byte_t mr_visit(
    mr_node_t *node);
mr_byte_t mr_visit_int(
    mr_node_t *node);
mr_byte_t mr_visit_binary_op(
    mr_node_t *node);

mr_byte_t mr_optimizer(
    mr_optimizer_t *res, mr_node_t *nodes, mr_long_t size)
{
    mr_byte_t retcode;
    for (res->size = 0; res->size != size; res->size++)
    {
        retcode = mr_visit(nodes + res->size);
        if (retcode != MR_NOERROR)
        {
            while (++res->size != size)
                mr_node_free(nodes + res->size);
            free(nodes);
            return retcode;
        }
    }

    res->values = nodes;
    return MR_NOERROR;
}

mr_byte_t mr_visit(
    mr_node_t *node)
{
    switch (node->type)
    {
    case MR_NODE_INT:
        if (_mr_config.opt_const_fold)
            return mr_visit_int(node);
        else
            return MR_NOERROR;
    case MR_NODE_BINARY_OP:
        return mr_visit_binary_op(node);
    }

    return MR_NOERROR;
}

mr_byte_t mr_visit_int(
    mr_node_t *node)
{
    mr_node_data_t *data = (mr_node_data_t*)node->value;

    mr_value_cint_t *value = malloc(sizeof(mr_value_cint_t));
    if (!value)
    {
        free(data->data);
        free(data);
        return MR_ERROR_NOT_ENOUGH_MEMORY;
    }

    value->value = strtoull(data->data, NULL, 10);
    value->sidx = data->sidx;
    value->size = data->size;

    node->type = MR_VALUE_CINT;
    node->value = value;

    free(data->data);
    free(data);
    return MR_NOERROR;
}

mr_byte_t mr_visit_binary_op(
    mr_node_t *node)
{
    mr_node_binary_op_t *data = (mr_node_binary_op_t*)node->value;

    mr_visit(&data->left);
    mr_visit(&data->right);

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
