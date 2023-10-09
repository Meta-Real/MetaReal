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
        switch (code[++pos.idx])         \
        {                                \
        case d1:                         \
            res.tokens[size].type = t1;  \
            pos.idx++;                   \
            break;                       \
        case d2:                         \
            res.tokens[size].type = t2;  \
            pos.idx++;                   \
            break;                       \
        default:                         \
            res.tokens[size].type = t3;  \
            break;                       \
        }                                \
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

#define set_token_q(t1, t2, t3, t4, d1, d2, d3) \
    do                                          \
    {                                           \
        res.tokens[size].value = NULL;          \
        res.tokens[size].poss = pos;            \
                                                \
        switch (code[++pos.idx])                \
        {                                       \
        case d1:                                \
            if (code[++pos.idx] == d2)          \
            {                                   \
                res.tokens[size].type = t1;     \
                pos.idx++;                      \
                break;                          \
            }                                   \
                                                \
            res.tokens[size].type = t2;         \
            break;                              \
        case d3:                                \
            res.tokens[size].type = t3;         \
            pos.idx++;                          \
            break;                              \
        default:                                \
            res.tokens[size].type = t4;         \
            break;                              \
        }                                       \
                                                \
        res.tokens[size++].pose = pos;          \
    } while (0)

#define KEYWORD_MAX_LEN 5
#define KEYWORDS_LEN 7
#define KEYWORD_PAD NOT_KT

void process_id(token_t *token, const char *code, pos_t *pos);
void process_num(token_t *token, const char *code, pos_t *pos);
void process_sub(token_t *token, const char *code, pos_t *pos);

uint8_t check_id(const char *id, uint64_t len);

const char *keywords[KEYWORDS_LEN] =
{
    "not", "and", "or",
    "var",
    "const",
    "true", "false"
};

uint8_t keyword_lens[KEYWORDS_LEN] =
{
    3, 3, 2,
    3,
    5,
    4, 5
};

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

        if ((code[pos.idx] >= 'a' && code[pos.idx] <= 'z') || (code[pos.idx] >= 'A' && code[pos.idx] <= 'Z')
            || code[pos.idx] == '_')
        {
            process_id(res.tokens + size++, code, &pos);
            continue;
        }
        if ((code[pos.idx] >= '0' && code[pos.idx] <= '9') || code[pos.idx] == '.')
        {
            process_num(res.tokens + size++, code, &pos);
            continue;
        }

        switch (code[pos.idx])
        {
        case ';':
            set_token(SEMICOLON_T);
            break;
        case '+':
            set_token_t1(INC_T, ADD_EQ_T, ADD_T, '+', '=');
            break;
        case '-':
            process_sub(res.tokens + size++, code, &pos);
            break;
        case '*':
            set_token_q(POW_EQ_T, POW_T, MUL_EQ_T, MUL_T, '*', '=', '=');
            break;
        case '/':
            set_token_q(QUOT_EQ_T, QUOT_T, DIV_EQ_T, DIV_T, '/', '=', '=');
            break;
        case '%':
            set_token_d(MOD_EQ_T, MOD_T, '=');
            break;
        case '&':
            set_token_t1(AND_KT, B_AND_T, B_AND_T, '&', '=');
            break;
        case '|':
            set_token_t1(AND_KT, B_OR_EQ_T, B_OR_T, '|', '=');
            break;
        case '^':
            set_token_d(B_XOR_EQ_T, B_XOR_T, '=');
            break;
        case '<':
            set_token_q(LSHIFT_EQ_T, LSHIFT_T, LTE_T, LT_T, '<', '=', '=');
            break;
        case '>':
            set_token_q(RSHIFT_EQ_T, RSHIFT_T, GTE_T, GT_T, '>', '=', '=');
            break;
        case '~':
            set_token(B_NOT_T);
            break;
        case '=':
            set_token_t2(EX_EQ_T, EQ_T, ASSIGN_T, '=', '=');
            break;
        case '!':
            set_token_t2(EX_NEQ_T, NEQ_T, NOT_KT, '=', '=');
            break;
        case '(':
            set_token(LPAREN_T);
            break;
        case ')':
            set_token(RPAREN_T);
            break;
        case '[':
            set_token(LSQUARE_T);
            break;
        case ']':
            set_token(RSQUARE_T);
            break;
        case ',':
            set_token(COMMA_T);
            break;
        default:
            while (size)
                mr_free(res.tokens[--size].value);
            mr_free(res.tokens);
            res.tokens = NULL;

            res.error = illegal_char_set(code[pos.idx], 0, pos);
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

void process_id(token_t *token, const char *code, pos_t *pos)
{
    token->value = mr_alloc(LEX_ID_SIZE);
    token->poss = *pos;

    uint64_t size = 0;
    uint64_t alloc = LEX_ID_SIZE;
    do
    {
        if (size == alloc)
            token->value = mr_realloc(token->value, alloc += LEX_ID_SIZE);

        token->value[size++] = code[pos->idx++];
    } while ((code[pos->idx] >= 'a' && code[pos->idx] <= 'z') ||
        (code[pos->idx] >= 'A' && code[pos->idx] <= 'Z') ||
        (code[pos->idx] >= '0' && code[pos->idx] <= '9') || code[pos->idx] == '_');

    token->type = check_id(token->value, size);
    if (token->type != ID_T)
    {
        mr_free(token->value);
        token->value = NULL;
    }
    else
    {
        token->value = mr_realloc(token->value, size + 1);
        token->value[size] = '\0';
    }

    token->pose = *pos;
}

void process_num(token_t *token, const char *code, pos_t *pos)
{
    token->type = INT_T;
    token->value = mr_alloc(LEX_NUM_SIZE);
    token->poss = *pos;

    uint64_t size = 0;
    uint64_t alloc = LEX_NUM_SIZE;
    do
    {
        if (code[pos->idx] == '.')
        {
            if (token->type == FLOAT_T)
                break;
            token->type = FLOAT_T;
        }

        if (size == alloc)
            token->value = mr_realloc(token->value, alloc += LEX_NUM_SIZE);

        token->value[size++] = code[pos->idx++];
    } while ((code[pos->idx] >= '0' && code[pos->idx] <= '9') || code[pos->idx] == '.');

    if (code[pos->idx] == 'i')
    {
        token->type = IMAG_T;
        pos->idx++;
    }

    token->value = mr_realloc(token->value, size + 1);
    token->value[size] = '\0';
    token->pose = *pos;
}

void process_sub(token_t *token, const char *code, pos_t *pos)
{
    token->value = NULL;
    token->poss = *pos;

    switch (code[++pos->idx])
    {
    case '-':
        token->type = DEC_T;
        break;
    case '>':
        token->type = LINK_T;
        break;
    case '=':
        token->type = SUB_EQ_T;
        break;
    default:
        token->type = SUB_T;
        token->pose = *pos;
        return;
    }

    pos->idx++;
    token->pose = *pos;
}

uint8_t check_id(const char *id, uint64_t len)
{
    if (len > KEYWORD_MAX_LEN)
        return ID_T;

    uint8_t j;
    for (uint8_t i = 0; i < KEYWORDS_LEN; i++)
    {
        if (len != keyword_lens[i])
            continue;

        for (j = 0; j < len; j++)
            if (id[j] != keywords[i][j])
                break;

        if (j == len)
            return i + KEYWORD_PAD;
    }

    return ID_T;
}
