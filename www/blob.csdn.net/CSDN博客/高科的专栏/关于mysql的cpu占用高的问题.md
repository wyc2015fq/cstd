
# 关于mysql的cpu占用高的问题 - 高科的专栏 - CSDN博客

2015年01月12日 17:06:35[高科](https://me.csdn.net/pbymw8iwm)阅读数：7105


现在游戏开了泰服 ，发现泰服的cpu占用率总是比繁体或者大陆的高很多，每次都是占用了300%多
top - 15:34:06 up 222 days,  2:51,  2 users,  load average: 0.75, 0.73, 0.66
Tasks: 215 total,   1 running, 214 sleeping,   0 stopped,   0 zombie
Cpu(s): 52.4%us,  8.5%sy,  0.0%ni, 37.8%id,  1.0%wa,  0.0%hi,  0.3%si,  0.0%st
Mem:  32862732k total, 31396120k used,  1466612k free,   194100k buffers
Swap: 12361720k total,    36232k used, 12325488k free, 29657368k cached

PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
5032 mysql     20   0 8960m 370m 4696 S 494.9  1.2  29146,13 mysqld
386 root      20   0     0    0    0 S  1.7  0.0   4479:15 jbd2/dm-0-8
9208 root      20   0 97084 4076 3012 S  1.7  0.0   0:00.05 sshd
9210 root      20   0 96832 3920 2996 S  1.0  0.0   0:00.03 sshd
58 root      20   0     0    0    0 S  0.3  0.0 938:02.46 kblockd/0
59 root      20   0     0    0    0 S  0.3  0.0 826:25.48 kblockd/1
9207 root      20   0 15172 1420 1008 R  0.3  0.0   0:00.02 top
但是我的mysql运行是正常的，首先一般最常用的方法就是看mysql的配置

但是我的配置应该也是没有问题的，所以我mysql等进来看看负载最大的语句是那些：
![](https://img-blog.csdn.net/20150112170835101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGJ5bXc4aXdt/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
show processlist;

查找负荷最重的 SQL 语句，找到了负载大的语句，我发现原来是我的表结构是联合主键，但是没有设置索引，我于是创建了索引就降下来了。
关于后边如果还有其他发现，会及时更新到本片文章中



