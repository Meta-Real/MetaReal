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

#include <config.h>
#include <string.h>

mr_config_t _mr_config = {NULL, NULL, NULL, NULL, NULL, 0};

void mr_config_opt(mr_byte_t olevel)
{
    switch (olevel)
    {
    case OPT_LEVELD:
        break;
    case OPT_LEVEL0:
        break;
    }
}
