/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_TOKEN__
#define __MR_TOKEN__

#include <pos.h>

enum __TOKEN_TYPES
{
    EOF_T,

    NEWLINE_T,
    SEMICOLON_T,

    ID_T,
    INT_T,
    FLOAT_T,
    IMAG_T,

    MUL_T,
    DIV_T,
    MOD_T,
    QUOT_T,
    POW_T,

    B_AND_T,
    B_OR_T,
    B_XOR_T,
    LSHIFT_T,
    RSHIFT_T,

    EQ_T,
    NEQ_T,
    EX_EQ_T,
    EX_NEQ_T,

    LT_T,
    GT_T,
    LTE_T,
    GTE_T,

    INC_T,
    DEC_T,
    ASSIGN_T,
    LINK_T,

    ADD_EQ_T,
    SUB_EQ_T,
    MUL_EQ_T,
    DIV_EQ_T,
    MOD_EQ_T,
    QUOT_EQ_T,
    POW_EQ_T,

    B_AND_EQ_T,
    B_OR_EQ_T,
    B_XOR_EQ_T,
    LSHIFT_EQ_T,
    RSHIFT_EQ_T,

    LPAREN_T,
    RPAREN_T,

    LSQUARE_T,
    RSQUARE_T,

    COMMA_T,

    ADD_T,
    SUB_T,
    B_NOT_T,

    NOT_KT,
    AND_KT,
    OR_KT,

    VAR_KT,

    CONST_KT,

    NONE_KT,
    TRUE_KT,
    FALSE_KT
};

struct __TOKEN_T
{
    uint8_t type;
    char *value;

    pos_t poss;
    pos_t pose;
};
typedef struct __TOKEN_T token_t;

extern const char *token_labels[57];

void tokens_free(token_t *tokens);
void tokens_print(const token_t *tokens);

#endif /* __MR_TOKEN__ */
