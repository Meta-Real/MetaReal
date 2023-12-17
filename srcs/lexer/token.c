/**
 * @file token.c
 * This file contains the function definitions of the "token.h" file.
*/

#include <lexer/token.h>
#include <stdio.h>
#include <alloc.h>

mr_str_ct mr_token_label[MR_TOKEN_COUNT] =
{
    "TOKEN_EOF", "TOKEN_NEWLINE", "TOKEN_SEMICOLON",
    "TOKEN_INT", "TOKEN_FLOAT", "TOKEN_IMAGINARY",
    "TOKEN_CHR", "TOKEN_STR", "TOKEN_FSTR_START", "TOKEN_FSTR_END",
    "TOKEN_PLUS", "TOKEN_MINUS", "TOKEN_MULTIPLY", "TOKEN_DIVIDE",
"TOKEN_MODULO", "TOKEN_QUOTIENT", "TOKEN_POWER",
    "TOKEN_B_AND", "TOKEN_B_OR", "TOKEN_B_XOR", "TOKEN_L_SHIFT",
"TOKEN_R_SHIFT", "TOKEN_B_NOT",
    "TOKEN_AND", "TOKEN_OR", "TOKEN_NOT",
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
    "TOKEN_TRUE_K", "TOKEN_FALSE_K", "TOKEN_NONE_K",
    "TOKEN_FUNC_K", "TOKEN_STRUCT_K", "TOKEN_CLASS_K", "TOKEN_ENUM_K",
    "TOKEN_PRIVATE_K", "TOKEN_PUBLIC_K", "TOKEN_LOCAL_K", "TOKEN_GLOBAL_K",
"TOKEN_CONST_K", "TOKEN_STATIC_K",
    "TOKEN_IS_K", "TOKEN_ARE_K", "TOKEN_IN_K", "TOKEN_AND_K",
"TOKEN_OR_K", "TOKEN_NOT_K",
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

void mr_token_free(mr_token_t *tokens)
{
    mr_token_t *ptr = tokens;

    while (ptr->type != MR_TOKEN_EOF)
        mr_aligned_free(ptr++->value);

    mr_free(tokens);
}

void mr_token_print(mr_token_t *tokens)
{
    do
    {
        fputs(mr_token_label[tokens->type], stdout);

        if (tokens->type == MR_TOKEN_CHR)
            fprintf(stdout, ": %c", (mr_chr_t)tokens->size);
        else if (tokens->value)
            fprintf(stdout, ": %s", tokens->value);

        fputc('\n', stdout);
    } while (tokens++->type != MR_TOKEN_EOF);
}
