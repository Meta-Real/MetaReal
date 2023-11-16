/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_CONSTS__
#define __MR_CONSTS__

#define MR_VERSION "1.0.0"          /**< Current version of the MetaReal */

#define DEF_FILE_NAME "<stdin>"     /**< Name of the default input stream */
#define DEF_CODE_SIZE 1024          /**< Maximum code size inputted from the stdin */

/* lexer */

#define LEX_TOKEN_LIST_LEN 128      /**< Default length of a list of tokens */

#define LEX_ID_SIZE 16              /**< Default length of an identifier in characters */
#define LEX_NUM_SIZE 8              /**< Default length of a number in characters */
#define LEX_STR_SIZE 64             /**< Default length of a string in characters */

/* parser */

#define PARSE_NODE_LIST_LEN 64      /**< Default length of a list of nodes */

#define PARSE_LIST_LEN 16           /**< Default length of a list object */
#define PARSE_TUPLE_LEN 16          /**< Default length of a tuple object */

/* optimizer */

#define OPT_VARS_LIST_LEN 16        /**< Default length of the list of variables */

/* generator */

#define GEN_CONSTS_LEN 128          /**< Default length of the assembly consts section in characters */
#define GEN_MAIN_LEN 1024           /**< Default length of the assembly main section in characters */

#define GEN_CONSTS_LIST_LEN 8       /**< Default length of the list of constants */

/* debug */

#define TRACK_ALLOC 0               /**< Debugging the allocation process (1) or not (0) */

#endif /* __MR_CONSTS__ */
