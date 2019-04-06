@echo off
for /f "tokens=1 delims=*" %%a in ('pclip') do (
you-get --playlist -o "D:\VDownload" "%%a"
)
del D:\VDownload\*.xml
