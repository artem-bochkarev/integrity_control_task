#include "Filehash.h"
#include <fstream>
#include <memory>


gost::block count_hash( const char* filename, const gost::replace_key& rkey, const gost::key& key )
{
    std::ifstream in( filename, std::ios::in | std::ios::binary | std::ios::ate );
    //in.open();
    if ( in.is_open() )
    {
        std::ifstream::pos_type size, m_size;
        m_size = size = in.tellg();
        if ( size%8 > 0 )
            m_size += (8 - (size%8));

        std::auto_ptr<char> memblock(new char [m_size]);
        in.seekg (0, std::ios::beg);
        in.read (memblock.get(), size);
        in.close();
        
        return gost::vstavka( reinterpret_cast<uint*>(memblock.get()), m_size/8, rkey, key );
    }
    return gost::block();
}