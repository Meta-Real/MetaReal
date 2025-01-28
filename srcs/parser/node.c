/*
MIT License

Copyright (c) 2023 MetaReal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

/**
 * @file node.c
 * File that contains definitions of the node data structure.
*/

#include <parser/node.h>
#include <lexer/token.h>
#include <stack.h>

/**
 * @def mr_node_sidx_std(typ)
 * It handles the standard case of the \a mr_node_sidx function where the starting index is one of the node's fields.
 * @param typ
 * Type of the structure.
*/
#define mr_node_sidx_std(typ)                        \
    {                                                \
        typ *value;                                  \
                                                     \
        value = (typ*)(_mr_stack.data + node.value); \
        return MR_IDX_EXTRACT(value->sidx);          \
    }

/**
 * @def mr_node_sidx_elem(typ, elem)
 * It handles a case of the \a mr_node_sidx function where the starting index is inside of one of the node's elements.
 * @param typ
 * Type of the structure.
 * @param elem
 * The element that contains the starting index of the whole node.
*/
#define mr_node_sidx_elem(typ, elem)                 \
    {                                                \
        typ *value;                                  \
                                                     \
        value = (typ*)(_mr_stack.data + node.value); \
        return mr_node_sidx(value->elem);            \
    }

/**
 * @def mr_node_eidx_std(typ)
 * It handles the standard case of the \a mr_node_eidx function where the ending index is one of the node's fields.
 * @param typ
 * Type of the structure.
*/
#define mr_node_eidx_std(typ)                        \
    {                                                \
        typ *value;                                  \
                                                     \
        value = (typ*)(_mr_stack.data + node.value); \
        return MR_IDX_EXTRACT(value->eidx);          \
    }

/**
 * @def mr_node_eidx_elem(typ, elem)
 * It handles a case of the \a mr_node_eidx function where the ending index is inside of one of the node's elements.
 * @param typ
 * Type of the structure.
 * @param elem
 * The element that contains the ending index of the whole node.
*/
#define mr_node_eidx_elem(typ, elem)                 \
    {                                                \
        typ *value;                                  \
                                                     \
        value = (typ*)(_mr_stack.data + node.value); \
        return mr_node_eidx(value->elem);            \
    }

/**
 * Returns the corrsponding token type given its node type.
 * @param type
 * Type of the specified node.
 * @return It returns the corrsponding token type. \n
 * The function returns \a MR_TOKEN_EOF in case of an error.
*/
mr_byte_t mr_node_get_token(
    mr_byte_t type);

mr_long_t mr_node_sidx(
    mr_node_t node)
{
    switch (node.type)
    {
    case MR_NODE_NULL:
        return MR_INVALID_IDX_CODE;
    case MR_NODE_NONE:
    case MR_NODE_INT:
    case MR_NODE_FLOAT:
    case MR_NODE_IMAGINARY:
    case MR_NODE_CHR:
    case MR_NODE_STR:
    case MR_NODE_VAR_ACCESS:
        return node.value;
    case MR_NODE_BOOL:
    case MR_NODE_TYPE:
    {
        mr_idx_t idx;

        idx = ((mr_token_t*)&node.value)->idx;
        return MR_IDX_EXTRACT(idx);
    }
    case MR_NODE_FSTR:
    case MR_NODE_LIST:
    case MR_NODE_DICT:
    case MR_NODE_SET:
    case MR_NODE_MULTILINE:
        mr_node_sidx_std(mr_node_list_t);
    case MR_NODE_TUPLE:
    case MR_NODE_MULTILINE_TUPLE:
    {
        mr_node_tuple_t *value;
        mr_node_t *elems;

        value = (mr_node_tuple_t*)(_mr_stack.data + node.value);
        elems = (mr_node_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->elems)];
        return mr_node_sidx(*elems);
    }
    case MR_NODE_BINARY_OP:
        mr_node_sidx_elem(mr_node_binary_op_t, left);
    case MR_NODE_UNARY_OP:
        mr_node_sidx_std(mr_node_unary_op_t);
    case MR_NODE_TERNARY_OP:
        mr_node_sidx_elem(mr_node_ternary_op_t, cond);
    case MR_NODE_SUBSCRIPT:
        mr_node_sidx_elem(mr_node_subscript_t, node);
    case MR_NODE_SUBSCRIPT_END:
        mr_node_sidx_elem(mr_node_subscript_end_t, node);
    case MR_NODE_SUBSCRIPT_STEP:
        mr_node_sidx_elem(mr_node_subscript_step_t, node);
    case MR_NODE_VAR_ASSIGN:
        mr_node_sidx_std(mr_node_var_assign_t);
    case MR_NODE_FUNC_CALL:
        mr_node_sidx_elem(mr_node_func_call_t, func);
    case MR_NODE_EX_FUNC_CALL:
        mr_node_sidx_elem(mr_node_ex_func_call_t, func);
    case MR_NODE_DOLLAR_METHOD:
        mr_node_sidx_std(mr_node_dollar_method_t);
    case MR_NODE_EX_DOLLAR_METHOD:
        mr_node_sidx_std(mr_node_ex_dollar_method_t);
    case MR_NODE_IF:
        mr_node_sidx_std(mr_node_if_t);
    case MR_NODE_IF_ELSE:
        mr_node_sidx_std(mr_node_if_else_t);
    case MR_NODE_IF_ELIF:
        mr_node_sidx_std(mr_node_if_elif_t);
    case MR_NODE_SWITCH:
        mr_node_sidx_std(mr_node_switch_t);
    case MR_NODE_SWITCH_DEF:
        mr_node_sidx_std(mr_node_switch_def_t);
    case MR_NODE_IMPORT:
    case MR_NODE_INCLUDE:
        mr_node_sidx_std(mr_node_import_t);
    default:
        return MR_INVALID_IDX_CODE;
    }
}

