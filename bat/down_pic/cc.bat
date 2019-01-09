@echo off

del jpg.txt
for /f "tokens=1,2 delims=:" %%a in (list.txt) do (
 if not exist .\jpg\%%b echo %%a:%%b>>jpg.txt
)

md jpg
cd jpg
wget -i ../jpg.txt -x


