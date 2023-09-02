/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_MEMORY__
#define __MR_MEMORY__

#include <stdint.h>

void *mr_alloc(uint64_t size);
void *mr_realloc(void *block, uint64_t size);

void mr_free(void *block);

#endif /* __MR_MEMORY__ */
