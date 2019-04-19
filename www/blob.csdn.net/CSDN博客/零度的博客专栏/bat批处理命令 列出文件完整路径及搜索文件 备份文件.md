# bat批处理命令:列出文件完整路径及搜索文件/备份文件 - 零度的博客专栏 - CSDN博客
2016年08月05日 15:00:56[零度anngle](https://me.csdn.net/zmx729618)阅读数：6655
list.bat：列出目录下所有文件的完整路径。
```
@echo off
set "PathName=C:\Users\zhangwenchao\Desktop\learn"
del list.txt
for %%a in (%PathName%) do for /f "delims=" %%b in ('dir /a-d/b/s *') do ( 
   echo %%b >>list.txt 
) 
pause
```
效果：列出C:\Users\zhangwenchao\Desktop\learn目录下所有文件路径输出到list.txt中。
search©.bat：在某一目录下查询指定匹配的文件
```
@echo off
set "FileName=*settings.xml"
set "PathName=C:\Users\zhangwenchao"
del log.txt
for %%a in (%PathName%) do for /f "delims=" %%b in ('dir /a-d/b/s %%a\%FileName%') do ( 
   echo %%b >>log.txt 
   copy "%%b" "C:\Users\zhangwenchao\Desktop\learn"
) 
pause
```
效果：在C:\Users\zhangwenchao目录中查询所以以setting.xml结尾的文件。
