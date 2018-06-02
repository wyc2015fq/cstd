@echo off
set path=D:\soft\vc6sp6fix\VC98\Bin;%path%
for /f %%a in ('dir /b /s Debug\*.exe') do (
call FTIME.BAT %%~dpna
)
