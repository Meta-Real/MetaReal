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
 * Labels for different node types.
*/
static mr_str_ct mr_node_labels[MR_NODE_COUNT] =
{
    "NODE_NONE",
    "NODE_INT", "NODE_FLOAT", "NODE_IMAGINARY",
    "NODE_BINARY_OP", "NODE_UNARY_OP",
    "NODE_VAR_ACCESS",
    "NODE_FUNC_CALL", "NODE_EX_FUNC_CALL", "NODE_DOLLAR_METHOD", "NODE_EX_DOLLAR_METHOD"
};

void mr_node_print(
    mr_node_t node)
{
    mr_long_t size, idx;

    if (node.type == MR_NODE_NONE)
    {
        printf("%s", mr_node_labels[node.type]);
        return;
    }

    printf("%s: ", mr_node_labels[node.type]);

    switch (node.type)
    {
    case MR_NODE_INT:
        size = mr_token_getsize2(MR_TOKEN_INT, node.value);
        fwrite(_mr_config.code + node.value, sizeof(mr_chr_t), size, stdout);
        break;
    case MR_NODE_FLOAT:
        size = mr_token_getsize2(MR_TOKEN_FLOAT, node.value);
        fwrite(_mr_config.code + node.value, sizeof(mr_chr_t), size, stdout);
        break;
    case MR_NODE_IMAGINARY:
        size = mr_token_getsize2(MR_TOKEN_IMAGINARY, node.value);
        fwrite(_mr_config.code + node.value, sizeof(mr_chr_t), size, stdout);
        break;
    case MR_NODE_BINARY_OP:
    {
        mr_node_binary_op_t value;

        value = *(mr_node_binary_op_t*)(_mr_stack.data + node.value);

        printf("%s, (", mr_token_labels[value.op]);
        mr_node_print(value.left);
        fputs("), (", stdout);
        mr_node_print(value.right);
        putchar(')');
        break;
    }
    case MR_NODE_UNARY_OP:
    {
        mr_node_unary_op_t value;

        value = *(mr_node_unary_op_t*)(_mr_stack.data + node.value);

        printf("%s, (", mr_token_labels[value.op]);
        mr_node_print(value.operand);
        putchar(')');
        break;
    }
    case MR_NODE_VAR_ACCESS:
        size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, node.value);
        printf("\"%.*s\"", size, _mr_config.code + node.value);
        break;
    case MR_NODE_FUNC_CALL:
    {
        mr_node_call_arg_t *args;
        mr_node_func_call_t *value;

        value = (mr_node_func_call_t*)(_mr_stack.data + node.value);
        args = (mr_node_call_arg_t*)(_mr_stack.ptrs[MR_IDX_EXTRACT(value->args)]);


        putchar('(');
        mr_node_print(value->func);

        idx = MR_IDX_EXTRACT(args->idx);
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
            idx = MR_IDX_EXTRACT(args[i].idx);
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
        mr_node_ex_func_call_t value;

        value = *(mr_node_ex_func_call_t*)(_mr_stack.data + node.value);

        putchar('(');
        mr_node_print(value.func);
        putchar(')');
        break;
    }
    case MR_NODE_DOLLAR_METHOD:
    {
        mr_node_t *params;
        mr_node_dollar_method_t value;

        value = *(mr_node_dollar_method_t*)(_mr_stack.data + node.value);
        idx = MR_IDX_EXTRACT(value.idx);
        size = mr_token_getsize2(MR_TOKEN_IDENTIFIER, idx);
        params = (mr_node_t*)(_mr_stack.ptrs[MR_IDX_EXTRACT(value.params)]);

        printf("\"%.*s\", [(", size, _mr_config.code + idx);
        mr_node_print(*params);

        for (mr_byte_t i = 1; i != value.size; i++)
        {
            fputs("), (", stdout);
            mr_node_print(params[i]);
        }

        fputs(")]", stdout);
        break;
    }
    case MR_NODE_EX_DOLLAR_METHOD:
    {
        mr_node_ex_dollar_method_t value;

        value = *(mr_node_ex_dollar_method_t*)(_mr_stack.data + node.value);
        idx = MR_IDX_EXTRACT(value.idx);
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

#endif
