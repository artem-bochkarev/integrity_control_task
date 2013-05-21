#include "DatabaseImpl.h"
#include <sstream>

Tools::Logger* logger = 0;
int hashColumn = 1;

DatabaseImpl::DatabaseImpl(const char *name)
:valid(false)
{
    db = DatabaseImpl::standardDbOpen( name, true );
    valid = true;
}

DatabaseImpl::~DatabaseImpl()
{
    DatabaseImpl::standardDbClose( db );
}

Database::Result DatabaseImpl::checkFile( const char* name ) const
{
    sqlite3_stmt *ppStmt;
    std::string sql = "SELECT * FROM files WHERE filename = \'"; // ToDo
    sql += name;
    sql += "\';";
    //const char *pzTail;
    int rc = sqlite3_prepare_v2( db, sql.c_str(), -1, &ppStmt, 0 );

    if( rc != SQLITE_OK )
    {
        Tools::throwDetailedFailed( "Can't prepare statement", sqlite3_errmsg(db), logger );
    }

    rc = sqlite3_step( ppStmt );
    if( rc != SQLITE_ROW )
    {
        sqlite3_finalize( ppStmt );
        if ( rc != SQLITE_DONE )
            Tools::throwDetailedFailed( "Can't evaluate statement", sqlite3_errmsg(db), logger );
        return Database::NOT_EXIST;
    }
    
    sqlite3_int64 hash_db = sqlite3_column_int64( ppStmt, hashColumn );
    sqlite3_finalize( ppStmt );
    gost::block hash_file = count_hash( name, rkey, key );
    if ( hash_db == hash_file.toInt64() )
        return Database::MATCH;
    else
        return Database::NOT_MATCH;
}   

void DatabaseImpl::setKeys(const gost::replace_key& r , const gost::key& k)
{
    rkey = r;
    key = k;
}

void DatabaseImpl::runSimpleNoResultSQL( sqlite3 *db, const char* sqlstr, bool closeIfFail )
{
    sqlite3_stmt *ppStmt;
    int rc = sqlite3_prepare_v2( db, sqlstr, -1, &ppStmt, 0 );

    if( rc != SQLITE_OK )
    {
        std::string errmsg(sqlite3_errmsg(db));
        if ( closeIfFail )
            sqlite3_close(db);
        Tools::throwDetailedFailed( "Can't prepare statement", errmsg.c_str(), logger );
    }

    rc = sqlite3_step( ppStmt );
    if( rc != SQLITE_DONE )
    {
        std::string errmsg(sqlite3_errmsg(db));
        if ( closeIfFail )
            sqlite3_close(db);
        Tools::throwDetailedFailed( "Can't step statement", errmsg.c_str(), logger );
    }
    sqlite3_finalize( ppStmt );
}

std::string DatabaseImpl::insertFileStmt( const char* filename, const gost::replace_key& rkey, const gost::key& key )
{
    gost::block f = count_hash( filename, rkey, key );
    __int64 a = f.toInt64();

    std::stringstream value;
    value << a;
    std::string sql = "INSERT OR REPLACE INTO files values( \'";
    sql += filename;
    sql += "\', ";
    sql += value.str();
    sql += " );";
    return sql;
}

void DatabaseImpl::standardDbClose( sqlite3 *db, bool bThrowException )
{
    int rc = sqlite3_close(db);
    if( rc != SQLITE_OK )
    {
        std::string errmsg(sqlite3_errmsg(db));
        if ( bThrowException )
            Tools::throwDetailedFailed( "Can't prepare statement", errmsg.c_str(), logger );
        else
            assert(0);
    }
}

sqlite3* DatabaseImpl::standardDbOpen( const char* filename, bool bThrowException )
{
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(filename, &db);
    if( rc != SQLITE_OK )
    {
        std::string errmsg(sqlite3_errmsg(db));
        sqlite3_close(db);
        if ( bThrowException )
            Tools::throwDetailedFailed( "Can't open database", errmsg.c_str(), logger );
        else
            return 0;
    }
    return db;
}