mr_long_t mr_node_eidx(
    mr_node_t node)
{
    mr_long_t idx;

    switch (node.type)
    {
    case MR_NODE_NULL:
        return MR_INVALID_IDX_CODE;
    case MR_NODE_NONE:
        return node.value + mr_token_keyword_size[MR_TOKEN_NONE_K - MR_TOKEN_KEYWORD_PAD];
    case MR_NODE_INT:
    case MR_NODE_FLOAT:
    case MR_NODE_IMAGINARY:
    case MR_NODE_CHR:
    case MR_NODE_STR:
    case MR_NODE_VAR_ACCESS:
        return node.value + mr_token_getsize2(mr_node_get_token(node.type), node.value);
    case MR_NODE_BOOL:
    {
        mr_token_t token;

        token = *(mr_token_t*)&node.value;
        idx = MR_IDX_EXTRACT(token.idx);
        return idx + mr_token_keyword_size[token.type - MR_TOKEN_KEYWORD_PAD];
    }
    case MR_NODE_FSTR:
    case MR_NODE_LIST:
    case MR_NODE_DICT:
    case MR_NODE_SET:
    case MR_NODE_MULTILINE:
        mr_node_eidx_std(mr_node_list_t);
    case MR_NODE_TUPLE:
    case MR_NODE_MULTILINE_TUPLE:
    {
        mr_node_tuple_t *value;
        mr_node_t *nodes;

        value = (mr_node_tuple_t*)(_mr_stack.data + node.value);
        idx = MR_IDX_EXTRACT(value->size);
        nodes = (mr_node_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->elems)];
        return mr_node_eidx(nodes[idx - 1]);
    }
    case MR_NODE_TYPE:
    {
        mr_token_t token;

        token = *(mr_token_t*)&node.value;
        idx = MR_IDX_EXTRACT(token.idx);
        return idx + mr_token_type_size[token.type - MR_TOKEN_TYPE_PAD];
    }
    case MR_NODE_BINARY_OP:
        mr_node_eidx_elem(mr_node_binary_op_t, right);
    case MR_NODE_UNARY_OP:
        mr_node_eidx_elem(mr_node_unary_op_t, operand);
    case MR_NODE_TERNARY_OP:
        mr_node_eidx_elem(mr_node_ternary_op_t, right);
    case MR_NODE_SUBSCRIPT:
        mr_node_eidx_std(mr_node_subscript_t);
    case MR_NODE_SUBSCRIPT_END:
        mr_node_eidx_std(mr_node_subscript_end_t);
    case MR_NODE_SUBSCRIPT_STEP:
        mr_node_eidx_std(mr_node_subscript_step_t);
    case MR_NODE_VAR_ASSIGN:
        mr_node_eidx_elem(mr_node_var_assign_t, value);
    case MR_NODE_FUNC_CALL:
        mr_node_eidx_std(mr_node_func_call_t);
    case MR_NODE_EX_FUNC_CALL:
        mr_node_eidx_std(mr_node_ex_func_call_t);
    case MR_NODE_DOLLAR_METHOD:
    {
        mr_node_dollar_method_t *value;
        mr_node_t *params;

        value = (mr_node_dollar_method_t*)(_mr_stack.data + node.value);
        params = (mr_node_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->params)];
        return mr_node_eidx(params[value->size - 1]);
    }
    case MR_NODE_EX_DOLLAR_METHOD:
    {
        mr_node_ex_dollar_method_t *value;

        value = (mr_node_ex_dollar_method_t*)(_mr_stack.data + node.value);
        idx = MR_IDX_EXTRACT(value->name);
        return idx + mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
    }
    case MR_NODE_IF:
        mr_node_eidx_elem(mr_node_if_t, body);
    case MR_NODE_IF_ELSE:
        mr_node_eidx_elem(mr_node_if_else_t, ebody);
    case MR_NODE_IF_ELIF:
        mr_node_eidx_elem(mr_node_if_elif_t, ebody);
    case MR_NODE_SWITCH:
        mr_node_eidx_std(mr_node_switch_t);
    case MR_NODE_SWITCH_DEF:
        mr_node_eidx_std(mr_node_switch_def_t);
    case MR_NODE_IMPORT:
    case MR_NODE_INCLUDE:
    {
        mr_node_import_t *value;
        mr_idx_t *libs, last;

        value = (mr_node_import_t*)(_mr_stack.data + node.value);
        libs = (mr_idx_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->libs)];
        last = libs[value->size - 1];
        idx = MR_IDX_EXTRACT(last);
        return idx + mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
    }
    default:
        return MR_INVALID_IDX_CODE;
    }
}

