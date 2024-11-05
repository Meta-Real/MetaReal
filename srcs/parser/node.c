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

#ifdef __MR_DEBUG__
#include <lexer/token.h>
#include <stack.h>
#include <config.h>

/**
 * Number of valid nodes.
*/
#define MR_NODE_COUNT (MR_NODE_EX_DOLLAR_METHOD + 1)

/**
 * Returns the corrsponding token type given its node type.
 * @param type
 * Type of the specified node.
 * @return It returns the corrsponding token type. \n
 * The function returns \a MR_TOKEN_EOF in case of an error.
*/
mr_byte_t mr_node_get_token(
    mr_byte_t type);

/**
 * Labels for different node types.
*/
static mr_str_ct mr_node_labels[MR_NODE_COUNT] =
{
    "NODE_NONE",
    "NODE_INT", "NODE_FLOAT", "NODE_IMAGINARY", "NODE_BOOL", "NODE_CHR", "NODE_FSTR_FRAG",
    "NODE_STR", "NODE_FSTR", "NODE_LIST", "NODE_TUPLE", "NODE_DICT", "NODE_SET",
    "NODE_BINARY_OP", "NODE_UNARY_OP",
    "NODE_VAR_ACCESS", "NODE_VAR_ASSIGN", "NODE_VAR_REASSIGN",
    "NODE_FUNC_CALL", "NODE_EX_FUNC_CALL", "NODE_DOLLAR_METHOD", "NODE_EX_DOLLAR_METHOD"
};

void mr_node_print(
    mr_node_t node)
{
    mr_long_t size, idx;

    if (node.type == MR_NODE_NONE || node.type == MR_NODE_FSTR_FRAG)
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
        fputs(node.value ? "true" : "false", stdout);
        break;
    case MR_NODE_FSTR:
    case MR_NODE_LIST:
    case MR_NODE_TUPLE:
    case MR_NODE_SET:
    {
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

        for (mr_long_t i = 1; i != size; i++)
        {
            fputs("), (", stdout);
            mr_node_print(elems[i]);
        }

        fputs(")]", stdout);
        break;
    }
    case MR_NODE_DICT:
    {
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

        for (mr_long_t i = 1; i != size; i++)
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
    case MR_NODE_VAR_REASSIGN:
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

        printf("%.*s, public=%hhu, global=%hhu, const=%hhu, static=%hhu, link=%hhu, %s, (",
            size, _mr_config.code + idx, value->is_public, value->is_global,
            value->is_const, value->is_static, value->is_link, mr_token_labels[value->type]);
        mr_node_print(value->value);
        putchar(')');
    }
    case MR_NODE_FUNC_CALL:
    {
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
            printf("), [{%.*s: (", size, _mr_config.code + idx);
        }
        else
            fputs("), [{(", stdout);

        mr_node_print(args->value);

        for (mr_byte_t i = 1; i != value->size; i++)
        {
            idx = MR_IDX_EXTRACT(args[i].name);
            if (idx != MR_INVALID_IDX_CODE)
            {
                size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
                printf(")}, {%.*s: (", size, _mr_config.code + idx);
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
        mr_node_t *params;
        mr_node_dollar_method_t *value;

        value = (mr_node_dollar_method_t*)(_mr_stack.data + node.value);
        idx = MR_IDX_EXTRACT(value->name);
        size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
        params = (mr_node_t*)_mr_stack.ptrs[MR_IDX_EXTRACT(value->params)];

        printf("\"%.*s\", [(", size, _mr_config.code + idx);
        mr_node_print(*params);

        for (mr_byte_t i = 1; i != value->size; i++)
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
    }
}

void mr_node_prints(
    mr_node_t *nodes, mr_long_t size)
{
    if (!size)
        return;

    mr_node_print(*nodes);
    for (mr_long_t i = 1; i != size; i++)
    {
        putchar('\n');
        mr_node_print(nodes[i]);
    }
}


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

#endif
