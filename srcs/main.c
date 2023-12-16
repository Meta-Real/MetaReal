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

/**
 * @fn mr_byte_t mr_compile(mr_str_ct fname, FILE *code, mr_size_t size)
 * <pre>
 * It compiles the code according to the MetaReal compile rules. \n
 * Order of compilation: \n
 *     [code] -> lexer -> parser -> optimizer -> generator -> assembler -> linker -> [executable] \n
 * Also, the debugger will debug the code during the compilation process (if enabled). \n
 * Dollar methods are handled with a different mechanism in the optimization step.
 * </pre>
 * @param fname
 * The name of the source file (for displaying errors).
 * @param code
 * The file that contains the code.
 * @param size
 * The size of the \a code.
 * @return It returns the code which indicates if the process was successful or not. \n
 * If the process was successful, it returns 0. Otherwise, it returns the error code.
*/
mr_byte_t mr_compile(mr_str_ct fname, FILE *code, mr_size_t size)
{
    mr_lexer_t lexer;

    mr_lexer(&lexer, code, size);
    if (!lexer.tokens)
    {
        mr_illegal_chr_print(&lexer.error, fname, code, size);
        return ERROR_BAD_FORMAT;
    }

    return NO_ERROR;
}

/**
 * @fn void mr_print_help()
 * It prints out the help information (called with the \--help flag).
*/
void mr_print_help()
{
    fputs(
        "MetaReal [options] [output] [files]\nOptions:\n"
        "  --help\t\tDisplays the help information.\n"
        "  --version\t\tDisplays the version information.\n"
        "  --dumpver\t\tDisplays the version data.\n",
        stdout);
}

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

        FILE *code = fopen(argv[1], "rb");
        if (!code)
        {
            fprintf(stderr,
                "Internal error: can not find the file \"%s\"\n",
                argv[1]);
            return ERROR_FILE_NOT_FOUND;
        }

        fseek(code, 0, SEEK_END);
        mr_size_t size = ftell(code);
        rewind(code);

        if (!size)
        {
            fclose(code);
            return NO_ERROR;
        }

        mr_byte_t retcode = mr_compile(argv[1], code, size);
        fclose(code);
        return retcode;
    }

    fputs(
        "Internal error: invalid command.\n"
        "Write \"MetaReal --help\" for more information.\n",
        stderr);
    return ERROR_BAD_COMMAND;
}
