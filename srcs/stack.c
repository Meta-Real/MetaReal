/*
MIT License

Copyright (c) 2023 MetaReal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

/**
 * @file stack.c
 * File that contains function definitions of the stack data structure.
*/

#include <stack.h>
#include <stdlib.h>

mr_stack_t _mr_stack = {NULL, NULL, 0, 0, 0, 0, 0, 0};

mr_byte_t mr_stack_init(
    mr_long_t size, mr_long_t psize)
{
    _mr_stack = (mr_stack_t){malloc(size * sizeof(mr_byte_t)), malloc(psize * sizeof(mr_ptr_t)),
        size, 0, size, psize, 0, psize};

    if (!_mr_stack.data)
    {
        free(_mr_stack.ptrs);
        return MR_ERROR_NOT_ENOUGH_MEMORY;
    }

    if (!_mr_stack.ptrs)
    {
        free(_mr_stack.data);
        return MR_ERROR_NOT_ENOUGH_MEMORY;
    }

    return MR_NOERROR;
}

mr_byte_t mr_stack_push(
    mr_long_t *ptr, mr_byte_t size)
{
    if (_mr_stack.ptr + size > _mr_stack.size)
    {
        mr_byte_t *block;

        block = realloc(_mr_stack.data,
            (_mr_stack.size += _mr_stack.exalloc) * sizeof(mr_byte_t));
        if (!block)
            return MR_ERROR_NOT_ENOUGH_MEMORY;

        _mr_stack.data = block;
    }

    *ptr = _mr_stack.ptr;
    _mr_stack.ptr += size;
    return MR_NOERROR;
}

mr_byte_t mr_stack_palloc(
    mr_idx_t *ptr, mr_long_t size)
{
    if (_mr_stack.pptr == _mr_stack.psize)
    {
        mr_ptr_t *block;

        block = realloc(_mr_stack.ptrs,
            (_mr_stack.psize += _mr_stack.pexalloc) * sizeof(mr_ptr_t));
        if (!block)
            return MR_ERROR_NOT_ENOUGH_MEMORY;

        _mr_stack.ptrs = block;
    }

    _mr_stack.ptrs[_mr_stack.pptr] = malloc(size);
    if (!_mr_stack.ptrs[_mr_stack.pptr])
        return MR_ERROR_NOT_ENOUGH_MEMORY;

    *ptr = MR_IDX_DECOMPOSE(_mr_stack.pptr);
    _mr_stack.pptr++;
    return MR_NOERROR;
}

mr_byte_t mr_stack_prealloc(
    mr_long_t ptr, mr_long_t size)
{
    mr_ptr_t block;

    block = realloc(_mr_stack.ptrs[ptr], size);
    if (!block)
        return MR_ERROR_NOT_ENOUGH_MEMORY;

    _mr_stack.ptrs[ptr] = block;
    return MR_NOERROR;
}

void mr_stack_free(void)
{
    free(_mr_stack.data);

    while (_mr_stack.pptr--)
        free(_mr_stack.ptrs[_mr_stack.pptr]);
    free(_mr_stack.ptrs);
}