#ifdef __MR_DEBUG__
#include <config.h>

/**
 * Labels for different node types.
*/
static mr_str_ct mr_node_labels[MR_NODE_COUNT] =
{
    "NODE_NULL", "NODE_NONE",
    "NODE_INT", "NODE_FLOAT", "NODE_IMAGINARY", "NODE_BOOL", "NODE_CHR", "NODE_FSTR_FRAG",
    "NODE_STR", "NODE_FSTR", "NODE_LIST", "NODE_TUPLE", "NODE_DICT", "NODE_SET", "NODE_TYPE",
    "NODE_BINARY_OP", "NODE_UNARY_OP", "NODE_TERNARY_OP",
    "NODE_SUBSCRIPT", "NODE_SUBSCRIPT_END", "NODE_SUBSCRIPT_STEP",
    "NODE_VAR_ACCESS", "NODE_VAR_ASSIGN",
    "NODE_FUNC_CALL", "NODE_EX_FUNC_CALL",
    "NODE_DOLLAR_METHOD", "NODE_EX_DOLLAR_METHOD",
    "NODE_MULTILINE",
    "NODE_IF", "NODE_IF_ELSE", "NODE_IF_ELIF",
    "NODE_SWITCH", "NODE_SWITCH_DEF",
    "NODE_IMPORT", "NODE_INCLUDE"
};

