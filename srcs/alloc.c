/*
    MetaReal Compiler version 1.0.0
*/

#include <alloc.h>
#include <stdlib.h>
#include <stdio.h>

void *mr_alloc(uint64_t size)
{
    void *block = malloc(size);
    if (!block)
    {
        fprintf(stderr, "Internal Error: Could not allocate %llu bytes of memory\n", size);
        abort();
    }

    return block;
}

void *mr_realloc(void *block, uint64_t size)
{
    block = realloc(block, size);
    if (!block)
    {
        fprintf(stderr, "Internal Error: Could not allocate %llu bytes of memory\n", size);
        abort();
    }

    return block;
}

void mr_free(void *block)
{
    free(block);
}
