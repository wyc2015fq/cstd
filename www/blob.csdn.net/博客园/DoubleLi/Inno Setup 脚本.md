# Inno Setup 脚本 - DoubleLi - 博客园






给你个我用的例子：



Delphi/Pascal code


[?](https://bbs.csdn.net/topics/290002551#clipboardWindow)

```
`; 脚本用 Inno Setup 脚本向导生成。`

`; 查阅文档获取创建 INNO SETUP 脚本文件详细资料!`



`[Setup]`

`AppName=Common downloader`

`AppVerName=Common downloader`

`DefaultDirName=``{pf}``\XXX\Common downloader`

`DefaultGroupName=XXX\Common downloader`

`DisableProgramGroupPage=yes`

`OutputDir= .\Setup`

`OutputBaseFilename=Common downloader setup V1``.0.0`

`Compression=lzma`

`SolidCompression=yes`

`VersionInfoCompany=XXX`

`VersionInfoVersion=``1.0.0`



`[Languages]`

`Name: "Chinese"; MessagesFile: "compiler:Languages\Chinese``.``isl"`

`Name: "English"; MessagesFile: "compiler:Languages\English``.``isl"`



`[Tasks]`

`Name: "desktopicon"; Description: "``{cm:CreateDesktopIcon}``"; GroupDescription: "``{cm:AdditionalIcons}``"; Flags: dontinheritcheck`



`[Files]`

`Source: "CMDL``.``exe"; DestName: "Common downloader``.``exe"; DestDir: "``{app}``"; Flags: ignoreversion`

`Source: "DL``.``dll"; DestDir: "``{app}``"; Flags: ignoreversion`

`Source: "DM``.``dll"; DestDir: "``{app}``"; Flags: ignoreversion`

`Source: "config``.``ini"; DestDir: "``{app}``"; Flags: ignoreversion`

`Source: "App\*"; DestDir: "``{app}``\\App"; Flags: ignoreversion recursesubdirs createallsubdirs`

`Source: "Boot\*"; DestDir: "``{app}``\\Boot"; Flags: ignoreversion recursesubdirs createallsubdirs`

`Source: "Resource_cn\*"; DestDir: "``{app}``\Resource"; Languages: Chinese; Flags: ignoreversion recursesubdirs createallsubdirs`

`Source: "Resource_en\*"; DestDir: "``{app}``\Resource"; Languages: English; Flags: ignoreversion recursesubdirs createallsubdirs`

`; 注意: 不要在任何共享系统文件中使用“Flags: ignoreversion”`



`[Icons]`

`Name: "``{group}``\Common downloader"; Filename: "``{app}``\Common downloader``.``exe"`

`Name: "``{group}``\``{cm:UninstallProgram,Common downloader}``"; Filename: "``{uninstallexe}``"`

`Name: "``{userdesktop}``\Common downloader"; Filename: "``{app}``\Common downloader``.``exe"; Tasks: desktopicon`



`[Run]`

`Filename: "``{app}``\Common downloader``.``exe"; Description: "``{cm:LaunchProgram,Common downloader}``"; Flags: nowait postinstall skipifsilent`



`[Registry]`

`Root: HKLM; Subkey: "Software\XXX"; Flags: createvalueifdoesntexist`

`Root: HKLM; Subkey: "Software\XXX\Common downloader"; Flags: uninsdeletekeyifempty`

`Root: HKLM; Subkey: "Software\XXX\Common downloader"; ValueType: ``string``; ValueName: "Path"; ValueData: "``{app}``"`

`Root: HKLM; Subkey: "Software\XXX\Common downloader"; ValueType: ``string``; ValueName: "Version"; ValueData: "V1``.0.0``"`

`Root: HKLM; Subkey: "Software\XXX\Common downloader"; ValueType: dword; ValueName: "VersionNum"; ValueData: "``100``"`

`Root: HKLM; Subkey: "Software\XXX\Common downloader"; ValueType: ``string``; ValueName: "Release Date"; ValueData: "``2008``/``09``/``29``"`



`[Code]`

`function``InitializeSetup(): ``Boolean``;`

`var`

`   ``Version: ``string``;`

`   ``strPath: ``string``;`

`   ``ResultCode: ``Integer``;`

`begin`

`   ``Result:= ``TRUE``;`

`  ``//read the version`

`  ``if``RegQueryStringValue(HKEY_LOCAL_MACHINE, ``'Software\XXX\Common downloader'``,`

`     ``'Version'``, Version) ``then`

`  ``begin`

`    ``// Successfully read the value`

`    ``Version := Format(``'Exist the version %s,whether uninstall it?'``,[Version]);`

`    ``if``MsgBox(Version,mbConfirmation,MB_YESNO) = IDYES ``then`

`    ``begin`

`     ``//read the path`

`     ``if``RegQueryStringValue(HKEY_LOCAL_MACHINE, ``'Software\XXX\Common downloader'``,`

`     ``'Path'``, strPath) ``then`

`        ``begin`

`         ``// Successfully read the value`

`         ``strPath:= strPath + ``'\unins000.exe'``;`

`         ``Exec(ExpandConstant(strPath), ``''``, ``''``, SW_SHOW,`

`              ``ewWaitUntilTerminated, ResultCode);`

`        ``end``;`

`    ``end``else`

`    ``Result:= ``FALSE``;`

`  ``end``;`

`end``;`
```




这个例子可以在安装之前卸载以前的版本，并提示你卸载之前的版本信息。









