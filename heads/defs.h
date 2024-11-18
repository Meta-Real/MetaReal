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

typedef void *mr_ptr_t;

typedef char mr_chr_t;
typedef mr_chr_t *mr_str_t;
typedef const mr_chr_t *mr_str_ct;

#define MR_TRUE ((mr_bool_t)1)
#define MR_FALSE ((mr_bool_t)0)

/**
 * @struct __MR_IDX_T
 * A data structure that points to the position of characters in the source code.
 * @var __MR_IDX_T::lidx
 * Index of the character (low part).
 * @var __MR_IDX_T::hidx
 * Index of the character (high part).
*/
#pragma pack(push, 1)
struct __MR_IDX_T
{
    mr_short_t lidx;
    mr_byte_t hidx;
};
#pragma pack(pop)
typedef struct __MR_IDX_T mr_idx_t;

/**
 * Invalid index. \n
 * Used for indicating missing data.
*/
#define MR_INVALID_IDX ((mr_idx_t){.lidx=0xffff, .hidx=0xff})

/**
 * Zero index. \n
 * Used for lists that have no elements.
*/
#define MR_ZERO_IDX ((mr_idx_t){.lidx=0, .hidx=0})

/**
 * Invalid index converted to a long value.
*/
#define MR_INVALID_IDX_CODE ((mr_long_t)0xffffff)

/**
 * @def MR_IDX_EXTRACT(idx)
 * It extracts the index as 32 bit integer. \n
 * This is achieved by concatenating \a hidx and \a lidx fields.
 * @param idx
 * An instance of the \a mr_idx_t data structure
*/
#define MR_IDX_EXTRACT(idx) ((mr_long_t)((idx).hidx << 16) | (idx).lidx)

/**
 * @def MR_IDX_DECOMPOSE(idx)
 * Constructs an index structure based on the 32 bit <em>idx</em>.
 * @param idx
 * Index passed as a 32 bit integer.
*/
#define MR_IDX_DECOMPOSE(idx) ((mr_idx_t){.lidx=(idx) & 0xffff, .hidx=(mr_byte_t)((idx) >> 16)})

/* error codes */

#define MR_NOERROR 0
#define MR_ERROR_FILE_NOT_FOUND 2
#define MR_ERROR_NOT_ENOUGH_MEMORY 8
#define MR_ERROR_BAD_FORMAT 11
#define MR_ERROR_BAD_COMMAND 22
#define MR_ERROR_FILE_TOO_LARGE 223

#endif
