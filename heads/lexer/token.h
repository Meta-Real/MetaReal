/**
 * @file token.h
 * Definitions of the tokens which are used by the lexer to organize the code. \n
 * All the functions defined in this file have the prefix "mr_token".
*/

#ifndef __MR_TOKEN__
#define __MR_TOKEN__

#include <defs.h>

/**
 * @struct __MR_TOKEN_T
 * The word equivalent of the compilation process. \n
 * The token contains all the information needed about a code element. \n
 * There are two types of tokens: \n
 * 1. Symbols: tokens that don't have a value and only their type is important. \n
 *    Examples: plus sign, equal sign, semicolon, keywords, types, etc. \n
 * 2. Words: tokens that have a value and their value can change according to the code. \n
 *    Examples: numbers (int, float, complex), strings, identifiers, etc. \n
 * Tokens only hold the information about one symbol or word. Not the grammar.
 * @var mr_byte_t __MR_TOKEN_T::type
 * The type of the token.
 * @var mr_str_t __MR_TOKEN_T::value
 * The value of the token (if the token is a word, NULL otherwise).
 * @var mr_size_t __MR_TOKEN_T::size
 * The size of the \a value (if the token is a word, undefined otherwise).
 * @var mr_pos_t __MR_TOKEN_T::poss
 * The starting position of the token.
 * @var mr_pos_t __MR_TOKEN_T::pose
 * The ending position of the token.
*/
struct __MR_TOKEN_T
{
    mr_byte_t type;
    mr_str_t value;
    mr_size_t size;

    mr_pos_t poss;
    mr_pos_t pose;
};
typedef struct __MR_TOKEN_T mr_token_t;

#endif /* __MR_TOKEN__ */
