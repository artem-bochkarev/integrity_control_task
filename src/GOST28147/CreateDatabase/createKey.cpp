#include "createKey.h"
#include <time.h>

gost::key key0 = {
        0xBAADF00D,
        0xBAADF00D,
        0xBAADF00D,
        0xBAADF00D,
        0xBAADF00D,
        0xBAADF00D,
        0xBAADF00D,
        0xBAADF00D
    };

gost::replace_key rkey0 = 
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

CreateKey::CreateKey(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);

    key = key0;
    rkey = rkey0;

    QObject::connect( ui.generateButton, SIGNAL(clicked()), this, SLOT(generatePressed()) );
    ui.lineEdit->setText("BAADF00DxBAADF00DxBAADF00DxBAADF00DxBAADF00DxBAADF00DxBAADF00DxBAADF00D");

    
    //ui
}

CreateKey::~CreateKey()
{

}

void CreateKey::generatePressed()
{
    QString str = "";
    srand( time(0) );
    for ( int i=0; i<8; ++i )
    {
        key.X[i] = rand();
        key.X[i] <<= 16;
        key.X[i] |= rand();
        str += intToHex( key.X[i] );
        if ( i < 7 )
            str += "x";
    }
    ui.lineEdit->setText(str);
}

QString CreateKey::intToHex( uint x ) const
{
    QString res = "";
    for ( int i=0; i<8; ++i )
    {
        uint k = x & 0x0000000F;
        QChar q;
        switch (k)
        {
        case 0:
            q = '0';
            break;
        case 1:
            q = '1';
            break;
        case 2:
            q = '2';
            break;
        case 3:
            q = '3';
            break;
        case 4:
            q = '4';
            break;
        case 5:
            q = '5';
            break;
        case 6:
            q = '6';
            break;
        case 7:
            q = '7';
            break;
        case 8:
            q = '8';
            break;
        case 9:
            q = '9';
            break;
        case 10:
            q = 'A';
            break;
        case 11:
            q = 'B';
            break;
        case 12:
            q = 'C';
            break;
        case 13:
            q = 'D';
            break;
        case 14:
            q = 'E';
            break;
        case 15:
            q = 'F';
            break;
        default:
            q = 'F';
        }
        res += q;
        x >>= 4;
    }
    return res;
}

gost::key CreateKey::getKey() const
{
    return key;
}

gost::replace_key CreateKey::getRKey() const
{
    return rkey;
}