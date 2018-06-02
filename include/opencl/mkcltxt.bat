@echo off
del ocl.txt
for %%i in (opencl/*.cl) do (
  echo char* %%~ni_progstr = >>ocl.txt
  cat opencl/%%i|sed "s#//.*$##g"|more /s|sed "/^[ \t]*$/d"|sed "s/^/""/g"|sed "s/$/""/g"|more /s>>ocl.txt
  echo ;>>ocl.txt
  echo.>>ocl.txt
  echo.>>ocl.txt
)
