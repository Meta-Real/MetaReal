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

#include <optimizer/operation.h>
#include <stdlib.h>

void mr_operation_add(
    mr_node_t *left, mr_node_t *right)
{
    switch (left->type)
    {
    case MR_VALUE_CINT:
    {
        mr_value_cint_t *lvalue = left->value;
        switch (right->type)
        {
        case MR_VALUE_CINT:
        {
            mr_value_cint_t *rvalue = right->value;
            lvalue->value += rvalue->value;

            free(rvalue);
            return;
        }
        }
    }
    }
}
