@echo off



for /f %%a in ('dir /ad /b') do (
 if exist "%%a/build.bat" (
  pushd "%%a"
  call build.bat
  popd
 )
)
