/*
MIT License

Copyright (c) 2023 MetaReal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

/**
 * @file token.h
 * Definitions of the token data structure which is used by the lexer to organize code. \n
 * All things defined in \a token.c and this file have the \a mr_token prefix.
*/

#ifndef __MR_TOKEN__
#define __MR_TOKEN__

#include <defs.h>

/**
 * @struct __MR_TOKEN_T
 * Word equivalent in the compilation process. \n
 * The token contains all the information needed about a code element. \n
 * There are two types of tokens: \n
 * 1. Symbols: tokens that don't have a value and only their type is important. \n
 *    Examples: plus sign, equal sign, semicolon, keywords, types, etc. \n
 * 2. Words: tokens that have a value and their value can change according to the code. \n
 *    Examples: numbers (int, float, complex), strings, identifiers, etc. \n
 * Tokens only hold the information about one symbol or word at a time. Not the grammar.
 * @var mr_idx_t __MR_TOKEN_T::idx
 * Starting index of the token (low part).
 * @var mr_byte_t __MR_TOKEN_T::type
 * Type of the token.
*/
struct __MR_TOKEN_T
{
    mr_idx_t idx;
    mr_byte_t type;
};
typedef struct __MR_TOKEN_T mr_token_t;

/**
 * @enum __MR_TOKEN_ENUM
 * List of valid token types. \n
 * Note: <em>MR_TOKEN_PLUS</em>, <em>MR_TOKEN_MINUS<em>,
 * <em>MR_TOKEN_B_NOT</em>, and <em>MR_TOKEN_NOT_K</em>
 * are placed together for performance reasons (see the \a mr_parser_factor function).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EOF
 * <em>end of file</em> token type. \n
 * This token must be present at the end of \a tokens list (null terminator).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_NEWLINE
 * \a newline token type ('\\n' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_SEMICOLON
 * \a semicolon token type (';' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IDENTIFIER
 * \a identifier token type. The \a identifier token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INT
 * \a integer token type. The \a integer token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FLOAT
 * <em>float number</em> token type. The <em>float number</em> token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IMAGINARY
 * <em>imaginary number</em> token type. The <em>imaginary number</em> token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CHR
 * \a character token type. The \a character token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STR
 * \a string token type. The \a string token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FSTR
 * <em>formatted string</em> token type. The <em>formatted string</em> token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FSTR_START
 * <em>start of the formatted string</em> token type. \n
 * This token contains the information about the start and end position of the string.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FSTR_END
 * <em>end of the formatted string</em> token type. \n
 * This token only indicates the end of the string. Not its ending position.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PLUS
 * \a plus token type ('+' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MINUS
 * \a minus token type ('-' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MULTIPLY
 * \a multiply token type ('*' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DIVIDE
 * \a divide token type ('/' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MODULO
 * \a modulo token type ('%' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_QUOTIENT
 * \a quotient token type ('//' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_POWER
 * \a power token type ('**' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_AND
 * <em>binary and</em> token type ('&' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_OR
 * <em>binary or</em> token type ('|' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_XOR
 * <em>binary xor</em> token type ('^' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_SHIFT
 * <em>left shift</em> token type ('<<' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_SHIFT
 * <em>right shift</em> token type ('>>' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_NOT
 * <em>binary not</em> token type ('~' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EQUAL
 * \a equal token type ('==' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_NEQUAL
 * <em>not equal</em> token type ('!=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EX_EQUAL
 * <em>exactly equal</em> token type ('===' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EX_NEQUAL
 * <em>exactly not equal</em> token type ('!==' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LESS
 * <em>less than</em> token type ('<' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_GREATER
 * <em>greater than</em> token type ('>' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LESS_EQUAL
 * <em>less than or equal to</em> token type ('<=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_GREATER_EQUAL
 * <em>greater than or equal to</em> token type ('>=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INCREMENT
 * \a increment token type ('++' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DECREMENT
 * \a decrement token type ('--' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INCREMENT_POST
 * <em>postfix increment</em> token type ('++' equivalent, only used in the parser).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DECREMENT_POST
 * <em>postfix decrement</em> token type ('--' equivalent, only used in the parser).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ASSIGN
 * \a assign token type ('=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LINK
 * \a link token type ('->' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PLUS_ASSIGN
 * <em>plus and assign</em> token type ('+=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MINUS_ASSIGN
 * <em>minus and assign</em> token type ('-=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MULTIPLY_ASSIGN
 * <em>multiply and assign</em> token type ('*=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DIVIDE_ASSIGN
 * <em>divide and assign</em> token type ('/=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MODULO_ASSIGN
 * <em>modulo and assign</em> token type ('%=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_QUOTIENT_ASSIGN
 * <em>quotient and assign</em> token type ('//=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_POWER_ASSIGN
 * <em>power and assign</em> token type ('**=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_AND_ASSIGN
 * <em>binary and and assign</em> token type ('&=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_OR_ASSIGN
 * <em>binary or and assign</em> token type ('|=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_XOR_ASSIGN
 * <em>binary xor and assign</em> token type ('^=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_SHIFT_ASSIGN
 * <em>left shift and assign</em> token type ('<<=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_SHIFT_ASSIGN
 * <em>right shift and assign</em> token type ('>>=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_PAREN
 * <em>left parentheses</em> token type ('(' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_PAREN
 * <em>right parentheses</em> token type (')' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_SQUARE
 * <em>left square bracket</em> token type ('[' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_SQUARE
 * <em>right square bracket</em> token type (']' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_CURLY
 * <em>left curly bracket</em> token type ('{' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_CURLY
 * <em>right curly bracket</em> token type ('}' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_COMMA
 * \a comma token type (',' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DOT
 * \a dot token type ('.' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_COLON
 * \a colon token type (':' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_QUESTION
 * <em>question mark</em> token type ('?' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DOLLAR
 * <em>dollar sign</em> token type ('$' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ELLIPSIS
 * \a ellipsis token type ('...' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TRUE_K
 * \b true keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FALSE_K
 * \b false keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_NONE_K
 * \b none keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FUNC_K
 * \b func keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STRUCT_K
 * \b struct keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CLASS_K
 * \b class keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ENUM_K
 * \b enum keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PRIVATE_K
 * \b private keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PUBLIC_K
 * \b public keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PROTECTED_K
 * \b protected keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_GLOBAL_K
 * \b global keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CONST_K
 * \b const keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_READONLY_K
 * \b readonly keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STATIC_K
 * \b static keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IS_K
 * \b is keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ARE_K
 * \b are keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IN_K
 * \b in keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_AND_K
 * \b and keyword and \a and token type ('&&' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_OR_K
 * \b or keyword and \a or token type ('||' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_NOT_K
 * \b not keyword and \a not token type ('!' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IF_K
 * \b if keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ELIF_K
 * \b elif keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ELSE_K
 * \b else keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_SWITCH_K
 * \b switch keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CASE_K
 * \b case keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DEFAULT_K
 * \b default keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FOR_K
 * \b for keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TO_K
 * \b to keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STEP_K
 * \b step keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_WHILE_K
 * \b while keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DO_K
 * \b do keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TRY_K
 * \b try keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EXCEPT_K
 * \b except keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FINALLY_K
 * \b finally keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_RAISE_K
 * \b raise keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IMPORT_K
 * \b import keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INCLUDE_K
 * \b include keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_RETURN_K
 * \b return keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_BREAK_K
 * \b break keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CONTINUE_K
 * \b continue keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_OBJECT_T
 * \b object type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INT_T
 * \b int type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FLOAT_T
 * \b float type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_COMPLEX_T
 * \b complex type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_BOOL_T
 * \b bool type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CHAR_T
 * \b char type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STR_T
 * \b str type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LIST_T
 * \b list type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TUPLE_T
 * \b tuple type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DICT_T
 * \b dict type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_SET_T
 * \b set type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TYPE_T
 * \b type type token type.
*/
enum __MR_TOKEN_ENUM
{
    MR_TOKEN_EOF,
    MR_TOKEN_NEWLINE,
    MR_TOKEN_SEMICOLON,

