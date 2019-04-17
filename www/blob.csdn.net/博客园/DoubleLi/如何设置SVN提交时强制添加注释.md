# 如何设置SVN提交时强制添加注释 - DoubleLi - 博客园






**windows版本：**

1、新建一个名为pre-commit.bat的文件并将该文件放在创建的库文件的hooks文件夹中

2、pre-commit.bat文件的内容如下：
@echo off
set SVN_BINDIR="C:\ProgramFiles\Subversion"
set SVNLOOK="E:\freescm\freescm\SVN\bin\svnlook.exe"
setlocal
set REPOS=%1
set TXN=%2
rem check that logmessage contains at least 10 characters
%SVN_BINDIR%\svnlook log “%REPOS%” -t “%TXN%” | findstr “……….” > nul
if %errorlevel% gtr 0 goto err
exit 0
:err
echo "提交文件时必须添加注释，提交中止." 1>&2
exit 1

**Linux版本：**

1、在svn的hooks目录下新建一个名为pre-commit的文件并为其添加执行权限

2、pre-commit文件的内容如下：
#!/bin/sh
REPOS="$1"
TXN="$2"
SVNLOOK=/opt/CollabNet_Subversion/bin/svnlook
LOGMSG=$($SVNLOOK log -t "$TXN" "$REPOS" | grep "[a-zA-Z0-9]" | wc -c)
if [ "$LOGMSG" -lt 10 ]; then
echo -e "\n 提交文件时必须添加注释，提交中止."1>&2
exit 1
fi

**脚本命令解释**

LOGMSG=$($SVNLOOK log -t “$TXN” “$REPOS” | grep “[a-zA-Z0-9]” | wc -c)
将提交的日志信息作为便来那个输入之后，将grep英文和数字的结果作为变量


if [ "$LOGMSG" -lt 10 ]; 传入变量来判断变量长度是否小于10
-eq 等于号  -gt 大于号   -lt小于号

1. echo 错误提示信息的时候必须重定向到 stderr ，即 1>&2
2. pre-commit 文件拷贝到 hooks 目录之后，需要 chmod 0755 pre-commit ，给 pre-commit 文件增加可执行权限（或者至少把文件owner改为 svn 服务运行用户并加上owner的x权限）









