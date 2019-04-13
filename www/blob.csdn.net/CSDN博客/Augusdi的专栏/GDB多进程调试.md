
# GDB多进程调试 - Augusdi的专栏 - CSDN博客


2015年04月15日 14:18:42[Augusdi](https://me.csdn.net/Augusdi)阅读数：944


[GDB多进程调试(转)](http://www.cnblogs.com/ggjucheng/archive/2011/12/15/2288710.html)
GDB 是 linux 系统上常用的 c/c++ 调试工具，功能十分强大。对于较为复杂的系统，比如多进程系统，如何使用 GDB 调试呢？考虑下面这个三进程系统：
**进程**
![clip_image001](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/20111215125253912.jpg)
Proc2 是 Proc1 的子进程，Proc3 又是 Proc2 的子进程。如何使用 GDB 调试 proc2 或者 proc3 呢？
实际上，GDB 没有对多进程程序调试提供直接支持。例如，使用GDB调试某个进程，如果该进程fork了子进程，GDB会继续调试该进程，子进程会不受干扰地运行下去。如果你事先在子进程代码里设定了断点，子进程会收到SIGTRAP信号并终止。那么该如何调试子进程呢？其实我们可以利用GDB的特点或者其他一些辅助手段来达到目的。此外，GDB 也在较新内核上加入一些多进程调试支持。
接下来我们详细介绍几种方法，分别是 follow-fork-mode 方法，attach 子进程方法和 GDB wrapper 方法。
**follow-fork-mode**
在2.5.60版Linux内核及以后，GDB对使用fork/vfork创建子进程的程序提供了follow-fork-mode选项来支持多进程调试。
follow-fork-mode的用法为：
set follow-fork-mode [parent|child]
· parent: fork之后继续调试父进程，子进程不受影响。
· child: fork之后调试子进程，父进程不受影响。
因此如果需要调试子进程，在启动gdb后：
(gdb) set follow-fork-mode child
并在子进程代码设置断点。
此外还有detach-on-fork参数，指示GDB在fork之后是否断开（detach）某个进程的调试，或者都交由GDB控制：
set detach-on-fork [on|off]
· on: 断开调试follow-fork-mode指定的进程。
· off: gdb将控制父进程和子进程。follow-fork-mode指定的进程将被调试，另一个进程置于暂停（suspended）状态。
注意，最好使用GDB 6.6或以上版本，如果你使用的是GDB6.4，就只有follow-fork-mode模式。
follow-fork-mode/detach-on-fork的使用还是比较简单的，但由于其系统内核/gdb版本限制，我们只能在符合要求的系统上才能使用。而且，由于follow-fork-mode的调试必然是从父进程开始的，对于fork多次，以至于出现孙进程或曾孙进程的系统，例如上图3进程系统，调试起来并不方便。
**Attach子进程**
众所周知，GDB有附着（attach）到正在运行的进程的功能，即attach <pid>命令。因此我们可以利用该命令attach到子进程然后进行调试。
例如我们要调试某个进程RIM_Oracle_Agent.9i，首先得到该进程的pid
[root@tivf09 tianq]\#ps -ef|grep RIM_Oracle_Agent.9inobody 6722 6721 0 05:57 ? 00:00:00 RIM_Oracle_Agent.9i
root 7541 27816 0 06:10 pts/3 00:00:00 grep -i rim_oracle_agent.9i

通过pstree可以看到，这是一个三进程系统，oserv是RIM_Oracle_prog的父进程，RIM_Oracle_prog又是RIM_Oracle_Agent.9i的父进程。
启动GDB，attach到该进程
[root@tivf09 root]\#pstree -H 6722

**通过 pstree 察看进程**
![clip_image002](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252532374.jpg)
**用 GDB 连接进程**
![clip_image003](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252546900.jpg)
现在就可以调试了。一个新的问题是，子进程一直在运行，attach上去后都不知道运行到哪里了。有没有办法解决呢？
一个办法是，在要调试的子进程初始代码中，比如main函数开始处，加入一段特殊代码，使子进程在某个条件成立时便循环睡眠等待，attach到进程后在该代码段后设上断点，再把成立的条件取消，使代码可以继续执行下去。
至于这段代码所采用的条件，看你的偏好了。比如我们可以检查一个指定的环境变量的值，或者检查一个特定的文件存不存在。以文件为例，其形式可以如下：
void debug_wait(char *tag_file)
{while(1)
    {if(tag_file存在)
             睡眠一段时间;elsebreak;
      }
}

当attach到进程后，在该段代码之后设上断点，再把该文件删除就OK了。当然你也可以采用其他的条件或形式，只要这个条件可以设置/检测即可。
Attach进程方法还是很方便的，它能够应付各种各样复杂的进程系统，比如孙子/曾孙进程，比如守护进程（daemon process），唯一需要的就是加入一小段代码。
**GDB wrapper**
很多时候，父进程 fork 出子进程，子进程会紧接着调用 exec族函数来执行新的代码。对于这种情况，我们也可以使用gdb wrapper 方法。它的优点是不用添加额外代码。
其基本原理是以gdb调用待执行代码作为一个新的整体来被exec函数执行，使得待执行代码始终处于gdb的控制中，这样我们自然能够调试该子进程代码。
还是上面那个例子，RIM_Oracle_prog fork出子进程后将紧接着执行RIM_Oracle_Agent.9i的二进制代码文件。我们将该文件重命名为RIM_Oracle_Agent.9i.binary，并新建一个名为RIM_Oracle_Agent.9i的shell脚本文件，其内容如下：
[root@tivf09 bin]\# mv RIM_Oracle_Agent.9i RIM_Oracle_Agent.9i.binary
[root@tivf09 bin]\# cat RIM_Oracle_Agent.9i
\#!/bin/sh
gdb RIM_Oracle_Agent.binary
当fork的子进程执行名为RIM_Oracle_Agent.9i的文件时，gdb会被首先启动，使得要调试的代码处于gdb控制之下。
新的问题来了。子进程是在gdb的控制下了，但还是不能调试：如何与gdb交互呢？我们必须以某种方式启动gdb，以便能在某个窗口/终端与gdb交互。具体来说，可以使用xterm生成这个窗口。
xterm是X window系统下的模拟终端程序。比如我们在Linux桌面环境GNOME中敲入xterm命令：
**xterm**
![clip_image004](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252549125.jpg)
就会跳出一个终端窗口：
**终端**
![clip_image005](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252554175.jpg)
如果你是在一台远程linux服务器上调试，那么可以使用VNC(Virtual Network Computing) viewer从本地机器连接到服务器上使用xterm。在此之前，需要在你的本地机器上安装VNC viewer，在服务器上安装并启动VNC server。大多数linux发行版都预装了vnc-server软件包，所以我们可以直接运行vncserver命令。注意，第一次运行vncserver时会提示输入密码，用作VNC viewer从客户端连接时的密码。可以在VNC server机器上使用vncpasswd命令修改密码。
[root@tivf09 root]\# vncserver
New 'tivf09:1 (root)' desktop is tivf09:1
Starting applications specified in /root/.vnc/xstartup
Log file is /root/.vnc/tivf09:1.log
[root@tivf09 root]\#
[root@tivf09 root]\# ps -ef|grep -i vnc
root 19609 1 0 Jun05 ? 00:08:46 Xvnc :1 -desktop tivf09:1 (root)
-httpd /usr/share/vnc/classes -auth /root/.Xauthority -geometry 1024x768
-depth 16 -rfbwait 30000 -rfbauth /root/.vnc/passwd -rfbport 5901 -pn
root 19627 1 0 Jun05 ? 00:00:00 vncconfig -iconic
root 12714 10599 0 01:23 pts/0 00:00:00 grep -i vnc
[root@tivf09 root]\#

Vncserver是一个Perl脚本，用来启动Xvnc（X VNC server）。X client应用，比如xterm，VNC viewer都是和它通信的。如上所示，我们可以使用的DISPLAY值为tivf09:1。现在就可以从本地机器使用VNC viewer连接过去：
**VNC viewer：输入服务器**
![clip_image006](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252553128.jpg)
输入密码：
**VNC viewer：输入密码**
![clip_image007](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252555670.jpg)
登录成功，界面和服务器本地桌面上一样：
**VNC viewer**
![clip_image008](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252569672.jpg)
下面我们来修改RIM_Oracle_Agent.9i脚本，使它看起来像下面这样：
\#!/bin/sh
export DISPLAY=tivf09:1.0; xterm -e gdb RIM_Oracle_Agent.binary
如果你的程序在exec的时候还传入了参数，可以改成：
\#!/bin/sh
export DISPLAY=tivf09:1.0; xterm -e gdb --args RIM_Oracle_Agent.binary $@
最后加上执行权限
[root@tivf09 bin]\# chmod 755 RIM_Oracle_Agent.9i

现在就可以调试了。运行启动子进程的程序：
[root@tivf09 root]\# wrimtest -l 9i_linux
ResourceType: RIM
ResourceLabel: 9i_linux
Host Name : tivf09
User Name : mdstatus
Vendor : Oracle
Database : rim
Database Home : /data/oracle9i/920
Server ID : rim
Instance Home :
Instance Name :
Opening Regular Session...
程序停住了。从VNC viewer中可以看到，一个新的gdb xterm窗口在服务器端打开了
**gdb xterm 窗口**
![clip_image009](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252573326.jpg)
[root@tivf09 root]\# ps -ef|grep gdb
nobody 24312 24311 0 04:30 ? 00:00:00 xterm -e gdb RIM_Oracle_Agent.binary
nobody 24314 24312 0 04:30 pts/2 00:00:00 gdb RIM_Oracle_Agent.binary
root 24326 10599 0 04:30 pts/0 00:00:00 grep gdb

运行的正是要调试的程序。设置好断点，开始调试吧！
注意，下面的错误一般是权限的问题，使用 xhost 命令来修改权限：
**xterm 错误**
![clip_image010](http://images.cnblogs.com/cnblogs_com/ggjucheng/201112/201112151252576424.jpg)
[root@tivf09 bin]\# export DISPLAY=tivf09:1.0
[root@tivf09 bin]\# xhost +
access control disabled, clients can connect from any host
xhost + 禁止了访问控制，从任何机器都可以连接过来。考虑到安全问题，你也可以使用xhost + <你的机器名>。
**小结**
上述三种方法各有特点和优劣，因此适应于不同的场合和环境：
· follow-fork-mode方法：方便易用，对系统内核和GDB版本有限制，适合于较为简单的多进程系统
· attach子进程方法：灵活强大，但需要添加额外代码，适合于各种复杂情况，特别是守护进程
· GDB wrapper方法：专用于fork+exec模式，不用添加额外代码，但需要X环境支持（xterm/VNC）。
转自[http://www.ibm.com/developerworks/cn/linux/l-cn-gdbmp/](http://www.ibm.com/developerworks/cn/linux/l-cn-gdbmp/)
[http://www.cnblogs.com/ggjucheng/archive/2011/12/15/2288710.html](http://www.cnblogs.com/ggjucheng/archive/2011/12/15/2288710.html)
﻿﻿

