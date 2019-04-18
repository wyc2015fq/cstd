# bat批处理命令，搜索文件及列出文件完整路径，方便发布时备份文件 - z69183787的专栏 - CSDN博客
2014年08月18日 14:19:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5465
list.bat
@echo off
set "PathName=E:\Users\zhoushun\Desktop\bat"
del list.txt
for %%a in (%PathName%) do for /f "delims=" %%b in ('dir /a-d/b/s *') do ( 
  echo %%b >>list.txt 
) 
pause
search©.bat
@echo off
set "FileName=*jdbc.xml"
set "PathName=d:"
del log.txt /y
for %%a in (%PathName%) do for /f "delims=" %%b in ('dir /a-d/b/s %%a\%FileName%') do ( 
  echo %%b >>log.txt 
  copy "%%b" "E:\Users\zhoushun\Desktop\bat"
) 
pause

