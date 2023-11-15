/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_TOKEN__
#define __MR_TOKEN__

#include <pos.h>

enum __TOKEN_TYPES
{
    EOF_T,              /**< End of file */

    NEWLINE_T,          /**< Newline */
    SEMICOLON_T,        /**< Semicolon ; */

    ID_T,               /**< Identifier */
    INT_T,              /**< Integer number */
    FLOAT_T,            /**< Floating point number */
    IMAG_T,             /**< Imaginary number */
    CHAR_T,             /**< Character */
    STR_T,              /**< String */

    MUL_T,              /**< Multiply * */
    DIV_T,              /**< Divide / */
    MOD_T,              /**< Modulo % */
    QUOT_T,             /**< Quotient // */
    POW_T,              /**< Power ** */

    B_AND_T,            /**< Binary and & */
    B_OR_T,             /**< Binary or | */
    B_XOR_T,            /**< Binary xor ^ */
    LSHIFT_T,           /**< Left shift << */
    RSHIFT_T,           /**< Right shift >> */

    EQ_T,               /**< Equal == */
    NEQ_T,              /**< Not equal != */
    EX_EQ_T,            /**< Exact equal === */
    EX_NEQ_T,           /**< Exact not equal !== */

    LT_T,               /**< Less than < */
    GT_T,               /**< Greater than > */
    LTE_T,              /**< Less than or equal to <= */
    GTE_T,              /**< Greater than or equal to >= */

    INC_T,              /**< Increment ++ */
    DEC_T,              /**< Decrement -- */
    ASSIGN_T,           /**< Assign = */
    LINK_T,             /**< Link -> */

    ADD_EQ_T,           /**< Add and store += */
    SUB_EQ_T,           /**< Subtract and store -= */
    MUL_EQ_T,           /**< Multiply and store *= */
    DIV_EQ_T,           /**< Divide and store /= */
    MOD_EQ_T,           /**< Modulo and store %= */
    QUOT_EQ_T,          /**< Quotient and store //= */
    POW_EQ_T,           /**< Power and store **= */

    B_AND_EQ_T,         /**< Binary and and store &= */
    B_OR_EQ_T,          /**< Binary or and store |= */
    B_XOR_EQ_T,         /**< Binary xor and store ^= */
    LSHIFT_EQ_T,        /**< Left shift and store <<= */
    RSHIFT_EQ_T,        /**< Right shift and store >>= */

    LPAREN_T,           /**< Left parenthesis ( */
    RPAREN_T,           /**< Right parenthesis ) */

    LSQUARE_T,          /**< Left square bracket [ */
    RSQUARE_T,          /**< Right square bracket ] */

    COMMA_T,            /**< Comma , */

    ADD_T,              /**< Add + */
    SUB_T,              /**< Subtract - */
    B_NOT_T,            /**< Binary not ~ */

    /**
     * 'not' keyword
     * Not !
    */
    NOT_KT,
    /**
     * 'and' keyword
     * And &&
    */
    AND_KT,
    /**
     * 'or' keyword
     * Or ||
    */
    OR_KT,

    VAR_KT,             /**< 'var' keyword */

    CONST_KT,           /**< 'const' keyword */

    NONE_KT,            /**< 'none' keyword */
    TRUE_KT,            /**< 'true' keyword */
    FALSE_KT,           /**< 'false' keyword */

    INT_TT,             /**< 'int' datatype */
    FLOAT_TT,           /**< 'float datatype */
    COMPLEX_TT,         /**< 'complex' datatype */

    BOOL_TT,            /**< 'bool' datatype */

    CHAR_TT,            /**< 'char' datatype */
    STR_TT,             /**< 'str' datatype */

    LIST_TT,            /**< 'list' datatype */
    TUPLE_TT            /**< 'tuple' datatype */
}; /**< Token types */

#define TOKENS_LEN 67 /**< Number of token types */

struct __TOKEN_T
{
    uint8_t type;               /**< Type of the token (from TOKEN_TYPES enum) */

    char *value;                /**< Value of the token (NULL if the token is a symbol)*/
    uint64_t size;              /**< Size of the value */

    pos_t poss;                 /**< Starting position of the token in the MetaReal raw code */
    pos_t pose;                 /**< Ending position of the token in the MetaReal raw code */
};
typedef struct __TOKEN_T token_t; /**< Word equivalent in MetaReal translation process */

extern const char *token_labels[TOKENS_LEN]; /**< Label of the tokens (for debugging) */

/**
 * \fn void tokens_free(token_t *tokens)
 * \pre
 * Gets the list of tokens and deallocates all the tokens inside it (not the tokens pointer itself).
 * The tokens list should end with an EOF token.
 * \param tokens The list of tokens
*/
void tokens_free(token_t *tokens);

/**
 * \fn void tokens_print(const token_t *tokens)
 * \pre
 * Prints the list of tokens in the stdout stream (for debugging).
 * The tokens list should end with an EOF token.
 * Each token is printed in a separate line.
 * \param tokens The list of tokens
*/
void tokens_print(const token_t *tokens);

#endif /* __MR_TOKEN__ */
