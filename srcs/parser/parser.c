/**
 * @file parser.c
 * This file contains definitions of the \a parser.h file.
*/

#include <parser/parser.h>
#include <config.h>
#include <stdlib.h>
#include <consts.h>

/**
 * @def mr_parser_free
 * It deallocates the remaining list of tokens and its values. \n
 * Also, it deallocates the generated list of nodes.
*/
#define mr_parser_free                                                         \
    do                                                                         \
    {                                                                          \
        mr_nodes_free(res->nodes, res->size);                                  \
        free(tokens);                                                          \
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
        if (retcode != MR_NOERROR)                                           \
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
            if (retcode != MR_NOERROR)                                       \
            {                                                                \
                mr_node_free(&left);                                         \
                return retcode;                                              \
            }                                                                \
                                                                             \
            value = malloc(sizeof(mr_node_binary_op_t));                     \
            if (!value)                                                      \
            {                                                                \
                mr_node_free(res->nodes + res->size);                        \
                mr_node_free(&left);                                         \
                return MR_ERROR_NOT_ENOUGH_MEMORY;                           \
            }                                                                \
                                                                             \
            *value = (mr_node_binary_op_t){op, left, res->nodes[res->size]}; \
            left.type = MR_NODE_BINARY_OP;                                   \
            left.value.ptr = value;                                          \
        }                                                                    \
                                                                             \
        res->nodes[res->size] = left;                                        \
        return MR_NOERROR;                                                   \
    } while (0)

/**
 * @def mr_parser_advance_newline
 * If the current token is a newline, this macro will advance to the next token. \n
 * This macro is used after identifiers, data types, right parentheses, and return statements.
*/
#define mr_parser_advance_newline              \
    if ((++*tokens)->type == MR_TOKEN_NEWLINE) \
        (*tokens)++

/**
 * @def mr_parser_node_data_sub(typ)
 * A subroutine for generating a \a mr_node_data_t node value with the given type. \n
 * This macro returns from the function that called it.
 * @param typ
 * Type of the generated node.
*/
#define mr_parser_node_data_sub(typ)                                          \
    do                                                                        \
    {                                                                         \
        mr_node_t *node = res->nodes + res->size;                             \
        node->type = typ;                                                     \
        node->value.data = (mr_node_data_t){(*tokens)->idx, 20/*(*tokens)->size*/}; \
                                                                              \
        mr_parser_advance_newline;                                            \
        return MR_NOERROR;                                                    \
    } while (0)

