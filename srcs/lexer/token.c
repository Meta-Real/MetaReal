/*
    MetaReal Compiler version 1.0.0
*/

#include <lexer/token.h>
#include <alloc.h>
#include <stdio.h>

const char *token_labels[38] =
{
    "EOF",
    "SEMICOLON",
    "ID", "INT", "FLOAT", "IMAG",
    "ADD", "SUB", "MUL", "DIV", "MOD", "QUOT", "POW",
    "B_AND", "B_OR", "B_XOR", "LSHIFT", "RSHIFT", "B_NOT",
    "EQ", "NEQ", "EX_EQ", "EX_NEQ",
    "LT", "GT", "LTE", "GTE",
    "AND", "OR", "NOT",
    "ASSIGN",
    "LPAREN", "RPAREN",
    "AND_K", "OR_K", "NOT_K",
    "TRUE_K", "FALSE_K"
};

void free_tokens(token_t *tokens)
{
    while (tokens->type != EOF_T)
        mr_free(tokens++->value);
}

void print_tokens(const token_t *tokens)
{
    for (; tokens->type != EOF_T; tokens++)
    {
        fputs(token_labels[tokens->type], stdout);

        if (tokens->value)
            printf(": %s", tokens->value);

        putchar('\n');
    }

    fputs("EOF\n", stdout);
}
