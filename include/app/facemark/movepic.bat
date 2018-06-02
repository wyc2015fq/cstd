@echo off
md out

for %%a in (0 5 15 25 35 45 55 65 75) do (
  md out\region_%%a
  md out\region_%%a\F
  md out\region_%%a\M
  md out\region_%%a\_
)

for /f "tokens=1,2,3" %%a in (facemarkout.txt) do (
 echo %%a %%b %%c
 copy %%a out\region_%%c\%%b
)