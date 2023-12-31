/**
 * @file lexer.c
 * This file contains definitions of the \a lexer.h file.
*/

#include <lexer/lexer.h>
#include <stdlib.h>
#include <string.h>
#include <consts.h>

/**
 * @def mr_lexer_tokens_free
 * It deallocates tokens generated by the \a mr_lexer_match function. \n
 * The tokens list doesn't have to be terminated with an EOF token. \n
 * This macro should only be called from the \a mr_lexer function.
*/
#define mr_lexer_tokens_free                                                       \
    do                                                                             \
    {                                                                              \
        mr_token_t *token;                                                         \
        while (data.size--)                                                        \
        {                                                                          \
            token = data.tokens + data.size;                                       \
            if (token->type != MR_TOKEN_IDENTIFIER && token->type != MR_TOKEN_CHR) \
                free(token->value);                                                \
        }                                                                          \
        free(data.tokens);                                                         \
    } while (0)

/**
 * @def mr_lexer_tokens_realloc
 * It reallocates the \a tokens list. \n
 * This macro is used by the \a mr_lexer_generate_fstr function.
*/
#define mr_lexer_tokens_realloc                                       \
    do                                                                \
    {                                                                 \
        if (data->size == data->alloc)                                \
        {                                                             \
            block = realloc(data->tokens,                             \
                (data->alloc += data->exalloc) * sizeof(mr_token_t)); \
            if (!block)                                               \
            {                                                         \
                data->flag = MR_LEXER_MATCH_FLAG_MEMORY;              \
                return;                                               \
            }                                                         \
                                                                      \
            data->tokens = block;                                     \
        }                                                             \
    } while (0)

/**
 * @def mr_lexer_token_set(typ, inc)
 * It creates a new token with a given type. \n
 * Value of the created token is NULL (it's a symbol).
 * @param typ
 * Type of the token that needs to be created.
 * @param inc
 * Size of the token in characters (to increment the <em>pos</em>).
*/
#define mr_lexer_token_set(typ, inc) \
    do                               \
    {                                \
        token->type = typ;           \
        token->value = NULL;         \
        token->size = inc;           \
        token->idx = data->idx;      \
                                     \
        data->idx += inc;            \
        data->size++;                \
    } while (0)

/**
 * @def mr_lexer_token_setd(typ1, typ2, chr)
 * It creates a new token based on two different types. \n
 * The \a chr parameter determines which of the two should be considered as the token type. \n
 * Type1 structure: `[base][chr]` \n
 * Type2 structure: `[base]`
 * @param typ1
 * Type of the first case.
 * @param typ2
 * Type of the second case.
 * @param chr
 * Difference of the two tokens.
*/
#define mr_lexer_token_setd(typ1, typ2, chr)  \
    do                                        \
    {                                         \
        if (data->code[data->idx + 1] == chr) \
            mr_lexer_token_set(typ1, 2);      \
        else                                  \
            mr_lexer_token_set(typ2, 1);      \
    } while (0)

/**
 * @def mr_lexer_token_sett(typ1, typ2, typ3, chr1, chr2)
 * It creates a new token based on three different types. \n
 * The \a chr1 and \a chr2 parameters determine which of the
 * three should be considered as the token type. \n
 * Type1 structure: `[base][chr1]` \n
 * Type2 structure: `[base][chr2]` \n
 * Type3 structure: `[base]`
 * @param typ1
 * Type of the first case.
 * @param typ2
 * Type of the second case.
 * @param typ3
 * Type of the third case.
 * @param chr1
 * Character that determines first case.
 * @param chr2
 * Character that determines the second case.
*/
#define mr_lexer_token_sett(typ1, typ2, typ3, chr1, chr2) \
    do                                                    \
    {                                                     \
        switch (data->code[data->idx + 1])                \
        {                                                 \
        case chr1:                                        \
            mr_lexer_token_set(typ1, 2);                  \
            break;                                        \
        case chr2:                                        \
            mr_lexer_token_set(typ2, 2);                  \
            break;                                        \
        default:                                          \
            mr_lexer_token_set(typ3, 1);                  \
            break;                                        \
        }                                                 \
    } while (0)

