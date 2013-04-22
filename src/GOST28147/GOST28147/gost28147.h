#pragma once
#include <inttypes.h>

namespace gost
{
    struct key
    {
        int X0, X1, X2, X3, X4, X5, X6, X7;
    };

    struct replace_key
    {
        unsigned char table[16][8];
    };


}