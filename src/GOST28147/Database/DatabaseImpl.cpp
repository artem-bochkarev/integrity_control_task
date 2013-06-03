#include "DatabaseImpl.h"
#include <sstream>

Tools::Logger* logger = 0;
const int hashColumn = 1;
const int keyColumn = 1;
const int rkeyID = 1;
const int keyID = 2;

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

void checkPrepare( sqlite3 * db, int rc, bool closeIfFail )
{
    if( rc != SQLITE_OK )
    {
        std::string errmsg(sqlite3_errmsg(db));
        if ( closeIfFail )
            sqlite3_close(db);
        Tools::throwDetailedFailed( "Can't prepare statement", errmsg.c_str(), logger );
    }
}

void checkStepNoRes( sqlite3 * db, int rc, bool closeIfFail )
{
    if( rc != SQLITE_DONE )
    {
        std::string errmsg(sqlite3_errmsg(db));
        if ( closeIfFail )
            sqlite3_close(db);
        Tools::throwDetailedFailed( "Can't step statement", errmsg.c_str(), logger );
    }
}

void checkStepRes( sqlite3 * db, int rc, bool closeIfFail )
{
    if( rc != SQLITE_ROW )
    {
        if ( rc != SQLITE_DONE )
            Tools::throwDetailedFailed( "Can't evaluate statement", sqlite3_errmsg(db), logger );
        assert(0);
        //return Database::NOT_EXIST;
    }
}

Database::Result DatabaseImpl::checkFile( const char* name ) const
{
    sqlite3_stmt *ppStmt;
    std::string sql = "SELECT * FROM files WHERE filename = \'"; // ToDo
    sql += name;
    sql += "\';";
    //const char *pzTail;
    int rc = sqlite3_prepare_v2( db, sql.c_str(), -1, &ppStmt, 0 );
    checkPrepare( db, rc, false );

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

void DatabaseImpl::addFile( const char* name ) const
{
    std::string sql = DatabaseImpl::insertFileStmt( name, rkey, key );
    DatabaseImpl::runSimpleNoResultSQL( db, sql.c_str() );
}   

void DatabaseImpl::setKeys(const gost::replace_key& r , const gost::key& k)
{
    rkey = r;
    key = k;
}

void DatabaseImpl::readKeys()
{
    sqlite3_stmt *ppStmt;
    std::string sql = "SELECT * FROM keys WHERE id = ?1"; // ToDo
    int rc = sqlite3_prepare_v2( db, sql.c_str(), -1, &ppStmt, 0 );
    checkPrepare( db, rc, false );
    sqlite3_bind_int( ppStmt, keyColumn, rkeyID );

    rc = sqlite3_step( ppStmt );
    checkStepRes( db, rc, false );
    
    int len = sqlite3_column_bytes( ppStmt, keyColumn );
    assert( len == sizeof(gost::replace_key) );
    memcpy( rkey.table, sqlite3_column_blob( ppStmt, keyColumn ), len );

    sqlite3_reset( ppStmt );
    sqlite3_clear_bindings( ppStmt );
    sqlite3_bind_int( ppStmt, keyColumn, keyID );

    rc = sqlite3_step( ppStmt );
    checkStepRes( db, rc, false );
    
    len = sqlite3_column_bytes( ppStmt, keyColumn );
    assert( len == sizeof(gost::key) );
    memcpy( key.X, sqlite3_column_blob( ppStmt, keyColumn ), len );

    sqlite3_finalize( ppStmt );
}

void DatabaseImpl::runSimpleNoResultSQL( sqlite3 *db, const char* sqlstr, bool closeIfFail )
{
    sqlite3_stmt *ppStmt;
    int rc = sqlite3_prepare_v2( db, sqlstr, -1, &ppStmt, 0 );
    checkPrepare( db, rc, closeIfFail );

    rc = sqlite3_step( ppStmt );
    checkStepNoRes( db, rc, closeIfFail );
    sqlite3_finalize( ppStmt );
}

std::string DatabaseImpl::insertFileStmt( const char* filename, const gost::replace_key& rkey, const gost::key& key, bool replaceIfExist )
{
    gost::block f = count_hash( filename, rkey, key );
    __int64 a = f.toInt64();

    std::stringstream value;
    value << a;
    std::string sql;
    if (replaceIfExist == true)
        sql = "INSERT OR REPLACE INTO files values( \'";
    else
        sql = "INSERT INTO files values( \'";

    sql += filename;
    sql += "\', ";
    sql += value.str();
    sql += " );";
    return sql;
}

void DatabaseImpl::insertKey( sqlite3 *db, const char* bytes, int size, int id, bool replaceIfExist, bool closeIfFail )
{
    std::string sql;
    if (replaceIfExist == true)
        sql = "INSERT OR REPLACE INTO keys values( ?1, ?2 );\'";
    else
        sql = "INSERT INTO keys values( ?1, ?2 );\'";

    sqlite3_stmt *ppStmt;
    int rc = sqlite3_prepare_v2( db, sql.c_str(), -1, &ppStmt, 0 );
    checkPrepare( db, rc, closeIfFail );

    sqlite3_bind_int( ppStmt, 1, id );
    sqlite3_bind_blob( ppStmt, 2, bytes, size, SQLITE_STATIC );

    rc = sqlite3_step( ppStmt );
    checkStepNoRes( db, rc, closeIfFail );

    sqlite3_finalize( ppStmt );
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
        {
            Tools::throwDetailedFailed( "Can't open database", errmsg.c_str(), logger );
        }
        else
        {
            assert(0);
            return 0;
        }
    }
    return db;
}