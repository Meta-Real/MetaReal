/**
 * @file error.c
 * This file contains definitions of the "error.h" file.
*/

#include <error/error.h>

void mr_illegal_chr_print(mr_illegal_chr_t *error, mr_str_ct fname, mr_str_ct code)
{
    if (error->expected)
        fprintf(stderr, "\nExpected Character Error: '%c'\n", error->chr);
    else
        fprintf(stderr, "\nIllegal Character Error: '%c'\n", error->chr);

    fprintf(stderr, "File \"%s\", line %llu\n\n", fname, error->pos.ln);

    mr_size_t start;
    for (start = error->pos.idx; start; start--)
        if (code[start] == '\n')
        {
            start++;
            break;
        }

    mr_size_t i;
    mr_chr_t chr;
    for (i = start;; i++)
    {
        chr = code[i];
        if (chr == '\n' || chr == '\0')
            break;

        fputc(chr, stderr);
    }
    fputc('\n', stderr);

    for (i = start; i < error->pos.idx; i++)
        fputc(' ', stderr);
    fputs("^\n\n", stderr);
}

void mr_invalid_syntax_print(mr_invalid_syntax_t *error, mr_str_ct fname, mr_str_ct code)
{
    if (error->detail)
        fprintf(stderr, "\nInvalid Syntax Error: %s\n", error->detail);
    else
        fputs("\nInvalid Syntax Error\n", stderr);
    fprintf(stderr, "File \"%s\", line %llu\n\n", fname, error->poss.ln);

    mr_size_t start;
    for (start = error->poss.idx; start; start--)
        if (code[start] == '\n')
        {
            start++;
            break;
        }

    if (error->poss.ln != error->pose.ln)
    {
        mr_size_t end;
        mr_chr_t chr;
        for (end = error->poss.idx;; end++)
        {
            chr = code[end];
            if (chr == '\n' || chr == '\0')
                break;
        }

        fwrite(code + start, 1, end - start, stderr);
        fputc('\n', stderr);

        for (; start < error->poss.idx; start++)
            fputc(' ', stderr);
        for (; start < end; start++)
            fputc('^', stderr);
        fputs("~\n\n", stderr);

        return;
    }

    mr_size_t end;
    mr_chr_t chr;
    for (end = error->pose.idx;; end++)
    {
        chr = code[end];
        if (chr == '\n' || chr == '\0')
            break;
    }

    fwrite(code + start, 1, end - start, stderr);
    fputc('\n', stderr);

    for (; start < error->poss.idx; start++)
        fputc(' ', stderr);
    for (; start < error->pose.idx; start++)
        fputc('^', stderr);
    fputs("\n\n", stderr);
}
