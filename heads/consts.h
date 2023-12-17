/**
 * @file consts.h
 * This file contains all the constants defined in by the compiler. \n
 * Types of constants: \n
 * 1. Compiler information constants: constants that hold an information about the compiler. \n
 *    Examples: compiler version, core used by the compiler, core version, etc. \n
 * 2. Compiler process constants: constants that hold an information about the compile process. \n
 *    Examples: default length of the string, number, identifier, etc.
*/

#ifndef __MR_CONSTS__
#define __MR_CONSTS__

#define MR_VERSION_MAJOR "1"
#define MR_VERSION_MINOR "0"
#define MR_VERSION_PATCH "0"

#define MR_VERSION MR_VERSION_MAJOR "." MR_VERSION_MINOR "." MR_VERSION_PATCH

#define MR_CORE_VERSION_MAJOR "1"
#define MR_CORE_VERSION_MINOR "0"
#define MR_CORE_VERSION_PATCH "0"

#define MR_CORE "mr-core"
#define MR_CORE_VERSION MR_CORE_VERSION_MAJOR "." MR_CORE_VERSION_MINOR "." MR_CORE_VERSION_PATCH

#define MR_PORT_VERSION_MAJOR "1"
#define MR_PORT_VERSION_MINOR "0"
#define MR_PORT_VERSION_PATCH "0"

#define MR_PORT "mr-port"
#define MR_PORT_VERSION MR_PORT_VERSION_MAJOR "." MR_PORT_VERSION_MINOR "." MR_PORT_VERSION_PATCH

/* Lexer */

#define MR_LEXER_TOKENS_SIZE 0x400

#endif /* __MR_CONSTS__ */
