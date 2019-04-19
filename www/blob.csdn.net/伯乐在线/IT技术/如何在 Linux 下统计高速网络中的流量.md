# 如何在 Linux 下统计高速网络中的流量 - 文章 - 伯乐在线
原文出处： [Dan Nanni](http://xmodulo.com/measure-packets-per-second-throughput-high-speed-network-interface.html)   译文出处：[彭秦进]()
![nethogs2](http://jbcdn2.b0.upaiyun.com/2017/12/93caf558382f0e23253d7b6a7cc1b012.jpeg)
在Linux中有很多的流量监控工具，它们可以监控、分类网络流量，以花哨的图形用户界面提供实时流量分析报告。大多数这些工具（例如：[ntopng](http://xmodulo.com/2013/10/set-web-based-network-traffic-monitoring-linux.html), [](http://xmodulo.com/2013/10/set-web-based-network-traffic-monitoring-linux.html)[iftop](http://xmodulo.com/2012/06/how-to-install-iftop-on-linux.html)）都是基于[libpcap](http://www.tcpdump.org/pcap3_man.html) 库的 ，这个函数库是用来截取流经网卡的数据包的，可在用户空间用来监视分析网络流量。尽管这些工具功能齐全，然而基于libpcap库的流量监控工具无法处理高速（Gb以上）的网络接口，原因是由于在用户空间做数据包截取的系统开销过高所致。
在本文中我们介绍一种简单的Shell 脚本，它可以监控网络流量而且不依赖于缓慢的libpcap库。这些脚本支持Gb以上规模的高速网络接口，如果你对“汇聚型”的网络流量感兴趣的话，它们可统计每个网络接口上的流量。
脚本主要是基于[sysfs](http://lwn.net/Articles/31185/)虚拟文件系统，这是由内核用来将设备或驱动相关的信息输出到用户空间的一种机制。网络接口的相关分析数据会通过“/sys/class/net//statistics”输出。
举个例子，eth0的网口上分析报告会输出到这些文件中：
- **/sys/class/net/eth0/statistics/rx_packets:** 收到的数据包数据
- **/sys/class/net/eth0/statistics/tx_packets:** 传输的数据包数量
- **/sys/class/net/eth0/statistics/rx_bytes:** 接收的字节数
- **/sys/class/net/eth0/statistics/tx_bytes:** 传输的字节数
- **/sys/class/net/eth0/statistics/rx_dropped:** 当收到包数据包下降的数据量
- **/sys/class/net/eth0/statistics/tx_dropped:** 传输包数据包下降的数据量
这些数据会根据内核数据发生变更的时候自动刷新。因此，你可以编写一系列的脚本进行分析并计算流量统计。下面就是这样的脚本（感谢 [joemiller](https://gist.github.com/joemiller/4069513) 提供）。第一个脚本是统计每秒数据量，包含接收（RX）或发送（TX）。而后面的则是一个描述网络传输中的接收（RX）发送(TX)带宽。这些脚本中安装不需要任何的工具。
测量网口每秒数据包：


```
#!/bin/bash
INTERVAL="1"  # update interval in seconds
if [ -z "$1" ]; then
        echo
        echo usage: $0 [network-interface]
        echo
        echo e.g. $0 eth0
        echo
        echo shows packets-per-second
        exit
fi
IF=$1
while true
do
        R1=`cat /sys/class/net/$1/statistics/rx_packets`
        T1=`cat /sys/class/net/$1/statistics/tx_packets`
        sleep $INTERVAL
        R2=`cat /sys/class/net/$1/statistics/rx_packets`
        T2=`cat /sys/class/net/$1/statistics/tx_packets`
        TXPPS=`expr $T2 - $T1`
        RXPPS=`expr $R2 - $R1`
        echo "TX $1: $TXPPS pkts/s RX $1: $RXPPS pkts/s"
done
```
网络带宽测量


```
#!/bin/bash
INTERVAL="1"  # update interval in seconds
if [ -z "$1" ]; then
        echo
        echo usage: $0 [network-interface]
        echo
        echo e.g. $0 eth0
        echo
        exit
fi
IF=$1
while true
do
        R1=`cat /sys/class/net/$1/statistics/rx_bytes`
        T1=`cat /sys/class/net/$1/statistics/tx_bytes`
        sleep $INTERVAL
        R2=`cat /sys/class/net/$1/statistics/rx_bytes`
        T2=`cat /sys/class/net/$1/statistics/tx_bytes`
        TBPS=`expr $T2 - $T1`
        RBPS=`expr $R2 - $R1`
        TKBPS=`expr $TBPS / 1024`
        RKBPS=`expr $RBPS / 1024`
        echo "TX $1: $TKBPS kb/s RX $1: $RKBPS kb/s"
done
```
下面的屏幕截图显示了上面的两个脚本的输出。
![152940zdJ](http://jbcdn2.b0.upaiyun.com/2017/12/2f570edeab36f18c3bf5fc0f64a8feea.jpg)
