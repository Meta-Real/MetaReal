/**
 * @file token.c
 * This file contains definitions of the \a token.h file.
*/

#include <lexer/token.h>
#include <stdlib.h>
#include <stdio.h>

mr_str_ct mr_token_label[MR_TOKEN_COUNT] =
{
    "TOKEN_EOF", "TOKEN_NEWLINE",
    "TOKEN_IDENTIFIER",
    "TOKEN_INT", "TOKEN_FLOAT", "TOKEN_IMAGINARY",
    "TOKEN_CHR", "TOKEN_STR", "TOKEN_FSTR", "TOKEN_FSTR_START",
"TOKEN_FSTR_END",
    "TOKEN_MULTIPLY", "TOKEN_DIVIDE", "TOKEN_MODULO", "TOKEN_QUOTIENT",
"TOKEN_POWER",
    "TOKEN_B_AND", "TOKEN_B_OR", "TOKEN_B_XOR", "TOKEN_L_SHIFT",
"TOKEN_R_SHIFT",
    "TOKEN_EQUAL", "TOKEN_NEQUAL", "TOKEN_EX_EQUAL", "TOKEN_EX_NEQUAL",
"TOKEN_LESS", "TOKEN_GREATER", "TOKEN_LESS_EQUAL", "TOKEN_GREATER_EQUAL",
    "TOKEN_INCREMENT", "TOKEN_DECREMENT",
    "TOKEN_ASSIGN", "TOKEN_LINK",
    "TOKEN_PLUS_ASSIGN", "TOKEN_MINUS_ASSIGN", "TOKEN_MULTIPLY_ASSIGN", "TOKEN_DIVIDE_ASSIGN",
"TOKEN_MODULO_ASSIGN", "TOKEN_QUOTIENT_ASSIGN", "TOKEN_POWER_ASSIGN", "TOKEN_B_AND_ASSIGN",
"TOKEN_B_OR_ASSIGN", "TOKEN_B_XOR_ASSIGN", "TOKEN_L_SHIFT_ASSIGN", "TOKEN_R_SHIFT_ASSIGN",
    "TOKEN_L_PAREN", "TOKEN_R_PAREN", "TOKEN_L_SQUARE", "TOKEN_R_SQUARE",
"TOKEN_L_CURLY", "TOKEN_R_CURLY",
    "TOKEN_COMMA", "TOKEN_DOT", "TOKEN_COLON", "TOKEN_QUESTION",
"TOKEN_DOLLAR", "TOKEN_ELLIPSIS",
    "TOKEN_PLUS", "TOKEN_MINUS", "TOKEN_B_NOT", "TOKEN_NOT_K",
    "TOKEN_TRUE_K", "TOKEN_FALSE_K", "TOKEN_NONE_K",
    "TOKEN_FUNC_K", "TOKEN_STRUCT_K", "TOKEN_CLASS_K", "TOKEN_ENUM_K",
    "TOKEN_PRIVATE_K", "TOKEN_PUBLIC_K", "TOKEN_LOCAL_K", "TOKEN_GLOBAL_K",
"TOKEN_CONST_K", "TOKEN_STATIC_K",
    "TOKEN_IS_K", "TOKEN_ARE_K", "TOKEN_IN_K", "TOKEN_AND_K",
"TOKEN_OR_K",
    "TOKEN_IF_K", "TOKEN_ELIF_K", "TOKEN_ELSE_K",
    "TOKEN_SWITCH_K", "TOKEN_CASE_K", "TOKEN_DEFAULT_K",
    "TOKEN_FOR_K", "TOKEN_TO_K", "TOKEN_STEP_K",
    "TOKEN_WHILE_K", "TOKEN_DO_K",
    "TOKEN_TRY_K", "TOKEN_EXCEPT_K", "TOKEN_FINALLY_K", "TOKEN_RAISE_K",
    "TOKEN_IMPORT_K", "TOKEN_INCLUDE_K",
    "TOKEN_RETURN_K", "TOKEN_BREAK_K", "TOKEN_CONTINUE_K",
    "TOKEN_OBJECT_T", "TOKEN_INT_T", "TOKEN_FLOAT_T", "TOKEN_COMPLEX_T",
"TOKEN_BOOL_T", "TOKEN_CHAR_T", "TOKEN_STR_T", "TOKEN_LIST_T",
"TOKEN_TUPLE_T", "TOKEN_DICT_T", "TOKEN_SET_T", "TOKEN_TYPE_T"
};

mr_str_ct mr_token_keyword[MR_TOKEN_KEYWORD_COUNT] =
{
    "not",
    "true", "false", "none",
    "func", "struct", "class", "enum",
    "private", "public", "local", "global", "const", "static",
    "is", "are", "in", "and", "or",
    "if", "elif", "else",
    "switch", "case", "default",
    "for", "to", "step",
    "while", "do",
    "try", "except", "finally", "raise",
    "import", "include",
    "return", "break", "continue"
};

mr_byte_t mr_token_keyword_size[MR_TOKEN_KEYWORD_COUNT] =
{
    3, 4, 5, 4, 4, 6, 5, 4, 7, 6, 5, 6, 5,
    6, 2, 3, 2, 3, 2, 2, 4, 4, 6, 4, 7, 3,
    2, 4, 5, 2, 3, 6, 7, 5, 6, 7, 6, 5, 8
};

mr_str_ct mr_token_type[MR_TOKEN_TYPE_COUNT] =
{
    "object",
    "int", "float", "complex", "bool",
    "chr", "str",
    "list", "tuple", "dict", "set",
    "type"
};

mr_byte_t mr_token_type_size[MR_TOKEN_TYPE_COUNT] =
{ 6, 3, 5, 7, 4, 3, 3, 4, 5, 4, 3, 4 };

void mr_tokens_free(
    mr_token_t *tokens)
{
    mr_token_t *ptr = tokens;

    while (ptr->type != MR_TOKEN_EOF)
    {
        if (ptr->type != MR_TOKEN_IDENTIFIER && ptr->type != MR_TOKEN_CHR)
            free(ptr->value);

        ptr++;
    }

    free(tokens);
}

void mr_tokens_print(
    mr_token_t *tokens)
{
    do
    {
        fputs(mr_token_label[tokens->type], stdout);

        if (tokens->type == MR_TOKEN_CHR)
            fprintf(stdout, ": '%c'", (mr_chr_t)(uintptr_t)tokens->value);
        else if (tokens->value)
        {
            fputs(": ", stdout);

            if (tokens->type == MR_TOKEN_IDENTIFIER)
                fwrite(tokens->value, sizeof(mr_chr_t), tokens->size, stdout);
            else
                fputs(tokens->value, stdout);
        }

        fputc('\n', stdout);
    } while (tokens++->type != MR_TOKEN_EOF);
}
