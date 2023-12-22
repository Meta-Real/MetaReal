/**
 * @file error.h
 * This file contains all the error structures that MetaReal default debugger supports.
*/

#ifndef __MR_ERROR__
#define __MR_ERROR__

#include <stdio.h>
#include <defs.h>

/**
 * @struct __MR_ILLEGAL_CHR_T
 * The structure that holds information about illegal or missing characters. \n
 * This structure is used by the lexer.
 * @var mr_chr_t __MR_ILLEGAL_CHR_T::chr
 * The illegal or missing character.
 * @var mr_bool_t __MR_ILLEGAL_CHR_T::expected
 * It indicates whether the character is illegal (MR_FALSE) or is it missing (MR_TRUE).
 * @var mr_pos_t __MR_ILLEGAL_CHR_T::pos
 * Position of the error.
*/
struct __MR_ILLEGAL_CHR_T
{
    mr_chr_t chr;
    mr_bool_t expected;

    mr_pos_t pos;
};
typedef struct __MR_ILLEGAL_CHR_T mr_illegal_chr_t;

/**
 * @struct __MR_INVALID_SYNTAX_T
 * The structure that holds information about an invalid syntax in the code. \n
 * This structure is used by the parser.
 * @var mr_str_ct __MR_INVALID_SYNTAX_T::detail
 * Detail of the error. \n
 * This field will be NULL if the thrown error is a general invalid syntax error.
 * @var mr_pos_t __MR_INVALID_SYNTAX_T::poss
 * Starting postion of the error.
 * @var mr_pos_t __MR_INVALID_SYNTAX_T::pose
 * Ending position of the error.
*/
struct __MR_INVALID_SYNTAX_T
{
    mr_str_ct detail;

    mr_pos_t poss;
    mr_pos_t pose;
};
typedef struct __MR_INVALID_SYNTAX_T mr_invalid_syntax_t;

/**
 * It displays the \a error in <em>errstream</em>. \n
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
 * \a errstream is \a stderr by default and
 * can be changed with the \a $set_errstream dollar method.
 * @param error
 * Illegal character error that needs to be displayed.
 * @param fname
 * Name of the source file.
 * @param code
 * The source code.
*/
void mr_illegal_chr_print(mr_illegal_chr_t *error, mr_str_ct fname, mr_str_ct code);

/**
 * It displays the \a error in <em>errstream</em>. \n
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
 * Invalid Syntax Error: Expected EOF (End Of File) or EOL (End Of Line)
 * File "test.mr", line 6 \n
 * int a = 9 "multiline string
 *           ^^^^^^^^^^^^^^^^^~
 * </pre>
 * \a errstream is \a stderr by default and
 * can be changed with the \a $set_errstream dollar method.
 * @param error
 * Invalid syntax error that needs to be displayed.
 * @param fname
 * Name of the source file.
 * @param code
 * The source code.
*/
void mr_invalid_syntax_print(mr_invalid_syntax_t *error, mr_str_ct fname, mr_str_ct code);

#endif /* __MR_ERROR__ */
