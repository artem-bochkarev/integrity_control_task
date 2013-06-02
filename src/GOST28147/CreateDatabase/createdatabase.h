#ifndef CREATEDATABASE_H
#define CREATEDATABASE_H

#include <QtGui/QMainWindow>
#include "ui_createdatabase.h"
#include "Database/Database.h"

class CreateDatabase : public QMainWindow
{
    Q_OBJECT

public:
    CreateDatabase(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~CreateDatabase();

private slots:
    void createPressed();
    void addPressed();

private:
    Ui::CreateDatabaseClass ui;

    gost::replace_key rkey;
    gost::key key;

    DatabasePtr database;

    void changeSlashes( QString& ) const;
};

#endif // CREATEDATABASE_H
