/*
    MetaReal Compiler version 1.0.0
*/

#include <parser/parser.h>
#include <alloc.h>
#include <stddef.h>
#include <consts.h>

#define set_node(t, v, ps, pe)           \
    do                                   \
    {                                    \
        res->nodes[res->size].type = t;  \
        res->nodes[res->size].value = v; \
        res->nodes[res->size].poss = ps; \
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
        free_nodes(res->nodes, res->size); \
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
                free_node(&left);                           \
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

token_t *expr(parse_res_t *res, token_t *tokens);
token_t *term(parse_res_t *res, token_t *tokens);
token_t *factor(parse_res_t *res, token_t *tokens);
token_t *core(parse_res_t *res, token_t *tokens);

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

        tokens = expr(&res, tokens);
        if (!res.nodes)
        {
            free_tokens(tokens);
            mr_free(ptr);
            return res;
        }

        res.size++;
    } while (tokens->type == SEMICOLON_T);

    if (tokens->type)
    {
        res.error = set_invalid_syntax("Expected EOF", tokens->poss, tokens->pose);

        free_nodes(res.nodes, res.size);
        res.nodes = NULL;

        free_tokens(tokens);
        mr_free(ptr);
        return res;
    }

    mr_free(ptr);

    if (!res.size)
    {
        mr_free(res.nodes);
        return res;
    }

    res.nodes = mr_realloc(res.nodes, res.size * sizeof(node_t));
    return res;
}

token_t *expr(parse_res_t *res, token_t *tokens)
{
    bin_operation(term, term, tokens->type == ADD_T || tokens->type == SUB_T);
}

token_t *term(parse_res_t *res, token_t *tokens)
{
    bin_operation(factor, factor, tokens->type == MUL_T || tokens->type == DIV_T);
}

token_t *factor(parse_res_t *res, token_t *tokens)
{
    if (tokens->type == ADD_T || tokens->type == SUB_T)
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

    return core(res, tokens);
}

token_t *core(parse_res_t *res, token_t *tokens)
{
    if (tokens->type == LPAREN_T)
    {
        pos_t poss = tokens++->poss;

        tokens = expr(res, tokens);
        if (!res->nodes)
            return tokens;

        if (tokens->type != RPAREN_T)
        {
            free_node(res->nodes + res->size);
            set_error(set_invalid_syntax("Expected ')'", tokens->poss, tokens->pose));
            return tokens;
        }

        res->nodes[res->size].poss = poss;
        res->nodes[res->size].pose = tokens++->pose;
        return tokens;
    }

    switch (tokens->type)
    {
    case INT_T:
        set_node(INT_N, tokens->value, tokens->poss, tokens->pose);
        return ++tokens;
    case FLOAT_T:
        set_node(FLOAT_N, tokens->value, tokens->poss, tokens->pose);
        return ++tokens;
    }

    set_error(set_invalid_syntax(NULL, tokens->poss, tokens->pose));
    return tokens;
}
