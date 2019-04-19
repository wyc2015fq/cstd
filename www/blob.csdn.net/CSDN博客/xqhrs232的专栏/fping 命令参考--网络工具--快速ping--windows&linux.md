# fping 命令参考--网络工具--快速ping--windows&linux - xqhrs232的专栏 - CSDN博客
2017年08月14日 15:33:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：423
原文地址::[http://linux.chinaunix.net/techdoc/install/2008/06/05/1009496.shtml](http://linux.chinaunix.net/techdoc/install/2008/06/05/1009496.shtml)

相关文章
1、Linux 网络工具之fping 
 ----[http://blog.163.com/qiushuhui1989@126/blog/static/2701108920131021102129401/](http://blog.163.com/qiushuhui1989@126/blog/static/2701108920131021102129401/)
fping 命令手册
本命令手册以实用，简洁为原则，对于已经消化的内容，不附加详细的注解。
磁针石
联系方式：
QQ:37391319 gmail and gtalk: xurongzhong@gmail.com
参考资料 
＊《反黑客工具包 第3版》 §14.3    FPING
＊ 《黑客曝光 第5版》§2.1  确定目标系统是否真实存在
说明本次整合仅仅是2本书内容中的汇合，还没有涉及到windows下面的fping.
功能
fping - send ICMP ECHO_REQUEST packets to network hosts
UNIX和Windows环境都有许多可以用来进行ICMP ping扫描的工具。fping（http://packetstorm.securify.com/
Exploit_Code__Archive/ fping.tar.gz）是UNIX环境里久经考验的ping扫描工具之一。早期的ping扫描工具大都需要等待前一个被探测主机返回某种响应消息之后才能继续探测下一台主机是否存在，但fping却能以轮转方式并行地发出大量的ping请求。这么一来，用fping工具去扫描多个IP地址的速度要比ping快很多。fping工具有两种用法：既可以通过标准输入设备（stdin）向它提供一系列IP地址作为输入，也可以让它去读取某个文件。让fping工具去读取一个文件是件很简单的事情：创建一个文本文件并把IP地址按每行一个的格式写入，如下所示：
192.168.51.1
192.168.51.2
192.168.51.3
...
192.168.51.253
192.168.51.254
然后使用“-f”参数读入这个文件，如下所示：
[root]$ fping
–a –f in.txt
192.168.1.254 is alive
192.168.1.227 is alive
192.168.1.224 is alive
...
192.168.1.3 is alive
192.168.1.2 is alive
192.168.1.1 is alive
192.168.1.190 is alive
fping工具的“-a”选项表示只在输出报告里列出当前正在运行的系统。如果你愿意，还可以同时使用这个工具的“-d”选项去解析主机名。笔者比较喜欢使用fping工具的“-a”选项配合shell脚本和“-d”选项去扫描那些有着独一无二的主机名的系统。在编写进行ping扫描的脚本时，“-f”和fping工具的其他几个选项也很有用。fping工具的可用选项的完整清单可以通过执行“fping -h”命令来查看。
Unix 版本相关网站：
[http://www.fping.com/](http://www.fping.com/)
Windows版本相关网站：
[http://www.kwakkelflap.com/](http://www.kwakkelflap.com/)
实现机制：
       Fping
sends ICMP echo requests to a list of IP addresses, provided either on standard
input or from a file, in a parallelized fashion. It sends out Pings
in a "round-robin" fashion without waiting for a response. When responses
are eventually returned, fping notes whether the host is alive or not and waits
for more responses, all the while continuing its Ping
sweep.
       跟普通shell批量ping脚本相比，具有并行，结果易读的优点。
fping
-a -g 172.19.148.1 172.19.148.255 -s -n  >hosts
执行结果，显示通的IP:
[root@Meil_88
root]# vi hosts 
172.19.148.1
172.19.148.2
172.19.148.3
172.19.148.18
参数：
**fping Option   Description**
  -a 
  Lists targets that responded
  -A 
  Lists targets by address instead of hostname
  -b 
  Sends  bytes of data per ICMP packet (default 56)
  -B 
  Tells fping to wait  times longer for a reply after each
  successive failed request (default 1.5)
  -c 
  Number of Pings to send to each target
  (default 1)
  -C 
  Same as above but prints additional statistics for each host
  -e 
  Displays elapsed time on return packets
  -f 
  Reads the target list from  (use "-" for standard
  input)
  -g 
  Tells fping to generate a target list by specifying the start and end
  address (ex. ./fping -g 192.168.1.0 192.168.1.255) or an IP/subnet mask (ex.
  ./fping -g 192.168.1.0/24)
  -i 
  Interval (in milliseconds) to wait between Pings
  (default 25)
  -l 
  Sends Pings forever
  -m 
  Pings multiple interfaces on target host
  -n 
  Displays targets by name (-d is equivalent)
  -p 
  Interval (in milliseconds) between Pings
  to an individual target (in looping and counting modes, default 1000)
  -q 
  Doesn't show per-target/per-Ping results
  -Q 
  Same as -q, but show summary every  seconds
  -r 
  When a host doesn't respond, retries the host  times (default
  3)
  -s 
  Displays summary statistics
  -t 
  Timeout (in milliseconds) for individual targets (default 500)
  -u 
  Displays targets that are unreachable
  -v 
  Displays version number
**本文来自ChinaUnix博客，如果查看原文请点：**[http://blog.chinaunix.net/u/21908/showart_726624.html](http://blog.chinaunix.net/u/21908/showart_726624.html)