/**
 * It handles or (`||`, `or`) operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_or(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles and (`&&`, `and`) operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_and(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles less than `<`, greater than `>`,
 * less than or equal to `<=`, and greater than or equal to `>=` operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_order(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles equal `==`, not equal `!=`,
 * exactly equal `===`, and exactly not equal `!==` operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_cmp(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles binary or `|` operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_b_or(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles binary xor `^` operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_b_xor(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles binary and `&` operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_b_and(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles left shift `<<` and right shift `>>` operation nodes.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_shift(mr_parser_t *res, mr_token_t **tokens);

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

/**
 * It handles exponentiation `**`
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_power(mr_parser_t *res, mr_token_t **tokens);

/**
 * It handles function calls (both \a FUNC_CALL and \a EX_FUNC_CALL versions).
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
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

mr_byte_t mr_parser(
    mr_parser_t *res, mr_token_t *tokens)
{
    mr_long_t alloc = _mr_config.size / MR_PARSER_NODES_CHUNK + 1;
    res->nodes = malloc(alloc * sizeof(mr_node_t));
    if (!res->nodes)
        return MR_ERROR_NOT_ENOUGH_MEMORY;

    res->size = 0;
    mr_long_t size = alloc;

    mr_token_t *ptr = tokens;
    mr_byte_t retcode;
    mr_node_t *block;
    do
    {
        if (res->size == size)
        {
            block = realloc(res->nodes, (size += alloc) * sizeof(mr_node_t));
            if (!block)
            {
                mr_parser_free;
                return MR_ERROR_NOT_ENOUGH_MEMORY;
            }

            res->nodes = block;
        }

        retcode = mr_parser_or(res, &ptr);
        if (retcode != MR_NOERROR)
        {
            mr_parser_free;
            return retcode;
        }

        res->size++;
    } while (ptr[-1].type == MR_TOKEN_NEWLINE && ptr->type != MR_TOKEN_EOF);

    if (ptr->type != MR_TOKEN_EOF)
    {
        res->error = (mr_invalid_syntax_t){"Expected EOF",
            MR_TOKEN_IDX(ptr), ptr->type};

        mr_parser_free;
        return MR_ERROR_BAD_FORMAT;
    }

    free(tokens);
    return MR_NOERROR;
}

mr_byte_t mr_parser_or(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_and, mr_parser_and,
        (*tokens)->type == MR_TOKEN_OR_K);
}

mr_byte_t mr_parser_and(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_order, mr_parser_order,
        (*tokens)->type == MR_TOKEN_AND_K);
}

mr_byte_t mr_parser_order(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_cmp, mr_parser_cmp,
        (*tokens)->type >= MR_TOKEN_LESS && (*tokens)->type <= MR_TOKEN_GREATER_EQUAL);
}

mr_byte_t mr_parser_cmp(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_b_or, mr_parser_b_or,
        (*tokens)->type >= MR_TOKEN_EQUAL && (*tokens)->type <= MR_TOKEN_EX_NEQUAL);
}

mr_byte_t mr_parser_b_or(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_b_xor, mr_parser_b_xor,
        (*tokens)->type == MR_TOKEN_B_OR);
}

mr_byte_t mr_parser_b_xor(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_b_and, mr_parser_b_and,
        (*tokens)->type == MR_TOKEN_B_XOR);
}

mr_byte_t mr_parser_b_and(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_shift, mr_parser_shift,
        (*tokens)->type == MR_TOKEN_B_AND);
}

mr_byte_t mr_parser_shift(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_expr, mr_parser_expr,
        (*tokens)->type == MR_TOKEN_L_SHIFT || (*tokens)->type == MR_TOKEN_R_SHIFT);
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
        mr_long_t sidx = MR_TOKEN_IDX(*tokens);
        mr_byte_t op = (*tokens)++->type;

        mr_byte_t retcode = mr_parser_factor(res, tokens);
        if (retcode != MR_NOERROR)
            return retcode;

        mr_node_unary_op_t *value = malloc(sizeof(mr_node_unary_op_t));
        if (!value)
        {
            mr_node_free(res->nodes + res->size);
            return MR_ERROR_NOT_ENOUGH_MEMORY;
        }

        mr_node_t *node = res->nodes + res->size;

        *value = (mr_node_unary_op_t){op, *node, sidx};
        node->type = MR_NODE_UNARY_OP;
        node->value.ptr = value;
        return MR_NOERROR;
    }

    return mr_parser_call(res, tokens);
}

mr_byte_t mr_parser_power(mr_parser_t *res, mr_token_t **tokens)
{
    mr_parser_bin_op(mr_parser_call, mr_parser_factor,
        (*tokens)->type == MR_TOKEN_POWER);
}

mr_byte_t mr_parser_call(mr_parser_t *res, mr_token_t **tokens)
{
    mr_byte_t retcode = mr_parser_core(res, tokens);
    if (retcode != MR_NOERROR)
        return retcode;

    mr_node_t *node = res->nodes + res->size;
    if ((*tokens)->type == MR_TOKEN_L_PAREN)
    {
        if ((++*tokens)->type == MR_TOKEN_R_PAREN)
        {
            mr_node_ex_func_call_t *value = malloc(sizeof(mr_node_ex_func_call_t));
            if (!value)
            {
                mr_node_free(node);
                return MR_ERROR_NOT_ENOUGH_MEMORY;
            }

            tokens++;
            *value = (mr_node_ex_func_call_t){*node, MR_TOKEN_IDX(*tokens)};

            node->type = MR_NODE_EX_FUNC_CALL;
            node->value.ptr = value;
            return MR_NOERROR;
        }

        mr_node_func_call_t *value = malloc(sizeof(mr_node_func_call_t));
        if (!value)
        {
            mr_node_free(node);
            return MR_ERROR_NOT_ENOUGH_MEMORY;
        }

        value->args = malloc(MR_PARSER_FUNC_CALL_SIZE * sizeof(mr_node_call_arg_t));
        if (!value->args)
        {
            free(value);
            mr_node_free(node);
            return MR_ERROR_NOT_ENOUGH_MEMORY;
        }

        value->func = *node;
        value->size = 0;
        mr_byte_t alloc = MR_PARSER_FUNC_CALL_SIZE;

        mr_node_call_arg_t *block;
        do
        {
            if (value->size == alloc)
            {
                if (alloc == MR_PARSER_FUNC_CALL_MAX)
                {
                    mr_node_func_call_free(value);

                    res->error = (mr_invalid_syntax_t){
                        "Number of function call arguments exceeds the limit",
                        MR_TOKEN_IDX(*tokens), (*tokens)->type};
                    return MR_ERROR_BAD_FORMAT;
                }

                block = realloc(value->args,
                    (alloc += MR_PARSER_FUNC_CALL_SIZE) * sizeof(mr_node_call_arg_t));
                if (!block)
                {
                    mr_node_func_call_free(value);
                    return MR_ERROR_BAD_FORMAT;
                }
            }

            block = value->args + value->size;
            if ((*tokens)->type == MR_TOKEN_IDENTIFIER && (*tokens)[1].type == MR_TOKEN_ASSIGN)
            {
                block->name = (mr_node_data_t){(*tokens)->idx, 20};//(*tokens)->size};
                (*tokens) += 2;
            }
            else
                block->name.size = 0;

            retcode = mr_parser_or(res, tokens);
            if (retcode != MR_NOERROR)
            {
                mr_node_func_call_free(value);
                return retcode;
            }

            block->value = res->nodes[res->size];
            value->size++;

            if ((*tokens)->type != MR_TOKEN_COMMA)
                break;

            (*tokens)++;
        } while (1);

        if ((*tokens)->type != MR_TOKEN_R_PAREN)
        {
            mr_node_func_call_free(value);

            res->error = (mr_invalid_syntax_t){"Expected ')' or ','",
                MR_TOKEN_IDX(*tokens), (*tokens)->type};
            return MR_ERROR_BAD_FORMAT;
        }

        if (value->size != alloc)
        {
            block = realloc(value->args, value->size * sizeof(mr_node_call_arg_t));
            if (!block)
            {
                mr_node_func_call_free(value);
                return MR_ERROR_NOT_ENOUGH_MEMORY;
            }

            value->args = block;
        }

        mr_parser_advance_newline;

        node->type = MR_NODE_FUNC_CALL;
        node->value.ptr = value;
        return MR_NOERROR;
    }

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

        mr_byte_t retcode = mr_parser_or(res, tokens);
        if (retcode != MR_NOERROR)
            return retcode;

        if ((*tokens)->type != MR_TOKEN_R_PAREN)
        {
            mr_node_free(res->nodes + res->size);

            res->error = (mr_invalid_syntax_t){"Expected ')'",
                MR_TOKEN_IDX(*tokens), (*tokens)->type};
            return MR_ERROR_BAD_FORMAT;
        }

        (*tokens)++;
        return MR_NOERROR;
    case MR_TOKEN_DOLLAR:
        return mr_parser_handle_dollar_method(res, tokens);
    }

    res->error = (mr_invalid_syntax_t){NULL, MR_TOKEN_IDX(*tokens), (*tokens)->type};
    return MR_ERROR_BAD_FORMAT;
}

mr_byte_t mr_parser_handle_dollar_method(mr_parser_t *res, mr_token_t **tokens)
{
    mr_node_t *node = res->nodes + res->size;
    mr_long_t sidx = MR_TOKEN_IDX(*tokens);

    if ((++*tokens)->type != MR_TOKEN_IDENTIFIER)
    {
        res->error = (mr_invalid_syntax_t){"Expected an identifier",
            MR_TOKEN_IDX(*tokens), (*tokens)->type};
        return MR_ERROR_BAD_FORMAT;
    }

    mr_node_data_t name = {(*tokens)->idx, 20};//(*tokens)->size};
    mr_parser_advance_newline;

    if ((*tokens)->type != MR_TOKEN_COLON)
    {
        mr_node_ex_dollar_method_t *value = malloc(sizeof(mr_node_ex_dollar_method_t));
        if (!value)
            return MR_ERROR_NOT_ENOUGH_MEMORY;

        *value = (mr_node_ex_dollar_method_t){name, sidx};

        node->type = MR_NODE_EX_DOLLAR_METHOD;
        node->value.ptr = value;
        return MR_NOERROR;
    }

    mr_node_dollar_method_t *value = malloc(sizeof(mr_node_dollar_method_t));
    if (!value)
        return MR_ERROR_NOT_ENOUGH_MEMORY;

    value->params = malloc(MR_PARSER_DOLLAR_METHOD_SIZE * sizeof(mr_node_t));
    if (!value->params)
    {
        free(value);
        return MR_ERROR_NOT_ENOUGH_MEMORY;
    }

    value->size = 0;
    mr_byte_t alloc = MR_PARSER_DOLLAR_METHOD_SIZE;

    mr_byte_t retcode;
    mr_node_t *block;
    do
    {
        if (value->size == alloc)
        {
            if (alloc == MR_PARSER_DOLLAR_METHOD_MAX)
            {
                mr_nodes_free(value->params, alloc);
                free(value);

                res->error = (mr_invalid_syntax_t){
                    "Number of dollar method parameters exceeds the limit",
                    MR_TOKEN_IDX(*tokens), (*tokens)->type};
                return MR_ERROR_BAD_FORMAT;
            }

            block = realloc(value->params,
                (alloc += MR_PARSER_DOLLAR_METHOD_SIZE) * sizeof(mr_node_t));
            if (!block)
            {
                mr_nodes_free(value->params, value->size);
                free(value);
                return MR_ERROR_NOT_ENOUGH_MEMORY;
            }

            value->params = block;
        }

        ++*tokens;
        retcode = mr_parser_or(res, tokens);
        if (retcode != MR_NOERROR)
        {
            mr_nodes_free(value->params, value->size);
            free(value);
            return retcode;
        }

        value->params[value->size++] = res->nodes[res->size];
    } while ((*tokens)->type == MR_TOKEN_COMMA);

    value->name = name;
    value->sidx = sidx;

    node->type = MR_NODE_DOLLAR_METHOD;
    node->value.ptr = value;
    return MR_NOERROR;
}
