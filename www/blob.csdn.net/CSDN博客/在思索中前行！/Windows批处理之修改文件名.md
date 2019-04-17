# Windows批处理之修改文件名 - 在思索中前行！ - CSDN博客





2018年02月06日 09:00:42[_Tham](https://me.csdn.net/txl16211)阅读数：3687









### 用途

可以将任意的文件名批量修改有规律的文件名,如下:

![](https://img-blog.csdn.net/20180206085838999?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHhsMTk5MTA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


rename by keith.gif


### 使用方法

新建一个文本文件(.txt),将下面代码复制进去,保存,最后将文件后缀改成.bat,放到想要批量操作的文件所在的文件夹,直接双击就好.

### 实现代码

```php
```php
@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION
COLOR 0A
TITLE keith batch rename tool
ECHO.
ECHO =================================
ECHO     欢迎使用keith批量命名工具
ECHO =================================
ECHO.
ECHO 请选择要批量修改的处理的后缀名：
ECHO.
ECHO 1. 所有文件(只会修改文件名)
ECHO 2. txt
ECHO 3. png
ECHO 4. 其他,我自己来输入
ECHO.
CHOICE /C 1234 /m "你的选择是: "
IF %ERRORlEVEL% equ 1 SET suffix=*
IF %ERRORlEVEL% equ 2 SET suffix=txt
IF %ERRORlEVEL% equ 3 SET suffix=png
IF %ERRORlEVEL% equ 4 (
ECHO.
SET /P suffix="请输入要批量处理的后缀名: "
)
ECHO.
SET /p prefix="请输入你修改成的文件名的开头: "
ECHO.
ECHO 修改ing...
ECHO.
SET /a index=0
FOR %%i in (*.!suffix!) do (
    SET /a index=!index!+1
    SET name=!prefix!!index!.!suffix!
    REN "%%i" !name!
)
IF %ERRORlEVEL% equ 0 (
    ECHO 修改成功!
) ELSE (
    ECHO 修改失败!
)
ECHO.
PAUSE
```
```

### 最后

刚刚看了看windows batch script的知识,这个工具还是很简单,但也还是可以解决一点小问题,如果你们**有什么需求**想要批量处理的,可以在下面留言,我可以尽量去写,当作训练.

### Reference
- [Windows Batch Scripting](https://link.jianshu.com?t=https://en.wikibooks.org/wiki/Windows_Batch_Scripting)





**批量替换文件名中的字符**



@echo off
set /p str1= 请输入要替换的文件(文件夹)名字符串（可替换空格）：
set /p str2= 请输入替换后的文件(文件夹)名字符串（去除则直接回车）：
echo.
echo 正在修改文件(夹)名中，请稍候……
for /f "delims=" %%a in ('dir /s /b ^|sort /+65535') do (
                        if "%%~nxa" neq "%~nx0" (
                             set "file=%%a"
                             set "name=%%~na"
                             set "extension=%%~xa"
                             call set "name=%%name:%str1%=%str2%%%"
                             setlocal enabledelayedexpansion
                             ren "!file!" "!name!!extension!" 2>nul
                             endlocal
        )
    )
exit









