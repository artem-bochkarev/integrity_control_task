#pragma once

typedef unsigned int uint;

namespace gost
{
    struct key
    {
        uint X[8];
    };

    struct replace_key
    {
        unsigned char table[8][16];
    };

    struct block
    {
        uint A, B;
        __int64 toInt64()
        {
            __int64 a = A;
            a <<= 32;
            a |= B;
            return a;
        }
    };

    block vstavka(uint* in, uint size, gost::replace_key& rkey, gost::key& key);

};