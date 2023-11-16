/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __SEMANTIC_MR__
#define __SEMANTIC_MR__

#include <error/error.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

/**
 * \def access_datatype_error(t, ps, pe)
 * Generates an invalid semantic error for invalidly accessing a value. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param t Type of the value (uint8_t)
 * \param ps Starting position of the value (pos_t)
 * \param pe Ending position of the value (pos_t)
*/
#define access_datatype_error(t, ps, pe)                                       \
    do                                                                         \
    {                                                                          \
        res->error.detail = mr_alloc(39 + value_name_lens[t]);                 \
        sprintf(res->error.detail, "<%s> can not be accessed like a variable", \
            value_names[t]);                                                   \
                                                                               \
        res->value = NULL;                                                     \
        invalid_semantic_set(res->error, ACCESS_E, ps, pe);                    \
        return;                                                                \
    } while (0)

/**
 * \def access_statement_error(s, sl, ps, pe)
 * Generates an invalid semantic error for invalidly accessing a statement. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param s Name of the statement (const char*)
 * \param sl Length of the \a s (to avoid calling strlen) (uint64_t)
 * \param ps Starting position of the statement (pos_t)
 * \param pe Ending position of the statement (pos_t)
*/
#define access_statement_error(s, sl, ps, pe)                                \
    do                                                                       \
    {                                                                        \
        res->error.detail = mr_alloc(37 + sl);                               \
        strcpy(res->error.detail, s " can not be accessed like a variable"); \
                                                                             \
        res->value = NULL;                                                   \
        invalid_semantic_set(res->error, ACCESS_E, ps, pe);                  \
        return;                                                              \
    } while (0)

/**
 * \def not_def_error(s, ps, pe)
 * Generates an invalid semantic error for calling an undefined variable. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro. \n
 * It automatically deallocates name of the variable.
 * \param s Name of the variable (char*)
 * \param ps Starting position of the variable name (pos_t)
 * \param pe Ending position of the variable name (pos_t)
*/
#define not_def_error(s, ps, pe)                             \
    do                                                       \
    {                                                        \
        res->error.detail = mr_alloc(18 + strlen(s));        \
        sprintf(res->error.detail, "'%s' is not defined",    \
            s);                                              \
                                                             \
        res->value = NULL;                                   \
        invalid_semantic_set(res->error, NOT_DEF_E, ps, pe); \
                                                             \
        mr_free(s);                                          \
        return;                                              \
    } while (0)

/**
 * \def const_var_error(s, ps, pe)
 * Generates an invalid semantic error for changing a constant variable. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro. \n
 * It automatically deallocates name of the constant variable.
 * \param s Name of the constant variable (char*)
 * \param ps Starting position of the variable name (pos_t)
 * \param pe Ending position of the variable name (pos_t)
*/
#define const_var_error(s, ps, pe)                             \
    do                                                         \
    {                                                          \
        res->error.detail = mr_alloc(15 + strlen(s));          \
        sprintf(res->error.detail, "'%s' is constant",         \
            s);                                                \
                                                               \
        res->value = NULL;                                     \
        invalid_semantic_set(res->error, CONST_VAR_E, ps, pe); \
                                                               \
        mr_free(s);                                            \
        return;                                                \
    } while (0)

/**
 * \def type_mismatch(s, t1, t2, ps, pe)
 * Generates an invalid semantic error for mismatching type of the variable and its value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param s Name of the variable (const char*)
 * \param t1 Type of the variable (uint8_t)
 * \param t2 Type of the value (uint8_t)
 * \param ps Starting position of the assignment (pos_t)
 * \param pe Ending position of the assignment (pos_t)
*/
#define type_mismatch(s, t1, t2, ps, pe)                                                            \
    do                                                                                              \
    {                                                                                               \
        res->error.detail = mr_alloc(56 + strlen(s) + value_name_lens[t1] + value_name_lens[t2]);   \
        sprintf(res->error.detail, "Type of '%s' (<%s>) and type of the value (<%s>) do not match", \
            s, value_names[t1], value_names[t2]);                                                   \
                                                                                                    \
        res->value = NULL;                                                                          \
        invalid_semantic_set(res->error, TYPE_E, ps, pe);                                           \
    } while (0)

