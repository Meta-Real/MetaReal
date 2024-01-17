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

    mr_long_t storage;
};
#pragma pack(pop)
typedef struct __MR_VALUE_CINT_T mr_value_cint_t;

#define MR_VALUE_REG_RAX  0x00000001
#define MR_VALUE_REG_RRAX 0x00000002
#define MR_VALUE_REG_RBX  0x00000004
#define MR_VALUE_REG_RRBX 0x00000008
#define MR_VALUE_REG_RCX  0x00000010
#define MR_VALUE_REG_RRCX 0x00000020
#define MR_VALUE_REG_RDX  0x00000040
#define MR_VALUE_REG_RRDX 0x00000080
#define MR_VALUE_REG_RSI  0x00000100
#define MR_VALUE_REG_RRSI 0x00000200
#define MR_VALUE_REG_RDI  0x00000400
#define MR_VALUE_REG_RRDI 0x00000800
#define MR_VALUE_REG_RBP  0x00001000
#define MR_VALUE_REG_RRBP 0x00002000
#define MR_VALUE_REG_R8   0x00004000
#define MR_VALUE_REG_RR8  0x00008000
#define MR_VALUE_REG_R9   0x00010000
#define MR_VALUE_REG_RR9  0x00020000
#define MR_VALUE_REG_R10  0x00040000
#define MR_VALUE_REG_RR10 0x00080000
#define MR_VALUE_REG_R11  0x00100000
#define MR_VALUE_REG_RR11 0x00200000
#define MR_VALUE_REG_R12  0x00400000
#define MR_VALUE_REG_RR12 0x00800000
#define MR_VALUE_REG_R13  0x01000000
#define MR_VALUE_REG_RR13 0x02000000
#define MR_VALUE_REG_R14  0x04000000
#define MR_VALUE_REG_RR14 0x08000000
#define MR_VALUE_REG_R15  0x10000000
#define MR_VALUE_REG_RR15 0x20000000
#define MR_VALUE_REG_RSP  0x80000000

#define MR_VALUE_REG_ALL  0x55555555

#endif
