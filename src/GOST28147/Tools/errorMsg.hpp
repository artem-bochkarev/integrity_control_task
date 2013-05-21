#pragma once
#include <stdexcept>
#include <string>
#include "Tools/Logger.h"
#include <windows.h>
#include <cassert>

namespace Tools
{
    inline void throwFailed( const std::string& msg, Tools::Logger* logger = 0 )
    {
        OutputDebugStringA( msg.c_str() );
        OutputDebugStringA( "\n" );
        if ( logger != 0 )
            *logger << "[FAILED] " << msg << "\n";
        throw std::runtime_error( msg );
    }

    inline void throwDetailedFailed( const char* msg, const char* descr, Tools::Logger* logger = 0 )
    {
        OutputDebugStringA( msg );
        OutputDebugStringA( ":" );
        OutputDebugStringA( descr );
        OutputDebugStringA( "\n" );
        if ( logger != 0 )
        {
            *logger << "[FAILED] " << msg << "\n";
            *logger << "[DESCRIPTION] " << descr << "\n";
        }
        assert(0);
        throw std::runtime_error( msg );
    }

    inline void throwDetailedFailed( const std::string& msg, const char* descr, Tools::Logger* logger = 0 )
    {
        throwDetailedFailed( msg.c_str(), descr, logger );
    }


}