/*
    MetaReal Compiler version 1.0.0
*/

#include <lexer/token.h>
#include <alloc.h>
#include <stdio.h>

const char *token_labels[57] =
{
    "EOF",
    "NEWLINE", "SEMICOLON",
    "ID", "INT", "FLOAT", "IMAG",
    "MUL", "DIV", "MOD", "QUOT", "POW",
    "B_AND", "B_OR", "B_XOR", "LSHIFT", "RSHIFT",
    "EQ", "NEQ", "EX_EQ", "EX_NEQ",
    "LT", "GT", "LTE", "GTE",
    "INC", "DEC", "ASSIGN", "LINK",
    "ADD_EQ", "SUB_EQ", "MUL_EQ", "DIV_EQ", "MOD_EQ", "QUOT_EQ", "POW_EQ",
    "B_AND_EQ", "B_OR_EQ", "B_XOR_EQ", "LSHIFT_EQ", "RSHIFT_EQ",
    "LPAREN", "RPAREN",
    "LSQUARE", "RSQUARE",
    "COMMA",
    "ADD", "SUB", "B_NOT",
    "NOT_K", "OR_K", "NOT_K",
    "VAR_K",
    "CONST_K",
    "NONE_K", "TRUE_K", "FALSE_K"
};

void tokens_free(token_t *tokens)
{
    while (tokens->type)
        mr_free(tokens++->value);
}

void tokens_print(const token_t *tokens)
{
    for (; tokens->type; tokens++)
    {
        fputs(token_labels[tokens->type], stdout);

        if (tokens->value)
            printf(": %s", tokens->value);

        putchar('\n');
    }

    fputs("EOF\n", stdout);
}
