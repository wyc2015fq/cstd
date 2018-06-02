@echo off

for /f %%a in ('dir /b F:\public\bin\*.h') do (
  if exist "include\%%~nxa" copy /y "include\%%~nxa" F:\public\bin\>nul
)

astyle -b --style=kr -s2 --convert-tabs *.h 2>nul
astyle -b --style=kr -s2 --convert-tabs *.c 2>nul
astyle -b --style=kr -s2 --convert-tabs *.hpp 2>nul
astyle -b --style=kr -s2 --convert-tabs *.cpp 2>nul

if exist makefile (
  nmake /f makefile 2>nul
)

if exist include (
  pushd include
  astyle -b --style=kr -s2 --convert-tabs *.h 2>nul
  popd
)

if exist src (
pushd src
for /f %%a in ('dir /b /ad') do (
  pushd %%a
  astyle -b --style=kr -s2 --convert-tabs *.h 2>nul
  astyle -b --style=kr -s2 --convert-tabs *.c 2>nul
  astyle -b --style=kr -s2 --convert-tabs *.hpp 2>nul
  astyle -b --style=kr -s2 --convert-tabs *.cpp 2>nul
  if exist "makefile" (
    echo ===================== %%a =====================
    nmake /f makefile 2>nul
    if 1==%errorlevel% echo ´íÀ²! ´íÀ²!
    if 0==%errorlevel% echo done.
  )
  popd
)
popd
)

(del /s *.orig>nul) 2>nul
echo all done.
