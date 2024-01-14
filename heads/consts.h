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

#define MR_VERSION "0.1.0" /**< Current version of MetaReal compiler */
#define MR_VERSION_MAJOR 1 /**< Current version of MetaReal compiler (Major number) */
#define MR_VERSION_MINOR 1 /**< Current version of MetaReal compiler (Minor number) */
#define MR_VERSION_PATCH 1 /**< Current version of MetaReal compiler (Patch number) */

#define MR_CORE_VERSION "0.1.0" /**< Current version of MetaReal core */
#define MR_CORE_VERSION_MAJOR 0 /**< Current version of MetaReal core (Major number) */
#define MR_CORE_VERSION_MINOR 1 /**< Current version of MetaReal core (Minor number) */
#define MR_CORE_VERSION_PATCH 0 /**< Current version of MetaReal core (Patch number) */
#define MR_CORE "mr-core" /**< MetaReal core display name*/

#define MR_PORT_VERSION "0.1.0" /**< Current version of MetaReal portal */
#define MR_PORT_VERSION_MAJOR 0 /**< Current version of MetaReal portal (Major number) */
#define MR_PORT_VERSION_MINOR 1 /**< Current version of MetaReal portal (Minor number) */
#define MR_PORT_VERSION_PATCH 0 /**< Current version of MetaReal portal (Patch number) */
#define MR_PORT "mr-port" /**< MetaReal portal display name*/

/**
 * Maximum legal size of a single source file in characters
*/
#define MR_FILE_MAXSIZE 0x1000000ULL

/* Lexer */

/**
 * Default size of the tokens chunk. \n
 * This number is used to calculate allocation size of the \a tokens list. \n
 * Formula: `AllocationSize = SourceCodeSize / MR_LEXER_TOKENS_CHUNK`
*/
#define MR_LEXER_TOKENS_CHUNK 16

/**
 * Maximum legal size of a single identifier in characters
*/
#define MR_LEXER_IDENTIFIER_MAXSIZE 128

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

/* Generator */

/**
 * Factor for figuring out the assembly code allocation step. \n
 * Formula: `AllocationStep = OptimizerNodeSize * MR_GENERATOR_STRING_FACTOR`
*/
#define MR_GENERATOR_STRING_FACTOR 16

#endif
