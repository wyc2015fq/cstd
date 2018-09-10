@echo off

for %%i in ( imgcodecs ) do (
 (for /f %%a in ('dir /b %%i\src\*.cpp') do (
  echo #include "%%i\src\%%a"
 ))>lib%%i.cpp
)


