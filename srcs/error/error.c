/**
 * @file error.c
 * This file contains definitions of the \a error.h file.
*/

#include <error/error.h>

void mr_illegal_chr_print(mr_illegal_chr_t *error,
    mr_str_ct fname, mr_str_ct code, mr_long_t size)
{
    if (error->expected)
        fprintf(stderr, "\nExpected Character Error: '%c'\n", error->chr);
    else
        fprintf(stderr, "\nIllegal Character Error: '%c'\n", error->chr);

    fprintf(stderr, "File \"%s\", line %u\n\n", fname, error->pos.ln);

    mr_long_t start;
    for (start = error->pos.idx; start; start--)
        if (code[start] == '\n')
        {
            start++;
            break;
        }

    mr_long_t i;
    mr_chr_t chr;
    for (i = start; i < size; i++)
    {
        chr = code[i];
        if (chr == '\n')
            break;

        fputc(chr, stderr);
    }
    fputc('\n', stderr);

    for (i = start; i < error->pos.idx; i++)
        fputc(' ', stderr);
    fputs("^\n\n", stderr);
}

void mr_invalid_syntax_print(mr_invalid_syntax_t *error,
    mr_str_ct fname, mr_str_ct code, mr_long_t size)
{
    if (error->detail)
        fprintf(stderr, "\nInvalid Syntax Error: %s\n", error->detail);
    else
        fputs("\nInvalid Syntax Error\n", stderr);
    fprintf(stderr, "File \"%s\", line %u\n\n", fname, error->poss.ln);

    mr_long_t start;
    for (start = error->poss.idx; start; start--)
        if (code[start] == '\n')
        {
            start++;
            break;
        }

    mr_long_t end;
    mr_chr_t chr;
    for (end = start; end != size; end++)
    {
        chr = code[end];
        if (chr == '\n' || (chr == '\r' && code[end + 1] == '\n'))
            break;

        fputc(chr, stderr);
    }
    fputc('\n', stderr);

    mr_long_t i;
    for (i = start; i < error->poss.idx; i++)
        fputc(' ', stderr);

    if (end >= error->eidx)
        for (; i < error->eidx; i++)
            fputc('^', stderr);
    else
    {
        for (; i < end; i++)
            fputc('^', stderr);
        fputc('~', stderr);
    }

    fputs("\n\n", stderr);
}
