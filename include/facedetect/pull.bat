@echo off
for /l %%a in (1,1,10) do (
  adb pull /storage/emulated/pic/pic_%%a.jpg .
)