/**
 * @def mr_lexer_token_settl(typ1, typ2, typ3, chr1, chr2)
 * It creates a new token based on three different types. \n
 * The \a chr1 and \a chr2 parameters determine which of the
 * three should be considered as the token type. \n
 * Type1 structure: `[base][chr1][chr2]` \n
 * Type2 structure: `[base][chr1]` \n
 * Type3 structure: `[base]`
 * @param typ1
 * Type of the first case.
 * @param typ2
 * Type of the second case.
 * @param typ3
 * Type of the third case.
 * @param chr1
 * Character that determines first case.
 * @param chr2
 * Character that determines the second case.
*/
#define mr_lexer_token_settl(typ1, typ2, typ3, chr1, chr2) \
    do                                                     \
    {                                                      \
        if (data->code[data->idx + 1] == chr1)             \
        {                                                  \
            if (data->code[data->idx + 2] == chr2)         \
                mr_lexer_token_set(typ1, 3);               \
            else                                           \
                mr_lexer_token_set(typ2, 2);               \
        }                                                  \
        else                                               \
            mr_lexer_token_set(typ3, 1);                   \
    } while (0)

/**
 * @def mr_lexer_token_setq(typ1, typ2, typ3, typ4, chr1, chr2, chr3)
 * It creates a new token based on four different types. \n
 * The <em>chr1</em>, <em>chr2</em>, and \a chr3 parameters determine which of the
 * four should be considered as the token type. \n
 * Type1 structure: `[base][chr1]` \n
 * Type2 structure: `[base][chr2][chr3]` \n
 * Type3 structure: `[base][chr2]` \n
 * Type4 structure: `[base]`
 * @param typ1
 * Type of the first case.
 * @param typ2
 * Type of the second case.
 * @param typ3
 * Type of the third case.
 * @param typ4
 * Type of the forth case.
 * @param chr1
 * Character that determines first case.
 * @param chr2
 * Character that determines the second and third case.
 * @param chr3
 * Character that determines the second case.
*/
#define mr_lexer_token_setq(typ1, typ2, typ3, typ4, chr1, chr2, chr3) \
    do                                                                \
    {                                                                 \
        switch (data->code[data->idx + 1])                            \
        {                                                             \
        case chr1:                                                    \
            mr_lexer_token_set(typ1, 2);                              \
            break;                                                    \
        case chr2:                                                    \
            if (data->code[data->idx + 2] == chr3)                    \
                mr_lexer_token_set(typ2, 3);                          \
            else                                                      \
                mr_lexer_token_set(typ3, 2);                          \
            break;                                                    \
        default:                                                      \
            mr_lexer_token_set(typ4, 1);                              \
            break;                                                    \
        }                                                             \
    } while (0)

/**
 * @def mr_lexer_value_realloc(size)
 * It reallocates the value block ( \a value field of \a token structure). \n
 * If the reallocation process fails, it frees the block and throws an error. \n
 * The macro must be called from a \a mr_lexer_generate function.
 * @param size
 * New size for the allocated block.
*/
#define mr_lexer_value_realloc(size)                 \
    do                                               \
    {                                                \
        block = realloc(token->value, size);         \
        if (!block)                                  \
        {                                            \
            free(token->value);                      \
                                                     \
            data->flag = MR_LEXER_MATCH_FLAG_MEMORY; \
            return;                                  \
        }                                            \
                                                     \
        token->value = block;                        \
    } while (0)

/**
 * @def mr_lexer_skip_spaces(code, idx)
 * It skips space characters (' ', '\t', and '\r') from the \a code and advances <em>idx</em>.
 * @param chr
 * Current character of the code.
 * @param code
 * The source code.
 * @param idx
 * Index of the current character ( \a idx field of \a pos structure).
*/
#define mr_lexer_skip_spaces(chr, code, idx)         \
    while (chr == ' ' || chr == '\t' || chr == '\r') \
        chr = code[++idx]

/**
 * @def mr_lexer_add_newline(prev)
 * It determines that a newline token should be added or not.
 * @param prev
 * Type of the previous token.
 * @return If a newline should be added, It returns \a MR_TRUE and \a MR_FALSE otherwise.
*/
#define mr_lexer_add_newline(prev)                                                        \
    (prev <= MR_TOKEN_STR && prev >= MR_TOKEN_IDENTIFIER) || prev == MR_TOKEN_FSTR_END || \
    prev == MR_TOKEN_R_PAREN ||                                                           \
    (prev <= MR_TOKEN_NONE_K && prev >= MR_TOKEN_TRUE_K) ||                               \
    prev == MR_TOKEN_RETURN_K ||                                                          \
    prev >= MR_TOKEN_OBJECT_T

