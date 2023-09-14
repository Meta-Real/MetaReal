/*
    MetaReal Compiler version 1.0.0
*/

#include <lexer/lexer.h>
#include <alloc.h>
#include <consts.h>
#include <stddef.h>

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

#define set_token_d(t1, t2, d)          \
    do                                  \
    {                                   \
        res.tokens[size].value = NULL;  \
        res.tokens[size].poss = pos;    \
                                        \
        if (code[++pos.idx] == d)       \
        {                               \
            res.tokens[size].type = t1; \
            pos.idx++;                  \
        }                               \
        else                            \
            res.tokens[size].type = t2; \
                                        \
        res.tokens[size++].pose = pos;  \
    } while (0)

#define set_token_t1(t1, t2, t3, d1, d2) \
    do                                   \
    {                                    \
        res.tokens[size].value = NULL;   \
        res.tokens[size].poss = pos;     \
                                         \
        if (code[++pos.idx] == d1)       \
        {                                \
            res.tokens[size].type = t1;  \
            pos.idx++;                   \
        }                                \
        else if (code[pos.idx] == d2)    \
        {                                \
            res.tokens[size].type = t2;  \
            pos.idx++;                   \
        }                                \
        else                             \
            res.tokens[size].type = t3;  \
                                         \
        res.tokens[size++].pose = pos;   \
    } while (0)

#define set_token_t2(t1, t2, t3, d1, d2)    \
    do                                      \
    {                                       \
        res.tokens[size].value = NULL;      \
        res.tokens[size].poss = pos;        \
                                            \
        if (code[++pos.idx] == d1)          \
        {                                   \
            if (code[++pos.idx] == d2)      \
            {                               \
                res.tokens[size].type = t1; \
                pos.idx++;                  \
            }                               \
            else                            \
                res.tokens[size].type = t2; \
        }                                   \
        else                                \
            res.tokens[size].type = t3;     \
                                            \
        res.tokens[size++].pose = pos;      \
    } while (0)

token_t handle_num(const char *code, pos_t *pos);

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

        if ((code[pos.idx] >= '0' && code[pos.idx] <= '9') || code[pos.idx] == '.')
        {
            res.tokens[size++] = handle_num(code, &pos);
            continue;
        }

        switch (code[pos.idx])
        {
        case ';':
            set_token(SEMICOLON_T);
            break;
        case '+':
            set_token(ADD_T);
            break;
        case '-':
            set_token(SUB_T);
            break;
        case '*':
            set_token_d(POW_T, MUL_T, '*');
            break;
        case '/':
            set_token_d(QUOT_T, DIV_T, '/');
            break;
        case '%':
            set_token(MOD_T);
            break;
        case '&':
            set_token_d(AND_T, B_AND_T, '|');
            break;
        case '|':
            set_token_d(OR_T, B_OR_T, '|');
            break;
        case '^':
            set_token(B_XOR_T);
            break;
        case '<':
            set_token_t1(LSHIFT_T, LTE_T, LT_T, '<', '=');
            break;
        case '>':
            set_token_t1(RSHIFT_T, GTE_T, GT_T, '>', '=');
            break;
        case '~':
            set_token(B_NOT_T);
            break;
        case '=':
            set_token_t2(EX_EQ_T, EQ_T, ASSIGN_T, '=', '=');
            break;
        case '!':
            set_token_t2(EX_NEQ_T, NEQ_T, NOT_T, '=', '=');
            break;
        case '(':
            set_token(LPAREN_T);
            break;
        case ')':
            set_token(RPAREN_T);
            break;
        default:
            while (size)
                mr_free(res.tokens[--size].value);
            mr_free(res.tokens);
            res.tokens = NULL;

            res.error = set_illegal_char(code[pos.idx], 0, pos);
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

token_t handle_num(const char *code, pos_t *pos)
{
    token_t res;
    res.type = INT_T;
    res.value = mr_alloc(LEX_NUM_SIZE);
    res.size = 0;
    res.poss = *pos;

    uint64_t alloc = LEX_NUM_SIZE;

    do
    {
        if (code[pos->idx] == '.')
        {
            if (res.type == FLOAT_T)
                break;
            res.type = FLOAT_T;
        }

        if (res.size == alloc)
            res.value = mr_realloc(res.value, alloc += LEX_NUM_SIZE);

        res.value[res.size++] = code[pos->idx++];
    } while ((code[pos->idx] >= '0' && code[pos->idx] <= '9') || code[pos->idx] == '.');

    if (code[pos->idx] == 'i')
    {
        res.type = IMAG_T;
        pos->idx++;
    }

    res.value = mr_realloc(res.value, res.size + 1);
    res.value[res.size] = '\0';
    res.pose = *pos;
    return res;
}
