/*
    MetaReal Compiler version 1.0.0
*/

#include <parser/parser.h>
#include <alloc.h>
#include <stddef.h>
#include <consts.h>

#include <stdio.h>

#define set_node(t, v, ps, pe)           \
    do                                   \
    {                                    \
        res->nodes[res->size].type = t;  \
        res->nodes[res->size].value = v; \
        res->nodes[res->size].poss = ps; \
        res->nodes[res->size].pose = pe; \
    } while (0)

#define set_node_pso(t, v, pe)           \
    do                                   \
    {                                    \
        res->nodes[res->size].type = t;  \
        res->nodes[res->size].value = v; \
        res->nodes[res->size].pose = pe; \
    } while (0)

#define set_node_peo(t, v, ps)           \
    do                                   \
    {                                    \
        res->nodes[res->size].type = t;  \
        res->nodes[res->size].value = v; \
        res->nodes[res->size].poss = ps; \
    } while (0)

#define set_error(e)                       \
    do                                     \
    {                                      \
        res->error = e;                    \
                                           \
        nodes_free(res->nodes, res->size); \
        res->nodes = NULL;                 \
    } while (0)

#define bin_operation(f1, f2, c)                            \
    do                                                      \
    {                                                       \
        tokens = f1(res, tokens);                           \
        if (!res->nodes)                                    \
            return tokens;                                  \
                                                            \
        node_t left = res->nodes[res->size];                \
                                                            \
        bin_operation_node_t *value;                        \
        while (c)                                           \
        {                                                   \
            value = mr_alloc(sizeof(bin_operation_node_t)); \
            value->operator = tokens++->type;               \
            value->left = left;                             \
                                                            \
            tokens = f2(res, tokens);                       \
            if (!res->nodes)                                \
            {                                               \
                node_free(&left);                           \
                mr_free(value);                             \
                return tokens;                              \
            }                                               \
                                                            \
            value->right = res->nodes[res->size];           \
                                                            \
            left.type = BIN_OPERATION_N;                    \
            left.value = value;                             \
            left.pose = res->nodes[res->size].pose;         \
        }                                                   \
                                                            \
        res->nodes[res->size] = left;                       \
        return tokens;                                      \
    } while (0)

token_t *tuple(parse_res_t *res, token_t *tokens);
token_t *modify(parse_res_t *res, token_t *tokens);
token_t *or(parse_res_t *res, token_t *tokens);
token_t *and(parse_res_t *res, token_t *tokens);
token_t *cmp1(parse_res_t *res, token_t *tokens);
token_t *cmp2(parse_res_t *res, token_t *tokens);
token_t *b_or(parse_res_t *res, token_t *tokens);
token_t *b_xor(parse_res_t *res, token_t *tokens);
token_t *b_and(parse_res_t *res, token_t *tokens);
token_t *shift(parse_res_t *res, token_t *tokens);
token_t *expr(parse_res_t *res, token_t *tokens);
token_t *term(parse_res_t *res, token_t *tokens);
token_t *factor(parse_res_t *res, token_t *tokens);
token_t *power(parse_res_t *res, token_t *tokens);
token_t *prefix(parse_res_t *res, token_t *tokens);
token_t *postfix(parse_res_t *res, token_t *tokens);
token_t *core(parse_res_t *res, token_t *tokens);

token_t *handle_list(parse_res_t *res, token_t *tokens);
token_t *handle_var(parse_res_t *res, token_t *tokens);

parse_res_t parse(token_t *tokens)
{
    parse_res_t res;
    res.nodes = mr_alloc(PARSE_NODE_LIST_LEN * sizeof(node_t));
    res.size = 0;

    uint64_t alloc = PARSE_NODE_LIST_LEN;

    token_t *ptr = tokens;
    do
    {
        for (; tokens->type == SEMICOLON_T; tokens++);
        if (!tokens->type)
            break;

        if (res.size == alloc)
            res.nodes = mr_realloc(res.nodes, (alloc += PARSE_NODE_LIST_LEN) * sizeof(node_t));

        tokens = tuple(&res, tokens);
        if (!res.nodes)
        {
            tokens_free(tokens);
            mr_free(ptr);
            return res;
        }

        res.size++;
    } while (tokens->type == SEMICOLON_T);

    if (tokens->type)
    {
        res.error = invalid_syntax_set("Expected EOF", tokens->poss, tokens->pose);

        nodes_free(res.nodes, res.size);
        res.nodes = NULL;

        tokens_free(tokens);
        mr_free(ptr);
        return res;
    }

    if (res.size)
        res.nodes = mr_realloc(res.nodes, res.size * sizeof(node_t));
    else
        mr_free(res.nodes);

    mr_free(ptr);
    return res;
}