/**
 * @def mr_lexer_escape_chr
 * It replaces characters with escape sequences
 * (used by the <em>mr_lexer_generate_chr</em>, <em>mr_lexer_generate_str</em>,
 * and \a mr_lexer_generate_fstr functions). \n
 * If the current character is a newline ('\\n'),
 * it advances \a ln field of <em>pos</em> data structure.
*/
#define mr_lexer_escape_chr \
    do                      \
    {                       \
        switch (chr)        \
        {                   \
        case '0':           \
            chr = '\0';     \
            break;          \
        case 'a':           \
            chr = '\a';     \
            break;          \
        case 'b':           \
            chr = '\b';     \
            break;          \
        case 'f':           \
            chr = '\f';     \
            break;          \
        case 'n':           \
            chr = '\n';     \
            break;          \
        case 'r':           \
            chr = '\r';     \
            break;          \
        case 't':           \
            chr = '\t';     \
            break;          \
        case 'v':           \
            chr = '\v';     \
            break;          \
        }                   \
    } while (0)

/**
 * The subroutine for \a mr_lexer_generate_str and <em>mr_lexer_generate_fstr</em> functions.
 * @param exalloc
 * Extra allocation for the token value ( \a MR_TOKEN_STR or <em>MR_TOKEN_FSTR</em>). \n
 * If size of the value reaches its allocated limit, \a exalloc is used for allocation extra memory.
*/
#define mr_lexer_str_sub(exalloc)                                          \
    do                                                                     \
    {                                                                      \
        if (chr == '\0')                                                   \
        {                                                                  \
            data->flag = MR_LEXER_MATCH_FLAG_MISSING;                      \
            data->alloc = quot;                                            \
                                                                           \
            free(token->value);                                            \
            return;                                                        \
        }                                                                  \
                                                                           \
        if (chr == '\\' && esc)                                            \
        {                                                                  \
            chr = data->code[++data->idx];                                 \
            if (chr == '\0')                                               \
            {                                                              \
                data->flag = MR_LEXER_MATCH_FLAG_MISSING;                  \
                data->alloc = quot;                                        \
                                                                           \
                free(token->value);                                        \
                return;                                                    \
            }                                                              \
                                                                           \
            mr_lexer_escape_chr;                                           \
        }                                                                  \
                                                                           \
        if (token->size == alloc)                                          \
            mr_lexer_value_realloc((alloc += exalloc) * sizeof(mr_chr_t)); \
                                                                           \
        token->value[token->size++] = chr;                                 \
        chr = data->code[++data->idx];                                     \
    } while (0)

/**
 * @struct __MR_LEXER_MATCH_T
 * Input of the \a mr_lexer_match function.
 * @var mr_byte_t __MR_LEXER_MATCH_T::flag
 * The flag indicates that the matching process succeeded or failed. \n
 * If the process succeeded, flag will be 0.
 * Otherwise, its value will be the error code (<em>__MR_LEXER_MATCH_FLAG_ENUM</em>).
 * @var mr_token_t* __MR_LEXER_MATCH_T::tokens
 * List of tokens.
 * @var mr_long_t __MR_LEXER_MATCH_T::size
 * Size of the \a tokens list.
 * @var mr_long_t __MR_LEXER_MATCH_T::alloc
 * Allocated size of the \a tokens list. \n
 * It indicates maximum number of tokens that can be stored in the current \a tokens list.
 * @var mr_long_t __MR_LEXER_MATCH_T::exalloc
 * Extra allocation size. \n
 * If \a size reaches the \a alloc limit, the \a exalloc is used for allocating extra memory.
 * @var mr_str_ct __MR_LEXER_MATCH_T::code
 * The source code.
 * @var mr_long_t __MR_LEXER_MATCH_T::idx
 * An index for the current character of the code.
*/
struct __MR_LEXER_MATCH_T
{
    mr_byte_t flag;

    mr_token_t *tokens;
    mr_long_t size;
    mr_long_t alloc;
    mr_long_t exalloc;

    mr_str_ct code;
    mr_long_t idx;
};
typedef struct __MR_LEXER_MATCH_T mr_lexer_match_t;

/**
 * @enum __MR_LEXER_MATCH_FLAG_ENUM
 * List of possible codes returned by the \a mr_lexer_match function.
 * @var __MR_LEXER_MATCH_FLAG_ENUM::MR_LEXER_MATCH_FLAG_OK
 * The matching completed successfully.
 * @var __MR_LEXER_MATCH_FLAG_ENUM::MR_LEXER_MATCH_FLAG_ILLEGAL
 * The function found an illegal character during the matching process.
 * @var __MR_LEXER_MATCH_FLAG_ENUM::MR_LEXER_MATCH_FLAG_MISSING
 * A character is missing from the code.
 * @var __MR_LEXER_MATCH_FLAG_ENUM::MR_LEXER_MATCH_FLAG_MEMORY
 * The process failed due to a memory allocation failure.
*/
enum __MR_LEXER_MATCH_FLAG_ENUM
{
    MR_LEXER_MATCH_FLAG_OK,
    MR_LEXER_MATCH_FLAG_ILLEGAL,
    MR_LEXER_MATCH_FLAG_MISSING,
    MR_LEXER_MATCH_FLAG_MEMORY
};

