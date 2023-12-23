/**
 * @file parser.c
 * This file contains definitions of the \a parser.h file.
*/

#include <parser/parser.h>
#include <alloc.h>
#include <error.h>

#define mr_parser_reterror      \
    if (retcode || !res->nodes) \
        return retcode;

mr_byte_t mr_parser_expr(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_term(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_factor(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_call(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_core(mr_parser_t *res, mr_token_t **tokens);

mr_byte_t mr_parser(mr_parser_t *res, mr_token_t *tokens)
{
}

mr_byte_t mr_parser_expr(mr_parser_t *res, mr_token_t **tokens)
{
    mr_byte_t retcode = mr_parser_term(res, tokens);
    mr_parser_reterror;

    mr_byte_t op;
    mr_node_t left = res->nodes[res->size];

    mr_node_binary_op_t *value;
    while ((*tokens)->type == MR_TOKEN_PLUS || (*tokens)->type == MR_TOKEN_MINUS)
    {
        op = (*tokens)++->type;

        retcode = mr_parser_term(res, tokens);
        mr_parser_reterror;

        value = mr_alloc(sizeof(mr_node_binary_op_t));
        if (!value)
        {
            mr_node_free(&left);
            return ERROR_NOT_ENOUGH_MEMORY;
        }

        *value = (mr_node_binary_op_t){op, left, res->nodes[res->size]};
        left.type = MR_NODE_BINARY_OP;
        left.value = value;
    }

    return NO_ERROR;
}

mr_byte_t mr_parser_term(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_factor(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_call(mr_parser_t *res, mr_token_t **tokens);
mr_byte_t mr_parser_core(mr_parser_t *res, mr_token_t **tokens);
