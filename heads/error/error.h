/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_ERROR__
#define __MR_ERROR__

#include <pos.h>

struct __ILLEGAL_CHAR_T
{
    char chr;
    uint8_t expected;

    pos_t pos;
};
typedef struct __ILLEGAL_CHAR_T illegal_char_t;

struct __INVALID_SYNTAX_T
{
    const char *detail;

    pos_t poss;
    pos_t pose;
};
typedef struct __INVALID_SYNTAX_T invalid_syntax_t;

enum __INVALID_SEMANTIC_TYPES
{
    ILLEGAL_OP_E,
    VALUE_E,
    NOT_DEF_E,
    ACCESS_E,
    MEM_OVERFLOW_E,
    INDEX_E,
    DIV_BY_ZERO_E
};

struct __INVALID_SEMANTIC_T
{
    char *detail;
    uint8_t type;

    pos_t poss;
    pos_t pose;
};
typedef struct __INVALID_SEMANTIC_T invalid_semantic_t;

#define illegal_char_set(c, e, p) ((illegal_char_t){c, e, p})
#define invalid_syntax_set(d, ps, pe) ((invalid_syntax_t){d, ps, pe})

#define invalid_semantic_set(e, t, ps, pe) \
    do                                     \
    {                                      \
        e.type = t;                        \
        e.poss = ps;                       \
        e.pose = pe;                       \
    } while (0)                            \

void illegal_char_print(const illegal_char_t *error, const char *fname, const char *code);
void invalid_syntax_print(const invalid_syntax_t *error, const char *fname, const char *code);
void invalid_semantic_print(invalid_semantic_t *error, const char *fname, const char *code);

#endif /* __MR_ERROR__ */
