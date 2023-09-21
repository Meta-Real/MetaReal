/*
    MetaReal Compiler version 1.0.0
*/

#include <error/error.h>
#include <stdio.h>
#include <alloc.h>

const char *invalid_semantic_labels[6] =
{
    "IllegalOpError",
    "ValueError",
    "NotDefError",
    "MemOverflowError",
    "IndexError",
    "DivByZeroError"
};

void print_illegal_char(const illegal_char_t *error, const char *fname, const char *code)
{
    if (error->expected)
        fputs("\nExpected Character Error: ", stderr);
    else
        fputs("\nIllegal Character Error: ", stderr);

    fprintf(stderr, "'%c'\nFile \"%s\", line %llu\n\n", error->chr, fname, error->pos.ln);

    uint64_t s = error->pos.idx;
    for (; s; s--)
        if (code[s] == '\n')
        {
            s++;
            break;
        }

    uint64_t i;

    for (i = s; code[i] != '\n' && code[i] != '\0'; i++)
        putc(code[i], stderr);
    putc('\n', stderr);

    for (i = s; i < error->pos.idx; i++)
        putc(' ', stderr);
    fputs("^\n\n", stderr);
}

void print_invalid_syntax(const invalid_syntax_t *error, const char *fname, const char *code)
{
    fputs("\nInvalid Syntax Error", stderr);

    if (error->detail)
        fprintf(stderr, ": %s", error->detail);

    fprintf(stderr, "\nFile \"%s\", line %llu\n\n", fname, error->poss.ln);

    uint64_t s = error->poss.idx;
    for (; s; s--)
        if (code[s] == '\n')
        {
            s++;
            break;
        }

    uint64_t i;

    for (i = s; code[i] != '\n' && code[i] != '\0'; i++)
        putc(code[i], stderr);
    putc('\n', stderr);

    for (i = s; i < error->poss.idx; i++)
        putc(' ', stderr);

    if (error->poss.ln == error->pose.ln)
        for (; i < error->pose.idx; i++)
            putc('^', stderr);
    else
    {
        for (; code[i] != '\n'; i++)
            putc('^', stderr);
        putc('~', stderr);
    }

    fputs("\n\n", stderr);
}

void print_invalid_semantic(invalid_semantic_t *error, const char *fname, const char *code)
{
    fprintf(stderr, "\nInvalid Semantic Error: %s\nFile \"%s\", line %llu\nError Type: %s\n\n",
        error->detail, fname, error->poss.ln, invalid_semantic_labels[error->type]);
    mr_free(error->detail);

    uint64_t s = error->poss.idx;
    for (; s; s--)
        if (code[s] == '\n')
        {
            s++;
            break;
        }

    uint64_t i;

    for (i = s; code[i] != '\n' && code[i] != '\0'; i++)
        putc(code[i], stderr);
    putc('\n', stderr);

    for (i = s; i < error->poss.idx; i++)
        putc(' ', stderr);

    if (error->poss.ln == error->pose.ln)
        for (; i < error->pose.idx; i++)
            putc('^', stderr);
    else
    {
        for (; code[i] != '\n'; i++)
            putc('^', stderr);
        putc('~', stderr);
    }

    fputs("\n\n", stderr);
}
