@echo off
echo commit msg:
set /p msg=
git add -A
git commit -m "%msg%"
git push
