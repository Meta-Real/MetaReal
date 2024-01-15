/**
 * @file token.c
 * This file contains definitions of the \a token.h file.
*/

#include <lexer/token.h>
#include <config.h>
#include <stdlib.h>
#include <stdio.h>

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
{
    6, 3, 5, 7, 4, 3, 3, 4, 5, 4, 3, 4
};

mr_byte_t mr_token_symbol_size[MR_TOKEN_KEYWORD_PAD] =
{
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 2, 2, 1, 1, 1, 2,
    2, 2, 2, 3, 3, 1, 1, 2, 2, 2,
    2, 1, 2, 2, 2, 2, 2, 2, 3, 3,
    2, 2, 2, 3, 3, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 3, 1, 1, 1
};

mr_long_t mr_token_getsize(
    mr_byte_t type, mr_long_t idx)
{
    if (type >= MR_TOKEN_OBJECT_T)
        return mr_token_type_size[type - MR_TOKEN_TYPE_PAD];

    if (type == MR_TOKEN_AND_K)
        return _mr_config.code[idx] == '&' ? 2 :
            mr_token_keyword_size[MR_TOKEN_AND_K - MR_TOKEN_KEYWORD_PAD];
    if (type > MR_TOKEN_NOT_K)
        return mr_token_keyword_size[type - MR_TOKEN_KEYWORD_PAD];

    switch (type)
    {
    case MR_TOKEN_NOT_K:
        return _mr_config.code[idx] == '!' ? 1 : *mr_token_keyword_size;
    case MR_TOKEN_IDENTIFIER:
    {
        mr_long_t start = idx++;
        mr_chr_t chr = _mr_config.code[idx];

        while ((chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') ||
            (chr >= '0' && chr <= '9') || chr == '_')
            chr = _mr_config.code[++idx];

        return idx - start;
    }
    case MR_TOKEN_INT:
    {
        mr_long_t start = idx++;
        mr_chr_t chr = _mr_config.code[idx];

        while ((chr >= '0' && chr <= '9') || chr == '_')
            chr = _mr_config.code[++idx];

        return idx - start;
    }
    case MR_TOKEN_FLOAT:
    {
        mr_long_t start = idx++;
        mr_chr_t chr = _mr_config.code[idx];

        mr_bool_t dot = MR_FALSE;
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
    }
    case MR_TOKEN_IMAGINARY:
    {
        mr_long_t start = idx++;
        mr_chr_t chr = _mr_config.code[idx];

        mr_bool_t dot = MR_FALSE;
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
    }
    case MR_TOKEN_CHR:
        return 3 + (_mr_config.code[idx + 1] == '\\');
    case MR_TOKEN_STR:
    {
        mr_long_t start = idx++;
        mr_chr_t chr = _mr_config.code[idx];

        mr_bool_t esc = MR_FALSE;
        if (chr == '\\')
        {
            chr = _mr_config.code[++idx];
            if (chr == '\\')
                esc = MR_TRUE;

            if (chr == 'f')
                chr = _mr_config.code[++idx];
        }
        else if (chr == 'f')
        {
            chr = _mr_config.code[++idx];

            if (chr == '\\')
            {
                esc = MR_TRUE;
                chr = _mr_config.code[++idx];
            }
        }

        mr_chr_t quot = _mr_config.code[idx];
        chr = _mr_config.code[++idx];
        while (chr != quot)
        {
            if (chr == '\\' && esc)
                idx++;

            chr = _mr_config.code[++idx];
        }

        return idx - start + 1;
    }
    default:
        return mr_token_symbol_size[type];
    }
}
