@echo off
for /f "delims=*" %%a in ('echo %CD%') do (
"C:\Program Files\WinRAR\Rar.exe" a "%%~na.rar" -r  *.h *.c *.cpp *.hpp *.inl *.asm *.txt *.dsp makefile *.js *.bat *.m *.doc *.pdf *.rc *.sh *.xdc *.xml *.cfg *.ini *.dat
)