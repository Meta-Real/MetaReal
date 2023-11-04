/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __SEMANTIC_MR__
#define __SEMANTIC_MR__

#include <error/error.h>
#include <alloc.h>
#include <stdio.h>
#include <string.h>

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

#define type_mismatch(s, t1, t2, ps, pe)                                                          \
    do                                                                                            \
    {                                                                                             \
        res->error.detail = mr_alloc(52 + strlen(s) + value_name_lens[t1] + value_name_lens[t2]); \
        sprintf(res->error.detail, "Type of '%s' (<%s>) and type of value (<%s>) do not match",   \
            s, value_names[t1], value_names[t2]);                                                 \
                                                                                                  \
        res->value = NULL;                                                                        \
        invalid_semantic_set(res->error, TYPE_E, ps, pe);                                         \
    } while (0)

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
