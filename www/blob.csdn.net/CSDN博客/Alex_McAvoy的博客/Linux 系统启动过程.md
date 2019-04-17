# Linux 系统启动过程 - Alex_McAvoy的博客 - CSDN博客





2018年06月10日 22:17:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[Linux 学习笔记](https://blog.csdn.net/u011815404/article/category/7715914)









### 【概述】



Linux 启动时会看到许多启动信息，但 Linux系统的启动过程并没有想象中的复杂，其过程大致可分为7个阶段：
- 内核的引导
- 运行 init
- 系统初始化
- 建立终端 
- 用户登录系统
- 图形模式与文字模式的切换方式

- 关机

### 【内核引导】

当计算机打开电源启动后，首先进行的是BIOS开机自检，即：按照BIOS中设置的启动设备（通常是硬盘）来启动。

操作系统接管硬件以后，首先读入 /boot 目录下的内核文件，这个过程即为内核引导过程。

![](https://img-blog.csdn.net/20180610215852791)


### 【运行init】

#### 1.init文件类型

SysV: init, CentOS 5之前, 配置文件： /etc/inittab

Upstart: init,CentOS 6, 配置文件： /etc/inittab, /etc/init/*.conf

Systemd： systemd, CentOS 7,配置文件： /usr/lib/systemd/system、 /etc/systemd/system


注：init 进程是系统所有进程的起点，没有这个进程，系统中任何进程都不会启动，init 程序首先读取的配置文件是 /etc/inittab

![](https://img-blog.csdn.net/20180610220114439)


#### 2.运行级别



许多程序需要开机启动，这些程序在 Windows 中叫做"服务"（service），在 Linux 中叫做"守护进程"（daemon），init 进程的一大任务，就是去运行这些开机启动的程序。

不同的场合需要启动不同的程序（比如用作服务器时，需要启动 Apache，用作桌面则不需要），Linux 允许为不同的场合，分配不同的开机启动程序，这就叫做"运行级别"（runlevel），即：启动时根据"运行级别"，确定要运行的程序。
![](https://img-blog.csdn.net/20180610220122535)

Linux 系统有7个运行级别（runlevel）：
- 运行级别0：系统停机状态，系统默认运行级别不能设为0，否则不能正常启动
- 运行级别1：单用户工作状态，root权限，用于系统维护，禁止远程登陆
- 运行级别2：多用户状态（没有NFS）
- 运行级别3：完全的多用户状态（有NFS），登陆后进入控制台命令行模式
- 运行级别4：系统未使用，保留
- 运行级别5：X11控制台，登陆后进入图形GUI模式
- 运行级别6：系统正常关闭并重启，默认运行级别不能设为6，否则不能正常启动

### 【系统初始化】



在 init 的配置文件中，存在这么一行：si::sysinit:/etc/rc.d/rc.sysinit ，它调用执行了 /etc/rc.d/rc.sysinit ，而rc.sysinit 是一个 bash shell 的脚本，主要用于完成系统初始化的工作，rc.sysinit 是每一个运行级别都要首先运行的重要脚本。

它主要完成的工作有：激活交换分区、检查磁盘、加载硬件模块等需要优先执行的任务。
`l5:5:wait:/etc/rc.d/rc 5`
这一行表示以5为参数运行 /etc/rc.d/rc ，/etc/rc.d/rc 是一个 Shell 脚本，它接受5作为参数，去执行 /etc/rc.d/rc5.d/ 目录下的所有的 rc 启动脚本，/etc/rc.d/rc5.d/ 目录中的这些启动脚本实际上都是一些连接文件，而不是真正的 rc启动脚本，真正的 rc启动脚本 实际上都是放在 /etc/rc.d/init.d/ 目录下。

而这些 rc启动脚本 有着类似的用法，它们一般能接受 start、stop、restart、status 等参数。

/etc/rc.d/rc5.d/ 中的 rc启动脚本 通常是 K 或 S 开头的连接文件，对于以 S 开头的启动脚本，将以 start参数 来运行。

如果发现存在相应的脚本也存在 K 开头的连接，而且已经处于运行态了（以 /var/lock/subsys/ 下的文件作为标志），则首先以 stop 为参数停止这些已经启动的守护进程，然后再重新运行。这样做是为了保证是当 init 改变运行级别时，所有相关的守护进程都将重启。
至于在每个运行级中将运行哪些守护进程，用户可通过 chkconfig 或 setup 中的"System Services"来自行设定。

![](https://img-blog.csdn.net/20180610220622685)

### 【建立终端】



rc 执行完毕后，返回 init，此时基本系统环境已经设置好了，各种守护进程也已启动了，init 接下来会打开6个终端，以便用户登录系统。

在 inittab 中的以下6行就是定义了6个终端：

```
1:2345:respawn:/sbin/mingetty tty1
2:2345:respawn:/sbin/mingetty tty2
3:2345:respawn:/sbin/mingetty tty3
4:2345:respawn:/sbin/mingetty tty4
5:2345:respawn:/sbin/mingetty tty5
6:2345:respawn:/sbin/mingetty tty6
```

从上面可以看出在2、3、4、5的运行级别中都将以 respawn 方式运行 mingetty 程序。

mingetty 程序能打开终端、设置模式，同时它会显示一个文本登录界面，这个界面就是我们经常看到的登录界面，在界面中会提示用户输入用户名，而用户输入的用户将作为参数传给 login 程序来验证用户的身份。

### 【用户登录系统】



一般来说，用户的登录方式有三种：


- 命令行登录
- SSH（Secure Shell）登录
- 图形界面登录

![](https://img-blog.csdn.net/20180610222134731)


对于运行级别为5的图形方式用户来说，登录是通过一个图形化的登录界面，登录成功后可以直接进入 KDE、Gnome 等窗口管理器。

对于以文本方式登录的情况：当我们看到 mingetty 的登录界面时，就可以输入用户名和密码来登录系统。Linux 的账号验证程序是 login，login 会接收 mingetty 传来的用户名作为用户名参数，然后 login 会对用户名进行分析，如果用户名不是 root，且存在 /etc/nologin 文件，login 将输出 nologin 文件的内容，然后退出。

以文本方式登录通常用于系统维护时防止 非root用户 登录，只有在 /etc/securetty 中登记了的终端才允许 root用户 登录，如果不存在这个文件，则 root用户 可以在任何终端上登录。

### 【图形模式与文字模式的切换方式】



Linux预设提供了六个命令窗口终端机用于登录。

这个六个窗口分别为 tty1，tty2 … tty6，可以按下 Ctrl + Alt + F1 ~ F6 来切换，默认登录的就是第一个窗口，也就是 tty1。

如果安装了图形界面，默认情况下是直接进入图形界面，此时可以按 Ctrl + Alt + F1 ~ F6 来进入其中一个命令窗口界面，当进入命令窗口界面后再返回图形界面只要按下 Ctrl + Alt + F7 就可以返回。

如果你用的 VMware虚拟机，命令窗口切换的快捷键为 Alt + Space + F1~F6，如果在图形界面下按 Alt + Shift + Ctrl + F1~F6 来切换至命令窗口。

![](https://img-blog.csdn.net/20180610222732415)


### 【关机】



Linux 大多用在服务器上，很少遇到关机的操作，因为在服务器上跑一个服务是永无止境的，除非特殊情况下不得已才关机。

正确的关机流程为：sync > shutdown > reboot > halt

关机指令为：shutdown （具体的可以用 man shutdown 来看帮助文档）

例如可以运行如下命令关机：

```
sync 将数据由内存同步到硬盘中。
shutdown –h 10 ‘This server will shutdown after 10 mins’ 这个命令告诉大家，计算机将在10分钟后关机，并且会显示在登陆用户的当前屏幕中。
Shutdown –h now 立马关机
Shutdown –h 20:25 系统会在今天20:25关机
Shutdown –h +10 十分钟后关机
Shutdown –r now 系统立马重启
Shutdown –r +10 系统十分钟后重启
reboot 就是重启，等同于 shutdown –r now
halt 关闭系统，等同于shutdown –h now 和 poweroff
```

注：


- 无论是重启系统还是关闭系统，首先都要运行 sync 命令，将内存中的数据写到磁盘中。
- 常用的关机命令有 **shutdown –h now halt poweroff** 和 **init 0**
- 常用的重启命令有 **shutdown –r now reboot init 6**



