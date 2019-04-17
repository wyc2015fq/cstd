# Ubuntu终端命令查看端口占用及关闭 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月12日 20:49:14[boonya](https://me.csdn.net/boonya)阅读数：5221








## 查看服务端口

查看已经连接的服务端口（ESTABLISHED）：netstat -a


查看所有的服务端口（LISTEN，ESTABLISHED）： netstat -ap

## 查看指定端口

查看指定端口，可以结合grep命令：netstat -ap | grep 8080         


 也可以使用lsof命令：lsof -i:8080

## 关闭端口使用

若要关闭使用这个端口的程序，使用kill + 对应的pid:：   kill -9 PID号

*ps：kill就是给某个进程id发送了一个信号。默认发送的信号是SIGTERM，而kill -9发送的信号是SIGKILL，即exit。exit信号不会被系统阻塞，所以kill -9能顺利杀掉进程。*
注：此命令在centos下同样适用。



