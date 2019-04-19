# 每天一个 Linux 命令（15）：tail 命令 - 文章 - 伯乐在线
原文出处： [peida](http://www.cnblogs.com/peida/archive/2012/11/07/2758084.html)
tail 命令从指定点开始将文件写到标准输出.使用tail命令的-f选项可以方便的查阅正在改变的日志文件,tail -f filename会把filename里最尾部的内容显示在屏幕上,并且不但刷新,使你看到最新的文件内容.
## 1．命令格式：
> 
tail[必要参数][选择参数][文件]
## 2．命令功能：
用于显示指定文件末尾内容，不指定文件时，作为输入信息进行处理。常用查看日志文件。
## 3．命令参数：
> 
-f 循环读取
-q 不显示处理信息
-v 显示详细的处理信息
-c<数目> 显示的字节数
-n<行数> 显示行数
–pid=PID 与-f合用,表示在进程ID,PID死掉之后结束.
-q, –quiet, –silent 从不输出给出文件名的首部
-s, –sleep-interval=S 与-f合用,表示在每次反复的间隔休眠S秒
## 4．使用实例：
实例1：显示文件末尾内容
命令：


```
tail -n 5 log2014.log
```
输出：


```
[root@localhost test]# tail -n 5 log2014.log 
2014-09
2014-10
2014-11
2014-12
==============================[root@localhost test]#
```
说明：
显示文件最后5行内容
实例2：循环查看文件内容
命令：


```
tail -f test.log
```
输出：


```
[root@localhost ~]# ping 192.168.120.204 > test.log &
[1] 11891[root@localhost ~]# tail -f test.log 
PING 192.168.120.204 (192.168.120.204) 56(84) bytes of data.
64 bytes from 192.168.120.204: icmp_seq=1 ttl=64 time=0.038 ms
64 bytes from 192.168.120.204: icmp_seq=2 ttl=64 time=0.036 ms
64 bytes from 192.168.120.204: icmp_seq=3 ttl=64 time=0.033 ms
64 bytes from 192.168.120.204: icmp_seq=4 ttl=64 time=0.027 ms
64 bytes from 192.168.120.204: icmp_seq=5 ttl=64 time=0.032 ms
64 bytes from 192.168.120.204: icmp_seq=6 ttl=64 time=0.026 ms
64 bytes from 192.168.120.204: icmp_seq=7 ttl=64 time=0.030 ms
64 bytes from 192.168.120.204: icmp_seq=8 ttl=64 time=0.029 ms
64 bytes from 192.168.120.204: icmp_seq=9 ttl=64 time=0.044 ms
64 bytes from 192.168.120.204: icmp_seq=10 ttl=64 time=0.033 ms
64 bytes from 192.168.120.204: icmp_seq=11 ttl=64 time=0.027 ms
[root@localhost ~]#
```
说明：
ping 192.168.120.204 > test.log & //在后台ping远程主机。并输出文件到test.log；这种做法也使用于一个以上的档案监视。用Ctrl＋c来终止。
实例3：从第5行开始显示文件
命令：


```
tail -n +5 log2014.log
```
输出：


```
[root@localhost test]# cat log2014.log 
2014-01
2014-02
2014-03
2014-04
2014-05
2014-06
2014-07
2014-08
2014-09
2014-10
2014-11
2014-12
==============================
[root@localhost test]# tail -n +5 log2014.log
2014-05
2014-06
2014-07
2014-08
2014-09
2014-10
2014-11
2014-12
==============================
```
### 本系列文章：
[每天一个 Linux 命令（1）：ls命令](http://blog.jobbole.com/108531/)
[每天一个 Linux 命令（2）：cd命令](http://blog.jobbole.com/108805/)
[每天一个 Linux 命令（3）：pwd命令](http://blog.jobbole.com/108855/)
[每天一个 Linux 命令（4）：mkdir命令](http://blog.jobbole.com/108871/)
[每天一个 Linux 命令（5）：rm 命令](http://blog.jobbole.com/108901/)
[每天一个 Linux 命令（6）：rmdir 命令](http://blog.jobbole.com/108925/)
[每天一个 Linux 命令（7）：mv命令](http://blog.jobbole.com/108983/)
[每天一个 Linux 命令（8）：cp 命令](http://blog.jobbole.com/108988/)
[每天一个 Linux 命令（9）：touch 命令](http://blog.jobbole.com/108989/)
[每天一个 Linux 命令（10）：cat 命令](http://blog.jobbole.com/108990/)
[每天一个 Linux 命令（11）：nl 命令](http://blog.jobbole.com/108991/)
[每天一个 Linux 命令（12）：more 命令](http://blog.jobbole.com/108992/)
[每天一个 Linux 命令（13）：less 命令](http://blog.jobbole.com/108993/)
[每天一个 Linux 命令（14）：head 命令](http://blog.jobbole.com/108994/)
