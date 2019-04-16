# Linux细节 - 常用Linux下排查服务器性能命令 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年01月14日 16:35:02[initphp](https://me.csdn.net/initphp)阅读数：2212








### uptime

说明：此命令可以快速查看系统的负载值。负载值分别为1分钟、5分钟、15分钟。



```
[root@localhost ~]# uptime
 16:32:06 up 45 min,  2 users,  load average: 0.00, 0.01, 0.05
```





### dmesg | tail

说明：dmesg命令主要是进程崩溃的时候记录内核的相关信息日志。



```
[root@localhost ~]# dmesg | tail
[   10.964269] sd 2:0:0:0: Attached scsi generic sg0 type 0
[   10.967002] sr 1:0:0:0: Attached scsi generic sg1 type 5
[   11.458428] ip_tables: (C) 2000-2006 Netfilter Core Team
[   11.515758] nf_conntrack version 0.5.0 (16384 buckets, 65536 max)
[   11.548737] ip6_tables: (C) 2000-2006 Netfilter Core Team
[   11.636783] Ebtables v2.0 registered
[   11.670765] Bridge firewalling registered
[   12.143372] e1000: eno16777736 NIC Link is Up 1000 Mbps Full Duplex, Flow Control: None
[   12.146198] IPv6: ADDRCONF(NETDEV_UP): eno16777736: link is not ready
[   12.146220] IPv6: ADDRCONF(NETDEV_CHANGE): eno16777736: link becomes ready
```





### vmstat 1

说明：vmstat会不停输出系统重要的指标信息。

r：等待cpu资源的进程数。如果这个数值大于CPU的个数，则CPU使用已经饱和。

free：内存相关信息。

si, so：交换区写入和读取的数量。如果这个数据不为0，说明系统已经在使用交换区（swap），机器物理内存已经不足。

us, sy, id, wa, st：这些都代表了CPU时间的消耗，它们分别表示用户时间（user）、系统时间（sys）、空闲时间（idle）、IO等待时间（wait）和被偷走的时间（stolen，一般被其他虚拟机消耗）




```
[root@localhost ~]# vmstat 1
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
 1  0      0 3186572    884 108636    0    0     6     1   14   21  0  0 100  0  0
 0  0      0 3185436    884 108636    0    0     0     0 1064  530  0  3 96  0  0
 3  0      0 3184680    884 108636    0    0     0     0  716  373  0  3 97  0  0
 0  0      0 3183876    884 108636    0    0     0     0  902  430  0  3 97  0  0
 0  0      0 3183056    884 108636    0    0     0     0  974  493  0  2 98  0  0
 0  0      0 3181996    884 108636    0    0     0     0 1130  591  0  3 97  0  0
 0  0      0 3180568    884 108636    0    0     0     0  903  490  0  4 96  0  0
 0  0      0 3179952    884 108636    0    0     0     0  967  523  0  4 96  0  0
```


### **mpstat -P ALL 1**



说明：查看CPU的使用情况。如果单个CPU使用率非常大，则有可能某个线程比较繁忙。



```
[root@localhost togo]# mpstat -P ALL 1
Linux 3.10.0-123.el7.x86_64 (localhost.localdomain) 	2016年01月14日 	_x86_64_	(4 CPU)

17时36分02秒  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
17时36分03秒  all    0.00    0.00    1.79    0.00    0.00    0.51    0.00    0.00    0.00   97.70
17时36分03秒    0    0.00    0.00    3.00    0.00    0.00    1.00    0.00    0.00    0.00   96.00
17时36分03秒    1    0.00    0.00    2.08    0.00    0.00    3.12    0.00    0.00    0.00   94.79
17时36分03秒    2    0.00    0.00    1.00    0.00    0.00    0.00    0.00    0.00    0.00   99.00
17时36分03秒    3    0.00    0.00    1.02    0.00    0.00    0.00    0.00    0.00    0.00   98.98

17时36分03秒  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
17时36分04秒  all    0.25    0.00    1.51    0.00    0.00    1.01    0.00    0.00    0.00   97.23
17时36分04秒    0    0.00    0.00    1.02    0.00    0.00    0.00    0.00    0.00    0.00   98.98
17时36分04秒    1    1.03    0.00    1.03    0.00    0.00    2.06    0.00    0.00    0.00   95.88
17时36分04秒    2    0.00    0.00    2.00    0.00    0.00    1.00    0.00    0.00    0.00   97.00
17时36分04秒    3    0.00    0.00    1.00    0.00    0.00    0.00    0.00    0.00    0.00   99.00
```





### **pidstat 1**

说明：pidstat命令动态输出进程的CPU占用率。



```
[root@localhost togo]# pidstat 1
Linux 3.10.0-123.el7.x86_64 (localhost.localdomain) 	2016年01月14日 	_x86_64_	(4 CPU)

17时41分02秒   UID       PID    %usr %system  %guest    %CPU   CPU  Command
17时41分03秒     0       520    0.00    0.94    0.00    0.94     3  xfsaild/dm-1
17时41分03秒     0      3210    0.00    9.43    0.00    9.43     0  togo
17时41分03秒     0      3257    0.94    2.83    0.00    3.77     3  pidstat

17时41分03秒   UID       PID    %usr %system  %guest    %CPU   CPU  Command
17时41分04秒     0      3210    1.00    9.00    0.00   10.00     0  togo
17时41分04秒     0      3257    1.00    3.00    0.00    4.00     3  pidstat
```




