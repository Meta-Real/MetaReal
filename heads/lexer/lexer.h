/**
 * @file lexer.h
 * This file contains the definitions of the lexer that
 * converts the content of the code into a list of tokens.
*/

#ifndef __MR_LEXER__
#define __MR_LEXER__

#include <lexer/token.h>
#include <error/error.h>
#include <stdio.h>

/**
 * @struct __MR_LEXER_T
 * The result of the \code mr_lexer \endcode function.
 * @var mr_token_t* __MR_LEXER_T::tokens
 * The list of the tokens. \n
 * If there is an error in the code (illegal character error), the tokens list will be NULL.
 * @var mr_illegal_chr_t __MR_LEXER_T::error
 * The illegal character error (It will be filled if the \a tokens is NULL).
*/
struct __MR_LEXER_T
{
    mr_token_t *tokens;
    mr_illegal_chr_t error;
};
typedef struct __MR_LEXER_T mr_lexer_t;

/**
 * @fn void mr_lexer(mr_lexer_t *res, FILE *code, mr_size_t size)
 * It creates the list of the tokens based on the code. \n
 * If there is an illegal character in the code or a character is missing,
 * the function returns an error. \n
 * The function reads the file in chunks in order to save memory for other processes. \n
 * @param res
 * The result of the lexer process (it contains both the error and the tokens list).
 * @param code
 * The file that contains the code.
 * @param size
 * The size of the \a code.
*/
void mr_lexer(mr_lexer_t *res, FILE *code, mr_size_t size);

#endif /* __MR_LEXER__ */
