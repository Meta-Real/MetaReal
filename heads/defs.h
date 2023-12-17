/**
 * @file defs.h
 * This file contains all the primitive data types and their predefined values.
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
 * Holds the position of each element in the code file. \n
 * The main purpose of the structure is to help the debugger find the position of the error.
 * @var mr_size_t __MR_POS_T::idx
 * The index of the position (used by the \a fseek function).
 * @var mr_size_t __MR_POS_T::ln
 * The line of the position (for the debugger to show the information).
*/
struct __MR_POS_T
{
    mr_size_t idx;
    mr_size_t ln;
};
typedef struct __MR_POS_T mr_pos_t;

#endif /* __MR_DEFS__ */
