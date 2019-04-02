@echo off


:: set file=../doc/CPU与内存的那些事.md
set root=../doc
set root=./
for %%j in (%root%/*.md) do (
if not exist "%%~nj---" (
cat "%root%/%%j"|sed "s/!\[/\n!\[/g;s/)/)\n/g"|findstr "!\[.*)"|sed "s#//#/#g">list.txt
cat "%root%/%%j">_aaa.txt
 md "%%~nj"
 pushd "%%~nj"
for /f "tokens=2,3 delims=(:)" %%a in (../list.txt) do (
 echo %%a:/%%b
 wget -c -x "%%a:/%%b"
 cat ..\_aaa.txt|sed "s#%%a:/%%b#.%%b#g">..\_bbb.txt
 move /y ..\_bbb.txt ..\_aaa.txt
)
move ..\_aaa.txt readme.md
 popd
 )
)

:end
