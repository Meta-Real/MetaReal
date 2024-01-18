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
 * @file value.h
 * Definitions of the value data structure which is an extension of the node data structure. \n
 * Value helps the compiler to optimize the program at compile time. \n
 * All things defined in this file have the \a mr_value prefix.
*/

#ifndef __MR_VALUE__
#define __MR_VALUE__

#include <parser/node.h>

/**
 * @enum __MR_VALUE_ENUM
 * List of valid value types (extension of the __MR_NODE_ENUM)
 * @var __MR_VALUE_ENUM::MR_VALUE_CINT
 * C-style integer. \n
 * It can be 8, 16, 32, or 64 bits long and its either signed or unsigned.
*/
enum __MR_VALUE_ENUM
{
    MR_VALUE_CINT = MR_NODE_COUNT,

    MR_VALUE_BIFUNC
};

enum __MR_BIFUNC_ENUM
{
    MR_VALUE_BIFUNC_PRINT
};

/**
 * @struct __MR_VALUE_CINT_T
 * Data structure that holds C-style integers.
 * @var mr_longlong_t __MR_VALUE_CINT_T::value
 * Value of the integer.
 * @var mr_idx_t __MR_VALUE_CINT_T::idx
 * Starting index of the integer.
*/
#pragma pack(push, 1)
struct __MR_VALUE_CINT_T
{
    mr_longlong_t value;
    mr_idx_t idx;
};
#pragma pack(pop)
typedef struct __MR_VALUE_CINT_T mr_value_cint_t;

#endif
