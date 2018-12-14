@echo off

:: https://github.com/SpaceVim/SpaceVim
for /f "tokens=1,2,3,4 delims=/" %%a in ('pclip') do (
 echo %%a//%%b/%%c/%%d
 md D:\code\git\%%c
pushd D:\code\git\%%c
 git clone "%%a//%%b/%%c/%%d"
popd
)
