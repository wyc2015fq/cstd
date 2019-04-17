# svn文件提交时强制写注释 - DoubleLi - 博客园






这个操作需要修改版本库中的一个钩子文件，在你创建的版本库中有一个hooks文件夹，初始的时候其中有一个文件叫pre-commit.tmpl

在windows下将其修改为pre-commit.bat，在linux下将其改为pre-commit



linux下(文件名是: pre-commit,放到版本库的/hooks目录下)
==========================================================

#!/bin/sh
REPOS="$1"
TXN="$2"
SVNLOOK=/usr/bin/svnlook

# check that logmessage contains at least 10 alphanumeric characters
LOGMSG=`$SVNLOOK log -t "$TXN" "$REPOS" | grep "[a-zA-Z0-9]" | wc -c`
if [ "$LOGMSG" -lt 10 ];
then
echo -e "/nEmpty log message not allowed. Commit aborted!" 1>&2
exit 1
fi

windows下(保存到版本库下的hooks文件夹下/hooks/pri-commit.bat)
=================================================================

@echo off
setlocal
set REPOS=%1
set TXN=%2
rem check that logmessage contains at least 10 characters
svnlook log "%REPOS%" -t "%TXN%" | findstr "....." > nul
if %errorlevel% gtr 0 goto err
exit 0
:err
echo 你必须输入至少5个字符! 1>&2
exit 1









