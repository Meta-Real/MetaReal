/**
 * @file alloc.h
 * This file contains all the functions and processes that work with memory.
*/

#ifndef __MR_ALLOC__
#define __MR_ALLOC__

#include <defs.h>
#include <stdlib.h>

/**
 * It allocates \a size bytes from memory and returns the pointer pointing to the allocated section. \n
 * The function returns NULL if the allocation process fails.
 * @param size
 * Size of the allocated block in bytes.
 * @return A pointer pointing to the allocated block.
*/
inline mr_ptr_t mr_alloc(mr_size_t size)
{
    return malloc(size);
}

/**
 * It reallocates \a block and changes its size to \a size bytes. \n
 * The function returns NULL if the reallocation process fails.
 * @param block
 * The allocated block.
 * @param size
 * New size of the new allocated block in bytes.
 * @return A pointer pointing to the reallocated block.
*/
inline mr_ptr_t mr_realloc(mr_ptr_t block, mr_size_t size)
{
    return realloc(block, size);
}

/**
 * It deallocates the \a block from memory.
 * The \a block pointer must be valid and allocated.
 * @param block
 * The allocated block.
*/
inline void mr_free(mr_ptr_t block)
{
    free(block);
}

#endif
