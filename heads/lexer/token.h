/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_TOKEN__
#define __MR_TOKEN__

#include <pos.h>

enum __TOKEN_TYPES
{
    EOF_T,

    INT_T,

    ADD_T,
    SUB_T,
    MUL_T
};

struct __TOKEN_T
{
    uint8_t type;
    char *value;
    uint64_t size;

    pos_t poss;
    pos_t pose;
};
typedef struct __TOKEN_T token_t;

extern const char *token_labels[MUL_T + 1];

void free_tokens(token_t *tokens);
void print_tokens(const token_t *tokens);

#endif /* __MR_TOKEN__ */