token_t *tuple(parse_res_t *res, token_t *tokens)
{
    tokens = modify(res, tokens);
    if (!res->nodes)
        return tokens;

    if (tokens->type == COMMA_T)
    {
        list_node_t *node = mr_alloc(sizeof(list_node_t));
        node->elements = mr_alloc(PARSE_TUPLE_LEN * sizeof(node_t));
        *node->elements = res->nodes[res->size];
        node->size = 1;

        uint64_t alloc = PARSE_TUPLE_LEN;

        pos_t pose;
        pose.ln = 0;
        do
        {
            if ((++tokens)->type == RPAREN_T || tokens->type == SEMICOLON_T || tokens->type == EOF_T)
            {
                pose = tokens[-1].pose;
                break;
            }

            tokens = modify(res, tokens);
            if (!res->nodes)
            {
                list_node_free(node);
                return tokens;
            }

            if (node->size == alloc)
                node->elements = mr_realloc(node->elements, (alloc += PARSE_TUPLE_LEN) * sizeof(node_t));

            node->elements[node->size++] = res->nodes[res->size];
        } while (tokens->type == COMMA_T);

        if (!pose.ln)
            pose = node->elements[node->size - 1].pose;

        node->elements = mr_realloc(node->elements, node->size * sizeof(node_t));
        set_node(TUPLE_N, node, node->elements->poss, pose);
        return tokens;
    }

    return tokens;
}

token_t *modify(parse_res_t *res, token_t *tokens)
{
    tokens = or(res, tokens);
    if (!res->nodes)
        return tokens;

    if (tokens->type >= ASSIGN_T && tokens->type <= RSHIFT_EQ_T)
    {
        bin_operation_node_t *node = mr_alloc(sizeof(bin_operation_node_t));
        node->operator = tokens++->type;
        node->left = res->nodes[res->size];

        tokens = tuple(res, tokens);
        if (!res->nodes)
        {
            node_free(&node->left);
            mr_free(node);
            return tokens;
        }

        node->right = res->nodes[res->size];
        set_node_peo(VAR_MODIFY_N, node, node->left.poss);
        return tokens;
    }

    return tokens;
}

token_t *or(parse_res_t *res, token_t *tokens)
{
    bin_operation(and, and, tokens->type == OR_KT);
}

token_t *and(parse_res_t *res, token_t *tokens)
{
    bin_operation(cmp1, cmp1, tokens->type == AND_KT);
}

token_t *cmp1(parse_res_t *res, token_t *tokens)
{
    bin_operation(cmp2, cmp2, tokens->type >= LT_T && tokens->type <= GTE_T);
}

token_t *cmp2(parse_res_t *res, token_t *tokens)
{
    bin_operation(b_or, b_or, tokens->type >= EQ_T && tokens->type <= EX_NEQ_T);
}

token_t *b_or(parse_res_t *res, token_t *tokens)
{
    bin_operation(b_xor, b_xor, tokens->type == B_OR_T);
}

token_t *b_xor(parse_res_t *res, token_t *tokens)
{
    bin_operation(b_and, b_and, tokens->type == B_XOR_T);
}

token_t *b_and(parse_res_t *res, token_t *tokens)
{
    bin_operation(shift, shift, tokens->type == B_AND_T);
}

token_t *shift(parse_res_t *res, token_t *tokens)
{
    bin_operation(expr, expr, tokens->type == LSHIFT_T || tokens->type == RSHIFT_T);
}

token_t *expr(parse_res_t *res, token_t *tokens)
{
    bin_operation(term, term, tokens->type == ADD_T || tokens->type == SUB_T);
}

token_t *term(parse_res_t *res, token_t *tokens)
{
    bin_operation(factor, factor, tokens->type >= MUL_T && tokens->type <= QUOT_T);
}

token_t *factor(parse_res_t *res, token_t *tokens)
{
    if (tokens->type >= ADD_T && tokens->type <= NOT_KT)
    {
        unary_operation_node_t *value = mr_alloc(sizeof(unary_operation_node_t));
        value->operator = tokens->type;

        pos_t poss = tokens++->poss;

        tokens = factor(res, tokens);
        if (!res->nodes)
        {
            mr_free(value);
            return tokens;
        }

        value->operand = res->nodes[res->size];
        set_node_peo(UNARY_OPERATION_N, value, poss);
        return tokens;
    }

    return power(res, tokens);
}

token_t *power(parse_res_t *res, token_t *tokens)
{
    bin_operation(prefix, factor, tokens->type == POW_T);
}

token_t *prefix(parse_res_t *res, token_t *tokens)
{
    if (tokens->type == INC_T || tokens->type == DEC_T)
    {
        unary_operation_node_t *value = mr_alloc(sizeof(unary_operation_node_t));
        value->operator = tokens->type;

        pos_t poss = tokens++->poss;

        tokens = prefix(res, tokens);
        if (!res->nodes)
        {
            mr_free(value);
            return tokens;
        }

        value->operand = res->nodes[res->size];
        set_node_peo(VAR_FMODIFY_N, value, poss);
        return tokens;
    }

    return postfix(res, tokens);
}

