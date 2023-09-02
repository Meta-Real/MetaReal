/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_ERROR__
#define __MR_ERROR__

#include <pos.h>

struct __ILLEGAL_CHAR_T
{
    char chr;

    pos_t pos;
};
typedef struct __ILLEGAL_CHAR_T illegal_char_t;

#define set_illegal_char(c, p) ((illegal_char_t){c, p})

void print_illegal_char(const illegal_char_t *error, const char *fname, const char *code);

#endif /* __MR_ERROR__ */
