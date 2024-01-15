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
 * @file stack.h
 * This file contains definitions of the stack data structure. \n
 * The stack is used for allocating node and value data structures. \n
 * All things defined in \a stack.c and this file have the \a mr_stack prefix.
*/

#ifndef __MR_STACK__
#define __MR_STACK__

#include <defs.h>

/**
 * @struct __MR_STACK_T
 * The structure that organizes allocations in the parser and optimizer steps.
 * @var mr_byte_t* __MR_STACK_T::data
 * Data of the stack.
 * @var mr_long_t __MR_STACK_T::size
 * Allocated size of the data in bytes.
 * @var mr_long_t __MR_STACK_T::ptr
 * Pointer that points to the end of the stack data (stack pointer).
 * @var mr_long_t __MR_STACK_T::exalloc
 * Allocation step used for reallocating the stack data.
 * @var mr_ptr_t* __MR_STACK_T::ptrs
 * List of allocated pointers by the stack.
 * @var mr_long_t __MR_STACK_T::psize
 * Allocated size of the \a ptrs list.
 * @var mr_long_t __MR_STACK_T::pptr
 * Pointer that points to the end of the \a ptrs list.
 * @var mr_long_t __MR_STACK_T::pexalloc
 * Allocation step used for reallocating the \a ptrs list.
*/
#pragma pack(push, 1)
struct __MR_STACK_T
{
    mr_byte_t *data;
    mr_long_t size;
    mr_long_t ptr;
    mr_long_t exalloc;

    mr_ptr_t *ptrs;
    mr_long_t psize;
    mr_long_t pptr;
    mr_long_t pexalloc;
};
#pragma pack(pop)
typedef struct __MR_STACK_T mr_stack_t;

/**
 * The \a mr_stack_t instant that stores parser and optimizer allocations.
*/
extern mr_stack_t _mr_stack;

/**
 * It initializes the stack. \n
 * If the initialization failed, it returns an error.
 * @param size
 * Size of the \a data field.
 * @param psize
 * Size of the \a ptrs field.
 * @return
 * It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns <em>MR_NOERROR</em>.
 * Otherwise, it returns the error code (defined in \a defs.h header file).
*/
mr_byte_t mr_stack_init(
    mr_long_t size, mr_long_t psize);

/**
 * It pushes a new data into the stack. \n
 * The stack will be reallocated if necessary.
 * @param ptr
 * Pointer to the allocated data.
 * @param size
 * Requested size for the block.
 * @return
 * It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns <em>MR_NOERROR</em>.
 * Otherwise, it returns the error code (defined in \a defs.h header file).
*/
mr_byte_t mr_stack_push(
    mr_long_t *ptr, mr_byte_t size);

/**
 * It allocates a new dynamic pointer and stores it in the \a ptrs list. \n
 * The returned value will be the index of the pointer in \a ptrs list.
 * @param ptr
 * Pointer to the allocated data.
 * @param size
 * Requested size for the block.
 * @return
 * It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns <em>MR_NOERROR</em>.
 * Otherwise, it returns the error code (defined in \a defs.h header file).
*/
mr_byte_t mr_stack_palloc(
    mr_long_t *ptr, mr_long_t size);

/**
 * It reallocates pointer that is stored in the specified index of the \a ptrs list. \n
 * The new pointer will be placed at the specified index.
 * @param ptr
 * Pointer to the allocated data.
 * @param size
 * Requested size for the block.
 * @return
 * It returns a code which indicates if the process was successful or not. \n
 * If the process was successful, it returns <em>MR_NOERROR</em>.
 * Otherwise, it returns the error code (defined in \a defs.h header file).
*/
mr_byte_t mr_stack_prealloc(
    mr_long_t ptr, mr_long_t size);

/**
 * It clears the stack and its data.
*/
void mr_stack_free(void);

#endif
