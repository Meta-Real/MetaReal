/**
 * @file parser.c
 * This file contains definitions of the \a parser.h file.
*/

#include <parser/parser.h>
#include <alloc.h>
#include <error.h>

#define mr_parser_bin_op(func1, func2, cond)                                 \
    do                                                                       \
    {                                                                        \
        mr_byte_t retcode = func1(res, tokens);                              \
        if (retcode || !res->nodes)                                          \
            return retcode;                                                  \
                                                                             \
        mr_byte_t op;                                                        \
        mr_node_t left = res->nodes[res->size];                              \
                                                                             \
        mr_node_binary_op_t *value;                                          \
        while (cond)                                                         \
        {                                                                    \
            op = (*tokens)++->type;                                          \
                                                                             \
            retcode = func2(res, tokens);                                    \
            if (retcode || !res->nodes)                                      \
            {                                                                \
                mr_node_free(&left);                                         \
                return retcode;                                              \
            }                                                                \
                                                                             \
            value = mr_alloc(sizeof(mr_node_binary_op_t));                   \
            if (!value)                                                      \
            {                                                                \
                mr_node_free(res->nodes + res->size);                        \
                mr_node_free(&left);                                         \
                return ERROR_NOT_ENOUGH_MEMORY;                              \
            }                                                                \
                                                                             \
            *value = (mr_node_binary_op_t){op, left, res->nodes[res->size]}; \
            left.type = MR_NODE_BINARY_OP;                                   \
            left.value = value;                                              \
        }                                                                    \
                                                                             \
        res->nodes[res->size] = left;                                        \
        return NO_ERROR;                                                     \
    } while (0)

mr_byte_t mr_parser_expr(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_term(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_factor(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_call(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_core(mr_parser_t *res, mr_token_t **tokens);

mr_byte_t mr_parser(mr_parser_t *res, mr_token_t *tokens)
{
    res->nodes = mr_alloc(sizeof(mr_node_t));
    if (!res->nodes)
        return ERROR_NOT_ENOUGH_MEMORY;

    res->size = 0;

    mr_token_t *ptr = tokens;
    mr_byte_t retcode = mr_parser_expr(res, &ptr);
    if (retcode != NO_ERROR)
    {
        mr_nodes_free(res->nodes, res->size);
        return retcode;
    }

    res->size = 1;

    if (ptr->type != MR_TOKEN_EOF && res->nodes)
    {
        mr_nodes_free(res->nodes, res->size);

        res->nodes = NULL;
        res->error.detail = "Expected EOF";
        res->error.poss = ptr->poss;
        res->error.eidx = ptr->eidx;
    }

    mr_free(tokens);
    return NO_ERROR;
}

mr_byte_t mr_parser_expr(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_term, mr_parser_term,
        (*tokens)->type == MR_TOKEN_PLUS || (*tokens)->type == MR_TOKEN_MINUS);
}

mr_byte_t mr_parser_term(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_factor, mr_parser_factor,
        (*tokens)->type >= MR_TOKEN_MULTIPLY && (*tokens)->type <= MR_TOKEN_QUOTIENT);
}

mr_byte_t mr_parser_factor(mr_parser_t *res, mr_token_t **tokens)
{
    if ((*tokens)->type >= MR_TOKEN_PLUS && (*tokens)->type <= MR_TOKEN_NOT_K)
    {
        mr_pos_t poss = (*tokens)->poss;
        mr_byte_t op = (*tokens)++->type;

        mr_byte_t retcode = mr_parser_factor(res, tokens);
        if (retcode || !res->nodes)
            return retcode;

        mr_node_unary_op_t *value = mr_alloc(sizeof(mr_node_unary_op_t));
        if (!value)
        {
            mr_node_free(res->nodes + res->size);
            return ERROR_NOT_ENOUGH_MEMORY;
        }

        mr_node_t *node = res->nodes + res->size;

        *value = (mr_node_unary_op_t){op, *node, poss};
        node->type = MR_NODE_UNARY_OP;
        node->value = value;
        return NO_ERROR;
    }

    return mr_parser_call(res, tokens);
}

mr_byte_t mr_parser_call(mr_parser_t *res, mr_token_t **tokens)
{
    return mr_parser_core(res, tokens);
}

mr_byte_t mr_parser_core(mr_parser_t *res, mr_token_t **tokens)
{
    if ((*tokens)->type == MR_TOKEN_INT)
    {
        mr_node_data_t *value = mr_alloc(sizeof(mr_node_data_t));
        if (!value)
            return ERROR_NOT_ENOUGH_MEMORY;

        value->data = (*tokens)->value;
        value->size = (*tokens)->size;
        value->poss = (*tokens)->poss;
        value->eidx = (*tokens)++->eidx;

        mr_node_t *node = res->nodes + res->size;
        node->type = MR_NODE_INT;
        node->value = value;
        return NO_ERROR;
    }

    mr_nodes_free(res->nodes, res->size);

    res->nodes = NULL;
    res->error.detail = NULL;
    res->error.poss = (*tokens)->poss;
    res->error.eidx = (*tokens)->eidx;
    return NO_ERROR;
}