    MR_TOKEN_IDENTIFIER,

    MR_TOKEN_INT,
    MR_TOKEN_FLOAT,
    MR_TOKEN_IMAGINARY,
    MR_TOKEN_CHR,
    MR_TOKEN_STR,
    MR_TOKEN_FSTR_END,
    MR_TOKEN_FSTR,
    MR_TOKEN_FSTR_START,

    MR_TOKEN_MULTIPLY,
    MR_TOKEN_DIVIDE,
    MR_TOKEN_MODULO,
    MR_TOKEN_QUOTIENT,
    MR_TOKEN_POWER,

    MR_TOKEN_B_AND,
    MR_TOKEN_B_OR,
    MR_TOKEN_B_XOR,
    MR_TOKEN_L_SHIFT,
    MR_TOKEN_R_SHIFT,

    MR_TOKEN_EQUAL,
    MR_TOKEN_NEQUAL,
    MR_TOKEN_EX_EQUAL,
    MR_TOKEN_EX_NEQUAL,
    MR_TOKEN_LESS,
    MR_TOKEN_GREATER,
    MR_TOKEN_LESS_EQUAL,
    MR_TOKEN_GREATER_EQUAL,

    MR_TOKEN_INCREMENT,
    MR_TOKEN_DECREMENT,
    MR_TOKEN_INCREMENT_POST,
    MR_TOKEN_DECREMENT_POST,

