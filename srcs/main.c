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
#include <stdlib.h>
#include <consts.h>

#include <time.h>

int main(int argc, char const **argv)
{
    puts("MetaReal Compiler version " MR_VERSION "\n");

    lex_res_t lex_res;
    parse_res_t parse_res;
    opt_res_t opt_res;
    gen_res_t gen_res;

    char *code;
    FILE *file;

    if (argc == 2)
    {
        file = fopen(argv[1], "rb");
        if (!file)
        {
            fprintf(stderr, "Internal Error: \"%s\" file not found\n", argv[1]);
            abort();
        }

        fseek(file, 0, SEEK_END);
        uint64_t size = ftell(file);
        rewind(file);

        if (!size)
        {
            fclose(file);
            return 0;
        }

        code = mr_alloc(size + 1);
        fread(code, size, 1, file);
        code[size] = '\0';

        fclose(file);

        lex_res = lex(code);
        if (!lex_res.tokens)
        {
            illegal_char_print(&lex_res.error, DEF_FILE_NAME, code);

            mr_free(code);
            return 0;
        }

        parse_res = parse(lex_res.tokens);
        if (!parse_res.nodes)
        {
            invalid_syntax_print(&parse_res.error, DEF_FILE_NAME, code);

            mr_free(code);
            return 0;
        }

        if (!parse_res.size)
        {
            mr_free(code);
            return 0;
        }

        clock_t s = clock();

        opt_res = optimize(parse_res.nodes, parse_res.size);
        if (!opt_res.values)
        {
            invalid_semantic_print(&opt_res.error, DEF_FILE_NAME, code);

            mr_free(code);
            return 0;
        }

        printf("%ld milliseconds\n\n", clock() - s);

        if (!opt_res.size)
        {
            mr_free(code);
            return 0;
        }

        gen_res = generate(opt_res.values, opt_res.size);

        //file = fopen("test.s", "w");
        //fputs(gen_res.consts, file);
        //fputs(gen_res.main, file);
        //fclose(file);
        fputs(gen_res.consts, stdout);
        fputs(gen_res.main, stdout);

        mr_free(gen_res.main);
        mr_free(gen_res.consts);
        mr_free(code);
        return 0;
    }

    code = mr_alloc(DEF_CODE_SIZE);
    char *ptr;
    while (1)
    {
        fputs(">>> ", stdout);
        fgets(code, DEF_CODE_SIZE, stdin);

        ptr = strchr(code, '\n');
        if (ptr)
            *ptr = '\0';

        if (!*code)
            break;

        lex_res = lex(code);
        if (!lex_res.tokens)
        {
            illegal_char_print(&lex_res.error, DEF_FILE_NAME, code);
            continue;
        }

        parse_res = parse(lex_res.tokens);
        if (!parse_res.nodes)
        {
            invalid_syntax_print(&parse_res.error, DEF_FILE_NAME, code);
            continue;
        }

        if (!parse_res.size)
            continue;

        clock_t s = clock();

        opt_res = optimize(parse_res.nodes, parse_res.size);
        if (!opt_res.values)
        {
            invalid_semantic_print(&opt_res.error, DEF_FILE_NAME, code);
            continue;
        }

        printf("%ld milliseconds\n\n", clock() - s);

        if (!opt_res.size)
            continue;

        gen_res = generate(opt_res.values, opt_res.size);

        //file = fopen("test.s", "w");
        //fputs(gen_res.consts, file);
        //fputs(gen_res.main, file);
        //fclose(file);
        fputs(gen_res.consts, stdout);
        fputs(gen_res.main, stdout);

        mr_free(gen_res.main);
        mr_free(gen_res.consts);
    }

    mr_free(code);
    return 0;
}
