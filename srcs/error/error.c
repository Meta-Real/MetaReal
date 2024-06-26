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
 * @file error.c
 * This file contains definitions of the \a error.h file.
*/

#include <error/error.h>
#include <lexer/token.h>
#include <config.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Number of invalid semantic types
*/
#define MR_INVALID_SEMANTIC_COUNT (MR_INVALID_SEMANTIC_DOLLAR_METHOD + 1)

/**
 * Display names for different invalid semantic error types
*/
mr_str_ct mr_invalid_semantic_label[MR_INVALID_SEMANTIC_COUNT] =
{
    "DivByZeroError",
    "NotDefinedError",
    "InvalidDollarMethod"
};

void mr_illegal_chr_print(
    mr_illegal_chr_t *error)
{
    mr_long_t idx, i, ln, start;
    mr_chr_t chr;

    if (error->expected)
        fprintf(stderr, "\nExpected Character Error: '%c'\n", error->chr);
    else
        fprintf(stderr, "\nIllegal Character Error: '%c'\n", error->chr);

    idx = MR_IDX_EXTRACT(error->idx);
    ln = 1;
    start = 0;
    for (i = 0; i != idx; i++)
        if (_mr_config.code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(stderr, "File \"%s\", line %" PRIu32 "\n\n", _mr_config.fname, ln);

    for (i = start; i != _mr_config.size; i++)
    {
        chr = _mr_config.code[i];
        if (chr == '\n' || (chr == '\r' && _mr_config.code[i + 1] == '\n'))
            break;

        fputc(chr, stderr);
    }
    fputc('\n', stderr);

    for (i = start; i != idx; i++)
        fputc(' ', stderr);
    fputs("^\n\n", stderr);
}

void mr_invalid_syntax_print(
    mr_invalid_syntax_t *error)
{
    mr_long_t idx, i, ln, start, eidx, end;
    mr_chr_t chr;

    if (error->detail)
        fprintf(stderr, "\nInvalid Syntax Error: %s\n", error->detail);
    else
        fputs("\nInvalid Syntax Error\n", stderr);

    idx = MR_IDX_EXTRACT(error->idx);
    ln = 1;
    start = 0;
    for (i = 0; i != idx; i++)
        if (_mr_config.code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(stderr, "File \"%s\", line %" PRIu32 "\n\n", _mr_config.fname, ln);

    eidx = idx + mr_token_getsize(error->type, idx);
    if (eidx > _mr_config.size)
    {
        fwrite(_mr_config.code, sizeof(mr_chr_t), _mr_config.size - start, stderr);
        fputc('\n', stderr);

        for (i = start; i != idx; i++)
            fputc(' ', stderr);
        fputs("^\n\n", stderr);
        return;
    }

    for (end = start; end != _mr_config.size; end++)
    {
        chr = _mr_config.code[end];
        if (chr == '\n' || (chr == '\r' && _mr_config.code[end + 1] == '\n'))
            break;

        fputc(chr, stderr);
    }
    fputc('\n', stderr);

    for (i = start; i != idx; i++)
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
    mr_invalid_semantic_t *error)
{
    mr_long_t idx, i, ln, start, eidx, end;
    mr_chr_t chr;

    fprintf(stderr, "\nInvalid Semantic Error: %s\n", error->detail);
    free(error->detail);

    fprintf(stderr, "Error Type: %s\n", mr_invalid_semantic_label[error->type]);

    idx = MR_IDX_EXTRACT(error->idx);
    ln = 1;
    start = 0;
    for (i = 0; i != idx; i++)
        if (_mr_config.code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(stderr, "File \"%s\", line %" PRIu32 "\n\n", _mr_config.fname, ln);

    eidx = idx + error->size;
    if (error->etype != MR_TOKEN_EOF)
        eidx += mr_token_getsize(error->etype, eidx);

    for (end = start; end != _mr_config.size; end++)
    {
        chr = _mr_config.code[end];
        if (chr == '\n' || (chr == '\r' && _mr_config.code[end + 1] == '\n'))
            break;

        fputc(chr, stderr);
    }
    fputc('\n', stderr);

    for (i = start; i != idx; i++)
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
