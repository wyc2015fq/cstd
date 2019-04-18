# 一键备份MySQL数据库 - weixin_33985507的博客 - CSDN博客
2011年07月05日 21:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
 将如下代码另存为.bat文件
```
@echo off
echo.
echo      MySQL数据库备份
echo *****************************
echo.
echo 今天是 %date%
echo 时间是 %time%
echo.
echo *****************************
set "Ymd=%date:~,4%%date:~5,2%%date:~8,2%"
md "D:\%Ymd%"
"C:\Program Files\MySQL\MySQL Server 5.1\bin\mysqldump" --opt -Q -uroot -proot --default-character-set=gbk mysql > "D:\%Ymd%\mySQLteble.sql"
echo.
echo MySQL数据库备份完成，请进行检查。。。
echo.
echo.
pause
```
