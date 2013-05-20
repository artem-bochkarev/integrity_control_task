#include "DatabaseImpl.h"
#include <map>
#include <string>

std::map<std::string, DatabasePtr> databases;


void Database::createNewDatabase( const char* name )
{
}

DatabasePtr  Database::openDatabase( const char* name )
{
    std::string fname(name);
    DatabasePtr ptr;
    if ( databases.find(fname) != databases.end() )
    {
        ptr = databases[fname];
    }else
    {
        ptr = DatabasePtr(new DatabaseImpl(name));
        databases[fname] = ptr;
    }
    return ptr;
}