### **iostat -xz 1**

说明：iostat主要检查磁盘的IO情况。

r/s, w/s, rkB/s, wkB/s：分别表示每秒读写次数和每秒读写数据量

await：IO操作的平均等待时间，单位是毫秒。如果这个数值过大，可能是硬件设备遇到了瓶颈或者出现故障。

avgqu-sz：向设备发出的请求平均数量。如果这个数值大于1，可能是硬件设备已经饱和

%util：设备利用率。这个数值表示设备的繁忙程度，经验值是如果超过60，可能会影响IO性能




```
[root@localhost togo]# iostat -xz 1
Linux 3.10.0-123.el7.x86_64 (localhost.localdomain) 	2016年01月14日 	_x86_64_	(4 CPU)

avg-cpu:  %user   %nice %system %iowait  %steal   %idle
           0.04    0.00    0.60    0.01    0.00   99.36

Device:         rrqm/s   wrqm/s     r/s     w/s    rkB/s    wkB/s avgrq-sz avgqu-sz   await r_await w_await  svctm  %util
fd0               0.00     0.00    0.00    0.00     0.00     0.00     8.00     0.00   44.00   44.00    0.00  44.00   0.00
sda               0.11     0.04    0.74    0.54    18.33     2.26    32.13     0.00    1.08    0.92    1.31   0.42   0.05
scd0              0.00     0.00    0.00    0.00     0.01     0.00     8.00     0.00    2.25    2.25    0.00   2.25   0.00
dm-0              0.00     0.00    0.03    0.00     0.14     0.00     8.00     0.00    0.26    0.26    0.00   0.26   0.00
dm-1              0.00     0.00    0.74    0.51    15.05     1.97    27.32     0.00    1.26    0.87    1.82   0.40   0.05
```



### **free –m**

说明：free命令主要查看使用的内存情况。



```
[root@localhost togo]# free -m
             total       used       free     shared    buffers     cached
Mem:          3785       1704       2081          9          0        106
-/+ buffers/cache:       1597       2188
Swap:         2047          0       2047
```




### **sar -n DEV 1**

说明：查看网络设备的吞吐量



rxpck/s：每秒钟接收的数据包

txpck/s：每秒钟发送的数据包

rxbyt/s：每秒钟接收的字节数

txbyt/s：每秒钟发送的字节数

rxcmp/s：每秒钟接收的压缩数据包

txcmp/s：每秒钟发送的压缩数据包

rxmcst/s：每秒钟接收的多播数据包





```
[root@localhost togo]# sar -n DEV 1
Linux 3.10.0-123.el7.x86_64 (localhost.localdomain) 	2016年01月14日 	_x86_64_	(4 CPU)

17时54分54秒     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
17时54分55秒 eno16777736    957.00    863.00    760.60   1110.15      0.00      0.00      0.00
17时54分55秒        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00

17时54分55秒     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s
17时54分56秒 eno16777736   1066.00   1004.00    844.21   1323.74      0.00      0.00      0.00
17时54分56秒        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00
```






### **sar -n TCP,ETCP 1**

说明：查看TCP连接状况

active/s：每秒本地发起的TCP连接数，既通过connect调用创建的TCP连接；

passive/s：每秒远程发起的TCP连接数，即通过accept调用创建的TCP连接；

retrans/s：每秒TCP重传数量；

iseg/s：数据接收量

oseg/s：数据发送量




```
[root@localhost togo]# sar -n TCP,ETCP 1
Linux 3.10.0-123.el7.x86_64 (localhost.localdomain) 	2016年01月14日 	_x86_64_	(4 CPU)

17时59分10秒  active/s passive/s    iseg/s    oseg/s
17时59分11秒      0.00      0.00    641.00    987.00

17时59分10秒  atmptf/s  estres/s retrans/s isegerr/s   orsts/s
17时59分11秒      0.00      0.00      0.00      0.00      0.00

17时59分11秒  active/s passive/s    iseg/s    oseg/s
17时59分12秒      0.00      0.00    577.00    878.00

17时59分11秒  atmptf/s  estres/s retrans/s isegerr/s   orsts/s
17时59分12秒      0.00      0.00      0.00      0.00      0.00
```






### **top**

说明：[点击见详细](http://blog.csdn.net/wudiyi815/article/details/7494847)



```
[root@localhost togo]# top
top - 18:03:27 up  2:17,  3 users,  load average: 0.04, 0.12, 0.14
Tasks: 512 total,   3 running, 509 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.1 us,  1.6 sy,  0.0 ni, 97.8 id,  0.0 wa,  0.0 hi,  0.5 si,  0.0 st
KiB Mem:   3876532 total,  1744984 used,  2131548 free,      884 buffers
KiB Swap:  2097148 total,        0 used,  2097148 free.   108992 cached Mem

  PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND                                                                                                       
 3210 root      20   0 1739500 1.023g    696 S   9.9 27.7   4:51.56 togo                                                                                                          
 3334 root      20   0  123924   1992   1168 R   1.0  0.1   0:00.22 top
```



常用命令：



```java
grep '123 ' test.log.2016-05-04 |wc -l #搜索文件
```





















