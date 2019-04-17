# linux下网络排错与查看 - DoubleLi - 博客园






**基本的故障排除错误**

故障的排除一定是先简单后复杂的，有的人把上述的文件反复配置，就是上不了网，一直都认为是[系统](http://www.2cto.com/os/)出了故障，想重装机子。结果发现原来是网线压根就没插上。

排错要慢慢的按部就班的来：

（1）首先看网线是否插好，灯亮就是OK的。
（2）我们要看自己的网卡驱动是否正常，建立一个ip然后ping它，看是否ping的通，通过说明网卡是正常的。
（3）使用ifconfig命令查看ip是否正确，路由信息是否正确
（4）ping统一网段的机子，不行的话网关设置错误
（5）查看dns的状态（这个是经常出现的，我们可以ping通局域网，但是外网却链接不上，那就是dns有问题了）
（6）查看服务是否能开启，一般我们把NetworkManager服务关闭。

操作流程

（1）首先关掉NetworkManager:
![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509180814.png)
开机也关掉它：
![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509180815.png)
（2）查看路由：
![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509180816.png)
（3）查看dns设定：
![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509180817.png)
（4）查看ifcfg-eth0和ifcfg-br0：
![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509180818.png)
（5）查看网络设备识别规则：
![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509180819.png)
看看规则指定的是eth0和我们的设定是相符的。

注：如果上述的问题都解决了还是不能上网，看看是否是selinux的原因。

**查看网络命令解析：**

**网络配置查看：ifconfig**

用途：配置或显示 TCP/IP 网络的网络接口参数

示例操作：ifconfig

![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509180921.png)

Link encap: 这个字段代表该网络设备位于OSI物理层的名称。
HWaddr：网卡的硬件地址，也就是MAC地址。 inet addr：显示网卡的IP地址 Bcast：该接口的网络广播地址 Mask：该接口的网络子网掩码。 MTU：该接口的最大传输单元 Metric：默认的路由成本。
RX ：已接受的封包统计。
TX：已传送或者传送中的封包统计 collisions：网卡接口发生封包碰撞的次数 txqueuelen:网卡设置的传送队列长度 RX bytes：已接收的封包数据量，单位是字节。 TX bytes：已传送的封包数据量，单位是字节 interrupt:这张网卡接口的IRQ中断值。

**两台主机间的沟通：ping**

?ping命令一般用于检测网络通与不通，也叫时延，其值越大，速度越慢PING(PacketInternetGrope)，因特网包探索器，用于测试网络连接量的程序。

?ping发送一个ICMP回声请求消息给目的地并报告是否收到所希望的ICMP回声应答。它是用来检查网络是否通畅或者网络连接速度的命令。

?原理：网络上的机器都有唯一确定的IP地址，我们给目标IP地址发送一个数据包，对方就要返回一个同样大小的数据包，根据返回的数据包我们可以确定目标主机的存在，可以初步判断目标主机的操作系统等。


示例操作：ping -c 5 www.baidu.com（-c 5 这个参数表示执行ping命令5次）

![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509181022.png)

**两主机间各节点的分析：traceroute**

通过traceroute我们可以知道信息从你的计算机到互联网另一端的主机是走的什么路径。当然每次数据包由某一同样的出发点（source）到达某一同样的目的地(destination)走的路径可能会不一样，但基本上来说大部分时候所走的路由是相同的。linux系统中，我们称之为traceroute,在MS Windows中为tracert。 traceroute通过发送小的数据包到目的设备直到其返回，来测量其需要多长时间。一条路径上的每个设备traceroute要测3次。输出结果中包括每次测试的时间(ms)和设备的名称（如有的话）及其IP地址。


示例操作：traceroute www.baidu.com

![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509181224.png)
我们发现有的返回的是* * *，代表该node可能设有某些防护措施让我们所发送的数据包信息被丢弃。

**查看本机的网络链接与后门：netstat**

Netstat用于显示与IP、TCP、UDP和ICMP协议相关的统计数据，一般用于检验本机各端口的网络连接情况。
如果你的计算机有时候接收到的数据报导致出错数据或故障，你不必感到奇怪，TCP/IP可以容许这些类型的错误，并能够自动重发数据报。但如果累计的出错 情况数目占到所接收的IP数据报相当大的百分比，或者它的数目正迅速增加，那么你就应该使用Netstat查一查为什么会出现这些情况了。

参数详解：


-a或-all 显示所有连线中的socket -n 已建立的有效链接 -o 显示计时器 -p -programs 显示正在使用的程序 -t　显示tcp传输的连线状况 -u　显示udp传输的连线状 -l 显示监控中的服务器的socket 示例操作：netstat -a

![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509181326.png)

**检测主机名与IP的对应：host，nslookup**

两个命令都差不多，都可以追踪到服务器的IP地址；但是后者还可以找到主机名。

示例操作：host www.baidu.com

nslookup www.baidu.com

![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509181428.png)

![\](http://www.2cto.com/uploadfile/Collfiles/20140925/2014092509181529.png)









