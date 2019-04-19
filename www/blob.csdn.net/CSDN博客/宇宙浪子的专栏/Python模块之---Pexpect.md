# Python模块之---Pexpect - 宇宙浪子的专栏 - CSDN博客
2016年03月03日 13:32:44[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：625
转自：[http://blog.csdn.net/lwnylslwnyls/article/details/8239791](http://blog.csdn.net/lwnylslwnyls/article/details/8239791)
### **概述**
*Pexpect* 是一个用来启动子程序并对其进行自动控制的 Python 模块,它可以用来和像 ssh、ftp、passwd、telnet 等命令行程序进行自动交互。
### 下载
Pexpect 可以从 SourceForge 网站下载。下载地址：http://sourceforge.net/projects/pexpect/
### 依赖
    Python 版本 2.4 或者 2.5
    pty module ，pty 是任何 Posix 系统标准库的一部分
由于其依赖 pty module ，所以 Pexpect 还不能在 Windows 的标准 [python](http://lib.csdn.net/base/11) 环境中执行，如果想在 Windows 平台使用，可以使用在 Windows 中运行 Cygwin 做为替代方案。
### run() 函数
函数 run 可以用来运行命令，其作用与 Python os 模块中 system() 函数相似。run() 是通过 Pexpect 类实现的。如果命令的路径没有完全给出，则 run 会使用 which 命令尝试搜索命令的路径 。
使用方法
**[python]**[view
 plain](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)[copy](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)
- import os,sys  
- from pexpect import *  
- print'run("ls") have not log!'
- run("ls")  
- print'logfile = run("ls") : log is in logfile!'
- log = run("ls")  
- print log,  
- print'run("ls",logfile=sys.stdout): log standard output'
- run("ls",logfile=sys.stdout)  
- 
- (command_output, exitstatus) = run ('ls -l /bin', withexitstatus=1)  #capture exit status
- run ('scp foo myname@host.example.com:.', events={'(?i)password': mypassword}) #很实用
### **spawn() 类**
#### **使用 Pexpect 启动子程序**
**[python]**[view
 plain](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)[copy](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)
- child = pexpect.spawn ('/usr/bin/ftp') #执行ftp客户端命令
- child = pexpect.spawn ('/usr/bin/ftp', []) #使用一个参数的列表
- fout = file('mylog.txt','w')  
- child.logfile = fout  #指定了 Pexpect 产生的日志的记录位置
- child.logfile = sys.stdout #log标准输出
- child.logfile_send = sys.stdout #不记录向子程序输入的日志，只记录子程序的输出
注意：Pexpect不支持管道，重定向或者通配符wild cards(如*)。如果需要使用，需重新打开一个shell
**[python]**[view
 plain](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)[copy](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)
- child = pexpect.spawn('/bin/bash -c "ls -l | grep LOG > log_list.txt"')  
#### **使用 Pexpect 控制子程序**
为了控制子程序，等待子程序产生特定输出，做出特定的响应，可以使用 expect 方法。
expect() 在执行中可能会抛出两种类型的异常分别是 EOF and TIMEOUF，其中 EOF 通常代表子程序的退出， TIMEOUT 代表在等待目标正则表达式中出现了超时。
##### **使用并捕获异常**
**[python]**[view
 plain](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)[copy](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)
- try:  
-     index = pexpect (['good', 'bad'])  
- if index == 0:  
-             do_something()  
- elif index == 1:  
-             do_something_else()  
- except EOF:  
-         do_some_other_thing()  
- except TIMEOUT:  
-     do_something_completely_different()  
##### **before 和after**
expect 不断从读入缓冲区中匹配目标正则表达式，当匹配结束时 pexpect 的 before 成员中保存了缓冲区中匹配成功处之前的内容， pexpect 的 after 成员保存的是缓冲区中与目标正则表达式相匹配的内容。
**[python]**[view
 plain](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)[copy](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)
- child = pexpect.spawn('/bin/ls /')  
- child.expect (pexpect.EOF)  
- print child.before  
此时 child.before 保存的就是在根目录下执行 ls 命令的结果。
##### send 系列函数
**[python]**[view
 plain](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)[copy](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)
- send(self, s)  
- sendline(self, s='')  
- sendcontrol(self, char)  
这些方法用来向子程序发送命令，模拟输入命令的行为。 与 send() 不同的是 sendline() 会额外输入一个回车符 ，更加适合用来模拟对子程序进行输入命令的操作。 当需要模拟发送 “Ctrl+c” 的行为时，还可以使用 sendcontrol() 发送控制字符。
##### sendcontrol
**[python]**[view
 plain](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)[copy](http://blog.csdn.net/lwnylslwnyls/article/details/8239791#)
- child.sendcontrol('c') #执行 ctrl+c
由于 send() 系列函数向子程序发送的命令会在终端显示，所以也会在子程序的输入缓冲区中出现，因此不建议使用 expect 匹配最近一次 sendline() 中包含的字符。否则可能会在造成不希望的匹配结果。
参考资料：http://www.ibm.com/developerworks/cn/linux/l-cn-pexpect1/
