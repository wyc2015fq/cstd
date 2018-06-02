@echo off
set out=TagKeywords.txt
(for /f "tokens=3,4,5 delims=;<>, " %%a in ('findstr "typedef" D:\code\lib\gdcm\gdcm-2.6.7\Source\DataDictionary\gdcmTagKeywords.h') do (
  echo TAGKEYWORDDEF(%%a, %%b, %%c^)
))>%out%
