/**
 * @file parser.c
 * This file contains definitions of the \a parser.h file.
*/

#include <parser/parser.h>
#include <stack.h>
#include <config.h>
#include <stdlib.h>
#include <consts.h>

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
#define mr_parser_bin_op(func1, func2, cond)                            \
    do                                                                  \
    {                                                                   \
        mr_long_t ptr;                                                  \
        mr_byte_t retcode, op;                                          \
        mr_node_binary_op_t *value;                                     \
        mr_node_t left;                                                 \
                                                                        \
        retcode = func1(res, tokens);                                   \
        if (retcode != MR_NOERROR)                                      \
            return retcode;                                             \
                                                                        \
        left = res->nodes[res->size];                                   \
        while (cond)                                                    \
        {                                                               \
            op = (*tokens)++->type;                                     \
                                                                        \
            retcode = func2(res, tokens);                               \
            if (retcode != MR_NOERROR)                                  \
                return retcode;                                         \
                                                                        \
            retcode = mr_stack_push(&ptr, sizeof(mr_node_binary_op_t)); \
            if (retcode != MR_NOERROR)                                  \
                return MR_ERROR_NOT_ENOUGH_MEMORY;                      \
                                                                        \
            value = (mr_node_binary_op_t*)(_mr_stack.data + ptr);       \
            *value = (mr_node_binary_op_t){                             \
                .left=left, .right=res->nodes[res->size], .op=op};      \
                                                                        \
            left = (mr_node_t){.type = MR_NODE_BINARY_OP, .value=ptr};  \
        }                                                               \
                                                                        \
        res->nodes[res->size] = left;                                   \
        return MR_NOERROR;                                              \
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
#define mr_parser_node_data_sub(typ)                                           \
    do                                                                         \
    {                                                                          \
        mr_node_t *node;                                                       \
                                                                               \
        node = res->nodes + res->size;                                         \
        *node = (mr_node_t){.type=typ, .value=MR_IDX_EXTRACT((*tokens)->idx)}; \
                                                                               \
        mr_parser_advance_newline;                                             \
        return MR_NOERROR;                                                     \
    } while (0)

/**
 * It handles variable reassignments.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_reassign(mr_parser_t *res, mr_token_t **tokens);

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
 * It generates lists.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_handle_list(mr_parser_t *res, mr_token_t **tokens);

/**
 * It generates dictionaries and starts defining sets.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_handle_dict(mr_parser_t *res, mr_token_t **tokens);

/**
 * It generates sets.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_handle_set(mr_parser_t *res, mr_token_t **tokens);

/**
 * It generates variable assignments.
 * @param res
 * Result of the \a mr_parser function passed as a pointer.
 * @param tokens
 * List of tokens passed as a pointer.
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_parser_handle_var_assign(mr_parser_t *res, mr_token_t **tokens);

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
    mr_long_t alloc, size;
    mr_byte_t retcode;
    mr_node_t *block;
    mr_token_t *ptr;

    alloc = _mr_config.size / MR_PARSER_NODES_CHUNK + 1;
    res->nodes = malloc(alloc * sizeof(mr_node_t));
    if (!res->nodes)
        return MR_ERROR_NOT_ENOUGH_MEMORY;

    res->size = 0;
    size = alloc;

    ptr = tokens;
    do
    {
        if (res->size == size)
        {
            block = realloc(res->nodes, (size += alloc) * sizeof(mr_node_t));
            if (!block)
            {
                free(res->nodes);
                return MR_ERROR_NOT_ENOUGH_MEMORY;
            }

            res->nodes = block;
        }

        retcode = mr_parser_or(res, &ptr);
        if (retcode != MR_NOERROR)
        {
            free(res->nodes);
            return retcode;
        }

        res->size++;
        if (ptr->type == MR_TOKEN_SEMICOLON)
            ptr++;
        else if (ptr[-1].type != MR_TOKEN_NEWLINE)
            break;
    } while (ptr->type != MR_TOKEN_EOF);

    if (ptr->type != MR_TOKEN_EOF)
    {
        res->error = (mr_invalid_syntax_t){.detail="Expected EOF", .token=ptr};

        free(res->nodes);
        return MR_ERROR_BAD_FORMAT;
    }

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
        mr_long_t ptr;
        mr_byte_t op, retcode;
        mr_node_unary_op_t *value;
        mr_node_t *node;
        mr_idx_t sidx;

        sidx = (*tokens)->idx;
        op = (*tokens)++->type;

        retcode = mr_parser_factor(res, tokens);
        if (retcode != MR_NOERROR)
            return retcode;

        retcode = mr_stack_push(&ptr, sizeof(mr_node_unary_op_t));
        if (retcode != MR_NOERROR)
            return retcode;

        value = (mr_node_unary_op_t*)(_mr_stack.data + ptr);
        node = res->nodes + res->size;
        *value = (mr_node_unary_op_t){.operand=*node, .sidx=sidx, .op=op};

        *node = (mr_node_t){.type=MR_NODE_UNARY_OP, .value=ptr};
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
    mr_byte_t retcode;
    mr_node_t *node;

    retcode = mr_parser_core(res, tokens);
    if (retcode != MR_NOERROR)
        return retcode;

    node = res->nodes + res->size;
    if ((*tokens)->type == MR_TOKEN_L_PAREN)
    {
        mr_long_t ptr, idx;
        mr_byte_t alloc;
        mr_node_func_call_t *value;
        mr_node_call_arg_t *args, *arg;

        if ((++*tokens)->type == MR_TOKEN_R_PAREN)
        {
            mr_node_ex_func_call_t *ex_value;

            retcode = mr_stack_push(&ptr, sizeof(mr_node_ex_func_call_t));
            if (retcode != MR_NOERROR)
                return retcode;

            ex_value = (mr_node_ex_func_call_t*)(_mr_stack.data + ptr);
            *ex_value = (mr_node_ex_func_call_t){.func=*node, .eidx=(++*tokens)->idx};

            mr_parser_advance_newline;

            *node = (mr_node_t){.type=MR_NODE_EX_FUNC_CALL, .value=ptr};
            return MR_NOERROR;
        }

        retcode = mr_stack_push(&ptr, sizeof(mr_node_func_call_t));
        if (retcode != MR_NOERROR)
            return retcode;

        value = (mr_node_func_call_t*)(_mr_stack.data + ptr);
        retcode = mr_stack_palloc(&value->args,
            MR_PARSER_FUNC_CALL_SIZE * sizeof(mr_node_call_arg_t));
        if (retcode != MR_NOERROR)
            return retcode;

        value->func = *node;
        value->size = 0;

        alloc = MR_PARSER_FUNC_CALL_SIZE;

        idx = MR_IDX_EXTRACT(value->args);
        args = _mr_stack.ptrs[idx];
        do
        {
            if (value->size == alloc)
            {
                if (alloc == MR_PARSER_FUNC_CALL_MAX)
                {
                    res->error = (mr_invalid_syntax_t){
                        .detail="Number of function call arguments exceeds the limit", .token=*tokens};
                    return MR_ERROR_BAD_FORMAT;
                }

                retcode = mr_stack_prealloc(idx,
                    (alloc += MR_PARSER_FUNC_CALL_SIZE) * sizeof(mr_node_call_arg_t));
                if (retcode != MR_NOERROR)
                    return MR_ERROR_BAD_FORMAT;

                args = _mr_stack.ptrs[idx];
            }

            arg = (mr_node_call_arg_t*)(args + value->size);
            if ((*tokens)->type == MR_TOKEN_IDENTIFIER &&
                (*tokens)[1].type == MR_TOKEN_ASSIGN)
            {
                arg->name = (*tokens)->idx;
                *tokens += 2;
            }
            else
                arg->name = MR_INVALID_IDX;

            retcode = mr_parser_or(res, tokens);
            if (retcode != MR_NOERROR)
                return retcode;

            arg->value = res->nodes[res->size];
            value->size++;

            if ((*tokens)->type != MR_TOKEN_COMMA)
                break;

            ++*tokens;
        } while (1);

        if ((*tokens)->type != MR_TOKEN_R_PAREN)
        {
            res->error = (mr_invalid_syntax_t){.detail="Expected ')' or ','", .token=*tokens};
            return MR_ERROR_BAD_FORMAT;
        }

        if (value->size != alloc)
        {
            retcode = mr_stack_prealloc(idx, value->size * sizeof(mr_node_call_arg_t));
            if (retcode != MR_NOERROR)
                return retcode;
        }

        mr_parser_advance_newline;

        *node = (mr_node_t){.type=MR_NODE_FUNC_CALL, .value=ptr};
        return MR_NOERROR;
    }

    return retcode;
}

mr_byte_t mr_parser_core(mr_parser_t *res, mr_token_t **tokens)
{
    mr_byte_t retcode;

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
        ++*tokens;

        retcode = mr_parser_or(res, tokens);
        if (retcode != MR_NOERROR)
            return retcode;

        if ((*tokens)->type != MR_TOKEN_R_PAREN)
        {
            res->error = (mr_invalid_syntax_t){.detail="Expected ')'", .token=*tokens};
            return MR_ERROR_BAD_FORMAT;
        }

        mr_parser_advance_newline;
        return MR_NOERROR;
    case MR_TOKEN_L_SQUARE:
        return mr_parser_handle_list(res, tokens);
    case MR_TOKEN_DOLLAR:
        return mr_parser_handle_dollar_method(res, tokens);
    }

    res->error = (mr_invalid_syntax_t){.detail=NULL, .token=*tokens};
    return MR_ERROR_BAD_FORMAT;
}

mr_byte_t mr_parser_handle_list(mr_parser_t *res, mr_token_t **tokens)
{
    mr_long_t ptr, pidx, size, alloc;
    mr_byte_t retcode;
    mr_node_list_t *value;
    mr_node_t *node, *elems;
    mr_idx_t sidx;

    node = res->nodes + res->size;
    sidx = (*tokens)++->idx;

    retcode = mr_stack_push(&ptr, sizeof(mr_node_list_t));
    if (retcode != MR_NOERROR)
        return retcode;

    value = (mr_node_list_t*)(_mr_stack.data + ptr);
    value->sidx = sidx;

    if ((*tokens)->type == MR_TOKEN_R_SQUARE)
    {
        value->size = (mr_idx_t){.lidx=0, .hidx=0};
        value->eidx = (*tokens)++->idx;

        *node = (mr_node_t){.type=MR_NODE_LIST, .value=ptr};
        return MR_NOERROR;
    }

    retcode = mr_stack_palloc(&value->elems, MR_PARSER_LIST_SIZE * sizeof(mr_node_t));
    if (retcode != MR_NOERROR)
        return retcode;

    pidx = MR_IDX_EXTRACT(value->elems);
    elems = (mr_node_t*)_mr_stack.ptrs[pidx];

    size = 0;
    alloc = MR_PARSER_LIST_SIZE;
    do
    {
        if (size == alloc)
        {
            retcode = mr_stack_prealloc(pidx, (alloc += MR_PARSER_LIST_SIZE) * sizeof(mr_node_t));
            if (retcode != MR_NOERROR)
                return retcode;

            elems = (mr_node_t*)_mr_stack.ptrs[pidx];
        }

        retcode = mr_parser_or(res, tokens);
        if (retcode != MR_NOERROR)
            return retcode;

        elems[size++] = res->nodes[res->size];
        if ((*tokens)->type != MR_TOKEN_COMMA || (++*tokens)->type == MR_TOKEN_R_SQUARE)
            break;
    } while (1);

    if ((*tokens)->type != MR_TOKEN_R_SQUARE)
    {
        res->error = (mr_invalid_syntax_t){.detail="Expected ']' or ','", .token=*tokens};
        return MR_ERROR_BAD_FORMAT;
    }

    if (size != alloc)
    {
        retcode = mr_stack_prealloc(pidx, size * sizeof(mr_node_t));
        if (retcode != MR_NOERROR)
            return retcode;
    }

    value->size = MR_IDX_DECOMPOSE(size);
    value->eidx = (*tokens)++->idx;

    *node = (mr_node_t){.type=MR_NODE_LIST, .value=ptr};
    return MR_NOERROR;
}

mr_byte_t mr_parser_handle_dollar_method(mr_parser_t *res, mr_token_t **tokens)
{
    mr_long_t ptr, pidx;
    mr_byte_t retcode, alloc;
    mr_node_dollar_method_t *value;
    mr_node_t *node, *params;
    mr_idx_t sidx, name;

    node = res->nodes + res->size;
    sidx = (*tokens)->idx;

    if ((++*tokens)->type != MR_TOKEN_IDENTIFIER)
    {
        res->error = (mr_invalid_syntax_t){.detail="Expected an identifier", .token=*tokens};
        return MR_ERROR_BAD_FORMAT;
    }

    name = (*tokens)->idx;
    mr_parser_advance_newline;

    if ((*tokens)->type != MR_TOKEN_COLON)
    {
        mr_node_ex_dollar_method_t *ex_value;

        retcode = mr_stack_push(&ptr, sizeof(mr_node_ex_dollar_method_t));
        if (retcode != MR_NOERROR)
            return retcode;

        ex_value = (mr_node_ex_dollar_method_t*)(_mr_stack.data + ptr);
        *ex_value = (mr_node_ex_dollar_method_t){.name=name, .sidx=sidx};

        *node = (mr_node_t){.type=MR_NODE_EX_DOLLAR_METHOD, .value=ptr};
        return MR_NOERROR;
    }

    retcode = mr_stack_push(&ptr, sizeof(mr_node_dollar_method_t));
    if (retcode != MR_NOERROR)
        return retcode;

    value = (mr_node_dollar_method_t*)(_mr_stack.data + ptr);

    retcode = mr_stack_palloc(&value->params, MR_PARSER_DOLLAR_METHOD_SIZE * sizeof(mr_node_t));
    if (retcode != MR_NOERROR)
        return retcode;

    pidx = MR_IDX_EXTRACT(value->params);
    params = (mr_node_t*)_mr_stack.ptrs[pidx];

    value->size = 0;
    alloc = MR_PARSER_DOLLAR_METHOD_SIZE;
    do
    {
        if (value->size == alloc)
        {
            if (alloc == MR_PARSER_DOLLAR_METHOD_MAX)
            {
                res->error = (mr_invalid_syntax_t){.detail="Number of dollar method parameters exceeds the limit", .token=*tokens};
                return MR_ERROR_BAD_FORMAT;
            }

            retcode = mr_stack_prealloc(pidx, (alloc += MR_PARSER_DOLLAR_METHOD_SIZE) * sizeof(mr_node_t));
            if (retcode != MR_NOERROR)
                return retcode;

            params = (mr_node_t*)_mr_stack.ptrs[pidx];
        }

        ++*tokens;
        retcode = mr_parser_or(res, tokens);
        if (retcode != MR_NOERROR)
            return retcode;

        params[value->size++] = res->nodes[res->size];
    } while ((*tokens)->type == MR_TOKEN_COMMA);

    if (value->size != alloc)
    {
        retcode = mr_stack_prealloc(pidx, value->size * sizeof(mr_node_t));
        if (retcode != MR_NOERROR)
            return retcode;
    }

    value->name = name;
    value->sidx = sidx;

    *node = (mr_node_t){.type=MR_NODE_DOLLAR_METHOD, .value=ptr};
    return MR_NOERROR;
}
