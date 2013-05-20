#pragma once
#include "Database.h"
#include "sqlite3.h"
#include "Filehash.h"

class DatabaseImpl : public Database
{
public:
    DatabaseImpl(const char* name);
    virtual ~DatabaseImpl();
    virtual Result checkFile( const char* name ) const;

private:
    DatabaseImpl(const DatabaseImpl&);
    DatabaseImpl();
    DatabaseImpl& operator=(const DatabaseImpl&);

    sqlite3 *db;
    bool valid;
    gost::replace_key rkey;
    gost::key key;
};