/**
 * @file lexer.h
 * This file contains the definitions of the lexer that
 * converts the content of the code into a list of tokens. \n
 * All the things defined in \a lexer.c and this file have the prefix <em>mr_lexer</em>.
*/

#ifndef __MR_LEXER__
#define __MR_LEXER__

#include <lexer/token.h>
#include <error/error.h>
#include <stdio.h>

/**
 * @struct __MR_LEXER_T
 * The result of the \a mr_lexer function.
 * @var mr_token_t* __MR_LEXER_T::tokens
 * The list of tokens. \n
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
 * It creates the list of tokens based on the <em>code</em>. \n
 * If there is an illegal character in the \a code or a character is missing,
 * the function returns an error. \n
 * The function reads the file in chunks in order to save memory for other processes. \n
 * @param res
 * The result of the lexer process (it contains both the error and the tokens list).
 * @param code
 * The source code.
 * @return It returns the code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_lexer(mr_lexer_t *res, mr_str_ct code);

#endif /* __MR_LEXER__ */
