/**
 * @file lexer.c
 * This file contains the function definitions of the "lexer.h" file.
*/

#include <lexer/lexer.h>

void mr_lexer(mr_lexer_t *res, FILE *code, mr_size_t size)
{
    // dummy
    if (!size)
        return;

    res->tokens = NULL;
    res->error = (mr_illegal_chr_t){fgetc(code), 0, (mr_pos_t){0, 1}};
}
