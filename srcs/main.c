/**
 * @file main.c
 * Main file of the compiler that aggregates all compiler parts together. \n
 * Command line prompts are also defined here.
*/

#include <lexer/lexer.h>
#include <parser/parser.h>
#include <optimizer/optimizer.h>
#include <optimizer/value.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <consts.h>

/**
 * It compiles the \a code according to MetaReal compile rules. \n
 * Order of compilation:
 * <pre>
 *     [code] -> lexer -> parser -> optimizer -> generator -> assembler -> linker -> [executable]
 * </pre>
 * Also, debugger will debug the \a code during compilation process (if enabled). \n
 * Dollar methods are handled with a different mechanism in the optimization step.
 * @param fname
 * Name of the source file (for displaying errors).
 * @param code
 * The source code.
 * @param size
 * Size of the source code in characters.
 * @return It returns a code which indicates if process was successful or not. \n
 * If process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_compile(mr_str_ct fname, mr_str_ct code, mr_long_t size);

/**
 * It prints out the help information (called with a \--help flag).
*/
void mr_print_help(void);

int main(int argc, mr_str_ct argv[])
{
    if (argc == 1)
    {
        fputs(
            "Internal error: invalid command, nothing to process.\n"
            "Write \"MetaReal --help\" for more information.\n",
            stderr);
        return MR_ERROR_BAD_COMMAND;
    }

    if (argc == 2)
    {
        if (!strcmp(argv[1], "--help"))
        {
            mr_print_help();
            return MR_NOERROR;
        }
        if (!strcmp(argv[1], "--version"))
        {
            fputs(
                "MetaReal compiler version " MR_VERSION "\n"
                "MetaReal core verified as " MR_CORE ", version " MR_CORE_VERSION "\n"
                "MetaReal port verified as " MR_PORT ", version " MR_PORT_VERSION "\n",
                stdout);
            return MR_NOERROR;
        }
        if (!strcmp(argv[1], "--dumpver"))
        {
            fputs(
                MR_VERSION " "
                MR_CORE " " MR_CORE_VERSION " "
                MR_PORT " " MR_PORT_VERSION "\n",
                stdout);
            return MR_NOERROR;
        }

#if defined(__GNUC__) || defined(__clang__)
        FILE *file = fopen(argv[1], "rb");
        if (!file)
#elif defined(_MSC_VER)
        FILE *file;
        if (fopen_s(&file, argv[1], "rb"))
#endif
        {
            fprintf(stderr,
                "Internal error: can not find the file \"%s\"\n",
                argv[1]);
            return MR_ERROR_FILE_NOT_FOUND;
        }

        fseek(file, 0, SEEK_END);
        mr_long_t size = ftell(file);
        rewind(file);

        if (!size)
        {
            fclose(file);
            return MR_NOERROR;
        }

        mr_str_t code = malloc((size + 1) * sizeof(mr_chr_t));
        if (!code)
        {
            fclose(file);
            fputs("Internal error: not enough memory.\n", stderr);
            return MR_ERROR_NOT_ENOUGH_MEMORY;
        }

        fread(code, sizeof(mr_chr_t), size, file);
        fclose(file);
        code[size] = '\0';

        mr_byte_t retcode = mr_compile(argv[1], code, size);
        free(code);

        if (retcode == MR_ERROR_NOT_ENOUGH_MEMORY)
            fputs("Internal error: not enough memory.\n", stderr);
        printf("!!\n"); // dummy
        return retcode;
    }

    fputs(
        "Internal error: invalid command.\n"
        "Write \"MetaReal --help\" for more information.\n",
        stderr);
    return MR_ERROR_BAD_COMMAND;
}

mr_byte_t mr_compile(mr_str_ct fname, mr_str_ct code, mr_long_t size)
{
    mr_lexer_t lexer;
    mr_byte_t retcode = mr_lexer(&lexer, code, size / MR_LEXER_TOKENS_CHUNK + 1);
    if (retcode != MR_NOERROR)
    {
        if (retcode == MR_ERROR_BAD_FORMAT)
            mr_illegal_chr_print(&lexer.error, fname, code, size);
        return retcode;
    }

    if (lexer.tokens->type == MR_TOKEN_EOF)
    {
        free(lexer.tokens);
        return MR_NOERROR;
    }

    mr_parser_t parser;
    retcode = mr_parser(&parser, lexer.tokens, size / MR_PARSER_NODES_CHUNK + 1);
    if (retcode != MR_NOERROR)
    {
        if (retcode == MR_ERROR_BAD_FORMAT)
            mr_invalid_syntax_print(&parser.error, fname, code, size);
        return retcode;
    }

    mr_optimizer_t optimizer;
    retcode = mr_optimizer(&optimizer, parser.nodes, parser.size);
    if (retcode != MR_NOERROR)
    {
        if (retcode == MR_ERROR_BAD_FORMAT)
            mr_invalid_semantic_print(&optimizer.error, fname, code, size);
        return retcode;
    }

    mr_values_print(optimizer.values, optimizer.size);
    mr_values_free(optimizer.values, optimizer.size);
    return MR_NOERROR;
}

void mr_print_help(void)
{
    fputs(
        "MetaReal [options] [output] [files]\nOptions:\n"
        "  --help\t\tDisplays the help information.\n"
        "  --version\t\tDisplays the version information.\n"
        "  --dumpver\t\tDisplays the version data.\n",
        stdout);
}
