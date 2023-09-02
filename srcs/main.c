/*
    MetaReal Compiler version 1.0.0
*/

#include <lexer/lexer.h>
#include <stdio.h>
#include <alloc.h>
#include <string.h>
#include <consts.h>

int main()
{
    puts("MetaReal Compiler version " MR_VERSION);

    char *code = mr_alloc(DEF_CODE_SIZE);
    char *ptr;

    lex_res_t lex_res;

    while (1)
    {
        fputs(">>> ", stdout);
        fgets(code, DEF_CODE_SIZE, stdin);

        ptr = strchr(code, '\n');
        if (ptr)
            *ptr = '\0';

        lex_res = lex(code);
        if (!lex_res.tokens)
        {
            print_illegal_char(&lex_res.error, DEF_FILE_NAME, code);
            continue;
        }

        print_tokens(lex_res.tokens);
        free_tokens(lex_res.tokens);
    }

    mr_free(code);
    return 0;
}
