
# Linux命令之env - 显示当前用户的环境变量 - Augusdi的专栏 - CSDN博客


2015年08月22日 10:45:24[Augusdi](https://me.csdn.net/Augusdi)阅读数：3386


﻿﻿
## 用途说明
env命令可以显示当前用户的环境变量，还可以用来在指定环境变量下执行其他命令。下面来比较一下set,env和export命令的异同：set命令显示当前shell的变量，包括当前用户的变量;env命令显示当前用户的变量;export命令显示当前导出成用户变量的shell变量。每个shell有自己特有的变量（set）显示的变量，这个和用户变量是不同的，当前用户变量和你用什么shell无关，不管你用什么shell都在，比如HOME,SHELL等这些变量，但shell自己的变量不同shell是不同的，比如BASH_ARGC， BASH等，这些变量只有set才会显示，是bash特有的，export不加参数的时候，显示哪些变量被导出成了用户变量，因为一个shell自己的变量可以通过export “导出”变成一个用户变量。（注：此处来自资料1）

## 常用方式
格式：env
打印当前用户的环境变量。
格式：env -i NAME1=VALUE1 NAME2=VALUE2 <command-line>
使用指定的环境变量执行命令行<command-line>。

## 使用示例
### 示例一 比较env,set和export
set命令打印的信息最多，包括自定义函数等，以至于要使用more才能看得清。
[root@web ~]\#env
HOSTNAME=web
TERM=linux
SHELL=/bin/bash
HISTSIZE=1000
SSH_CLIENT=58.222.185.248 1113 22
SSH_TTY=/dev/pts/6
ANT_HOME=/opt/apache/apache-ant-1.8.1
USER=root
LS_COLORS=no=00:fi=00:di=01;34:ln=01;36:pi=40;33:so=01;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=01;32:*.cmd=01;32:*.exe=01;32:*.com=01;32:*.btm=01;32:*.bat=01;32:*.sh=01;32:*.csh=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.gz=01;31:*.bz2=01;31:*.bz=01;31:*.tz=01;31:*.rpm=01;31:*.cpio=01;31:*.jpg=01;35:*.gif=01;35:*.bmp=01;35:*.xbm=01;35:*.xpm=01;35:*.png=01;35:*.tif=01;35:
SSH_AUTH_SOCK=/tmp/ssh-amfuW25778/agent.25778
MAIL=/var/spool/mail/root
PATH=/usr/kerberos/sbin:/usr/kerberos/bin:/opt/apache/apache-ant-1.8.1/bin:/usr/java/jdk1.6.0_20/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin
INPUTRC=/etc/inputrc
PWD=/root
JAVA_HOME=/usr/java/jdk1.6.0_20
LANG=zh_CN.GB18030
SSH_ASKPASS=/usr/libexec/openssh/gnome-ssh-askpass
SHLVL=1
HOME=/root
LOGNAME=root
CVS_RSH=ssh
SSH_CONNECTION=58.222.185.248 1113 218.23.142.44 22
LESSOPEN=|/usr/bin/lesspipe.sh %s
G_BROKEN_FILENAMES=1
_=/bin/env
[root@web ~]\#export
declare -x ANT_HOME="/opt/apache/apache-ant-1.8.1"
declare -x CVS_RSH="ssh"
declare -x G_BROKEN_FILENAMES="1"
declare -x HISTSIZE="1000"
declare -x HOME="/root"
declare -x HOSTNAME="web"
declare -x INPUTRC="/etc/inputrc"
declare -x JAVA_HOME="/usr/java/jdk1.6.0_20"
declare -x LANG="zh_CN.GB18030"
declare -x LESSOPEN="|/usr/bin/lesspipe.sh %s"
declare -x LOGNAME="root"
declare -x LS_COLORS="no=00:fi=00:di=01;34:ln=01;36:pi=40;33:so=01;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=01;32:*.cmd=01;32:*.exe=01;32:*.com=01;32:*.btm=01;32:*.bat=01;32:*.sh=01;32:*.csh=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.gz=01;31:*.bz2=01;31:*.bz=01;31:*.tz=01;31:*.rpm=01;31:*.cpio=01;31:*.jpg=01;35:*.gif=01;35:*.bmp=01;35:*.xbm=01;35:*.xpm=01;35:*.png=01;35:*.tif=01;35:"
declare -x MAIL="/var/spool/mail/root"
declare -x OLDPWD
declare -x PATH="/usr/kerberos/sbin:/usr/kerberos/bin:/opt/apache/apache-ant-1.8.1/bin:/usr/java/jdk1.6.0_20/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin"
declare -x PWD="/root"
declare -x SHELL="/bin/bash"
declare -x SHLVL="1"
declare -x SSH_ASKPASS="/usr/libexec/openssh/gnome-ssh-askpass"
declare -x SSH_AUTH_SOCK="/tmp/ssh-amfuW25778/agent.25778"
declare -x SSH_CLIENT="58.222.185.248 1113 22"
declare -x SSH_CONNECTION="58.222.185.248 1113 218.23.142.44 22"
declare -x SSH_TTY="/dev/pts/6"
declare -x TERM="linux"
declare -x USER="root"
[root@web ~]\#set | more
ANT_HOME=/opt/apache/apache-ant-1.8.1
BASH=/bin/bash
BASH_ARGC=()
BASH_ARGV=()
BASH_COMPLETION=/etc/bash_completion
BASH_COMPLETION_DIR=/etc/bash_completion.d
BASH_LINENO=()
BASH_SOURCE=()
BASH_VERSINFO=([0]="3" [1]="2" [2]="25" [3]="1" [4]="release" [5]="i686-redhat-linux-gnu")
BASH_VERSION='3.2.25(1)-release'
COLORS=/etc/DIR_COLORS
COLUMNS=132
CVS_RSH=ssh
DIRSTACK=()
EUID=0
GROUPS=()
G_BROKEN_FILENAMES=1
HISTFILE=/root/.bash_history
HISTFILESIZE=1000
HISTSIZE=1000
HOME=/root
HOSTNAME=web
HOSTTYPE=i686
IFS=$' \t\n'
INPUTRC=/etc/inputrc
JAVA_HOME=/usr/java/jdk1.6.0_20
LANG=zh_CN.GB18030
LESSOPEN='|/usr/bin/lesspipe.sh %s'
LINES=41
LOGNAME=root
LS_COLORS='no=00:fi=00:di=01;34:ln=01;36:pi=40;33:so=01;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=01;32:*.cmd=01;3
2:*.exe=01;32:*.com=01;32:*.btm=01;32:*.bat=01;32:*.sh=01;32:*.csh=01;32:*.tar=01;31:*.tgz=01;31:*.arj=01;31:*.taz=01;31:*.lzh=01;31
:*.zip=01;31:*.z=01;31:*.Z=01;31:*.gz=01;31:*.bz2=01;31:*.bz=01;31:*.tz=01;31:*.rpm=01;31:*.cpio=01;31:*.jpg=01;35:*.gif=01;35:*.bmp
=01;35:*.xbm=01;35:*.xpm=01;35:*.png=01;35:*.tif=01;35:'
MACHTYPE=i686-redhat-linux-gnu
MAIL=/var/spool/mail/root
MAILCHECK=60
OPTERR=1
OPTIND=1
OSTYPE=linux-gnu
--More--

### 示例二 service命令的源代码中用到了env命令
可以看出service在执行别的命令式，只有三个环境变量：LANG, PATH, TERM。
[root@web ~]\# grep env /sbin/service
env -i LANG="$LANG" PATH="$PATH" TERM="$TERM" "${SERVICEDIR}/${SERVICE}" status
env -i LANG="$LANG" PATH="$PATH" TERM="$TERM" "${SERVICEDIR}/${SERVICE}" stop
env -i LANG="$LANG" PATH="$PATH" TERM="$TERM" "${SERVICEDIR}/${SERVICE}" start
env -i LANG="$LANG" PATH="$PATH" TERM="$TERM" "${SERVICEDIR}/${SERVICE}" ${OPTIONS}
[root@web ~]\#

## 问题思考
## 相关资料
【1】飞诺网 Linux的set,env和export的区别是什么
http://dev.firnow.com/course/6_system/linux/Linuxjs/2008913/142616.html
【2】linvar linux set,env, export
http://linvar.iteye.com/blog/584696
【3】鸟哥的私房菜 用 env 觀察環境變數與常見環境變數說明
http://linux.vbird.org/linux_basic/0320bash.php\#env
【4】金星专栏 linux profile of env
http://blog.csdn.net/liujinxing/archive/2008/10/24/3139044.aspx

