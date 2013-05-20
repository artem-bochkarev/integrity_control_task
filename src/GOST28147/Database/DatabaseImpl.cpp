#include "DatabaseImpl.h"
#include "Tools/errorMsg.hpp"

Tools::Logger * logger = 0;
int hashColumn = 1;

DatabaseImpl::DatabaseImpl(const char *name)
:valid(false)
{
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_open(name, &db);
    if( rc != SQLITE_OK )
    {
        Tools::throwDetailedFailed( "Can't open database", sqlite3_errmsg(db), logger );
        sqlite3_close(db);
    }else
    {
        valid = true;
    }
}

DatabaseImpl::~DatabaseImpl()
{
    sqlite3_close(db);
}

Database::Result DatabaseImpl::checkFile( const char* name ) const
{
    sqlite3_stmt *ppStmt;
    std::string sql; // ToDo
    const char *pzTail;
    int rc = sqlite3_prepare_v2( db, sql.c_str(), 0, &ppStmt, &pzTail );

    if( rc != SQLITE_OK )
    {
        Tools::throwDetailedFailed( "Can't prepare statement", sqlite3_errmsg(db), logger );
    }

    rc = sqlite3_step( ppStmt );
    if( rc != SQLITE_ROW )
    {
        if ( rc != SQLITE_DONE )
            Tools::throwDetailedFailed( "Can't evaluate statement", sqlite3_errmsg(db), logger );
        return Database::NOT_EXIST;
    }
    
    sqlite3_int64 hash_db = sqlite3_column_int64( ppStmt, hashColumn );
    gost::block hash_file = count_hash( name, rkey, key );
    if ( hash_db == hash_file.toInt64() )
        return Database::MATCH;
    else
        return Database::NOT_MATCH;
}   