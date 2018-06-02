@echo off
del *.jpg
del libGMM3DTrack.rar
for /f %%a in (updata.txt) do (
  copy /y %%a .\ >nul
  rar a libGMM3DTrack.rar %%~nxa -idq
  echo %%a
)
