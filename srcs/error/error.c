/**
 * @file error.c
 * This file contains definitions of the "error.h" file.
*/

#include <error/error.h>

void mr_error_illegal_chr_print(mr_illegal_chr_t *error, mr_str_ct fname, mr_str_ct code)
{
    if (error->expected)
        fprintf(stderr, "\nExpected Character Error: '%c'\n", error->chr);
    else
        fprintf(stderr, "\nIllegal Character Error: '%c'\n", error->chr);

    fprintf(stderr, "File \"%s\", line %llu\n\n", fname, error->pos.ln);

    mr_size_t start = error->pos.idx;
    for (; start; start--)
        if (code[start] == '\n')
        {
            start++;
            break;
        }

    mr_size_t i;
    for (i = start; code[i] != '\n' && code[i] != '\0'; i++)
        putc(code[i], stderr);
    putc('\n', stderr);

    for (i = start; i < error->pos.idx; i++)
        putc(' ', stderr);
    fputs("^\n\n", stderr);
}
