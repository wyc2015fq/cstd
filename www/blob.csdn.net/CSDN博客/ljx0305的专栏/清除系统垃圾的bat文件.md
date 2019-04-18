# 清除系统垃圾的bat文件 - ljx0305的专栏 - CSDN博客
2008年07月09日 10:39:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：8531标签：[internet																[cookies																[工具																[磁盘](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=cookies&t=blog)](https://so.csdn.net/so/search/s.do?q=internet&t=blog)
1.第一种
@echo off           
echo 正在清除系统垃圾文件，请稍等......           
del /f /s /q %systemdrive%/*.tmp       --临时文件    
del /f /s /q %systemdrive%/*._mp           
del /f /s /q %systemdrive%/*.log           --日志文件
del /f /s /q %systemdrive%/*.gid           --帮助文件
del /f /s /q %systemdrive%/*.chk           --磁盘检查文件
del /f /s /q %systemdrive%/*.old           --临时备份文件
del /f /s /q %systemdrive%/recycled/*.*           
del /f /s /q %windir%/*.bak           
del /f /s /q %windir%/prefetch/*.*           
rd /s /q %windir%/temp & md %windir%/temp
del /f /q %userprofile%/cookies/*.*           
del /f /q %userprofile%/recent/*.*           
del /f /s /q "%userprofile%/Local Settings/Temporary Internet Files/*.*"           
del /f /s /q "%userprofile%/Local Settings/Temp/*.*"           
del /f /s /q "%userprofile%/recent/*.*"           
echo 清除系统LJ完成！           
echo. & pause       
2.第二种
@echo off
color 2f
title 系统垃圾文件清理处理工具-by zqling
:clearauto
cls
echo                        计算机系统垃圾文件清理工具/
echo.
echo                  网址:http://www.ynboke.com/html/72/2272-95648.html
echo.
echo                               制作：zqling
echo.
echo.
echo   本程序清除文件夹temporary internet files,history,temp,cookies,recent内的文件
echo   以及*.log，*.chk，*.bak，*.tmp等类型文件
echo.
echo.
echo          [1]删除local settings/temporary internet files/目录下的文件
echo          [2]删除local settings/history/目录下的文件
echo          [3]删除local settings/temp/目录下的文件
echo          [4]删除cookies/目录下的文件
echo          [5]删除recent/目录下的文件
echo          [6]删除回收站中的内容
echo          [7]删除*.log,*old,*chk等文件
echo          [8]全删除系统垃圾文件
echo          [0]退出
echo.
echo.
set /p clearslt=  请输入您的选择(1/2/3/4/5/6/7/8/0):
if "%clearslt%"==""   goto clearauto
if "%clearslt%"=="1"  goto clearauto1
if "%clearslt%"=="2"  goto clearauto2
if "%clearslt%"=="3"  goto clearauto3
if "%clearslt%"=="4"  goto clearauto4
if "%clearslt%"=="5"  goto clearauto5
if "%clearslt%"=="6"  goto clearauto6
if "%clearslt%"=="7"  goto clearauto7
if "%clearslt%"=="8"  goto clearauto8
if "%clearslt%"=="0"  exit
:clearauto1
del /f /s /q "%userprofile%/local settings/temporary internet files/."
cls
echo temporary internet files文件夹清除内容清除完毕 ,任意键返回......
pause>nul
goto clearauto
:clearauto2
del /f /s /q "%userprofile%/local settings/history/."
cls
echo history文件夹清除完毕,任意键返回......
pause>nul
goto clearauto 
:clearauto3
del /f /s /q "%userprofile%/local settings/temp/."
cls
echo temp文件夹清除完毕,任意键返回......
pause>nul
goto clearauto
:clearauto4
cls
del /f /s /q "%userprofile%/cookies/."
cls
echo cookies文件夹清除完毕，任意键返回......
1pause>nul
goto clearauto
:clearauto5
del /f /s /q "%userprofile%/recent/."
cls
echo recent文件夹清除完毕，任意键返回......
pause>nul
goto clearauto
:clearauto6
del /f /s /q %systemdirve%/recycled/*.*
cls
echo 清除完毕。任意键返回......
pause>nul
goto clearauto
:clearauto7
del /f /s /q %systemdirve%/recycled/*.*
cls
echo 给类文件清除完毕。任意键返回......
pause>nul
goto clearauto
:clearauto8
del /f /s /q "%userprofile%/local settings/temporary internet files/."
del /f /s /q "%userprofile%/local settings/history/."
del /f /s /q "%userprofile%/local settings/temp/."
del /f /s /q "%userprofile%/cookies/."
del /f /s /q "%userprofile%/recent/."
del /f /s /q %systemroot%/temp/.
del /f /s /q %systemroot%/prcfctch/.
del /f /s /q %systemdirve%/*.log
del /f /s /q %systemdirve%/*.old
del /f /s /q %systemdirve%/*.chk
del /f /s /q %systemdirve%/*.bak
del /f /s /q %systemdirve%/*.tmp
cls
echo 系统垃圾文件删除完毕，感谢使用!
set /p clearslt=[返修请输入9，退出请按esc]
if "%clearslt%"=="9" goto clearauto
if "%clearslt%"==""  exit
