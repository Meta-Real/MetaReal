/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_LEXER__
#define __MR_LEXER__

#include <lexer/token.h>
#include <error/error.h>

struct __LEX_RES_T
{
    token_t *tokens;

    illegal_char_t error;
};
typedef struct __LEX_RES_T lex_res_t;

/**
 * \fn lex_res_t lex(const char *code)
 * Main function for converting raw code string into tokens
 * \param code The raw MetaReal code
 * \return res The result of analysis as a list of tokens. If there was an IllegalCharacterError, The error is returned with a NULL tokens list.
*/
lex_res_t lex(const char *code);

#endif /* __MR_LEXER__ */
