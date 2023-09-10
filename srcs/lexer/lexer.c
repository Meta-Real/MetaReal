/*
    MetaReal Compiler version 1.0.0
*/

#include <lexer/lexer.h>
#include <alloc.h>
#include <consts.h>

#define set_token(t)                   \
    do                                 \
    {                                  \
        res.tokens[size].type = t;     \
        res.tokens[size].value = NULL; \
                                       \
        res.tokens[size].poss = pos;   \
        pos.idx++;                     \
        res.tokens[size++].pose = pos; \
    } while (0)

token_t handle_int(const char *code, pos_t *pos);

lex_res_t lex(const char *code)
{
    lex_res_t res;
    res.tokens = mr_alloc(LEX_TOKEN_LIST_LEN * sizeof(token_t));

    uint64_t size = 0;
    uint64_t alloc = LEX_TOKEN_LIST_LEN;

    pos_t pos = set_pos(0, 1);
    while (code[pos.idx])
    {
        if (code[pos.idx] == ' ' || code[pos.idx] == '\t')
        {
            pos.idx++;
            continue;
        }

        if (size == alloc)
            res.tokens = mr_realloc(res.tokens, (alloc += LEX_TOKEN_LIST_LEN) * sizeof(token_t));

        if (code[pos.idx] >= '0' && code[pos.idx] <= '9')
        {
            res.tokens[size++] = handle_int(code, &pos);
            continue;
        }

        switch (code[pos.idx])
        {
        case '+':
            set_token(ADD_T);
            break;
        case '-':
            set_token(SUB_T);
            break;
        case '*':
            set_token(MUL_T);
            break;
        default:
            while (size)
                mr_free(res.tokens[--size].value);
            mr_free(res.tokens);
            res.tokens = NULL;

            res.error = set_illegal_char(code[pos.idx], pos);
            return res;
        }
    }

    res.tokens = mr_realloc(res.tokens, (size + 1) * sizeof(token_t));

    res.tokens[size].type = EOF_T;
    res.tokens[size].poss = pos;
    pos.idx++;
    res.tokens[size].pose = pos;

    return res;
}

token_t handle_int(const char *code, pos_t *pos)
{
    token_t res;
    res.type = INT_T;
    res.value = mr_alloc(LEX_NUM_SIZE);
    res.size = 0;
    res.poss = *pos;

    uint64_t alloc = LEX_NUM_SIZE;

    do
    {
        if (res.size == alloc)
            res.value = mr_realloc(res.value, alloc += LEX_NUM_SIZE);

        res.value[res.size++] = code[pos->idx++];
    } while (code[pos->idx] >= '0' && code[pos->idx] <= '9');

    res.value = mr_realloc(res.value, res.size + 1);
    res.value[res.size] = '\0';
    res.pose = *pos;

    return res;
}
