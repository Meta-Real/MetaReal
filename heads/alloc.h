/**
 * @file alloc.h
 * This file contains all the functions and processes that work with the memory.
*/

#ifndef __MR_ALLOC__
#define __MR_ALLOC__

#include <defs.h>
#include <stdlib.h>

/**
 * It allocates \a size bytes from memory and returns the pointer pointing to the allocated section. \n
 * The function returns NULL if the allocation process fails.
 * @param size
 * The size of the allocated block in bytes.
 * @return The pointer pointing to the allocated block.
*/
inline mr_ptr_t mr_alloc(mr_size_t size)
{
    return malloc(size);
}

/**
 * It allocates \a size bytes from memory and returns the pointer pointing to the allocated section. \n
 * The returned pointer is aligned by the \a align parameter. \n
 * The function returns NULL if the allocation process fails.
 * @param size
 * The size of the allocated block in bytes.
 * @param align
 * The alignment of the allocated block (must be a power of 2).
 * @return The aligned pointer pointing to the allocated block.
*/
inline mr_ptr_t mr_aligned_alloc(mr_size_t size, mr_size_t align)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    return _aligned_malloc(size, align);
#else
    return NULL; // will support POSIX
#endif
}

/**
 * It reallocates \a block and changes its size to \a size bytes. \n
 * The function returns NULL if the reallocation process fails.
 * @param block
 * The allocated block.
 * @param size
 * The new size of the new allocated block in bytes.
 * @return The new pointer pointing to the reallocated block.
*/
inline mr_ptr_t mr_realloc(mr_ptr_t block, mr_size_t size)
{
    return realloc(block, size);
}

/**
 * It reallocates \a block and changes its size to \a size bytes. \n
 * The returned pointer is aligned by the \a align parameter. \n
 * The function returns NULL if the reallocation process fails.
 * @param block
 * The allocated block (must be aligned).
 * @param size
 * The new size of the new allocated block in bytes.
 * @param align
 * The alignment of the allocated block (must be a power of 2).
 * @return The new pointer pointing to the reallocated block.
*/
inline mr_ptr_t mr_aligned_realloc(mr_ptr_t block, mr_size_t size, mr_size_t align)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    return _aligned_realloc(block, size, align);
#else
    return NULL; // will support POSIX
#endif
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

/**
 * It deallocates the \a block from memory.
 * The \a block pointer must be valid and allocated.
 * @param block
 * The allocated block (must be aligned).
*/
inline void mr_aligned_free(mr_ptr_t block)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    _aligned_free(block);
#else
    // will support POSIX
#endif
}

#endif /* __MR_ALLOC__ */
