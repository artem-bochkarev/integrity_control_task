#include <iostream>
#include <conio.h>
#include <boost/format.hpp>
#include "FileHash.h"
#include "Database.h"
#include <cassert>
#include <stdexcept>

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

const char* dbname = "test_database.sql";

int main()
{
    //uint A, B;
    uint K = 0xBAADF00D;
    gost::block f1 = count_hash( "T:\\data\\programming\\asm\\2.bmp", rkey, key );
    gost::block f2 = count_hash( "T:\\data\\programming\\asm\\1.bmp", rkey, key );
    __int64 a = f1.toInt64();
    __int64 b = f2.toInt64();
    std::cout << boost::format("f1 = %016x \nf2 = %016x") % a % b << std::endl; 

    try
    {
        //Database::createNewDatabase(dbname);
        //Database::fillTestDatabase( dbname, rkey, key );

        DatabasePtr database = Database::openDatabase(dbname);
        database->setKeys( rkey, key );
        std::cout << "File 2.bmp: " << Database::resultToStr( database->checkFile("T:\\data\\programming\\asm\\2.bmp") ) << std::endl;
        std::cout << "File 1.bmp: " << Database::resultToStr( database->checkFile("T:\\data\\programming\\asm\\1.bmp") ) << std::endl;

        //Database::fillDatabase( dbname, "T:\\Data\\", 3, rkey, key );


        std::cout << "-------ALL should be MATCH--------" << std::endl;
        std::cout << "File ConsoleApplication1.exe: " << Database::resultToStr( database->checkFile("T:\\Data\\Programming\\asm\\ConsoleApplication1.exe") ) << std::endl;
        std::cout << "File simpleCPP.exe: " << Database::resultToStr( database->checkFile("T:\\Data\\Programming\\asm\\simpleCPP.exe") ) << std::endl;
        std::cout << "File bmpChanger.exe: " << Database::resultToStr( database->checkFile("T:\\Data\\Programming\\asm\\bmpChanger.exe") ) << std::endl;
        std::cout << "File a+b.exe: " << Database::resultToStr( database->checkFile("T:\\Data\\Programming\\asm\\a+b.exe") ) << std::endl;
        std::cout << "File maxima-5.17.0.exe: " << Database::resultToStr( database->checkFile("T:\\Data\\Files\\maxima\\maxima-5.17.0.exe") ) << std::endl;
        std::cout << "File WinDjView-0.4.2.exe: " << Database::resultToStr( database->checkFile("T:\\Data\\Files\\Books\\WinDjView-0.4.2.exe") ) << std::endl;

        std::cout << "-------ALL should be NOT_EXIST--------" << std::endl;
        std::cout << "File maxima-5.17.0.exe: " << Database::resultToStr( database->checkFile("T:\\Dat\\Files\\maxima\\maxima-5.17.0.exe") ) << std::endl;
        std::cout << "File WinDjView-0.4.2.exe: " << Database::resultToStr( database->checkFile("Data\\Files\\Books\\WinDjView-0.4.2.exe") ) << std::endl;
        std::cout << "File maxima-5.17.0.exe: " << Database::resultToStr( database->checkFile("T:\\Data\\Files\\maxima\\maxima-5.1.0.exe") ) << std::endl;
    }catch(std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl;
        assert(0);
    }

    _getch();

}
