/*
    MetaReal Compiler version 1.0.0
*/

#include <lexer/lexer.h>
#include <parser/parser.h>
#include <optimizer/optimizer.h>
#include <generator/generator.h>
#include <stdio.h>
#include <alloc.h>
#include <string.h>
#include <consts.h>

int main()
{
    puts("MetaReal Compiler version " MR_VERSION "\n");

    char *code = mr_alloc(DEF_CODE_SIZE);
    char *ptr;
    FILE *file;

    lex_res_t lex_res;
    parse_res_t parse_res;
    opt_res_t opt_res;
    gen_res_t gen_res;
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

        parse_res = parse(lex_res.tokens);
        if (!parse_res.nodes)
        {
            print_invalid_syntax(&parse_res.error, DEF_FILE_NAME, code);
            continue;
        }

        if (!parse_res.size)
            continue;

        opt_res = optimize(parse_res.nodes, parse_res.size);
        gen_res = generate(opt_res.values, opt_res.size);

        //file = fopen("test.s", "w");
        fputs(gen_res.consts, stdout);
        fputs(gen_res.main, stdout);
        //fclose(file);

        mr_free(gen_res.main);
        mr_free(gen_res.consts);
    }

    mr_free(code);
    return 0;
}
