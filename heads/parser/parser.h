/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_PARSER__
#define __MR_PARSER__

#include <parser/node.h>
#include <lexer/token.h>
#include <error/error.h>

struct __PARSE_RES_T
{
    node_t *nodes;
    uint64_t size;

    invalid_syntax_t error;
};
typedef struct __PARSE_RES_T parse_res_t;

parse_res_t parse(token_t *tokens);

#endif /* __MR_PARSER__ */
