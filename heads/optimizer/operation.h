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
 * @file operation.h
 * This file contains definitions of arithmetic and logical operations (used by the optimizer). \n
 * Operations of this file are used at compile time to calculate and optimize expressions. \n
 * All things defined in \a operation.c and this file have the \a mr_operation prefix.
*/

#ifndef __MR_OPERATION__
#define __MR_OPERATION__

#include <optimizer/value.h>

/**
 * It adds two expressions and stores the result in the \a left operand.
 * @param left
 * Left operand.
 * @param right
 * Right operand.
*/
void mr_operation_add(
    mr_node_t *left, mr_node_t *right);

#endif
