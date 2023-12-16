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

#endif /* __MR_DEFS__ */
