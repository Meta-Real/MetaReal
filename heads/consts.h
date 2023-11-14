/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_CONSTS__
#define __MR_CONSTS__

#define MR_VERSION "1.0.0"

#define DEF_FILE_NAME "<stdin>"
#define DEF_CODE_SIZE 1024

/* lexer */

#define LEX_TOKEN_LIST_LEN 128

#define LEX_ID_SIZE 16
#define LEX_NUM_SIZE 8
#define LEX_STR_SIZE 64

/* parser */

#define PARSE_NODE_LIST_LEN 64

#define PARSE_LIST_LEN 16
#define PARSE_TUPLE_LEN 16

/* optimizer */

#define OPT_VARS_LIST_LEN 16

/* generator */

#define GEN_CONSTS_LEN 128
#define GEN_MAIN_LEN 1024

#define GEN_CONSTS_LIST_LEN 8

/* debug */

#define TRACK_ALLOC 0

#endif /* __MR_CONSTS__ */
