# Linux命令行技巧 - 长歌行 - CSDN博客





2012年03月22日 09:24:36[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：463标签：[linux																[bash																[正则表达式																[工具																[工作																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)




[http://cloud.csdn.net/a/20120321/313378.html](http://cloud.csdn.net/a/20120321/313378.html)



下面我将和大家分享一些学习Linux命令行的技巧，希望能对大家有所助益。如果在本文中遇到不太熟悉的命令，可以在终端中键入以下命令寻求帮助。
- Man <command> 

![](http://articles.csdn.net/uploads/allimg/120321/120_120321144029_1.jpg)

Where there's a shell, there's a way.

**Linux入门必备**
- 了解bash：没有必要钻研长篇累牍的bash使用手册，通过man bash学习即可。 
- 了解vi：你在工作中也许会使用Emacs或Eclipse，但vi不可替代。 
- 了解ssh：学习无密码验证的基础知识。 
- bash作业管理基础知识：如使用&、Ctrl-C、fg、bg、Ctrl-Z、jobs和kill等。

- 文件管理命令：如硬链接和软链接，权限等。 
- 网络管理命令：如ifconfig等。 
- 如何使用正则表达式，如何使用grep、sed等。 
- 学习使用apt-get和yum管理软件包。 

**提升日常工作效率 **
- 在bash中，可以使用Ctrl+R命令搜索命令行历史记录。 
- 在bash中，可以使用Ctrl+W删除最后一个单词，使用Ctrl+U删除整行。 
- 使用cd -命令返回之前的工作目录，使用cd ..命令返回上层目录。 
- 学习如何使用xargs：
- $ find . -name *.py | xargs grep some_function  
- $ cat hosts | xargs -I{} ssh root@{} hostnameX 
- 使用pstree -p命令查看进程树。 
- 了解不同的信号。例如，使用kill -STOP [PID]终止进程。详细的信号列表可以在man 7 signal中查看。

- 如果你想让进程永远在后台运行，可以使用nohup和disown命令。 
- 使用netstat -lntp命令查看侦听进程，并参考lsof命令。 
- 在bash脚本中使用subshells对命令进行分组：
- #do something in current dir 
- (cd /some/other/dir; other-command)  
- #continue in original dir
- 字符串修剪（Trimming Strings）：${var%suffix}和${var#prefix}。例如if var=foo.pdf then echo ${var%.pdf}.txt prints "foo.txt"。

- 命令的输出可以当做文件使用。例如，比较本地和远程的/etc/hosts文件： 
- diff /etc/hosts <(ssh somehost cat /etc/hosts) 
- 了解bash中的“here documents”。 
- 了解标准输出和标准错误重定向。 
- 了解ASCII代码表。可以在终端中运行man ascii命令作为参考。 
- 当通过ssh远程工作时，可以使用screen 或 dtach保存回话。 
- 对Web开发者而言，curl、wget等命令非常实用。 
- 将HTML页面转化文本文件：
- lynx -dump -stdin 
- 如果要掌握XML，xmlstarlet命令是非常好的工具。
- 了解ssh端口映射。
- 使用Alt+shift+3快捷键可以在行前添加注释“#”。

**数据处理**
- 了解sort和uniq命令。
- 了解cut、paste和join命令。
- 了解如何对文本文件做交集、并集和差集：
- cat a b | sort | uniq > c # c is a union b  
- cat a b | sort | uniq -d > c # c is a intersect b  
- cat a b b | sort | uniq -u > c # c is set difference a – b 
- 对文本文件的第二列做摘要，下面的代码要比同样的Python代码更精炼、快速（3倍）：
- awk '{ x += $2 } END { print x }'
- 了解字符串和grep命令。 
- 文件分割：了解split（按大小分割）和csplit（按匹配分割）命令。

**系统诊断**
- 使用iostat、netstat、top和dstat命令查看磁盘、处理器和网络的状态。 
- 使用free、vmstat等命令了解系统内存状态。 
- 使用mtr网络诊断工具。 
- 使用iftop、nethogs等工具查看占用带宽的进程和带宽。 
- 使用ab工具快速洞悉Web服务器的性能。 
- 以wireshark和tshark命令应对复杂网络诊断。 
- 学习如何使用strace，并用于诊断运行的进程。这对分析程序运行错误的原因，非常有帮助。 
- 使用ldd命令查看共享库文件。 
- 了解如何使用gdb连接到运行中的进程，并进行堆栈跟踪。 
- 有关/proc的知识非常重要。 
- 如何诊断已发生的错误？Sar命令是一种很好的选择，它可以收集、保存并报告系统信息。 

**欢迎分享**

我想，应该会有许多技巧，本文尚未提及。希望大家在评论中与我共同分享。（张志平/编译）

**原文链接：**

[Linux Command Line tips that every Linux user should know](http://www.linuxstall.com/linux-command-line-tips-that-every-linux-user-should-know/)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)](https://so.csdn.net/so/search/s.do?q=bash&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




