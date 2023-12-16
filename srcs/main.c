/**
 * @file main.c
 * Main file of the compiler that aggregates all the compiler parts together. \n
 * Command line prompts are also defined here.
*/

#include <stdio.h>
#include <alloc.h>
#include <string.h>
#include <consts.h>

/**
 * @fn void mr_print_help()
 * It prints out the help informations (called with the --help flag).
*/
void mr_print_help()
{
    fputs(
        "MetaReal [options] [output] [files]\nOptions:\n"
        "  --help\t\tDisplays the help information.\n"
        "  --version\t\tDisplays the version information.\n"
        "  --dumpver\t\tDisplays the version data.\n",
        stderr);
}

int main(int argc, const char *argv[])
{
    if (argc == 1)
    {
        fputs(
            "Invalid syntax, nothing to process.\n"
            "Write \"MetaReal --help\" for more information.\n",
            stderr);
        return EXIT_FAILURE;
    }

    if (argc == 2)
    {
        if (!strcmp(argv[1], "--help"))
        {
            mr_print_help();
            return EXIT_SUCCESS;
        }
        if (!strcmp(argv[1], "--version"))
        {
            fputs(
                "MetaReal compiler version " MR_VERSION "\n"
                "MetaReal core verified as " MR_CORE ", version " MR_CORE_VERSION "\n"
                "MetaReal port verified as " MR_PORT ", version " MR_PORT_VERSION "\n",
                stderr);
            return EXIT_SUCCESS;
        }
        if (!strcmp(argv[1], "--dumpver"))
        {
            fputs(
                MR_VERSION " "
                MR_CORE " " MR_CORE_VERSION " "
                MR_PORT " " MR_PORT_VERSION "\n",
                stderr);
            return EXIT_SUCCESS;
        }

    }

    fputs(
        "Invalid syntax.\n"
        "Write \"MetaReal --help\" for more information.\n",
        stderr);
    return EXIT_FAILURE;
}
