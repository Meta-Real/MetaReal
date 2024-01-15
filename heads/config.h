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
 * @file config.h
 * This file contains configuration features for MetaReal compiler. \n
 * Also, global variables like the source code pointer are defined here
 * to avoid parameter passing overhead.
*/

#ifndef __MR_CONFIG__
#define __MR_CONFIG__

#include <defs.h>

/**
 * optimization level disable. \n
 * All optimization subroutines are turned off in this level.
*/
#define OPT_LEVELD ((mr_byte_t)-1)

/**
 * Optimization level 0. \n
 * Default optimization level of the MetaReal compiler. \n
 * It only handles basic optimizations. \n
 * Optimization subroutines: \n
 * 1. O-const-fold \n
 *    This optimization calculates trivial arithmetic operations at compile time. \n
 *    Example: 1 + 2 -> 3
*/
#define OPT_LEVEL0 ((mr_byte_t)0)

/**
 * Optimization level 1. \n
 * Basic optimization level of the MetaReal compiler. \n
 * The main focus of this level is on expressions and variables. \n
 * Optimization subroutines: \n
 * 1. O-rem-useless \n
 *    This optimization removes useless code that has no effect at runtime. \n
 *    Example: unused variables will be removed from the code.
*/
#define OPT_LEVEL1 ((mr_byte_t)1)

/**
 * Optimization level 2. \n
 * Intermediate optimization level of the MetaReal compiler. \n
 * The main focus of this level is on statements and data structures. \n
*/
#define OPT_LEVEL2 ((mr_byte_t)2)

/**
 * Optimization level 3. \n
 * Advanced optimization level of the MetaReal compiler. \n
 * The main focus of this level is on functions, threading, and multi-file projects.
*/
#define OPT_LEVEL3 ((mr_byte_t)3)

/**
 * Optimization level ultra. \n
 * Ultimate optimization level of the MetaReal compiler. \n
 * The compiler will slow down with this optimization level turned on.
 * So it's better suited for release builds.
*/
#define OPT_LEVELU ((mr_byte_t)4)

/**
 * @struct __MR_CONFIG_T
 * Main structure that holds information about
 * optimization levels, io system, debugging configurations, etc.
 * @var mr_str_ct __MR_CONFIG_T::code
 * The source code.
 * @var mr_long_t __MR_CONFIG_T::size
 * Size of the source code.
 * @var mr_str_ct __MR_CONFIG_T::fname
 * Name of the source file.
 * @var mr_bool_t __MR_CONFIG_T::opt_const_fold
 * A boolean that indicates whether or not the O-const-fold optimization subroutine is on.
 * @var mr_bool_t __MR_CONFIG_T::opt_rem_useless
 * A boolean that indicates whether or not the O-rem-useless optimization subroutine is on.
*/
#pragma pack(push, 1)
struct __MR_CONFIG_T
{
    mr_str_ct code;
    mr_long_t size;
    mr_str_ct fname;

    mr_bool_t opt_const_fold : 1;
    mr_bool_t opt_rem_useless : 1;
};
#pragma pack(pop)
typedef struct __MR_CONFIG_T mr_config_t;

/**
 * The \a mr_config_t instant that holds all information
 * about the code and its compilation process.
*/
extern mr_config_t _mr_config;

/**
 * It configures the optimization subroutines based on the optimization level passed to it.
 * @param olevel
 * Level of the optimization (one of 6 values)
*/
void mr_config_opt(
    mr_byte_t olevel);

#endif
