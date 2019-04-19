# 【My Ora Life】设置环境变量&Linux指令 - 三少GG - CSDN博客
2011年06月23日 17:53:00[三少GG](https://me.csdn.net/scut1135)阅读数：1042
My Solution:
**setenv ORACLE_HOME "/scratch/aime/prod_install/app/product/11.1.0/db_1"**
**setenv ORACLCE_SID orcl**
++++++++++++++++++++++++++++
检查shell环境：
echo $SHELL
/bin/tcsh
export  : 貌似适用于 bash环境,直接输入bash即可切换
setenv : 
# Linux 指令篇：系统设置--setenv
**setenv(set environment variable)**
**功能说明：**查询或显示环境变量。
**语　　法：**setenv [变量名称][变量值]
**补充说明：****setenv为tsch中查询或设置环境变量的指令。**
EXAMPLES　　setenv PATH "/bin:/usr/bin:/usr/sbin:ucb/bin"      - Sets the environment path to search for files in the /bin, /usr/bin, /usr/sbin and usb/bin directory.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++
类似说明（MOS里查到）：
- 
Bourne, Bash, or Korn shell:
$ ORACLE_HOME=Oracle_home
$ ORACLE_SID=sid
$ PATH=$ORACLE_HOME/bin:$ORACLE_HOME/bin/OPatch:$PATH:
$ export ORACLE_HOME ORACLE_SID PATH
- 
C shell:
% setenv ORACLE_HOME Oracle_home
% setenv ORACLE_SID sid
% setenv PATH $ORACLE_HOME/bin:$ORACLE_HOME/bin/OPatch:$PATH
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
一.暂时使用
亦即重启或注销后就失效了。
使用 PATH=$PATH:目录:目录... 直接赋值。如export PATH=$PATH:/sbin。
二.长期使用
在全局文件 /etc/profile 中或用户自定义文件 ~/.bash_profile 中添加上述命令即可！
八   环境变量
8.1   查看环境变量
$ env        显示所有的环境变量设置
$ echo $ENV_VARIABLE   显示指定环境变量的设置
例：
$ echo $PATH
/bin:/etc:/usr/bin:/tcb/bin
8.2   设定环境变量
$ ENV_VARIABLE=XXX;export ENV_VARIABLE
例：
$ PATH=$PATH:$INFORMIXDIR/bin;export PATH  将环境变量PATH设定为原PATH值+$INFORMIXDIR/bin
8.3   取消环境变量设置
$ unset $ENV_VARIABLE
例：
$ set GZJ=gzj;export GZJ    设置环境变量GZJ
$ echo $GZJ
gzj         显示环境变量值
$ unset $GZJ      取消环境变量GZJ的设置
$ echo $GZJ
         已取消+++++++++++++++++++++++++++++++++++++++++
**Linux 进程管理**如何在Linux中查看所有正在运行的进程：
你可以使用ps命令。它能显示当前运行中进程的相关信息，
包括进程的PID。Linux和UNIX都支持ps命令，显示所有运行中进程的相关信息。
ps命令能提供一份当前进程的快照。如果你想状态可以自动刷新，可以使用top命令。 **任务：查看系统中的每个进程。**
# ps -A
**# ps -e**
参考文档：** Linux 进程管理   **[**http://www.linuxsir.org/main/?q=node/210**](http://www.linuxsir.org/main/?q=node/210)摘要：**1.2 进程的属性；**
**进程ID（PID)：是唯一的数值，用来区分进程；**父进程和父进程的ID（PPID)；
启动进程的用户ID（UID）和所归属的组（GID）；
进程状态：状态分为运行R、休眠S、僵尸Z；
进程执行的优先级；
进程所连接的终端名；
进程资源占用：比如占用资源大小（内存、CPU占用量）；
**2、进程管理；**
对于Linux进程的管理，是通过进程管理工具实现的，比如**ps、kill、pgrep等工**具；
++++++++++++++++++++++++++++++++++++++++++++[http://xee123.blog.163.com/blog/static/277119942010112004950399/](http://xee123.blog.163.com/blog/static/277119942010112004950399/)  (好文章！！！可细看)
linux环境变量设置及保存地点
1. 显示环境变量HOME
$ echo $HOME
/home/redbooks
2. 设置一个新的环境变量hello
$ export HELLO="Hello!"
$ echo $HELLO
Hello!
3. 使用env命令显示所有的环境变量
$ env
HOSTNAME=redbooks.safe.org
PVM_RSH=/usr/bin/rsh
SHELL=/bin/bash
TERM=xterm
HISTSIZE=1000
...
4. 使用set命令显示所有本地定义的Shell变量
$ set
BASH=/bin/bash
BASH_VERSINFO=([0]="2"[1]="05b"[2]="0"[3]="1"[4]="release"[5]="i386-redhat-linux-gnu")
BASH_VERSION='2.05b.0(1)-release'
COLORS=/etc/DIR_COLORS.xterm
COLUMNS=80
DIRSTACK=()
DISPLAY=:0.0
...
5. 使用unset命令来清除环境变量
set可以设置某个环境变量的值。清除环境变量的值用unset命令。如果未指定值，则该变量值将被设为NULL。示例如下：
$ export TEST="Test..." #增加一个环境变量TEST
$ env|grep TEST #此命令有输入，证明环境变量TEST已经存在了
TEST=Test...
$ unset $TEST #删除环境变量TEST
$ env|grep TEST #此命令没有输出，证明环境变量TEST已经存在了
6. 使用readonly命令设置只读变量
如果使用了readonly命令的话，变量就不可以被修改或清除了。示例如下：
$ export TEST="Test..." #增加一个环境变量TEST
$ readonly TEST #将环境变量TEST设为只读
$ unset TEST #会发现此变量不能被删除
-bash: unset: TEST: cannot unset: readonly variable
$ TEST="New" #会发现此也变量不能被修改
-bash: TEST: readonly variable
环境变量的设置位于/etc/profile文件
如果需要增加新的环境变量可以添加下属行
export path=$path:/path1:/path2:/pahtN
