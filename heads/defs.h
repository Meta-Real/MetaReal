/**
 * @file defs.h
 * This file contains all primitive data types and their predefined values.
*/

#ifndef __MR_DEFS__
#define __MR_DEFS__

typedef unsigned char mr_byte_t;
typedef unsigned char mr_bool_t;
typedef unsigned long long mr_size_t;
typedef char mr_chr_t;

typedef void *mr_ptr_t;
typedef mr_chr_t *mr_str_t;
typedef const mr_chr_t *mr_str_ct;

#define MR_TRUE 1
#define MR_FALSE 0

/**
 * @struct __MR_POS_T
 * It holds the position of each element in the source code. \n
 * The main purpose of this structure is to help the debugger find the position of errors.
 * @var mr_size_t __MR_POS_T::idx
 * Index of the position (used by the \a fseek function).
 * @var mr_size_t __MR_POS_T::ln
 * Line of the position (for the debugger to show information).
*/
struct __MR_POS_T
{
    mr_size_t idx;
    mr_size_t ln;
};
typedef struct __MR_POS_T mr_pos_t;

#endif /* __MR_DEFS__ */
