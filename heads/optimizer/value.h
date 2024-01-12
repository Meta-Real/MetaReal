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

#ifndef __MR_VALUE__
#define __MR_VALUE__

#include <parser/node.h>

#define MR_VALUE_USELESS 1

enum __MR_VALUE_ENUM
{
    MR_VALUE_CINT = MR_NODE_COUNT,
};

#define MR_VALUE_COUNT (MR_VALUE_CINT - MR_NODE_COUNT + 1)

#pragma pack(push, 1)
struct __MR_VALUE_CINT_T
{
    mr_longlong_t value;

    mr_long_t sidx;
    mr_short_t size;
};
#pragma pack(pop)
typedef struct __MR_VALUE_CINT_T mr_value_cint_t;

void mr_value_free(
    mr_node_t *value);

void mr_values_free(
    mr_node_t *values, mr_long_t size);

void mr_values_print(
    mr_node_t *values, mr_long_t size);

#endif
