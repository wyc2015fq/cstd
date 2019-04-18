# Core Dump和/proc调试 - Orisun - 博客园







# [Core Dump和/proc调试](https://www.cnblogs.com/zhangchaoyang/articles/1936125.html)





**Core Dump**

通过Core Dump是事后调试（postmortem debug）。

首先把Core文件的大小设置为1024

orisun@zcypc:~$ ulimit -c

0

orisun@zcypc:~$ ulimit -c 1024

注意此修改仅在当前shell有效

root@zcypc:/home/orisun# echo "/corefile/core-%e-%p-%t" > /proc/sys/kernel/core_pattern

所有的core文件将会存放到/corefile中，文件名格式为:core-命令名-pid-时间戳

我们写一个脚本来模拟Core Dump行为

#script that dumps core

kill -s SIGSRGV $$

保存为crash.sh

#chmod +x crash.sh

#./crash.sh

Segmentation fault

信号SIGSEGV是由内存访问越界、空指针等错误导致的段错误（segmentation fault）造成的。



调试Core文件可以用gdb [exec file] [core file]



**/proc**

/proc是个虚拟文件系统，它只存在于内在当中。可以通过/proc得到系统信息，也可通过它修改系统参数。

![](https://pic002.cnblogs.com/images/2011/103496/2011011510045849.png)


可以cat /proc/cpuinfo一下

core dump调试中提到的echo "/corefile/core-%e-%p-%t" > /proc/sys/kernel/core_pattern就是在修改/proc文件




orisun@zcypc:~$ ps aux | grep init

root         1  0.0  0.1   2808  1556 ?        Ss   08:27   0:00 /sbin/init

orisun    2993  0.0  0.0   3552   820 pts/0    S+   10:10   0:00 grep init




orisun@zcypc:~$ pstree

init─┬─NetworkManager

     ├─acpid

     ├─apturl───apturl-gtk─┬─gksu───synaptic─┬─synaptic───dpkg───frontend───fla+

     │                     │                 └─{synaptic}

     │                     └─{apturl-gtk}

     ├─atd

     ├─avahi-daemon───avahi-daemon

     ├─bonobo-activati───{bonobo-activat}

     ├─chrome─┬─chrome

     │        ├─chrome───8*[{chrome}]

     │        └─27*[{chrome}]

     ├─chrome─┬─7*[chrome───{chrome}]

     │        └─chrome───4*[{chrome}]




fuser查看正在访问某个或某些文件（或目录）的进程ID，比如查看哪些进程使用了动态共享库ld-2.11.1.so


orisun@zcypc:~$ fuser -v /lib/ld-2.3.3.so

Specified filename /lib/ld-2.3.3.so does not exist.

orisun@zcypc:~$ fuser -v /lib/ld-2.11.1.so

                     用户     进程号 权限   命令

/lib/ld-2.11.1.so:   orisun     1336 ....m gnome-keyring-d

                     orisun     1354 ....m gnome-session

                     orisun     1392 ....m dbus-launch

                     orisun     1393 ....m dbus-daemon

                     orisun     1395 ....m fcitx

                     orisun     1397 ....m gconfd-2


lsof与fuser正好相反，它查看某个程序或进程打开了哪些文件。

lsof -c init



**跟踪（Trace）工具**

backtrace查看栈信息

strace用来跟踪和监视程序的每一个系统调用

ltrace用来跟踪和监视程序的每一个C/C++库函数调用

LTT（Linux Trace Toolkit）是一个内核补丁及一套相关工具，用来追踪内核的各种事件如进程的创建、挂起和调试，文件的读写，内存的管理等，并且提供数据分析工具，分析结果可以以图形化方式展示出来。













