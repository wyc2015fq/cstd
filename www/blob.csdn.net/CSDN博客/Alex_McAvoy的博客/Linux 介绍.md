# Linux 介绍 - Alex_McAvoy的博客 - CSDN博客





2018年06月05日 22:51:17[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：425








# 【Linux 是什么】

Linux 是一个操作系统，如同 Windows、Mac OS。

操作系统在整个计算机系统中，是在硬件→内核→系统调用→应用程序体系里负责内核→系统调用模块，但直观的看，操作系统还包含一些在其上运行的应用程序，比如文本编辑器、浏览器、电子邮件等。


![](https://img-blog.csdn.net/20180605225720958)


# 【Linux 与 Windows 的区别】

目前国内Linux更多的是应用于服务器上，而桌面操作系统更多使用的是 Windows。


![](https://img-blog.csdn.net/20180607235304237)


# 【Linux 桌面环境】



相对于Windows 系统，Linux 本身是没有图形界面的，在 Linux 发行版上看到的图形界面实质上都是运行在 Linux 系统之上的软件，类似 Windows95 之前的 Windows 的图形界面只是运行在 DOS 环境的一套软件。

而 Linux 上的这套软件以前是 XFree86，现在是 xorg（X.Org），这套软件是通过 X 窗口系统（X Window System，常被称为 X11 或 X）实现的，其本身只是工具包及架构协议，而 xorg 是 X 架构规范的一个实现体，即：它是实现了 X 协议规范的一个提供图形界面服务的服务器，就像实现了 http 协议提供 web 服务的 Apache 。

如果只有服务器则不能实现一个完整的桌面环境的，还需要一个客户端，我们称为 X Client，如目前最流行的实现客户端功能的桌面环境 KDE，GNOME，XFCE，LXDE等。

这也意味着在 Linux 上你可以自己选择安装不同的桌面环境，甚至可以定制自己的专属桌面。

![](https://img-blog.csdn.net/20180605231450841)



# 【终端】



终端（Terminal）也称终端设备，是计算机网络中处于网络最外围的设备，主要用于用户信息的输入以及处理结果的输出等。

Linux系统为实现在图形窗口中完成用户输入和显示输出，提供了一个叫做终端模拟器的程序。

常见的终端模拟器有： gnome-terminal、Konsole、xterm、rxvt、kvt、nxterm、eterm 。

![](https://img-blog.csdn.net/20180703022204373?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


终端本质上是对应着 Linux 上的 /dev/tty 设备，Linux 的多用户登陆就是通过不同的 /dev/tty 设备完成的，Linux 默认提供了 6 个纯命令行界面的 “terminal”（准确的说是 6 个 virtual consoles）让用户登录。

在物理机系统上可以通过[Ctrl]+[Alt]+[F1]～[F6]进行切换，当切换到其中一个终端后想要切换回图形界面，可以按下[Ctrl]+[Alt]+[F7]来完成。

# 【Shell（壳）】

在使用 Linux 时，并不是直接与系统打交道，而是通过一个叫做 Shell（壳） 的中间程序来完成的。

普通意义上的 Shell 是指可接受用户输入命令的程序，之所以被称作 Shell 是因为它隐藏了操作系统底层的细节。同样的 UNIX/Linux 下的图形用户界面 GNOME 和 KDE，有时也被叫做“虚拟 shell”或“图形 shell”。

有壳就有核，这里的核就是指 UNIX/Linux 内核，Shell 是指“提供给使用者使用界面”的软件（命令解析器），类似于 DOS 下的 command（命令行）和后来的 cmd.exe 。

综上， Shell 既是用户交互的界面，也是控制系统的脚本语言。

# 【基本操作】

## 1.输入与输出



在 Linux 中，最重要的就是命令，这就包含了2个过程：输入与输出。

输入，就是打开终端，然后按键盘输入，最后按回车。

输出，如果是查看，会返回你想要的结果，比如返回文件的内容；如果是执行，执行成功会没有输出，执行失败会告诉你哪里错了。



## 2.重要快捷键


|按键|作用|
|----|----|
|Tab|可得到提示，用于补全命令、目录、命令参数|
|Ctrl+c|强行终止当前程序|
|Ctrl+d|键盘输入结束或退出终端|
|Ctrl+s|暂停当前程序，暂停后按任意键继续|
|Ctrl+k|删除从光标所在位置到行末|
|Ctrl+a|将光标移至行头|
|Ctrl+e|将光标移至行末|
|Alt+Backspace|向前删除一个单词|
|Shieft+Pgup|将终端显示向上移动|
|Shieft+Pgdn|将终端显示向下移动|
3.通配符
通配符是一种特殊语句，主要有星号（*）和问号（?），用来对字符串进行模糊匹配（比如文件名、参数名）。

当查找文件夹时，不知道真正字符或者懒得输入完整名字时，常常使用通配符代替一个或多个真正字符。

终端里面输入的通配符是由 Shell 处理的，不是由所涉及的命令语句处理的，它只会出现在命令的“参数值”里（它不能出现在命令名称里， 命令不记得，那就用Tab补全）。

当 Shell 在“参数值”中遇到了通配符时，Shell 会将其当作路径或文件名在磁盘上搜寻可能的匹配：若符合要求的匹配存在，则进行代换（路径扩展）；否则就将该通配符作为一个普通字符传递给“命令”，然后再由命令进行处理。

总之，通配符实际上就是一种 Shell 实现的路径扩展功能。在通配符被处理后， Shell 会先完成该命令的重组，然后继续处理重组后的命令，直至执行该命令。

Shell 常用通配符
|字符|含义|
|----|----|
|*|匹配0或多个字符|
|？|匹配任一字符|
|[list]|匹配list中的任意单一字符|
|[!list]|匹配除list中的任意单一字符以外的字符|
|[c1-c2]|匹配c1-c2中的任意单一字符，如：[0-9]、[a-z]|
|{string1,string2,...}|匹配 string1 或 string2 (或更多)其一字符串|
|{c1..c2}|匹配 c1-c2 中全部字符，如{1..10}|
4.实例
常用的三个命令：touch 创建一个文件；cd 进入一个目录；pwd 查看当前目录

1）创建一个名为 test 的空文件

![](https://img-blog.csdn.net/20180703021524581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


2）进入 /dev 目录

![](https://img-blog.csdn.net/201807030216185?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


3）查看当前目录

![](https://img-blog.csdn.net/20180703021650249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


4）使用通配符一次创建多个文本文件

![](https://img-blog.csdn.net/20180703022030613?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 【内建命令与外部命令】



## 1.内建命令

内建命令属于 shell 程序的一部分，其中包含的是一些比较简单的 Linux 系统命令（写在 bash 源码里 builtins 的 .def 中），由 shell 程序识别并在 shell 程序内部完成运行。

shell 通常在 Linux 系统加载运行时，就被加载并驻留在系统内存中，且解析内部命令 shell 不需要创建子进程，因此其执行速度比外部命令快。

常见内建命令有：history、cd、exit 等。

## 2.外部命令

外部命令是 Linux 系统中的实用程序部分，因为实用程序的功能通常都比较强大，所以其包含的程序量也会很大，在系统加载时并不随系统一起被加载到内存中，而是在需要时才将其调入内存。

外部命令虽然其不包含在 shell 中，但是其命令执行过程是由 shell 程序控制的。

外部命令是在 Bash 之外额外安装的，通常放在 /bin，/usr/bin，/sbin，/usr/sbin 等目录中。

常见外部命令有：ls、vi等。

## 3.内建命令与外部命令的区分

1）使用 type 命令可以来区分内建命令与外部命令。

2）语法：**type 要判断的命令名**

3）命令结果

① 当显示结果为：xxx is a shell builtin 时，说明 xxx 命令是内建命令。

例：![](https://img-blog.csdn.net/20180703023604164?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

② 当显示结果为：xxx is /usr/bin/xxx 时，说明 xxx 命令是外部命令。

例：![](https://img-blog.csdn.net/20180703023616451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

③ 当显示结果为：xxx is an alias for xxx --xx 时，说明 xxx 命令为 xxx --xx 命令的别名。

例：![](https://img-blog.csdn.net/20180703023632935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






