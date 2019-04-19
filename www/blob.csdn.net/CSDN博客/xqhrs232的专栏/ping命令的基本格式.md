# ping命令的基本格式 - xqhrs232的专栏 - CSDN博客
2014年08月05日 13:51:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1068
原文地址::[http://blog.sina.com.cn/s/blog_78efec1501014b6i.html](http://blog.sina.com.cn/s/blog_78efec1501014b6i.html)
相关文章
1、P​i​n​g​命​令​的​语​法​格​式_百度文章----[http://wenku.baidu.com/link?url=KOsS9N_WvtmsnEB-0na15ZH_9PtDPQjZTzN4rkwI57Lla7T2OHSfDGlvfBDLUc67giWXQjQHtnJCMSFFr4wKCQi9wcW5wNoo8wlZfXRV7Q_](http://wenku.baidu.com/link?url=KOsS9N_WvtmsnEB-0na15ZH_9PtDPQjZTzN4rkwI57Lla7T2OHSfDGlvfBDLUc67giWXQjQHtnJCMSFFr4wKCQi9wcW5wNoo8wlZfXRV7Q_)
2、[ping命令的用法大全！](http://blog.csdn.net/sdzg_wq/article/details/629378)----[http://blog.csdn.net/sdzg_wq/article/details/629378](http://blog.csdn.net/sdzg_wq/article/details/629378)
什么是ping命令
      PING命令是用来检查要到达的目标IP地址并记录结果，显示目标是否响应以及接收答复所需的时间。如果在传递到目标过程中有错误，ping 命令将显示错误消息。
我们在HOST A使用一个ICMP Echo Request packet (回送请求)数据包来探测主机地址 HOST B 是否存活，通过简单的发送一个ICMP ECHO(Type 8)数据包到目标主机，如果ICMP ECHO Reply(ICMP type 0)(回送答复)数据包 HOST A 可以接受到，说明主机是存活状态，如果没有就可以初步判断主机没有在线或者使用了某些过滤设备过滤了ICMP的REPLY。
ping和其它一些工具如traceroute等配合使用可进行许多基本的网络测试。
ping命令的基本格式
PING的格式为：
ping [-t] [-a] [-n count] [-l length] [-f] [-i ttl] [-v tos] [-r count] [-s count] [[-j computer-list] | [-k computer-list]] [-w timeout] destination-list
参数说明：
-t ：一直Ping指定的计算机,直到从键盘按下Control-C中断。
-a ：将地址解析为计算机NetBios名。
-n ：发送count指定的ECHO数据包数。，通过这个命令可以自己定义发送的个数，对衡量**网络速度**很有帮助。能够测试发送数据包的返回平均时间，及时间的快慢程度。默认值为
 4。
-l ：发送指定数据量的ECHO数据包。默认为
 32 字节；最大值是65500字节。
-f ：在数据包中发送“不要分段”标志,数据包就不会被路由上的网关分段。通常你所发送的数据包都会通过路由分 段再发送给对方，加上此参数以后路由就不会再分段处理。
-i ：将“生存时间”字段设置为TTL指定的值。指定TTL值在对方的系统里停留的时间。同时检查网络运转情况的。
-v ：tos 将“服务类型”字段设置为 tos 指定的值。
-r ：在“记录路由”字段中记录传出和返回数据包的路由。通常情况下，发送的数据包是通过一系列路由才到达目标地址的，通过此参数可以设定，想探测经过路由的个数。限定能跟踪到9个路由。
-s ：指定 count 指定的跃点数的时间戳。与参数-r差不多，但此参数不记录数据包返回所经过的路由，最多只记录4个。
-j ：利用 computer-list 指定的计算机列表路由数据包。连续计算机可以被中间网关分隔(路由稀疏源) IP 允许的 最大数量为 9。
-k ：computer-list 利用 computer-list 指定的计算机列表路由数据包。连续计算机不能被中间网关分隔(路由严格 源)IP 允许的最大数量为 9。
-w：timeout 指定超时间隔，单位为毫秒。
destination-list： 指定要 ping 的远程计算机。
Ping的返回信息   有"Request Timed Out"、"Destination Net Unreachable"和"Bad IP address"还有"Source quench received"。 
"Request Timed Out"这个信息表示对方主机可以到达到TIME OUT，这种情况通常是为**对方拒绝接收你发给它的数据包**造成数据包丢失。大多数的原因可能是**对方装有防火墙或已下线**。 
"Destination Net Unreachable"这个信息表示**对方主机不存在**或者**没有跟对方建立连接**。这里要说明一下"destination host unreachable"和"time out"的**区别**，如果所经过的路由器的路由表中具有到达目标的路由，而目标因为其它原因不可到达，这时候会出现"time out"，如果路由表中连到达目标的路由都没有，那就会出现"destination host unreachable"。 
"Bad IP address" 这个信息表示你可能没有连接到DNS服务器所以无法解析这个IP地址，也可能是IP地址不存在。 
"Source quench received"信息比较特殊，它出现的机率很少。它表示对方或中途的服务器繁忙无法回应。 
怎样使用Ping这命令来**测试网络连通**呢？ 
连通问题是由许多原因引起的，如本地配置错误、远程主机协议失效等，当然还包括设备等造成的故障。 
首先我们讲一下使用Ping命令的步骤。 
使用Ping检查连通性有五个步骤： 
1. 使用ipconfig /all观察**本地网络设置**是否正确； 
2. Ping 127.0.0.1，127.0.0.1 回送地址Ping回送地址是为了**检查本地的TCP/IP协议**有没有设置好； 
3. Ping本机IP地址，这样是为了**检查本机的IP地址**是否设置有误； 
4. Ping本网网关或本网IP地址，这样的是为了**检查硬件设备**是否有问题，也可以**检查本机与本地网络连接**是否正常；（在非局域网中这一步骤可以忽略） 
5. Ping远程IP地址，这主要是检查**本网或本机与外部的连接**是否正常。 
在检查网络连通的过程中可能出现一些错误，这些**错误**总的来说分为两种最常见。 
1. Request Timed Out 
"request time out"这提示除了在《PING（一）》提到的对方可能装有防火墙或已关机以外，还有就是本机的IP不正确和网关设置错误。 
①、IP不正确： 
IP不正确主要是IP地址设置错误或IP地址冲突，这可以利用ipconfig /all这命令来检查。在WIN2000下IP冲突的情况很少发生，因为系统会自动检测在网络中是否有相同的IP地址并提醒你是否设置正确。在NT中不但会出现"request time out"这提示而且会出现"Hardware error"这提示信息比较特殊不要给它的提示所迷惑。 
②、网关设置错误：这个错误可能会在第四个步骤出现。网关设置错误主要是网关地址设置不正确或网关没有帮你转发数据，还有就是可能远程网关失效。这里主要是在你Ping外部网络地址时出错。错误表现为无法Ping外部主机返回信息"Request timeout"。 
2. Destination Host Unreachable 
当你在开始PING网络计算机时如果网络设备出错它返回信息会提示"destination host unreachable"。如果局域网中使用DHCP分配IP时，而碰巧DHCP失效，这时使用 PING命令就会产生此错误。因为在DHCP失效时客户机无法分配到IP系统只有自设IP，它往往会设为不同子网的IP。所以会出现"Destination Host Unreachable"。另外子网掩码设置错误也会出现这错误。 
还有一个比较特殊就是路由返回错误信息，它一般都会在"Destination Host Unreachable"前加上IP地址说明哪个路由不能到达目标主机。这说明你的机器与外部网络连接没有问题，但与某台主机连接存在问题。 
举个例子吧。 
我管理的网络有19台机，由一台100M集线器连接服务器，使用DHCP动态分配IP地址。 
有一次有位同事匆忙地告诉我"我的OUTLOOK打不开了"，我到他机器检查，首先我检查了本地网络设置，我用ipconfig /all看IP分配情况一切正常。接着我就开始PING网络中的其中一台机器，第一次PING结果很正常，但OUTLOOK还是无法使用其它网络软件和Copy网络文件都可以使用但网络速度很慢，第二次PING我用了一个参数-t（-t可以不中断地PING对方，当时我想PING一次可能发现不了问题）发现有time=30ms和request time out，从服务器PING这台机就更有趣，request time out比正常数据还多，在局域中竟然有time=30ms和request time out太不正常了。开始我认为是网卡的问题但换网卡后故障依旧，重做网线还是不能解决问题，这故障真有趣！最后我没办法了把它插在集线器端口上的另一端的网线换到另一个端口，哈！故障解决了。原来是集线器端口坏了。 
如何用Ping命令来判断一条**链路的速度**？ 
Ping这个命令除了可以检查网络的连通和检测故障以外，还有一个比较有趣的用途，那就是可以利用它的一些返回数据，来估算你跟某台主机之间的速度是多少字节每秒 
我们先来看看它有那些返回数据。 
Pinging 202.105.136.105 with 32 bytes of data: 
Reply from 202.105.136.105: bytes=32 time=590ms TTL=114 
Reply from 202.105.136.105: bytes=32 time=590ms TTL=114 
Reply from 202.105.136.105: bytes=32 time=590ms TTL=114 
Reply from 202.105.136.105: bytes=32 time=601ms TTL=114 
Ping statistics for 202.105.136.105: 
Packets: Sent = 4, Received = 4, Lost = 0 (0% loss), 
Approximate round trip times in milli-seconds: 
Minimum = 590ms, Maximum = 601ms, Average = 593ms 
在例子中"bytes=32"表示ICMP报文中有32个字节的测试数据（这是估算速度的关键数据），"time=590ms"是往返时间。 
怎样估算链路的速度呢？举个例子吧。我们把A和B之间设置为**PPP链路**。 
从上面的PING例子可以注意到，默认情况下发送的ICMP报文有32个字节。除了这32个字节外再加上20个字节的IP首部和8个字节的ICMP首部，整个IP数据报文的总长度就是60个字节（因为IP和ICMP是Ping命令的主要使用协议，所以整个数据报文要加上它们）。另外在使用Ping命令时还使用了另一个协议进行传输，那就是PPP协议（点对点协议），所以在数据的开始和结尾再加上8个字节。在传输过程中，由于每个字节含有8bit数据、1bit起始位和1bit结束位，因此传输速率是每个字节2.98ms。由此我们可以估计需要405ms。即68*2.98*2（乘2是因为我们还要计算它的往返时间）。 
我们来测试一下33600 b/s的链路： 
Pinging 202.105.36.125 with 32 bytes of data: 
Reply from 202.105.36.125: bytes=32 time=415ms TTL=114 
Reply from 202.105.36.125: bytes=32 time=415ms TTL=114 
Reply from 202.105.36.125: bytes=32 time=415ms TTL=114 
Reply from 202.105.36.125: bytes=32 time=421ms TTL=114 
Ping statistics for 202.105.36.125: 
Packets: Sent = 4, Received = 4, Lost = 0 (0% loss), 
Approximate round trip times in milli-seconds: 
Minimum = 415ms, Maximum = 421ms, Average = 417ms 
看是不是差不多啊。^_^ 
这里大家可能要注意到，**这估算值跟实际值是有误差的**，为什么呢？因为我们现在估算的是一个理论值，还有一些东西我们没有考虑。比如在**网络中的其它干扰**，这些干扰主要来之别的计算机。因为在你测试时不可能全部计算机停止使用网络给你做测试，这是不实际的。另外就是**传输设备**，因为有某些设备如MODEM它在传输时会把数据压缩后再发送，这大大减少了传输时间。这些东西产生的误差我们是不能避免的，但其数值大约在5%以内我们都可以接受（利用MODEM传输例外），但是可以减少误差的产生。比如把MODEM的压缩功能关闭和在网络没有那么繁忙时进行测试。有时候误差是无须消除的。因为我们需要这些误差跟所求得的理论值进行比较分析，从而找出网络的缺陷而进行优化。这时测试网络的所有数据包括误差都会成为我们优化的依据。 
还要注意，这种算法在局域网并不适用，因为在局域网中速度非常的快几乎少于1ms，而Ping的最小时间分辨率是1ms，所以根本无法用Ping命令来检测速度。如果想测试速度那就要用专门仪器来检测。 
总的来说，Ping命令是一个在故障检查方面很有用而且很便利的工具 
为了**测试光纤收发器的丢包率**，工程师建议使用PC的ping命令
于是ping -l 65500 -t -n 1000 192.168.30.84
即，对目标主机192.168.30.84发送数据量为65500的ECHO数据包1000个，然后看其结果

//=================================================================================================================================
备注::
1》ping ip -t-------持续ping包，直到ctrl+c中断，然后会出统计报告
Ping statistics for 180.97.33.67:
    Packets: Sent = 157, Received = 156, Lost = 1 (0% loss),
Approximate round trip times in milli-seconds:
    Minimum = 22ms, Maximum = 33ms, Average = 24ms
Control-C
^C

