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
 * @file error.h
 * This file contains all the error structures that MetaReal default debugger supports.
*/

#ifndef __MR_ERROR__
#define __MR_ERROR__

#include <lexer/token.h>

/**
 * @struct __MR_ILLEGAL_CHR_T
 * The structure that holds information about illegal or missing characters. \n
 * This structure is used by the lexer.
 * @var mr_chr_t __MR_ILLEGAL_CHR_T::chr
 * The illegal or missing character.
 * @var mr_bool_t __MR_ILLEGAL_CHR_T::expected
 * It indicates whether the character is illegal (MR_FALSE) or is it missing (MR_TRUE).
 * @var mr_long_t __MR_ILLEGAL_CHR_T::idx
 * Index of the error.
*/
struct __MR_ILLEGAL_CHR_T
{
    mr_chr_t chr;
    mr_bool_t expected;

    mr_long_t idx;
};
typedef struct __MR_ILLEGAL_CHR_T mr_illegal_chr_t;

/**
 * @struct __MR_INVALID_SYNTAX_T
 * The structure that holds information about an invalid syntax in the code. \n
 * This structure is used by the parser.
 * @var mr_str_ct __MR_INVALID_SYNTAX_T::detail
 * Details of the error. \n
 * This field will be NULL if the thrown error is a general invalid syntax error.
 * @var mr_token_t* __MR_INVALID_SYNTAX_T::token
 * Pointer to the token that caused the error.
*/
struct __MR_INVALID_SYNTAX_T
{
    mr_str_ct detail;
    mr_token_t *token;
};
typedef struct __MR_INVALID_SYNTAX_T mr_invalid_syntax_t;

/**
 * @struct __MR_INVALID_SEMANTIC_T
 * The structure that holds information about an invalid semantic in the code. \n
 * This structure is used by the optimizer.
 * @var mr_str_t __MR_INVALID_SEMANTIC_T::detail
 * Details of the error. \n
 * This field can be static or dynamic string.
 * @var mr_token_t* __MR_INVALID_SEMANTIC_T::token
 * Pointer to the last token that caused the error.
 * @var mr_bool_t __MR_INVALID_SEMANTIC_T::is_static
 * It determines that the \a detail is dynamic and should be freed or not.
 * @var mr_byte_t __MR_INVALID_SEMANTIC_T::type
 * Type of the error (from __MR_INVALID_SEMANTIC_ENUM).
 * @var mr_long_t __MR_INVALID_SEMANTIC_T::idx
 * Index of the start of the error.
 * @var mr_byte_t __MR_INVALID_SEMANTIC_T::size
 * Size of the error before the last token in characters.
*/
struct __MR_INVALID_SEMANTIC_T
{
    mr_str_t detail;
    mr_token_t *token;
    mr_bool_t is_static : 1;
    mr_byte_t type : 7;

    mr_long_t idx;
    mr_byte_t size;
};
typedef struct __MR_INVALID_SEMANTIC_T mr_invalid_semantic_t;

/**
 * @enum __MR_INVALID_SEMANTIC_ENUM
 * List of valid semantic error types. \n
 * These are default semantic error types and customized types are stored differently.
 * @var __MR_INVALID_SEMANTIC_ENUM::MR_INVALID_SEMANTIC_DIVBYZERO
 * Division by zero error.
 * @var __MR_INVALID_SEMANTIC_ENUM::MR_NOT_DEFINED_ERROR
 * Variable not defined error.
 * @var __MR_INVALID_SEMANTIC_ENUM::MR_INVALID_SEMANTIC_DOLLAR_METHOD
 * Calling invalid dollar method error.
*/
enum __MR_INVALID_SEMANTIC_ENUM
{
    MR_INVALID_SEMANTIC_DIVBYZERO,
    MR_INVALID_SEMANTIC_NOT_DEFINED,
    MR_INVALID_SEMANTIC_DOLLAR_METHOD
};

/**
 * It displays an illegal character error in <em>errstream</em>. \n
 * Example of an illegal character error:
 * <pre>
 * Illegal Character Error: '@'
 * File "test.mr", line 4 \n
 * a += 4 * 3 @
 *            ^
 * </pre>
 * Example of an expected character error:
 * <pre>
 * Expected Character Error: '"'
 * File "test.mr", line 7 \n
 * a = "Hello World
 *                 ^
 * </pre>
 * \a errstream is \a stderr by default and can be changed with the \a $set_errstream dollar method.
 * @param error
 * Illegal character error that needs to be displayed.
*/
void mr_illegal_chr_print(
    mr_illegal_chr_t error);

/**
 * It displays an invalid syntax error in <em>errstream</em>. \n
 * Example of a general invalid syntax error:
 * <pre>
 * Invalid Syntax Error
 * File "test.mr", line 82 \n
 * a = "Hello" + >>= "World"
 *               ^^^
 * </pre>
 * Example of an invalid syntax error:
 * <pre>
 * Invalid Syntax Error: Expected ')'
 * File "test.mr", line 20 \n
 * print(34 * 9 "A String"
 *              ^^^^^^^^^^
 * </pre>
 * Example of an invalid syntax error that surpasses one line:
 * <pre>
 * Invalid Syntax Error: Expected EOF
 * File "test.mr", line 6 \n
 * int a = 9 "multiline string
 *           ^^^^^^^^^^^^^^^^^~
 * </pre>
 * \a errstream is \a stderr by default and can be changed with the \a $set_errstream dollar method.
 * @param error
 * Invalid syntax error that needs to be displayed.
*/
void mr_invalid_syntax_print(
    mr_invalid_syntax_t *error);

/**
 * It displays an invalid semantic error in <em>errstream</em>. \n
 * Example of an invalid semantic error:
 * <pre>
 * Invalid Semantic Error: 'val' is not defined
 * Error Type: NotDefError
 * File "test.mr", line 2 \n
 * print(12 + val)
 *            ^^^
 * </pre>
 * Example of an invalid semantic error that surpasses one line:
 * <pre>
 * Invalid Semantic Error: Illegal operation (*) between <int> and <none>.
 * Error Type: IllegalOpError
 * File "test.mr", line 102 \n
 * var = 83 * if true {
 *            ^^^^^^^^^~
 * </pre>
 * \a errstream is \a stderr by default and can be changed with the \a $set_errstream dollar method.
 * @param error
 * Invalid semantic error that needs to be displayed.
*/
void mr_invalid_semantic_print(
    mr_invalid_semantic_t *error);

#endif
