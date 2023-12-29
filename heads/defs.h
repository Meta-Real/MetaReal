/**
 * @file defs.h
 * This file contains all primitive data types and their predefined values.
*/

#ifndef __MR_DEFS__
#define __MR_DEFS__

#include <stdint.h>

typedef uint8_t mr_bool_t;
typedef uint8_t mr_byte_t;
typedef uint16_t mr_short_t;
typedef uint32_t mr_long_t;
typedef uint64_t mr_size_t;

typedef void *mr_ptr_t;

typedef char mr_chr_t;
typedef mr_chr_t *mr_str_t;
typedef const mr_chr_t *mr_str_ct;

#define MR_TRUE 1
#define MR_FALSE 0

#endif