    MR_TOKEN_ASSIGN,
    MR_TOKEN_LINK,

    MR_TOKEN_PLUS_ASSIGN,
    MR_TOKEN_MINUS_ASSIGN,
    MR_TOKEN_MULTIPLY_ASSIGN,
    MR_TOKEN_DIVIDE_ASSIGN,
    MR_TOKEN_MODULO_ASSIGN,
    MR_TOKEN_QUOTIENT_ASSIGN,
    MR_TOKEN_POWER_ASSIGN,
    MR_TOKEN_B_AND_ASSIGN,
    MR_TOKEN_B_OR_ASSIGN,
    MR_TOKEN_B_XOR_ASSIGN,
    MR_TOKEN_L_SHIFT_ASSIGN,
    MR_TOKEN_R_SHIFT_ASSIGN,

    MR_TOKEN_L_PAREN,
    MR_TOKEN_R_PAREN,
    MR_TOKEN_L_SQUARE,
    MR_TOKEN_R_SQUARE,
    MR_TOKEN_L_CURLY,
    MR_TOKEN_R_CURLY,

    MR_TOKEN_COMMA,
    MR_TOKEN_DOT,
    MR_TOKEN_COLON,
    MR_TOKEN_QUESTION,
    MR_TOKEN_DOLLAR,
    MR_TOKEN_ELLIPSIS,

    MR_TOKEN_PLUS,
    MR_TOKEN_MINUS,
    MR_TOKEN_B_NOT,
    MR_TOKEN_NOT_K,

    MR_TOKEN_TRUE_K,
    MR_TOKEN_FALSE_K,
    MR_TOKEN_NONE_K,

    MR_TOKEN_FUNC_K,
    MR_TOKEN_STRUCT_K,
    MR_TOKEN_CLASS_K,
    MR_TOKEN_ENUM_K,

    MR_TOKEN_PRIVATE_K,
    MR_TOKEN_PUBLIC_K,
    MR_TOKEN_PROTECTED_K,

    MR_TOKEN_GLOBAL_K,
    MR_TOKEN_READONLY_K,
    MR_TOKEN_CONST_K,
    MR_TOKEN_STATIC_K,

    MR_TOKEN_IS_K,
    MR_TOKEN_ARE_K,
    MR_TOKEN_IN_K,
    MR_TOKEN_AND_K,
    MR_TOKEN_OR_K,

    MR_TOKEN_IF_K,
    MR_TOKEN_ELIF_K,
    MR_TOKEN_ELSE_K,

    MR_TOKEN_SWITCH_K,
    MR_TOKEN_CASE_K,
    MR_TOKEN_DEFAULT_K,

    MR_TOKEN_FOR_K,
    MR_TOKEN_TO_K,
    MR_TOKEN_STEP_K,

    MR_TOKEN_WHILE_K,
    MR_TOKEN_DO_K,

    MR_TOKEN_TRY_K,
    MR_TOKEN_EXCEPT_K,
    MR_TOKEN_FINALLY_K,
    MR_TOKEN_RAISE_K,

    MR_TOKEN_IMPORT_K,
    MR_TOKEN_INCLUDE_K,

