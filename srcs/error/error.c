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
static mr_str_ct mr_invalid_semantic_label[MR_INVALID_SEMANTIC_COUNT] =
{
    "DivByZeroError",
    "NotDefinedError",
    "InvalidDollarMethod"
};

void mr_illegal_chr_print(
    mr_illegal_chr_t error)
{
    mr_long_t i, ln, start;
    mr_chr_t chr;

    if (error.expected)
        fprintf(_mr_config.errstream, "\nExpected Character Error: '%c'\n", error.chr);
    else
        fprintf(_mr_config.errstream, "\nIllegal Character Error: '%c'\n", error.chr);

    ln = 1;
    start = 0;
    for (i = 0; i != error.idx; i++)
        if (_mr_config.code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(_mr_config.errstream, "File \"%s\", line %" PRIu32 "\n\n", _mr_config.fname, ln);

    for (i = start; i != _mr_config.size; i++)
    {
        chr = _mr_config.code[i];
        if (chr == '\n' || (chr == '\r' && _mr_config.code[i + 1] == '\n'))
            break;

        fputc(chr, _mr_config.errstream);
    }
    fputc('\n', _mr_config.errstream);

    for (i = start; i != error.idx; i++)
        fputc(' ', _mr_config.errstream);
    fputs("^\n\n", _mr_config.errstream);
}

void mr_invalid_syntax_print(
    mr_invalid_syntax_t *error)
{
    mr_long_t i, idx, ln, start, eidx, end;
    mr_chr_t chr;

    if (error->detail)
        fprintf(_mr_config.errstream, "\nInvalid Syntax Error: %s\n", error->detail);
    else
        fputs("\nInvalid Syntax Error\n", _mr_config.errstream);

    ln = 1;
    start = 0;
    idx = MR_IDX_EXTRACT(error->token->idx);
    for (i = 0; i != idx; i++)
        if (_mr_config.code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(_mr_config.errstream, "File \"%s\", line %" PRIu32 "\n\n", _mr_config.fname, ln);

    if (error->token->type == MR_TOKEN_EOF)
    {
        fprintf(_mr_config.errstream, "%.*s\n", _mr_config.size - start, _mr_config.code);

        for (i = start; i != idx; i++)
            fputc(' ', _mr_config.errstream);
        fputs("^\n\n", _mr_config.errstream);
        return;
    }

    eidx = idx + mr_token_getsize(error->token);
    for (end = start; end != _mr_config.size; end++)
    {
        chr = _mr_config.code[end];
        if (chr == '\n' || (chr == '\r' && _mr_config.code[end + 1] == '\n'))
            break;

        fputc(chr, _mr_config.errstream);
    }
    fputc('\n', _mr_config.errstream);

    for (i = start; i != idx; i++)
        fputc(' ', _mr_config.errstream);

    if (end >= eidx)
        for (; i != eidx; i++)
            fputc('^', _mr_config.errstream);
    else
    {
        for (; i != end; i++)
            fputc('^', _mr_config.errstream);
        fputc('~', _mr_config.errstream);
    }

    fputs("\n\n", _mr_config.errstream);
}

void mr_invalid_semantic_print(
    mr_invalid_semantic_t *error)
{
    mr_long_t i, ln, start, eidx, end;
    mr_chr_t chr;

    fprintf(_mr_config.errstream, "\nInvalid Semantic Error: %s\n", error->detail);
    if (error->is_static)
        free(error->detail);

    fprintf(_mr_config.errstream, "Error Type: %s\n", mr_invalid_semantic_label[error->type]);

    ln = 1;
    start = 0;
    for (i = 0; i != error->idx; i++)
        if (_mr_config.code[i] == '\n')
        {
            start = i + 1;
            ln++;
        }

    fprintf(_mr_config.errstream, "File \"%s\", line %" PRIu32 "\n\n", _mr_config.fname, ln);

    eidx = error->idx + error->size;
    if (error->token->type != MR_TOKEN_EOF)
        eidx += mr_token_getsize(error->token);

    for (end = start; end != _mr_config.size; end++)
    {
        chr = _mr_config.code[end];
        if (chr == '\n' || (chr == '\r' && _mr_config.code[end + 1] == '\n'))
            break;

        fputc(chr, _mr_config.errstream);
    }
    fputc('\n', _mr_config.errstream);

    for (i = start; i != error->idx; i++)
        fputc(' ', _mr_config.errstream);

    if (end >= eidx)
        for (; i != eidx; i++)
            fputc('^', _mr_config.errstream);
    else
    {
        for (; i != end; i++)
            fputc('^', _mr_config.errstream);
        fputc('~', _mr_config.errstream);
    }

    fputs("\n\n", _mr_config.errstream);
}
