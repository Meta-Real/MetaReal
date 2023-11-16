/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_MEMORY__
#define __MR_MEMORY__

#include <stdint.h>

/**
 * \fn void *mr_alloc(uint64_t size)
 * Allocates \a size bytes from memory and returns the pointer pointing to the allocated section. \n
 * Exits the program if the allocation process fails.
 * \param size Size of the allocated block in bytes
 * \return Pointer pointing to the allocated block
*/
void *mr_alloc(uint64_t size);

/**
 * \fn void *mr_realloc(void *block, uint64_t size)
 * Reallocates \a block and changes its size to \a size bytes. \n
 * Exits the program if the allocation process fails.
 * \param block Allocated block
 * \param size New size of the new allocated block in bytes
 * \return New pointer pointing to the reallocated block
*/
void *mr_realloc(void *block, uint64_t size);

/**
 * \fn void mr_free(void *block)
 * Deallocates the \a block from memory.
 * \param block Allocated block
*/
void mr_free(void *block);

#endif /* __MR_MEMORY__ */