token_t *postfix(parse_res_t *res, token_t *tokens)
{
    tokens = core(res, tokens);
    if (!res->nodes)
        return tokens;

    unary_operation_node_t *value;
    while (tokens->type == INC_T || tokens->type == DEC_T)
    {
        value = mr_alloc(sizeof(unary_operation_node_t));
        value->operator = tokens->type + ADD_T - INC_T;

        value->operand = res->nodes[res->size];
        set_node_pso(VAR_FMODIFY_N, value, tokens++->pose);
    }

    return tokens;
}

token_t *core(parse_res_t *res, token_t *tokens)
{
    if (tokens->type == LPAREN_T)
    {
        pos_t poss = tokens++->poss;

        tokens = tuple(res, tokens);
        if (!res->nodes)
            return tokens;

        if (tokens->type != RPAREN_T)
        {
            node_free(res->nodes + res->size);
            set_error(invalid_syntax_set("Expected ')'", tokens->poss, tokens->pose));
            return tokens;
        }

        res->nodes[res->size].poss = poss;
        res->nodes[res->size].pose = tokens++->pose;
        return tokens;
    }

    switch (tokens->type)
    {
    case ID_T:
        set_node(VAR_ACCESS_N, tokens->value, tokens->poss, tokens++->pose);
        return tokens;
    case INT_T:
        set_node(INT_N, tokens->value, tokens->poss, tokens++->pose);
        return tokens;
    case FLOAT_T:
        set_node(FLOAT_N, tokens->value, tokens->poss, tokens++->pose);
        return tokens;
    case IMAG_T:
        set_node(IMAG_N, tokens->value, tokens->poss, tokens++->pose);
        return tokens;
    case TRUE_KT:
        set_node(BOOL_N, (void*)1, tokens->poss, tokens++->pose);
        return tokens;
    case FALSE_KT:
        set_node(BOOL_N, NULL, tokens->poss, tokens++->pose);
        return tokens;
    case LSQUARE_T:
        return handle_list(res, tokens);
    case VAR_KT:
        return handle_var(res, tokens);
    }

    set_error(invalid_syntax_set(NULL, tokens->poss, tokens->pose));
    return tokens;
}

token_t *handle_list(parse_res_t *res, token_t *tokens)
{
    pos_t poss = tokens++->poss;

    if (tokens->type == RSQUARE_T)
    {
        set_node(LIST_N, NULL, poss, tokens++->pose);
        return tokens;
    }

    tokens = modify(res, tokens);
    if (!res->nodes)
        return tokens;

    list_node_t *node = mr_alloc(sizeof(list_node_t));
    node->elements = mr_alloc(PARSE_LIST_LEN * sizeof(node_t));
    *node->elements = res->nodes[res->size];

    node->size = 1;
    uint64_t alloc = PARSE_LIST_LEN;

    while (tokens->type == COMMA_T)
    {
        if ((++tokens)->type == RSQUARE_T)
            goto ret;

        tokens = modify(res, tokens);
        if (!res->nodes)
        {
            list_node_free(node);
            return tokens;
        }

        if (node->size == alloc)
            node->elements = mr_realloc(node->elements, (alloc += PARSE_LIST_LEN) * sizeof(node_t));

        node->elements[node->size++] = res->nodes[res->size];
    }

    if (tokens->type != RSQUARE_T)
    {
        list_node_free(node);
        set_error(invalid_syntax_set("Expected ',' or ']'", tokens->poss, tokens->pose));
        return tokens;
    }

ret:
    node->elements = mr_realloc(node->elements, node->size * sizeof(node_t));
    set_node(LIST_N, node, poss, tokens++->pose);
    return tokens;
}

token_t *handle_var(parse_res_t *res, token_t *tokens)
{
    pos_t poss = tokens++->poss;

    if (tokens->type != ID_T)
    {
        set_error(invalid_syntax_set("Expected identifier", tokens->poss, tokens->pose));
        return tokens;
    }

    var_assign_node_t *value = mr_alloc(sizeof(var_assign_node_t));
    value->name = tokens++->value;

    if (tokens->type != ASSIGN_T)
    {
        value->value.type = NONE_N;
        set_node(VAR_ASSIGN_N, value, poss, tokens[-1].pose);
        return tokens;
    }

    tokens = tuple(res, ++tokens);
    if (!res->nodes)
    {
        mr_free(value->name);
        mr_free(value);
        return tokens;
    }

    value->value = res->nodes[res->size];
    set_node_peo(VAR_ASSIGN_N, value, poss);
    return tokens;
}
