@echo off
for %%i in (test_bgfg2.dsp) do (
  md %%~ni
for /f "tokens=2 delims==" %%a in ('finds "SOURCE=" %%i') do (
  copy %%a .\%%~ni\
)
)