#include "createdatabase.h"
#include "createKey.h"
#include <QDir>
#include <QFileDialog>


const char* dbname = "database.sql";

CreateDatabase::CreateDatabase(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    QFileInfoList drives = QDir::drives();
    QFileInfo tmp;
    QString str = "";
    foreach( tmp, drives )
    {
        str += tmp.absolutePath() + ";";
    }
    ui.lineEdit->setText( str );

    QObject::connect( ui.createButton, SIGNAL(clicked()), this, SLOT(createPressed()) );
    QObject::connect( ui.addDirectory, SIGNAL(clicked()), this, SLOT(addPressed()) );
    CreateKey dialog( this );
    dialog.exec();

    key = dialog.getKey();
    rkey = dialog.getRKey();

}

CreateDatabase::~CreateDatabase()
{

}

void CreateDatabase::createPressed( )
{
    Database::createNewDatabase( dbname );
    QStringList list = ui.lineEdit->text().split(";",  QString::SkipEmptyParts);
    QString dir;
    foreach( dir, list )
    {
        Database::fillDatabase( dbname, dir.toAscii().constData(), -1, rkey, key );
    }
    Database::putKeysToDatabase( dbname, rkey, key );
    QApplication::quit();
}

void CreateDatabase::addPressed( )
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                        "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString res = ui.lineEdit->text();
    res += dir + ";";
    ui.lineEdit->setText(res);
}

void CreateDatabase::changeSlashes( QString& file ) const
{
    for ( int i=0; i<file.length(); ++i )
    {
        if ( file[i] == QChar('/') )
            file[i] = QChar('\\');
    }
}
