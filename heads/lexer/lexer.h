/**
 * @file lexer.h
 * This file contains definitions of the lexer that
 * converts contents of the code into a list of tokens. \n
 * All things defined in \a lexer.c and this file have the \a mr_lexer prefix.
*/

#ifndef __MR_LEXER__
#define __MR_LEXER__

#include <lexer/token.h>
#include <error/error.h>

/**
 * @struct __MR_LEXER_T
 * Result of the \a mr_lexer function.
 * @var mr_token_t* __MR_LEXER_T::tokens
 * List of tokens. \n
 * If there is an error in the code (illegal character error), the \a tokens list will be NULL.
 * @var mr_illegal_chr_t __MR_LEXER_T::error
 * Illegal character error (It will be filled with data if the \a tokens is NULL).
*/
#pragma pack(push, 1)
struct __MR_LEXER_T
{
    mr_token_t *tokens;
    mr_illegal_chr_t error;
};
#pragma pack(pop)
typedef struct __MR_LEXER_T mr_lexer_t;

/**
 * It creates a list of tokens based on content of the <em>code</em>. \n
 * If there is an illegal character in the \a code or a character is missing,
 * the function returns an error.
 * @param res
 * Result of the lexer process (it contains both error and tokens list).
 * @return It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_lexer(
    mr_lexer_t *res);

#endif