/**
 * \def type_change(s, t1, t2, ps, pe)
 * Generates an invalid semantic error for changing type of a fixed-type variable. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro. \n
 * It automatically deallocates name of the fixed-type variable.
 * \param s Name of the fixed-type variable (char*)
 * \param t1 Old type of the variable (uint8_t)
 * \param t2 New type of the variable (uint8_t)
 * \param ps Starting position of the assignment (pos_t)
 * \param pe Ending position of the assignment (pos_t)
*/
#define type_change(s, t1, t2, ps, pe)                                                            \
    do                                                                                            \
    {                                                                                             \
        res->error.detail = mr_alloc(44 + strlen(s) + value_name_lens[t1] + value_name_lens[t2]); \
        sprintf(res->error.detail, "Can not change the type of '%s' from <%s> to <%s>",           \
            s, value_names[t1], value_names[t2]);                                                 \
                                                                                                  \
        res->value = NULL;                                                                        \
        invalid_semantic_set(res->error, TYPE_E, ps, pe);                                         \
                                                                                                  \
        mr_free(s);                                                                               \
        return;                                                                                   \
    } while (0)

/* operations */

/**
 * \def ill_op_error(o, ol, t1, t2, ps, pe)
 * Generates an invalid semantic error for an illegal operation between two values. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param o Symbol of the operator (const char*)
 * \param ol Length of the \a o (to avoid calling strlen) (uint64_t)
 * \param t1 Type of the left operand (uint8_t)
 * \param t2 Type of the right operand (uint8_t)
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define ill_op_error(o, ol, t1, t2, ps, pe)                                                  \
    do                                                                                       \
    {                                                                                        \
        res->error.detail = mr_alloc(46 + ol + value_name_lens[t1] + value_name_lens[t2]);   \
        sprintf(res->error.detail, "Invalid binary operation (" o ") between <%s> and <%s>", \
            value_names[t1], value_names[t2]);                                               \
                                                                                             \
        res->value = NULL;                                                                   \
        invalid_semantic_set(res->error, ILLEGAL_OP_E, ps, pe);                              \
        return;                                                                              \
    } while (0)

/**
 * \def ill_unary_op_error(o, ol, t, ps, pe)
 * Generates an invalid semantic error for an illegal operation for a value. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param o Symbol of the operator (const char*)
 * \param ol Length of the \a o (to avoid calling strlen) (uint64_t)
 * \param t Type of the operand (uint8_t)
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define ill_unary_op_error(o, ol, t, ps, pe)                                   \
    do                                                                         \
    {                                                                          \
        res->error.detail = mr_alloc(34 + ol + value_name_lens[t]);            \
        sprintf(res->error.detail, "Invalid unary operation (" o ") for <%s>", \
            value_names[t]);                                                   \
                                                                               \
        res->value = NULL;                                                     \
        invalid_semantic_set(res->error, ILLEGAL_OP_E, ps, pe);                \
        return;                                                                \
    } while (0)

/**
 * \def ill_inc_dec_op_error(o, t, ps, pe)
 * Generates an invalid semantic error for an illegal incrementing or decrementing. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param o Name of the operation (must be "incremented" or "decremented") (const char*)
 * \param t Type of the operand (uint8_t)
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define ill_inc_dec_op_error(o, t, ps, pe)                      \
    do                                                          \
    {                                                           \
        res->error.detail = mr_alloc(26 + value_name_lens[t]);  \
        sprintf(res->error.detail, "<%s> can not be " o,        \
            value_names[t]);                                    \
                                                                \
        res->value = NULL;                                      \
        invalid_semantic_set(res->error, ILLEGAL_OP_E, ps, pe); \
        return;                                                 \
    } while (0)

/**
 * \def mem_overflow_error(ps, pe)
 * Generates an invalid semantic error for a memory overflow situation. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param ps Starting position of the expression (pos_t)
 * \param pe Ending position of the expression (pos_t)
*/
#define mem_overflow_error(ps, pe)                                \
    do                                                            \
    {                                                             \
        res->error.detail = mr_alloc(16);                         \
        strcpy(res->error.detail, "Memory overflow");             \
                                                                  \
        res->value = NULL;                                        \
        invalid_semantic_set(res->error, MEM_OVERFLOW_E, ps, pe); \
        return;                                                   \
    } while (0)