void mr_node_print(
    mr_node_t node)
{
    mr_long_t size, idx;

    if (node.type <= MR_NODE_NONE || node.type == MR_NODE_FSTR_FRAG)
    {
        printf("%s", mr_node_labels[node.type]);
        return;
    }

    printf("%s: ", mr_node_labels[node.type]);

    switch (node.type)
    {
    case MR_NODE_INT:
    case MR_NODE_FLOAT:
    case MR_NODE_IMAGINARY:
    case MR_NODE_CHR:
    case MR_NODE_STR:
        size = mr_token_getsize2(mr_node_get_token(node.type), node.value);
        fwrite(_mr_config.code + node.value, sizeof(mr_chr_t), size, stdout);
        break;
    case MR_NODE_BOOL:
    case MR_NODE_TYPE:
        fputs(mr_token_labels[((mr_token_t*)&node.value)->type], stdout);
        break;
    case MR_NODE_FSTR:
    case MR_NODE_LIST:
    case MR_NODE_SET:
    case MR_NODE_MULTILINE:
    {
        mr_long_t i;
        mr_node_list_t *value;
        mr_node_t *elems;

        value = (mr_node_list_t*)(_mr_stack.data + node.value);
        size = MR_IDX_EXTRACT(value->size);
        if (!size)
        {
            fputs("[]", stdout);
            break;
        }

        elems = (mr_node_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->elems)];

        fputs("[(", stdout);
        mr_node_print(*elems);

        for (i = 1; i != size; i++)
        {
            fputs("), (", stdout);
            mr_node_print(elems[i]);
        }

        fputs(")]", stdout);
        break;
    }
    case MR_NODE_TUPLE:
    case MR_NODE_MULTILINE_TUPLE:
    {
        mr_long_t i;
        mr_node_tuple_t *value;
        mr_node_t *elems;

        value = (mr_node_tuple_t*)(_mr_stack.data + node.value);
        size = MR_IDX_EXTRACT(value->size);
        elems = (mr_node_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->elems)];

        fputs("[(", stdout);
        mr_node_print(*elems);

        for (i = 1; i != size; i++)
        {
            fputs("), (", stdout);
            mr_node_print(elems[i]);
        }

        fputs(")]", stdout);
        break;
    }
    case MR_NODE_DICT:
    {
        mr_long_t i;
        mr_node_list_t *value;
        mr_node_keyval_t *elems;

        value = (mr_node_list_t*)(_mr_stack.data + node.value);
        size = MR_IDX_EXTRACT(value->size);
        if (!size)
        {
            fputs("[]", stdout);
            break;
        }

        elems = (mr_node_keyval_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->elems)];

        fputs("[{(", stdout);
        mr_node_print(elems->key);
        fputs("), (", stdout);
        mr_node_print(elems->value);

        for (i = 1; i != size; i++)
        {
            fputs(")}, {(", stdout);
            mr_node_print(elems[i].key);
            fputs("), (", stdout);
            mr_node_print(elems[i].value);
        }

        fputs(")}]", stdout);
        break;
    }
    case MR_NODE_BINARY_OP:
    {
        mr_node_binary_op_t *value;

        value = (mr_node_binary_op_t*)(_mr_stack.data + node.value);

        printf("%s, (", mr_token_labels[value->op]);
        mr_node_print(value->left);
        fputs("), (", stdout);
        mr_node_print(value->right);
        putchar(')');
        break;
    }
    case MR_NODE_UNARY_OP:
    {
        mr_node_unary_op_t *value;

        value = (mr_node_unary_op_t*)(_mr_stack.data + node.value);

        printf("%s, (", mr_token_labels[value->op]);
        mr_node_print(value->operand);
        putchar(')');
        break;
    }
    case MR_NODE_TERNARY_OP:
    {
        mr_node_ternary_op_t *value;

        value = (mr_node_ternary_op_t*)(_mr_stack.data + node.value);

        putchar('(');
        mr_node_print(value->cond);
        fputs("), (", stdout);
        mr_node_print(value->left);
        fputs("), (", stdout);
        mr_node_print(value->right);
        putchar(')');
        break;
    }
    case MR_NODE_SUBSCRIPT:
    {
        mr_node_subscript_t *value;

        value = (mr_node_subscript_t*)(_mr_stack.data + node.value);

        putchar('(');
        mr_node_print(value->node);
        fputs("), (", stdout);
        mr_node_print(value->idx);
        putchar(')');
        break;
    }
    case MR_NODE_SUBSCRIPT_END:
    {
        mr_node_subscript_end_t *value;

        value = (mr_node_subscript_end_t*)(_mr_stack.data + node.value);

        putchar('(');
        mr_node_print(value->node);
        fputs("), (", stdout);
        mr_node_print(value->start);
        fputs("), (", stdout);
        mr_node_print(value->end);
        putchar(')');
        break;
    }
    case MR_NODE_SUBSCRIPT_STEP:
    {
        mr_node_subscript_step_t *value;

        value = (mr_node_subscript_step_t*)(_mr_stack.data + node.value);

        putchar('(');
        mr_node_print(value->node);
        fputs("), (", stdout);
        mr_node_print(value->start);
        fputs("), (", stdout);
        mr_node_print(value->end);
        fputs("), (", stdout);
        mr_node_print(value->step);
        putchar(')');
        break;
    }
    case MR_NODE_VAR_ACCESS:
        size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, node.value);
        printf("\"%.*s\"", size, _mr_config.code + node.value);
        break;
    case MR_NODE_VAR_ASSIGN:
    {
        mr_node_var_assign_t *value;

        value = (mr_node_var_assign_t*)(_mr_stack.data + node.value);
        idx = MR_IDX_EXTRACT(value->name);
        size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);

        printf("\"%.*s\"", size, _mr_config.code + idx);
        switch (value->access)
        {
        case 0:
            break;
        case 1:
            fputs(", private", stdout);
            break;
        case 2:
            fputs(", public", stdout);
            break;
        case 3:
            fputs(", protected", stdout);
            break;
        }

        if (value->is_global)
            fputs(", global", stdout);
        if (value->is_readonly)
            fputs(", readonly", stdout);
        if (value->is_const)
            fputs(", const", stdout);
        if (value->is_static)
            fputs(", static", stdout);
        if (value->is_link)
            fputs(", link", stdout);
        if (value->type != MR_TOKEN_EOF)
            printf(", %s", mr_token_labels[value->type]);

        fputs(", (", stdout);
        mr_node_print(value->value);
        putchar(')');
        break;
    }
    case MR_NODE_FUNC_CALL:
    {
        mr_byte_t i;
        mr_node_call_arg_t *args;
        mr_node_func_call_t *value;

        value = (mr_node_func_call_t*)(_mr_stack.data + node.value);
        args = (mr_node_call_arg_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->args)];


        putchar('(');
        mr_node_print(value->func);

        idx = MR_IDX_EXTRACT(args->name);
        if (idx != MR_INVALID_IDX_CODE)
        {
            size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
            printf("), [{\"%.*s\": (", size, _mr_config.code + idx);
        }
        else
            fputs("), [{(", stdout);

        mr_node_print(args->value);

        for (i = 1; i != value->size; i++)
        {
            idx = MR_IDX_EXTRACT(args[i].name);
            if (idx != MR_INVALID_IDX_CODE)
            {
                size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
                printf(")}, {\"%.*s\": (", size, _mr_config.code + idx);
            }
            else
                fputs(")}, {(", stdout);

            mr_node_print(args[i].value);
        }

        fputs(")}]", stdout);
        break;
    }
    case MR_NODE_EX_FUNC_CALL:
    {
        mr_node_ex_func_call_t *value;

        value = (mr_node_ex_func_call_t*)(_mr_stack.data + node.value);

        putchar('(');
        mr_node_print(value->func);
        putchar(')');
        break;
    }
    case MR_NODE_DOLLAR_METHOD:
    {
        mr_byte_t i;
        mr_node_t *params;
        mr_node_dollar_method_t *value;

        value = (mr_node_dollar_method_t*)(_mr_stack.data + node.value);
        idx = MR_IDX_EXTRACT(value->name);
        size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
        params = (mr_node_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->params)];

        printf("\"%.*s\", [(", size, _mr_config.code + idx);
        mr_node_print(*params);

        for (i = 1; i != value->size; i++)
        {
            fputs("), (", stdout);
            mr_node_print(params[i]);
        }

        fputs(")]", stdout);
        break;
    }
    case MR_NODE_EX_DOLLAR_METHOD:
    {
        mr_node_ex_dollar_method_t *value;

        value = (mr_node_ex_dollar_method_t*)(_mr_stack.data + node.value);
        idx = MR_IDX_EXTRACT(value->name);
        size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);

        printf("\"%.*s\"", size, _mr_config.code + idx);
        break;
    }
    case MR_NODE_IF:
    {
        mr_node_if_t *value;

        value = (mr_node_if_t*)(_mr_stack.data + node.value);

        putchar('(');
        mr_node_print(value->cond);
        fputs("), (", stdout);
        mr_node_print(value->body);
        putchar(')');
        break;
    }
    case MR_NODE_IF_ELSE:
    {
        mr_node_if_else_t *value;

        value = (mr_node_if_else_t*)(_mr_stack.data + node.value);

        fputs("{(", stdout);
        mr_node_print(value->cond);
        fputs("), (", stdout);
        mr_node_print(value->body);
        fputs(")}, (", stdout);
        mr_node_print(value->ebody);
        putchar(')');
        break;
    }
    case MR_NODE_IF_ELIF:
    {
        mr_long_t i;
        mr_node_if_elif_t *value;
        mr_node_keyval_t *cases;

        value = (mr_node_if_elif_t*)(_mr_stack.data + node.value);
        size = MR_IDX_EXTRACT(value->size);
        cases = (mr_node_keyval_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->cases)];

        fputs("[{(", stdout);
        mr_node_print(cases->key);
        fputs("), (", stdout);
        mr_node_print(cases->value);
        fputs(")}", stdout);

        for (i = 1; i != size; i++)
        {
            fputs(", {(", stdout);
            mr_node_print(cases[i].key);
            fputs("), (", stdout);
            mr_node_print(cases[i].value);
            fputs(")}", stdout);
        }

        fputs("], (", stdout);
        mr_node_print(value->ebody);
        putchar(')');
        break;
    }
    case MR_NODE_SWITCH:
    {
        mr_long_t i;
        mr_node_switch_t *value;
        mr_node_keyval_t *cases;

        value = (mr_node_switch_t*)(_mr_stack.data + node.value);
        size = MR_IDX_EXTRACT(value->size);

        putchar('(');
        mr_node_print(value->value);
        putchar(')');

        if (!size)
            break;

        cases = (mr_node_keyval_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->cases)];

        fputs("), [{(", stdout);
        mr_node_print(cases->key);
        fputs("), (", stdout);
        mr_node_print(cases->value);
        fputs(")}", stdout);

        for (i = 1; i != size; i++)
        {
            fputs(", {(", stdout);
            mr_node_print(cases[i].key);
            fputs("), (", stdout);
            mr_node_print(cases[i].value);
            fputs(")}", stdout);
        }

        putchar(']');
        break;
    }
    case MR_NODE_SWITCH_DEF:
    {
        mr_long_t i;
        mr_node_switch_def_t *value;
        mr_node_keyval_t *cases;

        value = (mr_node_switch_def_t*)(_mr_stack.data + node.value);
        size = MR_IDX_EXTRACT(value->size);

        putchar('(');
        mr_node_print(value->value);
        fputs("), ", stdout);

        if (!size)
        {
            putchar('(');
            mr_node_print(value->dbody);
            putchar(')');
            break;
        }

        cases = (mr_node_keyval_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->cases)];

        fputs("[{(", stdout);
        mr_node_print(cases->key);
        fputs("), (", stdout);
        mr_node_print(cases->value);
        fputs(")}", stdout);

        for (i = 1; i != size; i++)
        {
            fputs(", {(", stdout);
            mr_node_print(cases[i].key);
            fputs("), (", stdout);
            mr_node_print(cases[i].value);
            fputs(")}", stdout);
        }

        fputs("], (", stdout);
        mr_node_print(value->dbody);
        putchar(')');
        break;
    }
    case MR_NODE_IMPORT:
    case MR_NODE_INCLUDE:
    {
        mr_byte_t i;
        mr_idx_t *libs;
        mr_node_import_t *value;

        value = (mr_node_import_t*)(_mr_stack.data + node.value);
        libs = (mr_idx_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->libs)];

        idx = MR_IDX_EXTRACT(*libs);
        size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
        printf("[\"%.*s\"", size, _mr_config.code + idx);

        for (i = 1; i != value->size; i++)
        {
            idx = MR_IDX_EXTRACT(libs[i]);
            size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
            printf(", \"%.*s\"", size, _mr_config.code + idx);
        }

        putchar(']');
        break;
    }
    }
}

void mr_node_prints(
    mr_node_t *nodes, mr_long_t size)
{
    mr_long_t i;

    if (!size)
        return;

    mr_node_print(*nodes);
    for (i = 1; i != size; i++)
    {
        putchar('\n');
        mr_node_print(nodes[i]);
    }
}

#endif

mr_byte_t mr_node_get_token(
    mr_byte_t type)
{
    switch (type)
    {
    case MR_NODE_INT:
        return MR_TOKEN_INT;
    case MR_NODE_FLOAT:
        return MR_TOKEN_FLOAT;
    case MR_NODE_IMAGINARY:
        return MR_TOKEN_IMAGINARY;
    case MR_NODE_CHR:
        return MR_TOKEN_CHR;
    case MR_NODE_STR:
        return MR_TOKEN_STR;
    default:
        return MR_TOKEN_EOF;
    }
}
