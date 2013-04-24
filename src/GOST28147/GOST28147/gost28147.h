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
    };

    block vstavka(uint* in, uint size, gost::replace_key& rkey, gost::key& key);

};

uint f( uint A, uint K, const gost::replace_key& key );