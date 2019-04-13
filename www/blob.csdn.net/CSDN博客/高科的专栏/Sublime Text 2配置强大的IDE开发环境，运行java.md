
# Sublime Text 2配置强大的IDE开发环境，运行java - 高科的专栏 - CSDN博客

2012年08月10日 18:55:31[高科](https://me.csdn.net/pbymw8iwm)阅读数：8882


Sublime Text 2是我无意中发现的、据说十分强大的、便捷的编辑器，许多程序员都投入到Sublime Text 2的怀抱中。
**1 配置java开发环境的方法如下：**
在jdk安装目录下的bin文件夹下新建一个bat格式的文件，文件命为javacexec.bat。
如果是在Windows平台下，则javacexec.bat的内容如下：
@ECHO OFF
cd %~dp1
javac %~nx1
java %~n1
如果是在Linux或者Mac平台下，则javacexec.bat的内容如下：
$ cat javacr.sh
\#!/bin/sh
javac $1.java
javac $1
对于在Linux或者Mac平台，don’t forget chmod +x and palce in PATH.
进入Sublime Text 2安装目录下的\Data\Packages\Java文件夹，找到JavaC.sublime-build这个文件，使用任意一个文本编辑器打开它，将javac修改成javacexec.bat （即刚才新建的.bat文件的文件名）。最终修改后的内容如下：
{
"cmd": ["javacexec.bat", "$file"],
"file_regex": "^(…*?):([0-9]*):?([0-9]*)",
"selector": "source.java"
}

