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

/**
 * @struct __MR_POS_T
 * It holds the position of each element in the source code. \n
 * The main purpose of this structure is to help the debugger find the position of errors.
 * @var mr_long_t __MR_POS_T::idx
 * Index of the position (for underlining the error).
 * @var mr_long_t __MR_POS_T::ln
 * Line of the position (for the debugger to show as an information).
*/
struct __MR_POS_T
{
    mr_long_t idx;
    mr_long_t ln;
};
typedef struct __MR_POS_T mr_pos_t;

#endif
