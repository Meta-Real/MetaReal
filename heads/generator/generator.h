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

#ifndef __MR_GENERATOR__
#define __MR_GENERATOR__

#include <parser/node.h>

struct __MR_GENERATOR_T
{
    mr_str_t data;
    mr_long_t size;
    mr_byte_t error;
};
typedef struct __MR_GENERATOR_T mr_generator_t;

mr_generator_t mr_generator(
    mr_node_t *nodes, mr_long_t size, mr_long_t alloc);

#endif
