@echo off
del ft.txt 2>nul
for /f "skip=2 tokens=1,2,3,4,5" %%a in (cas.txt) do (
  if #%%e#==#0# (
    echo %%a,>>ft.txt
  )
)
sort ft.txt>ft1.txt
