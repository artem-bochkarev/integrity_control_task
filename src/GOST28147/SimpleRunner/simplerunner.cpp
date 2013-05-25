#include "simplerunner.h"
#include <QFileDialog>
#include <QMessageBox>
#include <stdlib.h>

const char* dbname = "test_database.sql";

gost::replace_key rkey = 
{
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF,
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF
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

SimpleRunner::SimpleRunner(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);
    QObject::connect( ui.runButton, SIGNAL(clicked()), this, SLOT(runPressed()) );
    QObject::connect( ui.selectButton, SIGNAL(clicked()), this, SLOT(selectPressed()) );

    database = Database::openDatabase(dbname);
    database->setKeys( rkey, key );
}

SimpleRunner::~SimpleRunner()
{

}

void SimpleRunner::selectPressed()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open file to run"), "", tr("exe file") +  "(*.exe)");
    if ( fileName != "" )
    {
        QFileInfo fInfo(fileName);
        ui.lineEdit->setText( fileName );
    }
}

void SimpleRunner::changeSlashes( QString& file ) const
{
    for ( int i=0; i<file.length(); ++i )
    {
        if ( file[i] == QChar('/') )
            file[i] = QChar('\\');
    }
}

void SimpleRunner::runPressed()
{
    QString fileName = ui.lineEdit->text();
    if ( fileName != "" )
    {
        QFileInfo fInfo(fileName);
        if ( (fInfo.suffix() == "exe") && (fInfo.exists() == true ) )
        {
            //run
            changeSlashes( fileName );
            Database::Result result = database->checkFile( fileName.toAscii().constData() );
            if ( result == Database::MATCH )
            {
                //run
                system( fileName.toAscii().constData() );
            }else if ( result == Database::NOT_EXIST )
            {
                //ask to add
                QMessageBox msgBox(QMessageBox::Warning, tr("SimpleRunner"), tr("This file doesn't exist in database. \nDo you want to add it?") );
                QPushButton *addButton = msgBox.addButton(tr("Add"), QMessageBox::ActionRole);
                QPushButton *addrunButton = msgBox.addButton(tr("Add and run"), QMessageBox::ActionRole);
                QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);

                msgBox.exec();

                if (msgBox.clickedButton() == addButton) {
                    database->addFile( fileName.toAscii().constData() );
                } else if (msgBox.clickedButton() == addrunButton) {
                    database->addFile( fileName.toAscii().constData() );
                    system( fileName.toAscii().constData() );
                }
            }else
            {
                QMessageBox msgBox(QMessageBox::Critical, tr("SimpleRunner"), tr("This file was modified!\nDo you want to replace hash in database"));
                QPushButton *addButton = msgBox.addButton(tr("Replace"), QMessageBox::ActionRole);
                QPushButton *addrunButton = msgBox.addButton(tr("Replace and run"), QMessageBox::ActionRole);
                QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);

                msgBox.exec();

                if (msgBox.clickedButton() == addButton) {
                    database->addFile( fileName.toAscii().constData() );
                } else if (msgBox.clickedButton() == addrunButton) {
                    database->addFile( fileName.toAscii().constData() );
                    system( fileName.toAscii().constData() );
                }
            }
        }
        else
        {
            //err. message
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("SimpleRunner"));
            if ( fInfo.exists() == true )
                msgBox.setText(tr("It isn't an .exe file") );
            else
                msgBox.setText(tr("File doesn't exist") );
            msgBox.exec();
        }
    }
}