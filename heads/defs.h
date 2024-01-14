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
 * @file defs.h
 * This file contains all primitive data types and their predefined values.
*/

#ifndef __MR_DEFS__
#define __MR_DEFS__

#include <inttypes.h>
#include <stddef.h>

typedef uint8_t mr_bool_t;
typedef uint8_t mr_byte_t;
typedef uint16_t mr_short_t;
typedef uint32_t mr_long_t;
typedef uint64_t mr_longlong_t;

typedef size_t mr_size_t;

typedef void *mr_ptr_t;

typedef char mr_chr_t;
typedef mr_chr_t *mr_str_t;
typedef const mr_chr_t *mr_str_ct;

#define MR_TRUE ((mr_bool_t)1)
#define MR_FALSE ((mr_bool_t)0)

/* error codes */

#define MR_NOERROR 0
#define MR_ERROR_FILE_NOT_FOUND 2
#define MR_ERROR_NOT_ENOUGH_MEMORY 8
#define MR_ERROR_BAD_FORMAT 11
#define MR_ERROR_BAD_COMMAND 22
#define MR_ERROR_FILE_TOO_LARGE 223

#endif
