/**
 * @file defs.h
 * This file contains all primitive data types and their predefined values.
*/

#ifndef __MR_DEFS__
#define __MR_DEFS__

#include <inttypes.h>

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

#endif
