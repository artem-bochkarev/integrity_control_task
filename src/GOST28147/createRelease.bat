mkdir Product
copy /B "Release\SimpleRunner.exe" Product\
copy /B "Release\CreateDatabase.exe" Product\
copy %QTDIR32%\bin\QtGui4.dll Product\
copy %QTDIR32%\bin\QtCore4.dll Product\
copy sqlite3.dll Product\