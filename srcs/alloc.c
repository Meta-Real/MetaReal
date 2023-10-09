/*
    MetaReal Compiler version 1.0.0
*/

#include <alloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <consts.h>

#if TRACK_ALLOC
uint64_t i = 0;
#endif

void *mr_alloc(uint64_t size)
{
    void *block = malloc(size);
    if (!block)
    {
        fprintf(stderr, "Internal Error: Could not allocate %llu bytes of memory\n", size);
        abort();
    }

#if TRACK_ALLOC
    i++;
    printf("%llu >>>>>>>>>>>>>>> %p\n", i, block);
#endif

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
#if TRACK_ALLOC
    if (block)
    {
        i--;
        printf("%llu <<<<<<<<<<<< %p\n", i, block);
    }
#endif

    free(block);
}
