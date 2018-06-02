@echo off
del monofilt_feat_adaboost_list.txt
for /f "skip=2 tokens=1,5" %%a in (cas.txt) do (
  if #%%b#==#0# (
    echo %%a,>>monofilt_feat_adaboost_list.txt
  )
)