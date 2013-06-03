#pragma once
#include "Database.h"
#include "sqlite3.h"
#include "Filehash.h"
#include "Tools/errorMsg.hpp"

extern Tools::Logger * logger;

class DatabaseImpl : public Database
{
public:
    DatabaseImpl(const char* name);
    virtual void setKeys(const gost::replace_key&, const gost::key&);
    virtual ~DatabaseImpl();
    virtual Result checkFile( const char* name ) const;
    virtual void addFile( const char* name ) const;
    virtual void readKeys();

    static void runSimpleNoResultSQL( sqlite3 *db, const char* sqlstr, bool closeIfFail = false );
    static std::string insertFileStmt( const char* filename, const gost::replace_key&, const gost::key&, bool replaceIfExist = true );
    static void insertKey( sqlite3 *db, const char* bytes, int size, int id, bool replaceIfExist = true, bool closeIfFail = false );
    static void standardDbClose( sqlite3 *db, bool bThrowException = false );
    static sqlite3* standardDbOpen( const char* filename, bool bThrowException = false );

private:
    DatabaseImpl(const DatabaseImpl&);
    DatabaseImpl();
    DatabaseImpl& operator=(const DatabaseImpl&);

    sqlite3 *db;
    bool valid;
    gost::replace_key rkey;
    gost::key key;
};