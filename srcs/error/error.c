/**
 * @file error.c
 * This file contains definitions of the \a error.h file.
*/

#include <error/error.h>
#include <stdlib.h>
#include <stdio.h>

#define MR_INVALID_SEMANTIC_COUNT (MR_INVALID_SEMANTIC_DIVBYZERO + 1)
mr_str_ct mr_invalid_semantic_label[MR_INVALID_SEMANTIC_COUNT] =
{
    "DivByZeroError"
};

void mr_illegal_chr_print(
    mr_illegal_chr_t *error,
    mr_str_ct fname, mr_str_ct code, mr_long_t size)
{
    if (error->expected)
        fprintf(stderr, "\nExpected Character Error: '%c'\n", error->chr);
    else
        fprintf(stderr, "\nIllegal Character Error: '%c'\n", error->chr);

    mr_long_t i, ln = 1, start = 0;
    for (i = 0; i != error->pos; i++)
        if (code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(stderr, "File \"%s\", line %u\n\n", fname, ln);

    mr_chr_t chr;
    for (i = start; i != size; i++)
    {
        chr = code[i];
        if (chr == '\n' || (chr == '\r' && code[i + 1] == '\n'))
            break;

        fputc(chr, stderr);
    }
    fputc('\n', stderr);

    for (i = start; i != error->pos; i++)
        fputc(' ', stderr);
    fputs("^\n\n", stderr);
}

void mr_invalid_syntax_print(
    mr_invalid_syntax_t *error,
    mr_str_ct fname, mr_str_ct code, mr_long_t size)
{
    if (error->detail)
        fprintf(stderr, "\nInvalid Syntax Error: %s\n", error->detail);
    else
        fputs("\nInvalid Syntax Error\n", stderr);

    mr_long_t i, ln = 1, start = 0;
    for (i = 0; i != error->idx; i++)
        if (code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(stderr, "File \"%s\", line %u\n\n", fname, ln);

    mr_long_t eidx = error->idx + error->size;
    if (eidx > size)
    {
        fwrite(code, sizeof(mr_chr_t), size - start, stderr);
        fputc('\n', stderr);

        for (i = start; i != error->idx; i++)
            fputc(' ', stderr);
        fputs("^\n\n", stderr);
        return;
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

    for (i = start; i != error->idx; i++)
        fputc(' ', stderr);

    if (end >= eidx)
        for (; i != eidx; i++)
            fputc('^', stderr);
    else
    {
        for (; i != end; i++)
            fputc('^', stderr);
        fputc('~', stderr);
    }

    fputs("\n\n", stderr);
}

void mr_invalid_semantic_print(
    mr_invalid_semantic_t *error,
    mr_str_ct fname, mr_str_ct code, mr_long_t size)
{
    fprintf(stderr, "\nInvalid Semantic Error: %s\n", error->detail);
    free(error->detail);

    fprintf(stderr, "Error Type: %s\n", mr_invalid_semantic_label[error->type]);

    mr_long_t i, ln = 1, start = 0;
    for (i = 0; i != error->idx; i++)
        if (code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(stderr, "File \"%s\", line %u\n\n", fname, ln);

    mr_long_t eidx = error->idx + error->size;
    if (eidx > size)
    {
        fwrite(code, sizeof(mr_chr_t), size - start, stderr);
        fputc('\n', stderr);

        for (i = start; i != error->idx; i++)
            fputc(' ', stderr);
        fputs("^\n\n", stderr);
        return;
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

    for (i = start; i != error->idx; i++)
        fputc(' ', stderr);

    if (end >= eidx)
        for (; i != eidx; i++)
            fputc('^', stderr);
    else
    {
        for (; i != end; i++)
            fputc('^', stderr);
        fputc('~', stderr);
    }

    fputs("\n\n", stderr);
}
