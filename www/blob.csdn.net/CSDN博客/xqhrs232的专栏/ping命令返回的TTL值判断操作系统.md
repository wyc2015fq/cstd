# ping命令返回的TTL值判断操作系统 - xqhrs232的专栏 - CSDN博客
2017年08月14日 11:46:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：237
原文地址::[http://blog.csdn.net/u010240427/article/details/52585841](http://blog.csdn.net/u010240427/article/details/52585841)
相关文章
1、[Ping命令返回的TTL值详解](http://blog.csdn.net/u010240427/article/details/52585841)----[http://blog.csdn.net/u010240427/article/details/52585841](http://blog.csdn.net/u010240427/article/details/52585841)
通过简单的ping命令，查看返回的TTL值来判断对方的操作系统
生存时间(TTL)是IP分组中的一个值，网络中的路由器通过察看这个值就可以判断这个IP分组是不是已经在网络中停留了很久，进而决定是否要将其丢弃。出于多种原因，一个IP分组可能在很长一段时间内不能抵达目的地。例如：错误的路由有可能导致一个IP分组在网络中无限地循环。一种解决方法就是在一定时间后丢弃这个分组，然后发送一个信息通知这个分组的发送者，由它决定是否重发这个分组。TTL的初始值一般是系统缺省值，它位于IP分组的头部，占用8个二进制位。最初设定TTL值的目的是，让它来指定一段特定的时间（以秒为单位），当这段时间耗尽的时候就将这个分组丢弃。由于每个路由器至少会让这个TTL值减一，所以这个TTL只经常用来指定在一个分组被丢弃之前允许经过的路由器数。每个路由器收到一个分组后就将它的TTL
 值减一，一旦这个值被减为0，路由器就会丢弃这个分组，并发送一个ICMP信息给这个分组的最初的发送者。
UNIX 及类 UNIX 操作系统 ICMP 回显应答的 TTL 字段值为             255
Compaq Tru64 5.0 ICMP 回显应答的 TTL 字段值为                          64
Windows NT/2K操作系统 ICMP 回显应答的 TTL 字段值为              128
Windows 95 操作系统 ICMP 回显应答的 TTL 字段值为                    32
注：ICMP报文的类型包括如下：
ECHO (Request (Type 8), Reply (Type 0))--回显应答,
Time Stamp (Request (Type 13), Reply (Type 14))--时间戳请求和应答,
Information (Request (Type 15), Reply (Type16))--信息请求和应答,
Address Mask (Request (Type 17), Reply (Type 18))--地址掩码请求和应答等
不同的操作系统，它的TTL值是不相同的。默认情况下：
Linux系统的TTL值为64或255，
Windows NT/2000/XP系统的TTL值为128，
Windows 98系统的TTL值为32，
UNIX主机的TTL值为255。
公司使用的是多数为Windows 2000服务器，TTL值默认为128，如果将该值修改为255，攻击者可能会以为这个服务器是Linux系统或UNIX系统，那么他们就会针对Linux系统或UNIX系统来查找Windows 2000服务器的安全漏洞，不过他们是不会找到什么安全漏洞的，这样一来，服务器相来说增加了安全性。
　　具体实现方法：
　　修改TTL值其实非常简单，通过注册表编辑器就可以实现，点击“开始→运行”，在“运行”对话框中输入“regedit”命令并回车，弹出
“注册表编辑器”对话框，展开“HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Tcpip\ Parameters”，找到“DefaultTTL”，将该值修改为十进制的“255”，重新启动服务器系统后即可。
代码的世界，简单的内心

