@echo off
::set path="C:\Program Files\WinRAR\";%path%
cd C:\code\cstd\include
for %%a in (%CD%) do (
  Rar.exe a "E:\code\cstd\include_%date:/=_%.rar" -r *.py *.lua *.m *.java *.reg *.cs *.h *.c *.cpp *.hpp *.inl *.asm *.txt *.dsp makefile *.mak *.mk *.js *.bat *.m *.doc *.pdf *.rc *.sh *.xdc *.xml *.cfg *.ini *.md
)

