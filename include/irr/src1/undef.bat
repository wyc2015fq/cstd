@echo off
@(for /f  "tokens=2 delims=( " %%a in ('findstr "#define" aesgladman\*.*') do @(@echo #undef %%a ))|sort|uniq
