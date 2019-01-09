@echo off
for %%a in (%CD%) do (
  wget -m -A html -A htm -A shtml http://%%~nxa
)

