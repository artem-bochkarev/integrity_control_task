#include "DatabaseImpl.h"
#include "Tools/errorMsg.hpp"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <boost/filesystem.hpp>

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

void scanFolder( sqlite3 *db, const boost::filesystem::path& p, int depth, const gost::replace_key& rkey, const gost::key& key )
{
    using namespace boost::filesystem;
    try
    {
        if (exists(p))
        {
            if ( (is_regular_file(p)) && (p.extension().string()==".exe") )        // is p a .exe file?
            {
                DatabaseImpl::runSimpleNoResultSQL( db, DatabaseImpl::insertFileStmt( absolute(p).string().c_str(), rkey, key ).c_str() );
                std::cout << DatabaseImpl::insertFileStmt( absolute(p).string().c_str(), rkey, key ) << std::endl;
            }
            else if ( (is_directory(p)) && (depth != 0))      // is p a directory?
            {
                directory_iterator iter = directory_iterator(p);
                for ( ;iter != directory_iterator(); ++iter )
                        scanFolder( db, iter->path(), depth-1, rkey, key );
            }
        }
    }
    catch (const filesystem_error& ex)
    {
        //Tools::throwDetailedFailed( "Filesystem error", ex.what(), logger );
    }
}

void Database::fillDatabase( const char* name, const char* startFolder, int depth, const gost::replace_key& rkey, const gost::key& key )
{
    sqlite3 *db = DatabaseImpl::standardDbOpen( name, true );
    using namespace boost::filesystem;
    path start(startFolder);
    scanFolder( db, start, depth, rkey, key);
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