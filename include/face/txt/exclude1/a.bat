@echo off
del c.txt
sort a.txt > b.txt
for /f "tokens=1,2,3,4,5" %%a in (b.txt) do (
  if #0==#%%e echo %%a,>>c.txt
)
