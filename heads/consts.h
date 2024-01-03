/**
 * @file consts.h
 * This file contains all constants defined by the compiler. \n
 * Types of constants: \n
 * 1. Compiler information constants: constants that hold an information about the compiler. \n
 *    Examples: compiler version, core used by the compiler, core version, etc. \n
 * 2. Compiler process constants: constants that hold an information about the compile process. \n
 *    Examples: default length of strings, numbers, identifiers, etc.
*/

#ifndef __MR_CONSTS__
#define __MR_CONSTS__

#define MR_VERSION_MAJOR "0" /**< Current version of MetaReal compiler (Major number) */
#define MR_VERSION_MINOR "1" /**< Current version of MetaReal compiler (Minor number) */
#define MR_VERSION_PATCH "0" /**< Current version of MetaReal compiler (Patch number) */

/**
 * Current version of MetaReal compiler
*/
#define MR_VERSION MR_VERSION_MAJOR "." MR_VERSION_MINOR "." MR_VERSION_PATCH

#define MR_CORE_VERSION_MAJOR "0" /**< Current version of MetaReal core (Major number) */
#define MR_CORE_VERSION_MINOR "1" /**< Current version of MetaReal core (Minor number) */
#define MR_CORE_VERSION_PATCH "0" /**< Current version of MetaReal core (Patch number) */

#define MR_CORE "mr-core" /**< MetaReal core display name*/

/**
 * Current version of MetaReal core
*/
#define MR_CORE_VERSION MR_CORE_VERSION_MAJOR "." MR_CORE_VERSION_MINOR "." MR_CORE_VERSION_PATCH

#define MR_PORT_VERSION_MAJOR "0" /**< Current version of MetaReal portal (Major number) */
#define MR_PORT_VERSION_MINOR "1" /**< Current version of MetaReal portal (Major number) */
#define MR_PORT_VERSION_PATCH "0" /**< Current version of MetaReal portal (Major number) */

#define MR_PORT "mr-port" /**< MetaReal portal display name*/

/**
 * Current version of MetaReal portal
*/
#define MR_PORT_VERSION MR_PORT_VERSION_MAJOR "." MR_PORT_VERSION_MINOR "." MR_PORT_VERSION_PATCH

/* Lexer */

/**
 * Default size of the tokens chunk. \n
 * This number is used to calculate allocation size of the \a tokens list. \n
 * Formula: `AllocationSize = SourceCodeSize / MR_LEXER_TOKENS_CHUNK`
*/
#define MR_LEXER_TOKENS_CHUNK 16

/**
 * Default size of a \a number in characters (for allocation).
*/
#define MR_LEXER_NUMBER_SIZE 16

/**
 * Default size of the <em>exponent of a number</em> in characters (for allocation).
*/
#define MR_LEXER_NUMBER_EXP_SIZE 4

/**
 * Default size of a \a string in characters (for allocation).
*/
#define MR_LEXER_STR_SIZE 128

/**
 * Default size of a <em>formatted string</em> in characters (for allocation).
*/
#define MR_LEXER_FSTR_SIZE 128

/* Parser */

/**
 * Default size of the nodes chunk. \n
 * This number is used to calculate allocation size of the \a nodes list. \n
 * Formula: `AllocationSize = SourceCodeSize / MR_PARSER_NODES_CHUNK`
*/
#define MR_PARSER_NODES_CHUNK 64

/**
 * Default size of a <em>function call</em> argument list.
*/
#define MR_PARSER_FUNC_CALL_SIZE 4

/**
 * Maximum size of a <em>function call</em> argument list.
*/
#define MR_PARSER_FUNC_CALL_MAX (MR_PARSER_FUNC_CALL_SIZE * 16)

/**
 * Default size of a <em>dollar method</em> parameter list.
*/
#define MR_PARSER_DOLLAR_METHOD_SIZE 4

/**
 * Maximum size of a <em>dollar method</em> parameter list.
*/
#define MR_PARSER_DOLLAR_METHOD_MAX (MR_PARSER_DOLLAR_METHOD_SIZE * 4)

#endif