    MR_TOKEN_RETURN_K,
    MR_TOKEN_BREAK_K,
    MR_TOKEN_CONTINUE_K,

    MR_TOKEN_OBJECT_T,
    MR_TOKEN_INT_T,
    MR_TOKEN_FLOAT_T,
    MR_TOKEN_COMPLEX_T,
    MR_TOKEN_BOOL_T,
    MR_TOKEN_CHAR_T,
    MR_TOKEN_STR_T,
    MR_TOKEN_LIST_T,
    MR_TOKEN_TUPLE_T,
    MR_TOKEN_DICT_T,
    MR_TOKEN_SET_T,
    MR_TOKEN_TYPE_T
};

/**
 * Padding for the keyword tokens. \n
 * Used by the keyword detection subroutine to assign the correct token type for a keyword.
*/
#define MR_TOKEN_KEYWORD_PAD ((mr_byte_t)MR_TOKEN_NOT_K)

/**
 * Number of valid keywords.
*/
#define MR_TOKEN_KEYWORD_COUNT ((mr_byte_t)(MR_TOKEN_CONTINUE_K - MR_TOKEN_KEYWORD_PAD + 1))

/**
 * Maximum size of a keyword (<em>protected</em>). \n
 * Used by the keyword detection subroutine to optimize the search process.
*/
#define MR_TOKEN_KEYWORD_MAXSIZE ((mr_byte_t)9)

/**
 * Padding for the type tokens. \n
 * Used by the type detection subroutine to assign the correct token type for a type.
*/
#define MR_TOKEN_TYPE_PAD ((mr_byte_t)MR_TOKEN_OBJECT_T)

/**
 * Number of valid types.
*/
#define MR_TOKEN_TYPE_COUNT ((mr_byte_t)(MR_TOKEN_TYPE_T - MR_TOKEN_TYPE_PAD + 1))

/**
 * Maximum size of a type (<em>complex</em>). \n
 * Used by the type detection subroutine to optimize the search process.
*/
#define MR_TOKEN_TYPE_MAXSIZE ((mr_byte_t)7)

/**
 * List of keywords (used by the keyword detection subroutine).
*/
extern mr_str_ct mr_token_keyword[MR_TOKEN_KEYWORD_COUNT];

/**
 * List of keyword sizes (in characters). \n
 * Used by the keyword detection subroutine to optimize the search process.
*/
extern mr_byte_t mr_token_keyword_size[MR_TOKEN_KEYWORD_COUNT];

/**
 * List of types (used by the type detection subroutine).
*/
extern mr_str_ct mr_token_type[MR_TOKEN_TYPE_COUNT];

/**
 * List of type sizes (in characters). \n
 * Used by the type detection subroutine to optimize the search process.
*/
extern mr_byte_t mr_token_type_size[MR_TOKEN_TYPE_COUNT];

/**
 * Size of all token symbols in characters
*/
extern mr_byte_t mr_token_symbol_size[MR_TOKEN_KEYWORD_PAD];

/**
 * It returns size of the token in characters.
 * @param token
 * Pointer to the token with the needed data.
 * @return Size of the token in characters.
*/
mr_long_t mr_token_getsize(
    mr_token_t *token);

/**
 * It returns size of the token in characters.
 * @param type
 * Type of the token.
 * @param idx
 * Starting index of the token.
 * @return Size of the token in characters.
*/
mr_long_t mr_token_getsize2(
    mr_byte_t type, mr_long_t idx);

#ifdef __MR_DEBUG__

/**
 * Number of valid tokens.
*/
#define MR_TOKEN_COUNT ((mr_byte_t)(MR_TOKEN_TYPE_T + 1))

/**
 * Labels for different token types.
*/
extern mr_str_ct mr_token_labels[MR_TOKEN_COUNT];

/**
 * It prints out a token (only available in Debug builds).
 * @param token
 * Pointer to the token that needs to be printed.
 */
void mr_token_print(
    mr_token_t *token);

/**
 * It prints out a list of tokens until it hits and EOF token (only available in Debug builds). \n
 * The list must be ended with an EOF token.
 * @param tokens
 * The list of tokens.
*/
void mr_token_prints(
    mr_token_t *tokens);

#endif

#endif
