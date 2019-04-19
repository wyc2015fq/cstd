# Linux下使Shell 命令脱离终端在后台运行 - xqhrs232的专栏 - CSDN博客
2017年06月22日 15:37:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：299
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.sina.com.cn/s/blog_605f5b4f0100x28u.html](http://blog.sina.com.cn/s/blog_605f5b4f0100x28u.html)

你是否遇到过这样的情况：从终端软件登录远程的Linux主机，将一堆很大的文件压缩为一个.tar.gz文件，连续压缩了半个小时还没有完成，这时，突然你断网了，你登录不上远程Linux主机了，那么前面的半个小时就会前功尽弃，你非常气愤……
在Linux下，如果你要执行的shell命令耗时特别长，并且：（1）你的网络不稳定，随时可能断网；或者（2）你在执行了shell命令之后必须要关闭终端软件（例如SecureCRT）。
那么你就需要以脱离终端的方式在后台运行这个shell命令。
方法如下：
（1）输入命令：
**nohup 你的shell命令 &**
（2）回车，使终端回到shell命令行；
（3）输入exit命令退出终端：
exit
（4）现在可以关闭你的终端软件了，等过足够的时间，让你的shell命令执行完了再上去看结果吧。
其中，nohup命令可以让你的shell命令忽略SIGHUP信号，即可以使之脱离终端运行；“&”可以让你的命令在后台运行。
以脱离终端的方式在后台运行shell命令有这样几个好处：只要你执行过了命令，那么你的网络中断不会对你有任何影响，并且你就可以关闭终端软件了。
本篇文章来源于 Linux公社网站([www.linuxidc.com](http://www.linuxidc.com/))  原文链接：[http://www.linuxidc.com/Linux/2011-05/35723.htm](http://www.linuxidc.com/Linux/2011-05/35723.htm)
用运程终端登陆Linux后运行的程序，当关闭终端时程序也被终至，下面的方法可以让程序在后台运行。
Unix/Linux下一般比如想让某个程序在后台运行，**很多都是使用 & 在程序结尾来让程序自动运行**。比如我们要运行mysql在后台：
　　/usr/local/mysql/bin/mysqld_safe --user=mysql &
**但是加入我们很多程序并不象mysqld一样做成守护进程**，**可能我们的程序只是普通程序而已，一般这种程序使用 & 结尾，但是如果终端关闭，那么程序也会被关闭。**但是为了能够后台运行，那么我们就可以使用nohup这个命令，比如我们有个test.php需要在后台运行，并且希望在后台能够定期运行，那么就使用nohup：
　　nohup /root/test.php &
　　提示：
　　[~]$ appending output to nohup.out
　　嗯，证明运行成功，同时把程序运行的输出信息放到当前目录的 nohup.out 文件中去。
附：nohup命令参考
　　nohup 命令
　　用途：不挂断地运行命令。
　　语法：nohup Command [ Arg ... ] [　& ]
　　描述：nohup 命令运行由 Command 参数和任何相关的 Arg 参数指定的命令，**忽略所有挂断（SIGHUP）信号。**在注销后使用 nohup 命令运行后台中的程序。要运行后台中的 nohup 命令，添加 & （ 表示“and”的符号）到命令的尾部。
**无论是否将 nohup 命令的输出重定向到终端，输出都将附加到当前目录的 nohup.out 文件中。**如果当前目录的 nohup.out 文件不可写，输出重定向到 $HOME/nohup.out 文件中。如果没有文件能创建或打开以用于追加，那么 Command 参数指定的命令不可调用。如果标准错误是一个终端，那么把指定的命令写给标准错误的所有输出作为标准输出重定向到相同的文件描述符。
　　退出状态：该命令返回下列出口值：
　　126 可以查找但不能调用 Command 参数指定的命令。
　　127 nohup 命令发生错误或不能查找由 Command 参数指定的命令。
　　否则，nohup 命令的退出状态是 Command 参数指定命令的退出状态。
nohup命令及其输出文件
　　nohup命令：如果你正在运行一个进程，而且你觉得在退出帐户时该进程还不会结束，那么可以使用nohup命令。**该命令可以在你退出帐户/关闭终端之后继续运行相应的进程。nohup就是不挂起的意思( no hang up)。**
　　该命令的一般形式为：nohup command &
使用nohup命令提交作业
　　如果使用nohup命令提交作业，那么在缺省情况下该作业的所有输出都被重定向到一个名为nohup.out的文件中，**除非另外指定了输出文件：**
**　　nohup command > myout.file 2>&1 &**
　　在上面的例子中，输出被重定向到myout.file文件中。

