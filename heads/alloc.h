/**
 * @file alloc.h
 * This file contains all the functions and processes that work with memory.
*/

#ifndef __MR_ALLOC__
#define __MR_ALLOC__

#include <defs.h>
#include <stdlib.h>

/**
 * @def mr_alloc(size)
 * It allocates \a size bytes from memory and returns the pointer pointing to the allocated section. \n
 * The function returns NULL if the allocation process fails.
 * @param size
 * Size of the allocated block in bytes.
 * @return A pointer pointing to the allocated block.
*/
#define mr_alloc(size) (malloc((size_t)(size)))

/**
 * @def mr_realloc(block, size)
 * It reallocates \a block and changes its size to \a size bytes. \n
 * The function returns NULL if the reallocation process fails.
 * @param block
 * The allocated block.
 * @param size
 * New size of the new allocated block in bytes.
 * @return A pointer pointing to the reallocated block.
*/
#define mr_realloc(block, size) (realloc((block), (size_t)(size)))

/**
 * @def mr_free
 * It deallocates the \a block from memory.
 * The \a block pointer must be valid and allocated.
 * @param block
 * The allocated block.
*/
#define mr_free free

#endif
