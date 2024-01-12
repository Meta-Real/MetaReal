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

#ifndef __MR_CONFIG__
#define __MR_CONFIG__

#include <defs.h>

#define OPT_LEVELD ((mr_byte_t)-1)
#define OPT_LEVEL0 ((mr_byte_t)0)
#define OPT_LEVEL1 ((mr_byte_t)1)
#define OPT_LEVEL2 ((mr_byte_t)2)
#define OPT_LEVEL3 ((mr_byte_t)3)
#define OPT_LEVELU ((mr_byte_t)4)

#pragma pack(push, 1)
struct __MR_CONFIG_T
{
    mr_bool_t opt_const_fold : 1;
    mr_bool_t opt_rem_useless : 1;
};
#pragma pack(pop)
typedef struct __MR_CONFIG_T mr_config_t;

extern mr_config_t _mr_config;

void mr_config_opt(mr_byte_t olevel);

#endif
