@echo off

md jpg
cd jpg

for /f "tokens=1,2 delims=:" %%a in (..\list.txt) do (
 if not exist .%%b wget "%%a:%%b" -x
)


