#pragma once
#include <boost/shared_ptr.hpp>
#include "GOST28147/gost28147.h"

class Database;
typedef boost::shared_ptr<Database> DatabasePtr;

class Database
{
public:
    enum Result
    {
        MATCH,
        NOT_MATCH,
        NOT_EXIST
    };
    virtual ~Database() {};
    static void createNewDatabase( const char* name );
    static void fillDatabase( const char* name );
    static void fillTestDatabase( const char* name, const gost::replace_key& rkey, const gost::key& key );
    
    static DatabasePtr  openDatabase( const char* name );
    static const char* resultToStr( Result );

    virtual Result checkFile( const char* name ) const = 0;
    virtual void setKeys(const gost::replace_key&, const gost::key&) = 0;
    
};