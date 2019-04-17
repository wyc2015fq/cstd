# 鸟哥的私房菜：Bash shell(一)-Bash shell功能简介 - 在思索中前行！ - CSDN博客





2014年09月24日 20:19:14[_Tham](https://me.csdn.net/txl16211)阅读数：649







      Bash shell系列里，由变量谈起，先讲到环境变量的功能与修改的问题， 然后会继续提到历史指令的运用。接下来，就会谈一下『数据流重导向』这个重要概念，
 最后就是管线命令的利用！


## 一  Bash shell简介

## 管理整个硬件的其实是核心 (kernel)，一般使用者 (user) 则是以 shell 来跟核心沟通～ 
我们常常提到的 shell 其实是比较狭隘的定义，一般来说，在 Linux 里头，所谓的 shell 就是指 BASH 这个文字模式的 shell 。但是，广义的 shell 也可以是 KDE 之类的图形接口控制软件呢！ 因为他也可以帮我们与
 kernel 进行沟通。
那么我们来了解一下 Linux 使用的是哪一个 shell 呢？检查一下 /etc/shells 这个档案，至少就有底下这几个可以用的 shells：
    /bin/sh (已经被 /bin/bash 所取代)
    /bin/bash (就是 Linux 预设的 shell)
    /bin/ksh (Kornshell 由 AT&T Bell lab. 发展出来的，兼容于 bash)
    /bin/tcsh (整合 C Shell ，提供更多的功能)
    /bin/csh (已经被 /bin/tcsh 所取代)
    /bin/zsh (基于 ksh 发展出来的，功能更强大的 shell)
我这个使用者预设会取得哪一个 shell 啊？！

还记得我们在 首次进入 Linux -- 以文字方式登入 那个章节当中提到的登入动作吧？当我登入的时候，系统就会给我一个 shell 让我来工作了。 而这个登入取得的 shell 就记录在 /etc/passwd 这个档案内！这个档案的内容是啥？
[root@linux ~]# cat /etc/passwd
root:x:0:0:root:/root:/bin/bash
bin:x:1:1:bin:/bin:/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
.....(中间省略).....


## 二  Bash shell 功能简介
chap1    BASH 主要的优点有底下几个：

1) 命令编修能力（类似 DOS 的 doskey 功能）：
『记忆使用过的指令！』 指令列按『上下键』就可以找到前一个输入的指令！记录的档案在你的家目录的.bash_history ！ ~/.bash_history 记录的是前一次登入以前所执行过的指令，
 这一次登入所执行的指令都被暂存在暂内存中，当您成功的注销系统后，该指令记忆才会记录到 .bash_history 当中！ 

2) 命令与档案补全功能：
    [Tab] 接在一串指令的第一个字的后面，则为命令补全；
    [Tab] 接在一串指令的第二个字以后时，则为『档案补齐』！
    如果我想要知道我的环境中，所有可以执行的指令有几个？ 就直接在 bash 的提示字符后面输入两个 [tab][tab] 就能够输出所有的可执行指令了。 

3) 命令别名(alias)设定功能：
    alias lm='ls -al'

4) 工作控制(jobs)、前景背景控制：
    资源管理章节中会再提及

5) Shell scripts 的强大功能：

在 DOS 年代还记得将一堆指令写在一起的所谓的『批次档』吧？在 Linux 底下的 shell scripts 则发挥的更为强大的功能，可以将您日常生活当中常需要下达的连续指令写成一个档案， 该档案并且可以透过对谈交互式的方式来进行主机的侦测工作！整个设计下来几乎就是一个小型的程序语言了！以前在
 DOS 底下需要程序语言才能写的东西，在 Linux 底下使用简单的 shell scripts 就可以帮你达成了

6) 万用字符*

举例来说，想要知道 /usr/X11R6/bin 底下有多少以 xt 为开头的档案吗？ls -l /usr/X11R6/bin/xt* 就能够知道啰～此外，还有其它可供利用的万用字符， 这些都能够加快使用者的操作.

chap2  内建命令： type

为了方便 shell 的操作，其实 bash 已经『内建』了很多指令了，例如 cd ， 还有例如 umask 等等的指令， 那我怎么知道这个指令是来自于外部指令(指的是其它非 bash 套件所提供的指令) 或是内建在 bash 当中的呢:

[root@linux ~]# type [-tpa] name

参数：
：不加任何参数时，则 type 会显示出那个 name 是外部指令还是 bash 内建的指令！
-t ：当加入 -t 参数时，type 会将 name 以底下这些字眼显示出他的意义：
file ：表示为外部指令；
alias ：表示该指令为命令别名所设定的名称；
builtin ：表示该指令为 bash 内建的指令功能；
-p ：如果后面接的 name 为指令时，会显示完整文件名(外部指令)或显示为内建指令；
-a ：会将由 PATH 变量定义的路径中，将所有含有 name 的指令都列出来，包含 alias

范例一：查询一下 ls 这个指令是否为 bash 内建？
[root@linux ~]# type ls
ls is aliased to `ls --color=tty'
# 没有加上任何参数，仅列出 ls 这个指令的最主要使用情况
[root@linux ~]# type -t ls
alias
# -t 参数则仅列出 ls 这个指令的最主要使用情况说明
[root@linux ~]# type -a ls
ls is aliased to `ls --color=tty'
ls is /bin/ls
# 利用所有方法找出来的 ls 相关信息都会被列出来！

范例二：那么 cd 呢？
[root@linux ~]# type cd
cd is a shell builtin


