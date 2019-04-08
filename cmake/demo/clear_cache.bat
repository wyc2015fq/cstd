@echo off
for /f %%a in ('dir /ad /s /b build') do (
 echo %%a
 rmdir /q /s "%%a"
)