/**
 * \def index_out_error(ps, pe)
 * Generates an invalid semantic error for accessing an invalid address in the memory. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param ps Starting position of the expression (pos_t)
 * \param pe Ending position of the expression (pos_t)
*/
#define index_out_error(ps, pe)                            \
    do                                                     \
    {                                                      \
        res->error.detail = mr_alloc(19);                  \
        strcpy(res->error.detail, "Index out of range");   \
                                                           \
        res->value = NULL;                                 \
        invalid_semantic_set(res->error, INDEX_E, ps, pe); \
        return;                                            \
    } while (0)

/**
 * \def div_by_zero_error(ps, pe)
 * Generates an invalid semantic error for dividing by zero. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define div_by_zero_error(ps, pe)                                \
    do                                                           \
    {                                                            \
        res->error.detail = mr_alloc(17);                        \
        strcpy(res->error.detail, "Division by zero");           \
                                                                 \
        res->value = NULL;                                       \
        invalid_semantic_set(res->error, DIV_BY_ZERO_E, ps, pe); \
        return;                                                  \
    } while (0)

/**
 * \def mod_by_zero_error(ps, pe)
 * Generates an invalid semantic error for modulo by zero. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define mod_by_zero_error(ps, pe)                                \
    do                                                           \
    {                                                            \
        res->error.detail = mr_alloc(15);                        \
        strcpy(res->error.detail, "Modulo by zero");             \
                                                                 \
        res->value = NULL;                                       \
        invalid_semantic_set(res->error, DIV_BY_ZERO_E, ps, pe); \
        return;                                                  \
    } while (0)

/**
 * \def ztn_power_error(ps, pe)
 * Generates an invalid semantic error for raising zero to a negative power. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define ztn_power_error(ps, pe)                                                     \
    do                                                                              \
    {                                                                               \
        res->error.detail = mr_alloc(46);                                           \
        strcpy(res->error.detail, "Zero raised to the power of a negative number"); \
                                                                                    \
        res->value = NULL;                                                          \
        invalid_semantic_set(res->error, DIV_BY_ZERO_E, ps, pe);                    \
        return;                                                                     \
    } while (0)

/**
 * \def ztc_power_error(ps, pe)
 * Generates an invalid semantic error for raising zero to a complex power. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define ztc_power_error(ps, pe)                                                    \
    do                                                                             \
    {                                                                              \
        res->error.detail = mr_alloc(45);                                          \
        strcpy(res->error.detail, "Zero raised to the power of a complex number"); \
                                                                                   \
        res->value = NULL;                                                         \
        invalid_semantic_set(res->error, VALUE_E, ps, pe);                         \
        return;                                                                    \
    } while (0)

/**
 * \def neg_shift_error(ps, pe)
 * Generates an invalid semantic error for left or right shifting by a negative amount. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define neg_shift_error(ps, pe)                            \
    do                                                     \
    {                                                      \
        res->error.detail = mr_alloc(21);                  \
        strcpy(res->error.detail, "Negative shift count"); \
                                                           \
        res->value = NULL;                                 \
        invalid_semantic_set(res->error, VALUE_E, ps, pe); \
        return;                                            \
    } while (0)

/**
 * \def neg_multiplier_error(ps, pe)
 * Generates an invalid semantic error for multiplying by a negative number. \n
 * This error usually applies for strings and iterables. \n
 * It returns from the function that called it. \n
 * The function calling this macro should not return any value. \n
 * A visit_res must be defined as \a res before calling this macro.
 * \param ps Starting position of the operation (pos_t)
 * \param pe Ending position of the operation (pos_t)
*/
#define neg_multiplier_error(ps, pe)                       \
    do                                                     \
    {                                                      \
        res->error.detail = mr_alloc(20);                  \
        strcpy(res->error.detail, "Negative multiplier");  \
                                                           \
        res->value = NULL;                                 \
        invalid_semantic_set(res->error, VALUE_E, ps, pe); \
        return;                                            \
    } while (0)

#endif /* __SEMANTIC_MR__ */
