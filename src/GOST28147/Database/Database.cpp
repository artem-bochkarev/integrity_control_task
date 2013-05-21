#include "DatabaseImpl.h"
#include "Tools/errorMsg.hpp"
#include <map>
#include <string>
#include <vector>
#include <sstream>

std::map<std::string, DatabasePtr> databases;

const char* Database::resultToStr( Result res )
{
    switch (res)
    {
    case MATCH:
        return "MATCH";
    case NOT_MATCH:
        return "NOT_MATCH";
    default:
        return "NOT_EXIST";
    }
}


void Database::createNewDatabase( const char* name )
{
    sqlite3 *db = DatabaseImpl::standardDbOpen( name, true );

    std::string sql = "CREATE TABLE IF NOT EXISTS files ( \
                      filename VARCHAR(2048) PRIMARY KEY, \
                      hash INT8 \
                      );";
   
    DatabaseImpl::runSimpleNoResultSQL( db, sql.c_str() );
    DatabaseImpl::standardDbClose( db, true );
}

void Database::fillTestDatabase(const char* name, const gost::replace_key& rkey, const gost::key& key )
{
    sqlite3 *db = DatabaseImpl::standardDbOpen( name, true );

    std::vector<std::string> sqlStmts;
    sqlStmts.push_back( DatabaseImpl::insertFileStmt( "T:\\data\\programming\\asm\\1.bmp", rkey, key ) );
    sqlStmts.push_back( DatabaseImpl::insertFileStmt( "T:\\data\\programming\\asm\\2.bmp", rkey, key ) );

    for ( size_t i=0; i<sqlStmts.size(); ++i )
    {
        std::string sql = sqlStmts[i];
        DatabaseImpl::runSimpleNoResultSQL( db, sql.c_str() );
    }
    DatabaseImpl::standardDbClose( db, true );
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