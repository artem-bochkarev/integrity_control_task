#include "createdatabase.h"
#include "createKey.h"
#include <QDir>
#include <QFileDialog>


const char* dbname = "database.sql";

gost::replace_key rkey = 
{
  0x4,0xA,0x9,0x2,0xD,0x8,0x0,0xE,0x6,0xB,0x1,0xC,0x7,0xF,0x5,0x3,
  0xE,0xB,0x4,0xC,0x6,0xD,0xE,0xA,0x2,0x3,0x8,0x1,0x0,0x7,0x5,0x9,
  0x5,0x8,0x1,0xD,0xA,0x3,0x4,0x2,0xE,0xF,0xC,0x7,0x6,0x0,0x9,0xB,
  0x7,0xD,0xA,0x1,0x0,0x8,0x9,0xF,0xE,0x4,0x6,0xC,0xB,0x2,0x5,0x3,
  0x6,0xC,0x7,0x1,0x5,0xF,0xC,0x8,0x4,0xA,0x9,0xE,0x0,0x3,0xB,0x2,
  0x4,0xB,0xA,0x0,0x7,0x2,0x1,0xD,0x3,0x6,0x8,0x5,0x9,0xC,0xF,0xE,
  0xD,0xB,0x4,0x1,0x3,0xF,0x5,0x9,0x0,0xA,0xE,0x7,0x6,0x8,0x2,0xC,
  0x1,0xF,0xD,0x0,0x5,0x7,0xA,0x4,0x9,0x2,0x3,0xE,0x6,0xB,0x8,0xC
};

gost::key key =
{
    0xBAADF00D,
    0xBEEFCACE,
    0xCAFED00D,
    0xDEADBABE,
    0xDEADBEEF,
    0xDEADC0DE,
    0xDEADF00D,
    0xFADEDEAD
};

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
