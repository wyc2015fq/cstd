
# DPDK-KNI学习和利用（DNS服务） - 3-Number - CSDN博客


2017年11月03日 16:43:24[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：3698


# 0x01 缘由
看到这个例子，不禁想起前期我们做的一个项目，我们通过pfring捕获数据包，想让这个数据包再回到linux协议栈，然后访问我们的服务。我记得我们采用的方式是将两个物理网卡相连，然后数据包转发到另外一个普通网卡，让其进Linux协议栈处理流程。
同样KNI也是一种方式，用DPDK捕获数据包让其走linux协议栈去处理。
# 0x02 KNI是咋回事？
KNI（Kernel NIC Interface 内核网卡接口）是DPDK 允许用户态应用与内核协议栈交换数据包的一种解决方案。为了解决这个问题，DPDK用户态应用用IOCTL调用一个KNI虚拟设备。IOCTL调用提供接口信息和DPDK的物理地址空间，由KNI内核可加载模块将其重新映射到内核地址空间，该模块将信息保存到虚拟设备上下文中。 DPDK为每个分配的设备创建数据包入口和出口到内核模块的FIFO队列。KNI内核可加载模块是标准网络驱动程序，它在接收到IOCTL呼叫时访问DPDK的FIFO队列以从/向DPDK用户空间应用程序接收/发送分组。
 FIFO队列包含指向DPDK中数据包的指针。
**优势：**
提供更快的机制来与内核网络堆栈进行交互并消除系统调用；
使用标准的Linux *用户空间网络工具（tcpdump，ftp等）为DPDK提供便利；
消除数据包的copy_to_user和copy_from_user操作。
看下下图就明白了：
![](https://img-blog.csdn.net/20171103163926940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x03 DPDK标准例子
下面根据github（[https://github.com/pangyemeng/mydpdkdns](https://github.com/pangyemeng/mydpdkdns)）上找到的一个例子，学习和总结原理。此例子是做一个dns服务器的简单服务，主要将查询dns协议进行解析，不是dns包的直接返回到协议栈去处理。程序基于dpdk例子KNI开发。
**基本原理图：**
![](https://img-blog.csdn.net/20171103163943120?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**相关环境说明：**
Centos 7 x86_64（因为低版本有c++ 11等无法用，所以就用这个系统）
DPDK 17.02
**运行：**
```python
[root@localhost build]# ./bin/dserver -c 0xf -n 1 -- -p1 --config '(0,0,1,2,3)'
EAL: Detected 4 lcore(s)
EAL: Probing VFIO support...
EAL: PCI device 0000:03:00.0 on NUMA socket -1
EAL:   probe driver: 8086:109a net_e1000_em
APP: Initialising port 0 ...
KNI: pci: 03:00:00      8086:109a
Checking link status
..........................................................................................done
Port 0 Link Down
APP: Lcore 1 is writing to port 0
APP: Lcore 2 is working to port 0
APP: Lcore 0 is reading from port 0
APP: Lcore 3 is sending to port 0
```
**测试：**
将虚拟网卡配置一个局域网IP，将电脑网关和DNS服务群器配置成刚设置的IP，配置后如下图：
![](https://img-blog.csdn.net/20171103164017874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171103164035693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**抓包分析：**
![](https://img-blog.csdn.net/20171103164103047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x04 总结
此为简单的DNS实现，这个目前引流到DNS服务器，因为开发一个协议栈去处理各种异常，工作量较大，所以利用linux内核协议栈做一些简化。源码不做过多分析。设备可以做成串并方式。


