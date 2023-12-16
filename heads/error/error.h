/**
 * @file error.h
 * This file contains all the error structures that the MetaReal default debugger supports.
*/

#ifndef __MR_ERROR__
#define __MR_ERROR__

#include <stdio.h>
#include <defs.h>

/**
 * @struct __MR_ILLEGAL_CHR_T
 * The structure that holds the information about illegal or missing characters. \n
 * This structure is used by the lexer.
 * @var mr_chr_t __MR_ILLEGAL_CHR_T::chr
 * The illegal or missing character.
 * @var mr_bool_t __MR_ILLEGAL_CHR_T::expected
 * It indicates whether the character is illegal (MR_FALSE) or is it missing (MR_TRUE).
 * @var mr_pos_t __MR_ILLEGAL_CHR_T::pos
 * The position of the error.
*/
struct __MR_ILLEGAL_CHR_T
{
    mr_chr_t chr;
    mr_bool_t expected;

    mr_pos_t pos;
};
typedef struct __MR_ILLEGAL_CHR_T mr_illegal_chr_t;

/**
 * @fn void mr_illegal_chr_print(mr_illegal_chr_t *error, mr_str_ct fname, FILE *code, mr_size_t size)
 * <pre>
 * It displays the \a error in the \a error_stream. \n
 * Example of the illegal character error: \n \n
 * Illegal Character Error: '@' \n
 * File "test.mr", line 4 \n \n
 * a += 4 * 3 @ \n
 *            ^ \n \n
 * Example of the expected character error: \n \n
 * Expected Character Error: '"' \n
 * File "test.mr", line 7 \n \n
 * a = "Hello World \n
 *                 ^ \n \n
 * The \a error_stream is `stderr` by the dollar method `$set_error_stream`.
 * </pre>
 * @param error
 * The illegal character error that needs to be displayed.
 * @param fname
 * The name of the source file.
 * @param code
 * The file that contains the code.
 * @param size
 * The size of the \a code.
*/
void mr_illegal_chr_print(mr_illegal_chr_t *error, mr_str_ct fname, FILE *code, mr_size_t size);

#endif /* __MR_ERROR__ */
