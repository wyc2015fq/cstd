
# Bash: export - 设置或显示环境变量 - Augusdi的专栏 - CSDN博客


2015年08月22日 10:24:21[Augusdi](https://me.csdn.net/Augusdi)阅读数：1151


﻿﻿
## 用途说明
export命令可以用来设置或显示环境变量。在shell中，当一个变量创建时，只会在当前的shell中有 效，之后创建的子进程不会有该变量。而命令export可以将一个shell变量变成环境变量，在随后创建的子进程中也能访问到该环境变量，但是子进程在 修改了环境变量的值之后，父进程并不能知道，因为子进程将复制父进程的环境变量。因此，当shell退出时，那些在该shell中export出来的环境 变量也会消失。
在Linux中，有几个比较相近的命令：set,  env和export，它们都可以用来打印shell中的变量。set命令显示当前shell的变量，包括当前shell的环境变量；env命令显示当前 shell的环境变量；export命令显示当前导出成环境变量的shell变量。

## 常用参数
格式：export
格式：export -p
打印当前导出的环境变量。
格式：export VAR
导出变量VAR。
格式：export VAR=VALUE
设置环境变量，并导出。
格式：export -n VAR
取消导出变量VAR。
格式：unset VAR
删除变量VAR。

## 使用示例
### 示例一 打印环境变量
[root@jfht ~]\#export -p
declare -x ANT_HOME="/usr/apache/apache-ant-1.7.1"
declare -x CVS_RSH="ssh"
declare -x G_BROKEN_FILENAMES="1"
declare -x HISTSIZE="1000"
declare -x HOME="/root"
declare -x HOSTNAME="jfht"
declare -x INPUTRC="/etc/inputrc"
declare -x JAVA_HOME="/usr/java/jdk1.6.0_20"
declare -x LANG="zh_CN.GB18030"
declare -x LESSOPEN="|/usr/bin/lesspipe.sh %s"
declare -x LOGNAME="root"
declare -x  LS_COLORS="no=00:fi=00:di=01;34:ln=01;36:pi=40;33:so=01;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=01;32:*.cmd=01;32:*.exe=01;32:*.com=01;32:*.btm=01;32:*.bat=01;32:*.sh=01;32:*.csh=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.gz=01;31:*.bz2=01;31:*.bz=01;31:*.tz=01;31:*.rpm=01;31:*.cpio=01;31:*.jpg=01;35:*.gif=01;35:*.bmp=01;35:*.xbm=01;35:*.xpm=01;35:*.png=01;35:*.tif=01;35:"
declare -x MAIL="/var/spool/mail/root"
declare -x OLDPWD
declare -x  PATH="/usr/kerberos/sbin:/usr/kerberos/bin:/usr/apache/apache-ant-1.7.1/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin"
declare -x PWD="/root"
declare -x SHELL="/bin/bash"
declare -x SHLVL="1"
declare -x SSH_ASKPASS="/usr/libexec/openssh/gnome-ssh-askpass"
declare -x SSH_AUTH_SOCK="/tmp/ssh-XONvzI3100/agent.3100"
declare -x SSH_CLIENT="27.115.25.30 26926 2622"
declare -x SSH_CONNECTION="27.115.25.30 26926 211.103.28.26 2622"
declare -x SSH_TTY="/dev/pts/5"
declare -x TERM="linux"
declare -x USER="root"
[root@jfht ~]\#

### 示例二 一个关于变量export效果的测试
首先，准备一个简单的shell脚本，取名为xyz.sh，用于打印变量XYZ的值。
[root@jfht ~]\#cat xyz.sh
\#!/bin/sh
echo $XYZ
[root@jfht ~]\#chmod +x xyz.sh
现在，在当前shell中没有定义XYZ变量。
[root@jfht ~]\#echo $XYZ
[root@jfht ~]\#(echo $XYZ)
[root@jfht ~]\#export -p | grep XYZ
[root@jfht ~]\#./xyz.sh
[root@jfht ~]\#
现在，定义变量XYZ。
[root@jfht ~]\#XYZ=123
[root@jfht ~]\#echo $XYZ
123
[root@jfht ~]\#(echo $XYZ)
123
[root@jfht ~]\#export -p | grep XYZ
[root@jfht ~]\#./xyz.sh
[root@jfht ~]\#
现在，导出变量XYZ。
[root@jfht ~]\#export XYZ
[root@jfht ~]\#echo $XYZ
123
[root@jfht ~]\#(echo $XYZ)
123
[root@jfht ~]\#export -p | grep XYZ
declare -x XYZ="123"
[root@jfht ~]\#./xyz.sh
123
[root@jfht ~]\#
现在，取消导出。
[root@jfht ~]\#export -n XYZ
[root@jfht ~]\#echo $XYZ
123
[root@jfht ~]\#(echo $XYZ)
123
[root@jfht ~]\#export -p | grep XYZ
[root@jfht ~]\#./xyz.sh
[root@jfht ~]\#
现在，删除变量。
[root@jfht ~]\#unset XYZ
[root@jfht ~]\#echo $XYZ
[root@jfht ~]\#(echo $XYZ)
[root@jfht ~]\#export -p | grep XYZ
[root@jfht ~]\#./xyz.sh
[root@jfht ~]\#

## 问题思考
## 相关资料
【1】旅途 关于linux的export命令和shell变量的生命周期
http://www.cppblog.com/mydriverc/archive/2009/09/16/96476.html
【2】中国IT实验室 linux保存export出来的环境变量
http://tech.e800.com.cn/articles/2010/55/1273035447311_1.html
【3】留住你的每一感悟 linux export 的作用
http://www.cublog.cn/u2/87706/showart_1958932.html
【4】玉米疯收 在Linux里设置环境变量的方法（export PATH）
[http://www.cnblogs.com/amboyna/archive/2008/03/08/1096024.html](http://www.cnblogs.com/amboyna/archive/2008/03/08/1096024.html)


