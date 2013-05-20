#pragma once
#include <boost/shared_ptr.hpp>

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
    static DatabasePtr  openDatabase( const char* name );
    virtual Result checkFile( const char* name ) const = 0;
};