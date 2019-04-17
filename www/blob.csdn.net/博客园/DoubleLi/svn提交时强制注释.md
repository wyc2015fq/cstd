# svn提交时强制注释 - DoubleLi - 博客园






不少开发员提交修改的时候都不写注释，导致查看历史时很费劲，也不太符合规范。有的公司要求每次提交修改时都写上bug号或者任务描述，那么如何在工具上防止开发员们不写注释呢？ 

  利用svn的pre-commit钩子可简单实现此要求。 

进入仓库project1/hooks目录，找到pre-commit.tmpl文件，重命名，去掉后缀.tmpl。 
编辑pre-commit文件：（Linux系统和windows系统脚本内容见1和2） 

**1、Linux系统**

方法1：

将： 
$SVNLOOK log -t "$TXN" "$REPOS" | \ 
   grep "[a-zA-Z0-9]" > /dev/null || exit 1 
commit-access-control.pl "$REPOS" "$TXN" commit-access-control.cfg || exit 1 
这三行注释掉（前面加#符号）， 
在此位置添加如下几行： 

LOGMSG=`$SVNLOOK log -t "$TXN" "$REPOS" | grep "[a-zA-Z0-9]" | wc -c` 
if [ "$LOGMSG" -lt 5 ];#要求注释不能少于5个字符，您可自定义 
then 
  echo -e "\nLog message cann't be empty! you must input more than 5 chars as comment!." 1>&2 
  exit 1 
fi 

保存，退出。 
给pre-commit添加可执行权限： 
chmod +x pre-commit 
这里记得修改 $SVNLOOK=（svnlook的路径，/csvn的安装目录/bin/svnlook） 
配置结束，可以使用了。



**方法2：**

这两天把项目的开发机迁移到了linux环境，用python重写了原来的svn hook，保存为pre-commit放到svn的hooks目录下，chmod +x pre-commit加上执行权限即可。windows下写SVN钩子比较麻烦，只能是批处理或者exe，linux下随便用什么脚本语言写都可以，只要加上可执行权限就行了。


#! /usr/bin/env python
# -*- coding: utf-8 -*- 
"""
SVN提交前检查钩子
功能：
        1、强制填写提交注释，内容10字节以上
        2、强制注释格式为：xxx:xxx
        3、提交文件检查，过滤不允许提交的文件

作者: 李思杰 <lsj86@qq.com> <2012/04/28>
"""

import sys
import os
import re

def main(argv):
        (repos, txn) = argv
        badlist = (".*config\.php$", ".*/php/cache", ".*test", "config\.js$","^.*\.db$")
        message = "".join(os.popen("/usr/bin/svnlook log '%s' -t '%s'" % (repos, txn)).readlines()).strip()
        if len(message) < 10:
                sys.stderr.write("请输入本次提交的修改内容，10字节以上。");
                sys.exit(1)
        if message.find(':') < 1:
                sys.stderr.write("请按规范填写注释，格式为：功能名: 修改说明。");
                sys.exit(1)

        changelist = os.popen("/usr/bin/svnlook changed '%s' -t '%s'" % (repos, txn)).readlines()
        for line in changelist:
                for pattern in badlist:
                        if re.search(pattern, line):
                                sys.stderr.write("请不要把 %s 加入版本库。" % line[1:].strip());
                                sys.exit(1)
        sys.exit(0)

if __name__ == "__main__":
        main(sys.argv[1:])



** 2、windows强制注释脚本**



@echo off  
 setlocal 

 set REPOS=%1  
 set TXN=%2          



rem 保证输入8个字符 
 svnlook log %REPOS% -t %TXN% | findstr "........" > nul 
 if %errorlevel% gtr 0 goto :err_action



rem 过滤空格字符 
svnlook log %REPOS% -t %TXN% | findstr /ic:"        " > nul 
 if %errorlevel% gtr 0 goto :success 

 :err_action 
 echo 你本次版本提交未填写任何变更的日志说明信息.      >&2 
 echo 请补充日志说明信息后再提交代码,例如:功能说明等.  >&2 
 echo 输入的日志信息不少于8个字符说明(或4个汉字),谢谢! >&2 
 echo *******************禁止空格数据***************** >&2


 goto :err_exit



:err_exit 
 exit 1 

 :success 
 exit 0









