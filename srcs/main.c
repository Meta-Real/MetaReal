/**
 * @file main.c
 * Main file of the compiler that aggregates all the compiler parts together. \n
 * Command line prompts are also defined here.
*/

#include <lexer/lexer.h>
#include <alloc.h>
#include <string.h>
#include <consts.h>
#include <error.h>

#include <time.h>

/**
 * It compiles the \a code according to the MetaReal compile rules. \n
 * Order of compilation:
 * <pre>
 *     [code] -> lexer -> parser -> optimizer -> generator -> assembler -> linker -> [executable]
 * </pre>
 * Also, the debugger will debug the \a code during the compilation process (if enabled). \n
 * Dollar methods are handled with a different mechanism in the optimization step.
 * @param fname
 * Name of the source file (for displaying errors).
 * @param code
 * The source code.
 * @param size
 * Size of the source code in characters.
 * @return It returns the code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_compile(mr_str_ct fname, mr_str_ct code, mr_size_t size);

/**
 * It prints out the help information (called with the \--help flag).
*/
void mr_print_help();

int main(int argc, mr_str_ct argv[])
{
    if (argc == 1)
    {
        fputs(
            "Internal error: invalid command, nothing to process.\n"
            "Write \"MetaReal --help\" for more information.\n",
            stderr);
        return ERROR_BAD_COMMAND;
    }

    if (argc == 2)
    {
        if (!strcmp(argv[1], "--help"))
        {
            mr_print_help();
            return NO_ERROR;
        }
        if (!strcmp(argv[1], "--version"))
        {
            fputs(
                "MetaReal compiler version " MR_VERSION "\n"
                "MetaReal core verified as " MR_CORE ", version " MR_CORE_VERSION "\n"
                "MetaReal port verified as " MR_PORT ", version " MR_PORT_VERSION "\n",
                stdout);
            return NO_ERROR;
        }
        if (!strcmp(argv[1], "--dumpver"))
        {
            fputs(
                MR_VERSION " "
                MR_CORE " " MR_CORE_VERSION " "
                MR_PORT " " MR_PORT_VERSION "\n",
                stdout);
            return NO_ERROR;
        }

        FILE *file = fopen(argv[1], "rb");
        if (!file)
        {
            fprintf(stderr,
                "Internal error: can not find the file \"%s\"\n",
                argv[1]);
            return ERROR_FILE_NOT_FOUND;
        }

        fseek(file, 0, SEEK_END);
        mr_size_t size = ftell(file);
        rewind(file);

        if (!size)
        {
            fclose(file);
            return NO_ERROR;
        }

        mr_str_t code = mr_alloc((size + 1) * sizeof(mr_chr_t));
        if (!code)
        {
            fclose(file);
            fputs("Internal error: not enough memory.\n", stderr);
            return ERROR_NOT_ENOUGH_MEMORY;
        }

        fread(code, sizeof(mr_chr_t), size, file);
        fclose(file);
        code[size] = '\0';

        mr_byte_t retcode = mr_compile(argv[1], code, size);
        mr_free(code);

        if (retcode == ERROR_NOT_ENOUGH_MEMORY)
            fputs("Internal error: not enough memory.\n", stderr);
        printf("!!\n"); // dummy
        return retcode;
    }

    fputs(
        "Internal error: invalid command.\n"
        "Write \"MetaReal --help\" for more information.\n",
        stderr);
    return ERROR_BAD_COMMAND;
}

mr_byte_t mr_compile(mr_str_ct fname, mr_str_ct code, mr_size_t size)
{
    mr_lexer_t lexer;
    mr_byte_t retcode;

    retcode = mr_lexer(&lexer, code, size / MR_LEXER_TOKENS_CHUNK + 1);
    if (retcode != NO_ERROR)
        return retcode;
    if (!lexer.tokens)
    {
        mr_error_illegal_chr_print(&lexer.error, fname, code);
        return ERROR_BAD_FORMAT;
    }

    mr_token_print(lexer.tokens);
    mr_token_free(lexer.tokens);
    return NO_ERROR;
}

void mr_print_help()
{
    fputs(
        "MetaReal [options] [output] [files]\nOptions:\n"
        "  --help\t\tDisplays the help information.\n"
        "  --version\t\tDisplays the version information.\n"
        "  --dumpver\t\tDisplays the version data.\n",
        stdout);
}
