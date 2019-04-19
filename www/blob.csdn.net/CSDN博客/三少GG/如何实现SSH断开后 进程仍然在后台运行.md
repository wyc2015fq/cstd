# 如何实现SSH断开后 进程仍然在后台运行 - 三少GG - CSDN博客
2013年11月15日 12:57:39[三少GG](https://me.csdn.net/scut1135)阅读数：910
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)

解决方法：
1.nohup命令
功能：不挂断地运行命令，忽略HUP信号。
语法：nohup
 command &
实例：
- 
[root@DigMouse ~]# nohup ping 51osos.com > /dev/null &
- 
[1] 13683
- 
[root@DigMouse ~]# nohup: ignoring input and redirecting stderr to stdout
- 
[root@DigMouse ~]# ls
- 
Desktop    Downloads  nohup.out
- 
Documents  Music      Public    Videos 
- 
[root@DigMouse ~]# cat nohup.out
- 
PING cd447.gotoip.net (122.225.57.246) 56(84) bytes of data.
- 
64 bytes from 122.225.57.246: icmp_seq=1 ttl=56 time=48.6 ms
- 
64 bytes from 122.225.57.246: icmp_seq=2 ttl=56 time=47.8 ms
- 
64 bytes from 122.225.57.246: icmp_seq=3 ttl=56 time=49.9 ms
- 
64 bytes from 122.225.57.246: icmp_seq=4 ttl=56 time=49.5 ms
复制代码
- 
[root@DigMouse ~]# ps -ef | grep ping
- 
root     13683 13655  0 09:33 pts/0    00:00:00 ping 51osos.com
- 
root     13687 13655  0 09:36 pts/0    00:00:00 grep ping
- 
[root@DigMouse ~]#
复制代码
关闭此终端，打开另一个终端使用ps命令，仍然可以查看到ping进程。 无论是否将 nohup 命令的输出重定向到终端，输出都将附加到当前目录的 nohup.out 文件中。如果当前目录的 nohup.out 文件不可写，输出重定向到 $HOME/nohup.out 文件中。
#nohup
 command > command.out 2>&1 &
上面的例子中nohup
 command输出的内容输出到了command.out文件中，错误内容输出到了标准输出。
