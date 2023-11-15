/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_LEXER__
#define __MR_LEXER__

#include <lexer/token.h>
#include <error/error.h>

struct __LEX_RES_T
{
    token_t *tokens;            /**< List of tokens */

    illegal_char_t error;       /**< Illegal character error */
};
typedef struct __LEX_RES_T lex_res_t; /**< Result of the lex function */

/**
 * \fn lex_res_t lex(const char *code)
 * Converts raw MetaReal code into the list of tokens.
 * \param code The raw MetaReal code
 * \return
 * \pre
 * The result of analysis as a list of tokens.
 * If there was an illegal character error, the error is returned with a NULL tokens list.
 * If there wan an expected character error, the error is returned with a NULL tokens list.
*/
lex_res_t lex(const char *code);

#endif /* __MR_LEXER__ */
