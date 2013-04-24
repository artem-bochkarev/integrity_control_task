#include "gost28147.h"
#include "cassert"
#include "stdlib.h"

const uint IMITOVSTAVKA_CYCLES_CNT = 16;

uint f( uint A, uint K, const gost::replace_key& key )
{
    uint c = A ^ K;
    uint res = 0;
    for ( int i=0; i<8; ++i )
    {
        uint tmp = c >> 4*i;
        uint d = tmp & 0x0000000F;
        assert( d >= 0 );
        assert( d < 16 );
        uint out = key.table[i][d];
        res |= (out << 4*i);
    }
    res = _rotl( res, 11 );
    return res;
}

gost::block cycles( gost::block in, gost::replace_key& rkey, gost::key& key, uint cnt )
{
    gost::block res;
    res.A = in.A;
    res.B = in.B;
    for ( uint i=0; i<cnt; ++i)
    {
        uint k = i % 8;
        if ( i > 24 )
            k = 32 - i;
        uint tmpA = res.A;
        res.A = res.B ^ f( res.A, key.X[k], rkey );
        res.B = tmpA;
    }
    return res;
}

gost::block vstavka( uint* in, uint size, gost::replace_key& rkey, gost::key& key )
{
    gost::block curr;
    curr.A = 0;
    curr.B = 0;
    for ( int i=0; i<size/2; ++i )
    {
        curr.A ^= in[2*i];
        curr.B ^= in[2*i + 1];
        curr = cycles( curr, rkey, key, IMITOVSTAVKA_CYCLES_CNT );
    }
    if ( size % 2 != 0 ) {
        curr.A ^= in[size-1];
        curr = cycles( curr, rkey, key, IMITOVSTAVKA_CYCLES_CNT );
    }
    return curr;
}