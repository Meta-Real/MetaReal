/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_ERROR__
#define __MR_ERROR__

#include <pos.h>

/**
 * Illegal character error (lexer)
*/
struct __ILLEGAL_CHAR_T
{
    char chr;               /**< Illegal or missing character */
    uint8_t expected;       /**< Is the character missing (1) or is it illegal (0) */

    pos_t pos;              /**< Position of the error */
};
typedef struct __ILLEGAL_CHAR_T illegal_char_t; /**< Illegal character error (lexer) */

/**
 * Invalid syntax error (parser)
*/
struct __INVALID_SYNTAX_T
{
    const char *detail;     /**< Detail of the error (NULL if the error is a general invalid syntax) */

    pos_t poss;             /**< Starting position of the error */
    pos_t pose;             /**< Ending position of the error */
};
typedef struct __INVALID_SYNTAX_T invalid_syntax_t; /**< Invalid syntax error (parser) */

enum __INVALID_SEMANTIC_TYPES /** Invalid semantic error types */
{
    ILLEGAL_OP_E,       /**< Illegal operation error */
    VALUE_E,            /**< Value error */
    TYPE_E,             /**< Type error */
    NOT_DEF_E,          /**< Not defined error */
    CONST_VAR_E,        /**< Constant variable error */
    ACCESS_E,           /**< Access error */
    MEM_OVERFLOW_E,     /**< Memory overflow error */
    INDEX_E,            /**< Index out of range error */
    DIV_BY_ZERO_E       /**< Divide by zero error */
};

/**
 * Invalid semantic error (optimizer)
*/
struct __INVALID_SEMANTIC_T
{
    char *detail;           /**< Detail of the error */
    uint8_t type;           /**< Type of the error (from INVALID_SEMANTIC_TYPES enum) */

    pos_t poss;             /**< Starting position of the error */
    pos_t pose;             /**< Ending position of the error */
};
typedef struct __INVALID_SEMANTIC_T invalid_semantic_t; /**< Invalid semantic error (optimizer) */

/**
 * \def illegal_char_set(c, e, p)
 * Creates a new illegal character error with the specified character, expected flag and position.
 * \param c Character of the error (char)
 * \param e Expected flag of the error (uint8_t)
 * \param p Position of the error (pos_t)
 * \return A new illegal character error (illegal_char_t)
*/
#define illegal_char_set(c, e, p) ((illegal_char_t){c, e, p})

/**
 * \def invalid_syntax_set(d, ps, pe)
 * Creates a new invalid syntax error with the specified detail, position start and position end.
 * \param d Detail of the error (const char*)
 * \param ps Starting position of the error (pos_t)
 * \param pe Ending position of the error (pos_t)
 * \return A new invalid syntax error (invalid_syntax_t)
*/
#define invalid_syntax_set(d, ps, pe) ((invalid_syntax_t){d, ps, pe})

/**
 * \def invalid_semantic_set(e, t, ps, pe)
 * Creates a new invalid semantic error with the specified type, position start and position end. \n
 * Error details must be specified separately.
 * \param e Return value (invalid_semantic_t)
 * \param t Type of the error (uint8_t)
 * \param ps Starting position of the error (pos_t)
 * \param pe Ending position of the error (pos_t)
 * \return A new invalid semantic error (invalid_semantic_t)
*/
#define invalid_semantic_set(e, t, ps, pe) \
    do                                     \
    {                                      \
        e.type = t;                        \
        e.poss = ps;                       \
        e.pose = pe;                       \
    } while (0)                            \

/**
 * \fn void illegal_char_print(const illegal_char_t *error, const char *fname, const char *code)
 * Prints the \a error into a stderr stream. \n
 * Marks under the illegal character and prints the line containing it.
 * \param error Illegal character error to be printed
 * \param fname Name of the MetaReal code file
 * \param code Content of the code
*/
void illegal_char_print(const illegal_char_t *error, const char *fname, const char *code);

/**
 * \fn void invalid_syntax_print(const invalid_syntax_t *error, const char *fname, const char *code)
 * Prints the \a error into a stderr stream. \n
 * Marks under the invalid expression and prints the first line containing it.
 * \param error Invalid syntax error to be printed
 * \param fname Name of the MetaReal code file
 * \param code Content of the code
*/
void invalid_syntax_print(const invalid_syntax_t *error, const char *fname, const char *code);

/**
 * \fn void invalid_semantic_print(invalid_semantic_t *error, const char *fname, const char *code)
 * Prints the \a error into a stderr stream. \n
 * Marks under the invalid operation and prints the first line containing it.
 * \param error Invalid semantic error to be printed
 * \param fname Name of the MetaReal code file
 * \param code Content of the code
*/
void invalid_semantic_print(invalid_semantic_t *error, const char *fname, const char *code);

#endif /* __MR_ERROR__ */
