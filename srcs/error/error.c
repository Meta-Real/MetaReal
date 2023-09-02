/*
    MetaReal Compiler version 1.0.0
*/

#include <error/error.h>
#include <stdio.h>

void print_illegal_char(const illegal_char_t *error, const char *fname, const char *code)
{
    fprintf(stderr,
        "\nIllegal Character Error: '%c'\nFile \"%s\", line %llu\n\n",
        error->chr, fname, error->pos.ln);

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
