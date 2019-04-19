# Linux下设置帧和socket缓冲区的大小 - xqhrs232的专栏 - CSDN博客
2016年05月17日 10:14:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2303
原文地址::[http://www.cppblog.com/prayer/archive/2009/02/03/72873.html](http://www.cppblog.com/prayer/archive/2009/02/03/72873.html)
相关文章
1、发送大量数据，socket API最优分包长度为多少？解决办法----[http://www.myexception.cn/linux-unix/291994.html](http://www.myexception.cn/linux-unix/291994.html)
2、请问linux socket编程时, 最大一次可以发多少数据?----[http://bbs.csdn.net/topics/390348596](http://bbs.csdn.net/topics/390348596)
修改帧大小和socket缓冲区大小
MTU (最大传输单元)的缺省值为1500. 
通过下面命令将其改为9000（jumbo frame）
% ifconfig eth0 mtu 9000
socket buffer size缺省为64 kB.
理论上，最优的buffer大小为<bandwidth>*<round-trip delay>
可以通过下列命令改变其大小（如256KB）
% sysctl -w net.core.rmem_max=262144 ；最大的接收缓冲区大小（tcp连接）
% sysctl -w net.core.wmem_max=262144 ；最大的发送缓冲区大小（tcp连接）
% sysctl -w net.core.rmem_default=262144；缺省的接收缓冲区大小（tcp连接）
% sysctl -w net.core.wmem_default=262144；缺省的发送缓冲区大小（tcp连接）
这些参数在proc/sys下面可以看到；
sysctl -p | grep mem：显示当前缓冲区设置
round-trip delay 可由ping命令得到。
