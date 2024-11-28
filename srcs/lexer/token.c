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
 * @file token.c
 * This file contains definitions of the \a token.h file.
*/

#include <lexer/token.h>
#include <config.h>

mr_str_ct mr_token_keyword[MR_TOKEN_KEYWORD_COUNT] =
{
    "not",
    "true", "false", "none",
    "func", "struct", "class", "enum",
    "private", "public", "protected",
    "global", "readonly", "const", "static",
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
    3, 4, 5, 4, 4, 6, 5, 4, 7, 6, 9, 6, 8, 5,
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
{
    6, 3, 5, 7, 4, 3, 3, 4, 5, 4, 3, 4
};

mr_byte_t mr_token_symbol_size[MR_TOKEN_KEYWORD_PAD] =
{
    0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2,
    2, 2, 3, 3, 1, 1, 2, 2, 2, 2, 2,
    2, 1, 2, 2, 2, 2, 2, 2, 3, 3,
    2, 2, 2, 3, 3, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 3, 1, 1, 1
};

mr_long_t mr_token_getsize(
    mr_token_t *token)
{
    mr_long_t idx;

    idx = MR_IDX_EXTRACT(token->idx);
    if (token->type == MR_TOKEN_FSTR_START)
    {
        mr_long_t count;

        count = 0;
        while (1)
        {
            if ((++token)->type == MR_TOKEN_FSTR_START)
                count++;
            else if (token->type == MR_TOKEN_FSTR_END)
            {
                if (!count)
                    break;
                count--;
            }
        }

        return MR_IDX_EXTRACT(token->idx) - idx + 1;
    }

    return mr_token_getsize2(token->type, idx);
}

mr_long_t mr_token_getsize2(
    mr_byte_t type, mr_long_t idx)
{
    mr_long_t start;
    mr_chr_t chr, quot;
    mr_bool_t dot, esc;

    if (type >= MR_TOKEN_OBJECT_T)
        return mr_token_type_size[type - MR_TOKEN_TYPE_PAD];

    if (type == MR_TOKEN_AND_K)
        return _mr_config.code[idx] == '&' ? 2 : mr_token_keyword_size[MR_TOKEN_AND_K - MR_TOKEN_KEYWORD_PAD];
    if (type > MR_TOKEN_NOT_K)
        return mr_token_keyword_size[type - MR_TOKEN_KEYWORD_PAD];

    switch (type)
    {
    case MR_TOKEN_NOT_K:
        return _mr_config.code[idx] == '!' ? 1 : *mr_token_keyword_size;
    case MR_TOKEN_IDENTIFIER:
        start = idx++;
        chr = _mr_config.code[idx];

        while ((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') || (chr >= '0' && chr <= '9') || chr == '_')
            chr = _mr_config.code[++idx];

        return idx - start;
    case MR_TOKEN_INT:
        start = idx++;
        chr = _mr_config.code[idx];

        while ((chr >= '0' && chr <= '9') || chr == '_')
            chr = _mr_config.code[++idx];

        return idx - start;
    case MR_TOKEN_FLOAT:
        start = idx++;
        chr = _mr_config.code[idx];

        dot = MR_FALSE;
        while (1)
        {
            if (chr == '.')
            {
                if (dot)
                    break;
                dot = MR_TRUE;
            }
            else if (chr < '0' || (chr > '9' && chr != '_'))
                break;

            chr = _mr_config.code[++idx];
        }

        if (chr == 'e' || chr == 'E')
        {
            chr = _mr_config.code[++idx];
            if (chr == '+' || chr == '-')
                chr = _mr_config.code[++idx];

            while (chr >= '0' && chr <= '9')
                chr = _mr_config.code[++idx];
        }

        return idx - start;
    case MR_TOKEN_IMAGINARY:
        start = idx++;
        chr = _mr_config.code[idx];

        dot = MR_FALSE;
        while (1)
        {
            if (chr == '.')
            {
                if (dot)
                    break;
                dot = MR_TRUE;
            }
            else if (chr < '0' || (chr > '9' && chr != '_'))
                break;

            chr = _mr_config.code[++idx];
        }

        if (chr == 'e' || chr == 'E')
        {
            chr = _mr_config.code[++idx];
            if (chr == '+' || chr == '-')
                chr = _mr_config.code[++idx];

            while (chr >= '0' && chr <= '9')
                chr = _mr_config.code[++idx];
        }

        return idx - start + 1;
    case MR_TOKEN_CHR:
        return 3 + (_mr_config.code[idx + 1] == '\\');
    case MR_TOKEN_STR:
        start = idx;
        chr = _mr_config.code[idx];

        esc = MR_TRUE;
        if (chr == '\\')
        {
            esc = MR_FALSE;
            chr = _mr_config.code[++idx];
        }

        quot = chr;
        chr = _mr_config.code[++idx];
        while (chr != quot)
        {
            if (chr == '\\' && esc)
                idx++;

            chr = _mr_config.code[++idx];
        }

        return idx - start + 1;
    default:
        return mr_token_symbol_size[type];
    }
}

#ifdef __MR_DEBUG__

mr_str_ct mr_token_labels[MR_TOKEN_COUNT] =
{
    "TOKEN_EOF", "TOKEN_NEWLINE", "TOKEN_SEMICOLON",
    "TOKEN_IDENTIFIER",
    "TOKEN_INT", "TOKEN_FLOAT", "TOKEN_IMAGINARY", "TOKEN_CHR", "TOKEN_STR", "TOKEN_FSTR_END", "TOKEN_FSTR", "TOKEN_FSTR_START",
    "TOKEN_MULTIPLY", "TOKEN_DIVIDE", "TOKEN_MODULO", "TOKEN_QUOTIENT", "TOKEN_POWER",
    "TOKEN_B_AND", "TOKEN_B_OR", "TOKEN_B_XOR", "TOKEN_L_SHIFT", "TOKEN_R_SHIFT",
    "TOKEN_EQUAL", "TOKEN_NEQUAL", "TOKEN_EX_EQUAL", "TOKEN_EX_NEQUAL",
    "TOKEN_LESS", "TOKEN_GREATER", "TOKEN_LESS_EQUAL", "TOKEN_GREATER_EQUAL",
    "TOKEN_INCREMENT", "TOKEN_DECREMENT", "TOKEN_INCREMENT_POST", "TOKEN_DECREMENT_POST", "TOKEN_ASSIGN", "TOKEN_LINK",
    "TOKEN_PLUS_ASSIGN", "TOKEN_MINUS_ASSIGN",
    "TOKEN_MULTIPLY_ASSIGN", "TOKEN_DIVIDE_ASSIGN", "TOKEN_MODULO_ASSIGN", "TOKEN_QUOTIENT_ASSIGN", "TOKEN_POWER_ASSIGN",
    "TOKEN_B_AND_ASSIGN", "TOKEN_B_OR_ASSIGN", "TOKEN_B_XOR_ASSIGN", "TOKEN_L_SHIFT_ASSIGN", "TOKEN_R_SHIFT_ASSIGN",
    "TOKEN_L_PAREN", "TOKEN_R_PAREN",
    "TOKEN_L_SQUARE", "TOKEN_R_SQUARE",
    "TOKEN_L_CURLY", "TOKEN_R_CURLY",
    "TOKEN_COMMA", "TOKEN_DOT",
    "TOKEN_COLON", "TOKEN_QUESTION",
    "TOKEN_DOLLAR",
    "TOKEN_ELLIPSIS",
    "TOKEN_PLUS", "TOKENS_MINUS", "TOKEN_B_NOT", "TOKEN_NOT_K",
    "TOKEN_TRUE_K", "TOKEN_FALSE_K",
    "TOKEN_NONE_K",
    "TOKEN_FUNC_K", "TOKEN_STRUCT_K", "TOKEN_CLASS_K", "TOKEN_ENUM_K",
    "TOKEN_PRIVATE_K", "TOKEN_PUBLIC_K", "TOKEN_LOCAL_K", "TOKEN_GLOBAL_K", "TOKEN_CONST_K", "TOKEN_STATIC_K",
    "TOKEN_IS_K", "TOKEN_ARE_K", "TOKEN_IN_K", "TOKEN_AND_K", "TOKEN_OR_K",
    "TOKEN_IF_K", "TOKEN_ELIF_K", "TOKEN_ELSE_K",
    "TOKEN_SWITCH_K", "TOKEN_CASE_K", "TOKEN_DEFAULT_K",
    "TOKEN_FOR_K", "TOKEN_TO_K", "TOKEN_STEP_K",
    "TOKEN_WHILE_K", "TOKEN_DO_K",
    "TOKEN_TRY_K", "TOKEN_EXCEPT_K", "TOKEN_FINALLY_K",
    "TOKEN_RAISE_K",
    "TOKEN_IMPORT_K", "TOKEN_INCLUDE_K",
    "TOKEN_RETURN_K",
    "TOKEN_BREAK_K", "TOKEN_CONTINUE_K",
    "TOKEN_OBJECT_T",
    "TOKEN_INT_T", "TOKEN_FLOAT_T", "TOKEN_COMPLEX_T", "TOKEN_BOOL_T", "TOKEN_CHAR_T",
    "TOKEN_STR_T", "TOKEN_LIST_T", "TOKEN_TUPLE_T", "TOKEN_DICT_T", "TOKEN_SET_T",
    "TOKEN_TYPE_T"
};

void mr_token_print(
    mr_token_t *token)
{
    mr_long_t idx, size;

    printf("%s", mr_token_labels[token->type]);
    if (token->type == MR_TOKEN_EOF || token->type == MR_TOKEN_FSTR_START || token->type == MR_TOKEN_FSTR_END)
        return;

    idx = MR_IDX_EXTRACT(token->idx);
    if (_mr_config.code[idx] == '\n')
    {
        fputs(": \\n", stdout);
        return;
    }

    size = mr_token_getsize(token);
    if (size)
        printf(": %.*s", size, _mr_config.code + idx);
}

void mr_token_prints(
    mr_token_t *tokens)
{
    putchar('(');

    while (tokens->type != MR_TOKEN_EOF)
    {
        mr_token_print(tokens++);
        printf("), (");
    }

    printf("%s)", mr_token_labels[MR_TOKEN_EOF]);
}

#endif
