/**
 * @file error.h
 * This file contains all the error structures that MetaReal default debugger supports.
 * All things defined in \a error.c and this file have the \a mr_error prefix.
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
void mr_error_illegal_chr_print(mr_illegal_chr_t *error, mr_str_ct fname, mr_str_ct code);

#endif /* __MR_ERROR__ */