/**
 * It matches characters against lexer patterns and generates tokens one by one.
 * @param data
 * Data structure containing all necessary information about the code.
*/
void mr_lexer_match(
    mr_lexer_match_t *data);

/**
 * It skips comments (both singleline and multiline ones). \n
 * Singleline comment regex pattern:
 * `\#.*` \n
 * Multiline comment regex pattern:
 * `\#\*(.|\n)*?\*\#`
 * @param data
 * Data structure containing all necessary information about the code.
*/
void mr_lexer_skip_comment(
    mr_lexer_match_t *data);

/**
 * It generates an identifier, a keyword, or a type based on the code. \n
 * Identifier regex pattern:
 * `[a-zA-Z_]\w*`
 * @param data
 * Data structure containing all necessary information about the code.
*/
void mr_lexer_generate_identifier(
    mr_lexer_match_t *data);

/**
 * It generates a number (int, float, or imaginary) based on the code. \n
 * Int regex pattern:
 * `\d[0-9_]*` \n
 * Float regex pattern:
 * `((\d[0-9_]*\.|\.)[0-9_]*([eE][+-]?\d+)?|\d[0-9_]*[eE][+-]?\d+)` \n
 * Imaginary regex pattern:
 * `\d[0-9_]*\.?|\.?)[0-9_]*([eE][+-]?\d+)?i`
 * @param data
 * Data structure containing all necessary information about the code.
*/
void mr_lexer_generate_number(
    mr_lexer_match_t *data);

/**
 * It generates a character or a string based on the code.
 * @param data
 * Data structure containing all necessary information about the code.
*/
void mr_lexer_generate_chr(
    mr_lexer_match_t *data);

/**
 * It generates a string based on the code.
 * @param data
 * Data structure containing all necessary information about the code.
 * @param esc
 * If \a esc is equal to \a MR_TRUE (by using '\\' prefix), All escape sequences will be avoided.
*/
void mr_lexer_generate_str(
    mr_lexer_match_t *data, mr_bool_t esc);

/**
 * It generates a formatted string based on the code. \n
 * This function will be called by using 'f' prefix.
 * @param data
 * Data structure containing all necessary information about the code.
 * @param esc
 * If \a esc is equal to \a MR_TRUE (by using '\\' prefix), All escape sequences will be avoided.
*/
void mr_lexer_generate_fstr(
    mr_lexer_match_t *data, mr_bool_t esc);

/**
 * Generates a <em>MR_TOKEN_DOT</em>, <em>MR_TOKEN_ELLIPSIS</em>,
 * or a floating point number based on the code.
 * @param data
 * Data structure containing all necessary information about the code.
*/
void mr_lexer_generate_dot(
    mr_lexer_match_t *data);

mr_byte_t mr_lexer(
    mr_lexer_t *res, mr_str_ct code, mr_long_t alloc)
{
    mr_lexer_match_t data;
    data.flag = MR_LEXER_MATCH_FLAG_OK;

    data.tokens = malloc(alloc * sizeof(mr_token_t));
    if (!data.tokens)
        return MR_ERROR_NOT_ENOUGH_MEMORY;

    data.size = 0;
    data.alloc = alloc;
    data.exalloc = alloc;

    data.code = code;
    data.idx = 0;

    mr_chr_t chr = code[data.idx];
    while (1)
    {
        mr_lexer_skip_spaces(chr, code, data.idx);
        if (chr == '\n')
        {
            chr = code[++data.idx];
            continue;
        }
        if (chr == ';')
        {
            chr = code[++data.idx];
            continue;
        }

        if (chr == '#')
        {
            mr_lexer_skip_comment(&data);
            chr = code[data.idx];
            continue;
        }

        break;
    }

    mr_token_t *block;
    while (chr != '\0')
    {
        if (data.size == data.alloc)
        {
            block = realloc(data.tokens, (data.alloc += alloc) * sizeof(mr_token_t));
            if (!block)
            {
                mr_lexer_tokens_free;
                return MR_ERROR_NOT_ENOUGH_MEMORY;
            }

            data.tokens = block;
        }

        mr_lexer_match(&data);
        chr = code[data.idx];

        if (data.flag)
        {
            mr_lexer_tokens_free;

            if (data.flag == MR_LEXER_MATCH_FLAG_MEMORY)
                return MR_ERROR_NOT_ENOUGH_MEMORY;

            if (data.flag == MR_LEXER_MATCH_FLAG_ILLEGAL)
                res->error = (mr_illegal_chr_t){code[data.idx], MR_FALSE, data.idx};
            else
                res->error = (mr_illegal_chr_t){(mr_chr_t)data.alloc, MR_TRUE, data.idx};
            return MR_ERROR_BAD_FORMAT;
        }
    }

    if (data.size + 1 != data.alloc)
    {
        block = realloc(data.tokens, (data.size + 1) * sizeof(mr_token_t));
        if (!block)
        {
            mr_lexer_tokens_free;
            return MR_ERROR_NOT_ENOUGH_MEMORY;
        }

        data.tokens = block;
    }

    mr_token_t *token = data.tokens + data.size;
    token->type = MR_TOKEN_EOF;
    token->value = NULL;
    token->size = 1;
    token->idx = data.idx;

    res->tokens = data.tokens;
    return MR_NOERROR;
}

