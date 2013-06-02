[Setup]
AppName=Integrity Control
AppVersion=0.1
DefaultDirName={pf}\IntegrityControl

[Files]
Source: "SimpleRunner.exe"; DestDir: "{app}"
Source: "CreateDatabase.exe"; DestDir: "{app}"
;Source: "Database.dll"; DestDir: "{app}"
;Source: "GOST28147.dll"; DestDir: "{app}"
Source: "sqlite3.dll"; DestDir: "{app}"

[Run]
Filename: "{app}\CreateDatabase.exe"

[UninstallDelete]
Type: filesandordirs; Name: "{app}\Database.sql"
