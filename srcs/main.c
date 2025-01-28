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
 * @file main.c
 * Main file of the compiler that aggregates all compiler parts together. \n
 * Command line prompts are also defined here.
*/

#include <lexer/lexer.h>
#include <parser/parser.h>
#include <stack.h>
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <consts.h>

/**
 * Content of the \--help command.
*/
#define MR_HELP_CONTENT "MetaReal [output] [files] [options]\nOptions:\n" \
    "  --help\t\tDisplays the help information.\n"                        \
    "  --version\t\tDisplays the version information.\n"                  \
    "  --dumpver\t\tDisplays the version data.\n"

/**
 * It compiles the \a code according to MetaReal compile rules. \n
 * Order of compilation:
 * <pre>
 *     [code] -> lexer -> parser -> optimizer -> generator -> assembler -> linker -> [executable]
 * </pre>
 * Also, debugger will debug the \a code during compilation process (if enabled). \n
 * Dollar methods are handled with a different mechanism in the optimizer and parser steps.
 * @return It returns a code which indicates if process was successful or not. \n
 * If process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_compile(void);

/**
 * It handles arguments of the application. \n
 * Supported arguments:
 * <pre>
 *     -O[d0123u]
 * </pre>
 * @param argv
 * The list of arguments.
 * @param size
 * Number of arguments to process.
*/
void mr_handle_args(
    mr_str_ct argv[], mr_byte_t size);

int main(
    int argc, mr_str_ct argv[])
{
    mr_long_t size;
    mr_byte_t retcode;
    mr_str_t code;
    FILE *file;

    if (argc == 1)
    {
        fputs("Internal Error: Invalid command, nothing to process.\n"
            "Write \"MetaReal --help\" for more information.\n", stderr);
        return MR_ERROR_BAD_COMMAND;
    }

    if (!strcmp(argv[1], "--help"))
    {
        fputs(MR_HELP_CONTENT, stdout);
        return MR_NOERROR;
    }
    if (!strcmp(argv[1], "--version"))
    {
        fputs("MetaReal compiler version " MR_VERSION "\n"
            "MetaReal core verified as " MR_CORE ", version " MR_CORE_VERSION "\n"
            "MetaReal port verified as " MR_PORT ", version " MR_PORT_VERSION "\n", stdout);
        return MR_NOERROR;
    }
    if (!strcmp(argv[1], "--dumpver"))
    {
        fputs(MR_VERSION " " MR_CORE " " MR_CORE_VERSION " " MR_PORT " " MR_PORT_VERSION "\n", stdout);
        return MR_NOERROR;
    }

    if (argc > 2)
        mr_handle_args(argv + 2, (mr_byte_t)argc - 2);

#if defined(__GNUC__) || defined(__clang__)
    file = fopen(argv[1], "rb");
    if (!file)
#elif defined(_MSC_VER)
    if (fopen_s(&file, argv[1], "rb"))
#endif
    {
        fprintf(stderr, "Internal Error: Can not find the file \"%s\"\n", argv[1]);
        return MR_ERROR_FILE_NOT_FOUND;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    if (size > MR_FILE_MAXSIZE)
    {
        fprintf(stderr, "Internal error: File size exceeds the limit (%" PRIu32 ")", MR_FILE_MAXSIZE);

        fclose(file);
        return MR_ERROR_FILE_TOO_LARGE;
    }

    if (!size)
    {
        fclose(file);
        return MR_NOERROR;
    }

    code = malloc((size + 1) * sizeof(mr_chr_t));
    if (!code)
    {
        fclose(file);
        fputs("Internal error: not enough memory\n", stderr);
        return MR_ERROR_NOT_ENOUGH_MEMORY;
    }

    fread(code, sizeof(mr_chr_t), size, file);
    fclose(file);
    code[size] = '\0';

    _mr_config = (mr_config_t){.outstream=stdout, .instream=stdin, .errstream=stderr,
        .code=code, .fname=argv[1], .size=size};

    retcode = mr_compile();
    free(code);

    if (retcode == MR_ERROR_NOT_ENOUGH_MEMORY)
        fputs("Internal Error: Not enough memory\n", stderr);

    printf("END\n"); // dummy
    return retcode;
}

mr_byte_t mr_compile(void)
{
    mr_byte_t retcode;
    mr_lexer_t lexer;
    mr_parser_t parser;

    retcode = mr_lexer(&lexer);
    if (retcode != MR_NOERROR)
    {
        if (retcode == MR_ERROR_BAD_FORMAT)
            mr_illegal_chr_print(lexer.error);
        return retcode;
    }

    if (lexer.tokens->type == MR_TOKEN_EOF)
    {
        free(lexer.tokens);
        return MR_NOERROR;
    }

    retcode = mr_stack_init(_mr_config.size * MR_STACK_SIZE_FACTOR, _mr_config.size / MR_STACK_PSIZE_CHUNK + 1);
    if (retcode != MR_NOERROR)
    {
        free(lexer.tokens);
        return retcode;
    }

    mr_token_prints(lexer.tokens);
    putchar('\n');

    retcode = mr_parser(&parser, lexer.tokens);
    if (retcode != MR_NOERROR)
    {
        if (retcode == MR_ERROR_BAD_FORMAT)
            mr_invalid_syntax_print(&parser.error);

        free(lexer.tokens);
        mr_stack_free();
        return retcode;
    }

    free(lexer.tokens);
    mr_node_prints(parser.nodes, parser.size);

    mr_stack_free();
    return MR_NOERROR;
}

void mr_handle_args(
    mr_str_ct argv[], mr_byte_t size)
{
    mr_str_ct str;

    for (; size; size--)
    {
        str = *argv++;
        if (!strcmp(str, "-Od"))
            mr_config_opt(OPT_LEVELD);
        else if (!strcmp(str, "-O0"))
            mr_config_opt(OPT_LEVEL0);
        else if (!strcmp(str, "-O1"))
            mr_config_opt(OPT_LEVEL1);
        else if (!strcmp(str, "-O2"))
            mr_config_opt(OPT_LEVEL2);
        else if (!strcmp(str, "-O3"))
            mr_config_opt(OPT_LEVEL3);
        else if (!strcmp(str, "-Ou"))
            mr_config_opt(OPT_LEVELU);
    }
}