void mr_lexer_match(
    mr_lexer_match_t *data)
{
    mr_token_t *token = data->tokens + data->size;
    mr_chr_t chr = data->code[data->idx];

    if (chr == '#')
    {
        mr_lexer_skip_comment(data);
        chr = data->code[data->idx];
        mr_lexer_skip_spaces(chr, data->code, data->idx);
        return;
    }

    if (chr == '\n')
    {
        mr_token_t *prev = token - 1;
        if (mr_lexer_add_newline(prev->type))
        {
            mr_lexer_token_set(MR_TOKEN_NEWLINE, 1);
            chr = data->code[data->idx];
        }
        else
            chr = data->code[++data->idx];

        mr_lexer_skip_spaces(chr, data->code, data->idx);
        return;
    }

    if (chr >= '0' && chr <= '9')
    {
        mr_lexer_generate_number(data);
        if (data->flag)
            return;

        chr = data->code[data->idx];
        mr_lexer_skip_spaces(chr, data->code, data->idx);
        return;
    }

    if (chr == 'f')
    {
        mr_bool_t esc = MR_TRUE;

        chr = data->code[data->idx + 1];
        if (chr == '\\')
        {
            esc = MR_FALSE;
            chr = data->code[data->idx + 2];
        }

        if (chr == '\'' || chr == '"')
        {
            mr_lexer_generate_fstr(data, esc);
            if (data->flag)
                return;

            chr = data->code[data->idx];
            mr_lexer_skip_spaces(chr, data->code, data->idx);
            return;
        }
        else if (!esc)
        {
            data->idx++;
            data->flag = MR_LEXER_MATCH_FLAG_ILLEGAL;
            return;
        }

        mr_lexer_generate_identifier(data);
        if (data->flag)
            return;

        chr = data->code[data->idx];
        mr_lexer_skip_spaces(chr, data->code, data->idx);
        return;
    }

    if ((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') || chr == '_')
    {
        mr_lexer_generate_identifier(data);
        if (data->flag)
            return;

        chr = data->code[data->idx];
        mr_lexer_skip_spaces(chr, data->code, data->idx);
        return;
    }

    switch (chr)
    {
    case ';':
        if ((token - 1)->type == MR_TOKEN_NEWLINE)
        {
            data->idx++;
            break;
        }

        mr_lexer_token_set(MR_TOKEN_NEWLINE, 1);
        break;
    case '\\':
        switch (data->code[data->idx + 1])
        {
        case 'f':
            chr = data->code[data->idx + 2];
            if (chr == '\'' || chr == '"')
            {
                mr_lexer_generate_fstr(data, MR_FALSE);
                if (data->flag)
                    return;

                break;
            }

            break;
        case '\'':
        case '"':
            mr_lexer_generate_str(data, MR_FALSE);
            if (data->flag)
                return;

            break;
        default:
            data->flag = MR_LEXER_MATCH_FLAG_ILLEGAL;
            return;
        }

        break;
    case '\'':
        mr_lexer_generate_chr(data);
        if (data->flag)
            return;

        break;
    case '"':
        mr_lexer_generate_str(data, MR_TRUE);
        if (data->flag)
            return;

        break;
    case '+':
        mr_lexer_token_sett(
            MR_TOKEN_INCREMENT, MR_TOKEN_PLUS_ASSIGN,
            MR_TOKEN_PLUS,
            '+', '=');
        break;
    case '-':
        switch (data->code[data->idx + 1])
        {
        case '=':
            mr_lexer_token_set(MR_TOKEN_MINUS_ASSIGN, 2);
            break;
        case '-':
            mr_lexer_token_set(MR_TOKEN_DECREMENT, 2);
            break;
        case '>':
            mr_lexer_token_set(MR_TOKEN_LINK, 2);
            break;
        default:
            mr_lexer_token_set(MR_TOKEN_MINUS, 1);
            break;
        }
        break;
    case '*':
        mr_lexer_token_setq(
            MR_TOKEN_MULTIPLY_ASSIGN, MR_TOKEN_POWER_ASSIGN,
            MR_TOKEN_POWER, MR_TOKEN_MULTIPLY,
            '=', '*', '=');
        break;
    case '/':
        mr_lexer_token_setq(
            MR_TOKEN_DIVIDE_ASSIGN, MR_TOKEN_QUOTIENT_ASSIGN,
            MR_TOKEN_QUOTIENT, MR_TOKEN_DIVIDE,
            '=', '/', '=');
        break;
    case '%':
        mr_lexer_token_setd(
            MR_TOKEN_MODULO_ASSIGN, MR_TOKEN_MODULO,
            '=');
        break;
    case '&':
        mr_lexer_token_sett(
            MR_TOKEN_AND_K, MR_TOKEN_B_AND_ASSIGN,
            MR_TOKEN_B_AND,
            '&', '=');
        break;
    case '|':
        mr_lexer_token_sett(
            MR_TOKEN_OR_K, MR_TOKEN_B_OR_ASSIGN,
            MR_TOKEN_B_OR,
            '|', '=');
        break;
    case '^':
        mr_lexer_token_setd(
            MR_TOKEN_B_XOR_ASSIGN, MR_TOKEN_B_XOR,
            '=');
        break;
    case '~':
        mr_lexer_token_set(MR_TOKEN_B_NOT, 1);
        break;
    case '=':
        mr_lexer_token_settl(
            MR_TOKEN_EX_EQUAL, MR_TOKEN_EQUAL,
            MR_TOKEN_ASSIGN,
            '=', '=');
        break;
    case '!':
        mr_lexer_token_settl(
            MR_TOKEN_EX_NEQUAL, MR_TOKEN_NEQUAL,
            MR_TOKEN_NOT_K,
            '=', '=');
        break;
    case '<':
        mr_lexer_token_setq(
            MR_TOKEN_LESS_EQUAL, MR_TOKEN_L_SHIFT_ASSIGN,
            MR_TOKEN_L_SHIFT, MR_TOKEN_LESS,
            '=', '<', '=');
        break;
    case '>':
        mr_lexer_token_setq(
            MR_TOKEN_GREATER_EQUAL, MR_TOKEN_R_SHIFT_ASSIGN,
            MR_TOKEN_R_SHIFT, MR_TOKEN_GREATER,
            '=', '>', '=');
        break;
    case '(':
        mr_lexer_token_set(MR_TOKEN_L_PAREN, 1);
        break;
    case ')':
        mr_lexer_token_set(MR_TOKEN_R_PAREN, 1);
        break;
    case '[':
        mr_lexer_token_set(MR_TOKEN_L_SQUARE, 1);
        break;
    case ']':
        mr_lexer_token_set(MR_TOKEN_R_SQUARE, 1);
        break;
    case '{':
        mr_lexer_token_set(MR_TOKEN_L_CURLY, 1);
        break;
    case '}':
        mr_lexer_token_set(MR_TOKEN_R_CURLY, 1);
        break;
    case ',':
        mr_lexer_token_set(MR_TOKEN_COMMA, 1);
        break;
    case '.':
        mr_lexer_generate_dot(data);
        if (data->flag)
            return;

        break;
    case ':':
        mr_lexer_token_set(MR_TOKEN_COLON, 1);
        break;
    case '?':
        mr_lexer_token_set(MR_TOKEN_QUESTION, 1);
        break;
    case '$':
        mr_lexer_token_set(MR_TOKEN_DOLLAR, 1);
        break;
    default:
        data->flag = MR_LEXER_MATCH_FLAG_ILLEGAL;
        return;
    }

    chr = data->code[data->idx];
    mr_lexer_skip_spaces(chr, data->code, data->idx);
}

void mr_lexer_skip_comment(
    mr_lexer_match_t *data)
{
    mr_chr_t chr = data->code[++data->idx];

    if (chr != '*')
    {
        while (chr != '\0' && chr != '\n')
            chr = data->code[++data->idx];

        return;
    }

    chr = data->code[++data->idx];
    while (chr != '\0')
    {
        if (chr == '*')
        {
            chr = data->code[++data->idx];
            if (chr == '#')
            {
                data->idx++;
                return;
            }
        }

        chr = data->code[++data->idx];
    }
}

void mr_lexer_generate_identifier(
    mr_lexer_match_t *data)
{
    mr_token_t *token = data->tokens + data->size;

    token->value = (mr_str_t)(data->code + data->idx);
    token->size = 0;
    token->idx = data->idx;

    mr_chr_t chr;
    do
    {
        token->size++;
        chr = data->code[++data->idx];
    } while ((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') ||
        (chr >= '0' && chr <= '9') || chr == '_');

    mr_byte_t i;
    if (token->size <= MR_TOKEN_KEYWORD_MAXSIZE)
    {
        for (i = 0; i != MR_TOKEN_KEYWORD_COUNT; i++)
            if (token->size == mr_token_keyword_size[i] &&
                !memcmp(token->value, mr_token_keyword[i], token->size))
            {
                token->type = i + MR_TOKEN_KEYWORD_PAD;
                token->value = NULL;

                data->size++;
                return;
            }

        if (token->size <= MR_TOKEN_TYPE_MAXSIZE)
            for (i = 0; i != MR_TOKEN_TYPE_COUNT; i++)
                if (token->size == mr_token_type_size[i] &&
                    !memcmp(token->value, mr_token_type[i], token->size))
                {
                    token->type = i + MR_TOKEN_TYPE_PAD;
                    token->value = NULL;

                    data->size++;
                    return;
                }
    }

    token->type = MR_TOKEN_IDENTIFIER;
    data->size++;
}

void mr_lexer_generate_number(
    mr_lexer_match_t *data)
{
    mr_token_t *token = data->tokens + data->size;

    token->value = malloc(MR_LEXER_NUMBER_SIZE * sizeof(mr_chr_t));
    if (!token->value)
    {
        data->flag = MR_LEXER_MATCH_FLAG_MEMORY;
        return;
    }

    token->type = MR_TOKEN_INT;
    token->size = 0;
    token->idx = data->idx;

    mr_bool_t is_float = MR_FALSE;
    mr_short_t alloc = MR_LEXER_NUMBER_SIZE;

    mr_str_t block;
    mr_chr_t chr = data->code[data->idx];
    do
    {
        if (chr == '_')
        {
            chr = data->code[++data->idx];
            continue;
        }

        if (chr == '.')
        {
            if (is_float)
                break;

            token->type = MR_TOKEN_FLOAT;
            is_float = MR_TRUE;
        }
        else if (chr < '0' || chr > '9')
            break;

        if (token->size == alloc)
            mr_lexer_value_realloc((alloc += MR_LEXER_NUMBER_SIZE) * sizeof(mr_chr_t));

        token->value[token->size++] = chr;
        chr = data->code[++data->idx];
    } while (1);

    if (chr == 'e' || chr == 'E')
    {
        if (token->size == alloc)
            mr_lexer_value_realloc((alloc += MR_LEXER_NUMBER_EXP_SIZE) * sizeof(mr_chr_t));

        token->value[token->size++] = 'e';
        chr = data->code[++data->idx];
        if (chr == '+')
            chr = data->code[++data->idx];
        else if (chr == '-')
        {
            if (token->size == alloc)
                mr_lexer_value_realloc((alloc += MR_LEXER_NUMBER_EXP_SIZE) * sizeof(mr_chr_t));

            token->value[token->size++] = '-';
            chr = data->code[++data->idx];
        }

        while (chr >= '0' && chr <= '9')
        {
            if (token->size == alloc)
                mr_lexer_value_realloc((alloc += MR_LEXER_NUMBER_EXP_SIZE) * sizeof(mr_chr_t));

            token->value[token->size++] = chr;
            chr = data->code[++data->idx];
        }

        token->type = MR_TOKEN_FLOAT;
    }

    if (token->size + 1 != alloc)
        mr_lexer_value_realloc(token->size + 1);

    token->value[token->size] = '\0';

    if (data->code[data->idx] == 'i')
    {
        token->type = MR_TOKEN_IMAGINARY;
        data->idx++;
    }

    token->size = (mr_short_t)(data->idx - token->idx);
    data->size++;
}

void mr_lexer_generate_chr(
    mr_lexer_match_t *data)
{
    mr_chr_t chr = data->code[data->idx + 1];
    if (chr != '\\')
    {
        if (data->code[data->idx + 2] != '\'')
        {
            mr_lexer_generate_str(data, MR_TRUE);
            return;
        }

        mr_token_t *token = data->tokens + data->size++;

        token->type = MR_TOKEN_CHR;
        token->value = (mr_str_t)(uintptr_t)chr;
        token->size = 3;
        token->idx = data->idx;

        data->idx += 3;
        return;
    }

    if (data->code[data->idx + 3] != '\'')
    {
        mr_lexer_generate_str(data, MR_TRUE);
        return;
    }

    mr_token_t *token = data->tokens + data->size++;

    token->type = MR_TOKEN_CHR;
    token->idx = data->idx;
    token->size = 4;

    chr = data->code[data->idx += 2];
    mr_lexer_escape_chr;
    token->value = (mr_str_t)(uintptr_t)chr;

    data->idx += 2;
}

void mr_lexer_generate_str(
    mr_lexer_match_t *data, mr_bool_t esc)
{
    mr_token_t *token = data->tokens + data->size;
    token->type = MR_TOKEN_STR;
    token->idx = data->idx;

    if (!esc)
        data->idx++;

    mr_chr_t quot = data->code[data->idx++];
    mr_chr_t chr = data->code[data->idx];
    if (chr == quot)
    {
        token->value = NULL;
        token->size = (mr_short_t)(++data->idx - token->idx);

        data->size++;
        return;
    }

    token->value = malloc(MR_LEXER_STR_SIZE * sizeof(mr_chr_t));
    if (!token->value)
    {
        data->flag = MR_LEXER_MATCH_FLAG_MEMORY;
        return;
    }

    token->size = 0;
    mr_short_t alloc = MR_LEXER_STR_SIZE;

    mr_str_t block;
    do
        mr_lexer_str_sub(MR_LEXER_STR_SIZE);
    while (chr != quot);

    if (token->size + 1 != alloc)
        mr_lexer_value_realloc(token->size + 1);

    token->value[token->size] = '\0';
    token->size = (mr_short_t)(++data->idx - token->idx);
    data->size++;
}

void mr_lexer_generate_fstr(
    mr_lexer_match_t *data, mr_bool_t esc)
{
    mr_long_t fidx = data->size++;
    mr_token_t *token = data->tokens + fidx;

    token->type = MR_TOKEN_FSTR_START;
    token->value = NULL;
    token->idx = data->idx;

    if (!esc)
        data->idx += 2;
    else
        data->idx++;

    mr_chr_t quot = data->code[data->idx++];
    mr_chr_t chr = data->code[data->idx];

    mr_ptr_t block;
    if (chr == quot)
    {
        data->idx++;
        token->idx = data->idx;

        mr_lexer_tokens_realloc;

        token = data->tokens + data->size++;
        token->type = MR_TOKEN_FSTR_END;
        token->value = NULL;
        token->size = (mr_short_t)(data->idx - token->idx);
        return;
    }

    mr_short_t alloc;
    mr_long_t lcurly_count = 0;
    do
    {
        if (chr == '\0')
        {
            data->flag = MR_LEXER_MATCH_FLAG_MISSING;
            data->alloc = quot;
            return;
        }

        if (chr == '{')
        {
            chr = data->code[++data->idx];
            while (chr != '}' || lcurly_count)
            {
                if (data->code[data->idx] == '\0')
                {
                    data->flag = MR_LEXER_MATCH_FLAG_MISSING;
                    data->alloc = '}';
                    return;
                }

                mr_lexer_tokens_realloc;

                mr_lexer_match(data);
                if (data->flag)
                    return;

                token = data->tokens + data->size - 1;
                if (token->type == MR_TOKEN_L_CURLY)
                    lcurly_count++;
                else if (token->type == MR_TOKEN_R_CURLY)
                    lcurly_count--;

                chr = data->code[data->idx];
            }

            chr = data->code[++data->idx];
            if (chr == quot)
                break;
            continue;
        }

        mr_lexer_tokens_realloc;

        token = data->tokens + data->size;
        token->type = MR_TOKEN_FSTR;

        token->value = malloc(MR_LEXER_FSTR_SIZE * sizeof(mr_chr_t));
        if (!token->value)
        {
            data->flag = MR_LEXER_MATCH_FLAG_MEMORY;
            return;
        }

        token->size = 0;
        alloc = MR_LEXER_FSTR_SIZE;
        do
            mr_lexer_str_sub(MR_LEXER_FSTR_SIZE);
        while (chr != quot && chr != '{');

        if (token->size + 1 != alloc)
            mr_lexer_value_realloc(token->size + 1);

        token->value[token->size] = '\0';
        data->size++;
    } while (chr != quot);

    token = data->tokens + fidx;
    token->size = (mr_short_t)(data->idx - token->idx);

    mr_lexer_tokens_realloc;

    token = data->tokens + data->size++;
    token->type = MR_TOKEN_FSTR_END;
    token->value = NULL;
}

void mr_lexer_generate_dot(
    mr_lexer_match_t *data)
{
    mr_chr_t chr = data->code[data->idx + 1];

    if (chr >= '0' && chr <= '9')
    {
        mr_lexer_generate_number(data);
        return;
    }

    mr_token_t *token = data->tokens + data->size;
    if (chr == '.' && data->code[data->idx + 2] == '.')
        mr_lexer_token_set(MR_TOKEN_ELLIPSIS, 3);
    else
        mr_lexer_token_set(MR_TOKEN_DOT, 1);
}
