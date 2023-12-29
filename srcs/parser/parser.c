/**
 * @file parser.c
 * This file contains definitions of the \a parser.h file.
*/

#include <parser/parser.h>
#include <alloc.h>
#include <consts.h>
#include <error.h>

/**
 * @def mr_parser_free
 * It deallocates the remaining list of tokens and its values. \n
 * Also, it deallocates the generated list of nodes.
*/
#define mr_parser_free                                                         \
    do                                                                         \
    {                                                                          \
        mr_nodes_free(res->nodes, res->size);                                  \
                                                                               \
        while (ptr->type != MR_TOKEN_EOF)                                      \
        {                                                                      \
            if (ptr->type != MR_TOKEN_IDENTIFIER && ptr->type != MR_TOKEN_CHR) \
                mr_free(ptr->value);                                           \
                                                                               \
            ptr++;                                                             \
        }                                                                      \
        mr_free(tokens);                                                       \
    } while (0)

/**
 * @def mr_parser_bin_op(func1, func2, cond)
 * It handles binary operation formats.
 * @param func1
 * First function for getting the left node to generate the binary operation node.
 * @param func2
 * Second function for getting the right node to generate the binary operation node.
 * @param cond
 * Condition which indicates the node should be created or not. \n
 * The \a cond parameter is a list of valid token types for the \a op field.
*/
#define mr_parser_bin_op(func1, func2, cond)                                 \
    do                                                                       \
    {                                                                        \
        mr_byte_t retcode = func1(res, tokens);                              \
        if (retcode != NO_ERROR)                                             \
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
            if (retcode != NO_ERROR)                                         \
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

#define mr_parser_advance_newline                \
    if ((++(*tokens))->type == MR_TOKEN_NEWLINE) \
        (*tokens)++

#define mr_parser_node_data_sub(typ)                                                  \
    do                                                                                \
    {                                                                                 \
        mr_node_t *node = res->nodes + res->size;                                     \
        node->type = typ;                                                             \
                                                                                      \
        mr_node_data_t *value = mr_alloc(sizeof(mr_node_data_t));                     \
        if (!value)                                                                   \
            return ERROR_NOT_ENOUGH_MEMORY;                                           \
                                                                                      \
        *value = (mr_node_data_t){(*tokens)->value, (*tokens)->size, (*tokens)->idx}; \
        node->value = value;                                                          \
                                                                                      \
        mr_parser_advance_newline;                                                    \
        return NO_ERROR;                                                              \
    } while (0)

/**
 * It handles addition `+` and subtraction `-` nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_expr(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles multiplication `*`, division `/`,
 * modulo `%`, and quotient `//` operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_term(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles positive `+`, negative `-`,
 * binary not `~`, and logical not (`!`, `not`) unary operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_factor(mr_parser_t *res, mr_token_t **tokens);

mr_byte_t mr_parser_call(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles data types, parentheses, statements, and dollar functions.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_core(mr_parser_t *res, mr_token_t **tokens);

/**
 * It generates dollar methods (both \a DOLLAR_METHOD and \a EX_DOLLAR_METHOD versions).
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_handle_dollar_method(mr_parser_t *res, mr_token_t **tokens);

mr_byte_t mr_parser(mr_parser_t *res, mr_token_t *tokens, mr_long_t alloc)
{
    res->nodes = mr_alloc(alloc * sizeof(mr_node_t));
    if (!res->nodes)
        return ERROR_NOT_ENOUGH_MEMORY;

    res->size = 0;
    mr_long_t size = alloc;

    mr_token_t *ptr = tokens;
    mr_byte_t retcode;
    mr_node_t *block;
    do
    {
        if (res->size == size)
        {
            block = mr_realloc(res->nodes, (size += alloc) * sizeof(mr_node_t));
            if (!block)
            {
                mr_parser_free;
                return ERROR_NOT_ENOUGH_MEMORY;
            }

            res->nodes = block;
        }

        retcode = mr_parser_expr(res, &ptr);
        if (retcode != NO_ERROR)
        {
            mr_parser_free;
            return retcode;
        }

        res->size++;
    } while (ptr[-1].type == MR_TOKEN_NEWLINE && ptr->type != MR_TOKEN_EOF);

    if (ptr->type != MR_TOKEN_EOF)
    {
        res->error = (mr_invalid_syntax_t){"Expected EOF",
            ptr->idx, ptr->size};

        mr_parser_free;
        return ERROR_BAD_FORMAT;
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
        mr_long_t sidx = (*tokens)->idx;
        mr_byte_t op = (*tokens)++->type;

        mr_byte_t retcode = mr_parser_factor(res, tokens);
        if (retcode != NO_ERROR)
            return retcode;

        mr_node_unary_op_t *value = mr_alloc(sizeof(mr_node_unary_op_t));
        if (!value)
        {
            mr_node_free(res->nodes + res->size);
            return ERROR_NOT_ENOUGH_MEMORY;
        }

        mr_node_t *node = res->nodes + res->size;

        *value = (mr_node_unary_op_t){op, *node, sidx};
        node->type = MR_NODE_UNARY_OP;
        node->value = value;
        return NO_ERROR;
    }

    return mr_parser_call(res, tokens);
}

mr_byte_t mr_parser_call(mr_parser_t *res, mr_token_t **tokens)
{
    mr_byte_t retcode = mr_parser_core(res, tokens);
    if (retcode != NO_ERROR)
        return retcode;

    return retcode;
}

mr_byte_t mr_parser_core(mr_parser_t *res, mr_token_t **tokens)
{
    switch ((*tokens)->type)
    {
    case MR_TOKEN_IDENTIFIER:
        mr_parser_node_data_sub(MR_NODE_VAR_ACCESS);
    case MR_TOKEN_INT:
        mr_parser_node_data_sub(MR_NODE_INT);
    case MR_TOKEN_FLOAT:
        mr_parser_node_data_sub(MR_NODE_FLOAT);
    case MR_TOKEN_IMAGINARY:
        mr_parser_node_data_sub(MR_NODE_IMAGINARY);
    case MR_TOKEN_L_PAREN:
        (*tokens)++;

        mr_byte_t retcode = mr_parser_expr(res, tokens);
        if (retcode)
            return retcode;

        if ((*tokens)->type != MR_TOKEN_R_PAREN)
        {
            mr_node_free(res->nodes + res->size);

            res->error = (mr_invalid_syntax_t){"Expected ')'",
                (*tokens)->idx, (*tokens)->size};
            return ERROR_BAD_FORMAT;
        }

        (*tokens)++;
        return NO_ERROR;
    case MR_TOKEN_DOLLAR:
        return mr_parser_handle_dollar_method(res, tokens);
    }

    res->error = (mr_invalid_syntax_t){NULL, (*tokens)->idx, (*tokens)->size};
    return ERROR_BAD_FORMAT;
}

mr_byte_t mr_parser_handle_dollar_method(mr_parser_t *res, mr_token_t **tokens)
{
    mr_node_t *node = res->nodes + res->size;
    mr_long_t sidx = (*tokens)->idx;

    if ((++*tokens)->type != MR_TOKEN_IDENTIFIER)
    {
        res->error = (mr_invalid_syntax_t){"Expected an identifier",
            (*tokens)->idx, (*tokens)->size};
        return ERROR_BAD_FORMAT;
    }

    mr_node_data_t name = {(*tokens)->value, (*tokens)->size, (*tokens)->idx};
    mr_parser_advance_newline;

    if ((*tokens)->type != MR_TOKEN_COLON)
    {
        mr_node_ex_dollar_method_t *value = mr_alloc(sizeof(mr_node_ex_dollar_method_t));
        if (!value)
        {
            mr_free(name.data);
            return ERROR_NOT_ENOUGH_MEMORY;
        }

        *value = (mr_node_ex_dollar_method_t){name, sidx};

        node->type = MR_NODE_EX_DOLLAR_METHOD;
        node->value = value;
        return NO_ERROR;
    }

    mr_node_dollar_method_t *value = mr_alloc(sizeof(mr_node_dollar_method_t));
    if (!value)
    {
        mr_free(name.data);
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    value->args = mr_alloc(MR_PARSER_DOLLAR_METHOD_SIZE * sizeof(mr_node_t));
    if (!value->args)
    {
        mr_free(value);
        mr_free(name.data);
        return ERROR_NOT_ENOUGH_MEMORY;
    }

    value->size = 0;
    mr_byte_t alloc = MR_PARSER_DOLLAR_METHOD_SIZE;

    mr_byte_t retcode;
    mr_node_t *block;
    do
    {
        if (value->size == MR_PARSER_DOLLAR_METHOD_MAX)
        {
            mr_nodes_free(value->args, value->size);
            mr_free(value);
            mr_free(name.data);

            res->error = (mr_invalid_syntax_t){
                "Number of dollar method parameters exceeds the limit",
                (*tokens)->idx, (*tokens)->size};
            return ERROR_BAD_FORMAT;
        }

        ++*tokens;
        retcode = mr_parser_expr(res, tokens);
        if (retcode != NO_ERROR)
        {
            mr_nodes_free(value->args, value->size);
            mr_free(value);
            mr_free(name.data);
            return retcode;
        }

        if (value->size == alloc)
        {
            block = mr_realloc(value->args,
                (alloc += MR_PARSER_DOLLAR_METHOD_SIZE) * sizeof(mr_node_t));
            if (!block)
            {
                mr_nodes_free(value->args, value->size);
                mr_free(value);
                mr_free(name.data);
                return ERROR_NOT_ENOUGH_MEMORY;
            }

            value->args = block;
        }

        value->args[value->size++] = res->nodes[res->size];
    } while ((*tokens)->type == MR_TOKEN_COMMA);

    value->name = name;
    value->sidx = sidx;

    node->type = MR_NODE_DOLLAR_METHOD;
    node->value = value;
    return NO_ERROR;
}
