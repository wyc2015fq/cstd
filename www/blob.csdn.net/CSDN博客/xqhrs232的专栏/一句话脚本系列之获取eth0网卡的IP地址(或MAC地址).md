# 一句话脚本系列之获取eth0网卡的IP地址(或MAC地址) - xqhrs232的专栏 - CSDN博客
2017年12月06日 19:03:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：646
原文地址::[http://blog.csdn.net/nfer_zhuang/article/details/42609733](http://blog.csdn.net/nfer_zhuang/article/details/42609733)
相关文章
1、[Linux
 如何通过命令仅获取IP地址](http://www.cnblogs.com/kerrycode/p/4581030.html)----[https://www.cnblogs.com/kerrycode/archive/2015/06/16/4581030.html](https://www.cnblogs.com/kerrycode/archive/2015/06/16/4581030.html)
2、Linux C语言 获取本机（所有网卡）IP地址（IPV4）----[http://blog.csdn.net/ygm_linux/article/details/24661839](http://blog.csdn.net/ygm_linux/article/details/24661839)
3、Linux下编程获取本地IP地址的常见方法----[http://blog.csdn.net/k346k346/article/details/48231933](http://blog.csdn.net/k346k346/article/details/48231933)
## 引言
使用场景：
我使用的Linux服务器上有多个网卡，有时候就需要在脚本中使用到指定网卡的IP地址或MAC地址，那么这个时候就有两种做法，一是预先读取网卡的IP地址或MAC地址，然后写入到脚本中；另外一种方法就是通过shell命令实时获取指定网卡上的当前IP地址或MAC地址。
一般用法：- 
使用ifconfig eth0读取网卡信息
- 手动将IP地址或MAC地址拷贝出来，并填入到脚本中的对应变量中
一句话脚本用法：
[plain][view
 plain](http://blog.csdn.net/nfer_zhuang/article/details/42609733#)[copy](http://blog.csdn.net/nfer_zhuang/article/details/42609733#)
- ifconfig eth0 | grep "inet addr:" | awk '{print $2}' | cut -c 6-  (获取eth0网卡的IP地址)  
- 或  
- ifconfig eth0 | grep "HWaddr" | awk '{print $5}'  (获取eth0网卡的MAC地址)  
上面的脚本分解步骤是：- 获取eth0网卡的信息
- 过滤出IP地址的行或MAC地址的行
- 使用awk输出指定字段，对于MAC地址，第5个字段就是MAC；而对于IP地址，还需要对第2个字段截取第6个字符之后的内容
## ifconfig命令部分说明
先看一下ifconfig的man手册中的描述：
       Ifconfig  is  used  to  configure  the  kernel-resident network interfaces.  
       If  no  arguments  are  given,  ifconfig  displays the status of the currently active interfaces.  If a single
       interface argument is given, it displays the status of the given interface only; if a single  -a  argument  is
       given, it displays the status of all interfaces, even those that are down.  
ifconfig命令的主要作用是进行网卡配置，但是如果没有给定参数或者只是给定了网卡名称这一个参数，则就会显示网卡的状态信息。
因此，在这里，我们通过参数eth0来指定输出该网卡的信息
## grep命令部分说明
我们先看一下ifconfig的输出格式：
eth0      Link encap:Ethernet  HWaddr 08:00:27:f6:18:8e
 inet addr:192.168.56.101  Bcast:192.168.56.255  Mask:255.255.255.0
          inet6 addr: fe80::a00:27ff:fef6:188e/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:12575 errors:0 dropped:0 overruns:0 frame:0
          TX packets:3429 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:1931585 (1.9 MB)  TX bytes:465667 (465.6 KB)
其中红色部分中包含了MAC地址信息，蓝色部分包含了IPv4的地址信息。而如果我们指定的网卡不存在时，则输出信息如下：
nfer@nfer:~$ ifconfig eth1
eth1: error fetching interface information: Device not found
因此在上面的命令中，我们分别使用grep "inet addr:"和grep "HWaddr"来过滤出IPv4的地址和MAC地址所在的行，如果是该网卡不存在或者不处于活动状态就过滤不到对应的行。
## awk命令和cut命令部分说明
在上面的grep命令过滤出来的MAC地址和IPv4地址所在行的格式如下：
eth0      Link encap:Ethernet  HWaddr 08:00:27:f6:18:8e  
          inet addr:192.168.56.101  Bcast:192.168.56.255  Mask:255.255.255.0
因此，如果是获取MAC地址，只需要使用awk输出第5个字段的值即可：awk '{print $5}'；
而如果是要获取IPv4的地址，则需要先输出第2个字段的值：awk '{print $2}'，然后再使用cut命令，将"addr:"这5个字符去除，即从第6个字符到结尾的所有字符：cut -c 6-。
其中cut命令的-c参数以及后面的需要显示的字符列表的表述方式的描述如下：
       -c, --characters=LIST
              select only these characters
       Use one, and only one of -b, -c or -f.  Each LIST is made up of one range, or many ranges separated by commas.
       Selected  input  is written in the same order that it is read, and is written exactly once.  Each range is one of:
       N      N'th byte, character or field, counted from 1
       N-     from N'th byte, character or field, to end of line
       N-M    from N'th to M'th (included) byte, character or field
       -M     from first to M'th (included) byte, character or field
我们这里是按照字符操作的，所以使用了-c参数；需要显示的是从第6个字符到结尾的部分，所以使用了N-的模式表示LIST。
## 总结
本次一句话脚本使用到了以下知识：
- ifconfig命令
- grep命令
- awk命令
- cut命令的-c参数

