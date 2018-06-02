@echo off
::set path="C:\Program Files\WinRAR\";%path%

Rar.exe a "cstd_%date:/=_%.rar" -r *.m *.java *.reg *.cs *.h *.c *.cpp *.hpp *.inl *.asm *.txt *.dsp makefile *.mak *.mk *.js *.bat *.m *.doc *.pdf *.rc *.sh *.xdc *.xml *.cfg *.ini
