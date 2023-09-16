/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_TOKEN__
#define __MR_TOKEN__

#include <pos.h>

enum __TOKEN_TYPES
{
    EOF_T,

    SEMICOLON_T,

    ID_T,
    INT_T,
    FLOAT_T,
    IMAG_T,

    ADD_T,
    SUB_T,
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
    B_NOT_T,

    EQ_T,
    NEQ_T,
    EX_EQ_T,
    EX_NEQ_T,

    LT_T,
    GT_T,
    LTE_T,
    GTE_T,

    AND_T,
    OR_T,
    NOT_T,

    ASSIGN_T,

    LPAREN_T,
    RPAREN_T,

    AND_KT,
    OR_KT,
    NOT_KT,

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

extern const char *token_labels[38];

void free_tokens(token_t *tokens);
void print_tokens(const token_t *tokens);

#endif /* __MR_TOKEN__ */
