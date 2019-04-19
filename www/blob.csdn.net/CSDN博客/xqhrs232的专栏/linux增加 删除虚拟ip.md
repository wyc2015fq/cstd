# linux增加/删除虚拟ip - xqhrs232的专栏 - CSDN博客
2018年05月02日 16:12:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：256
原文地址::[https://blog.csdn.net/zhengwei125/article/details/53019651](https://blog.csdn.net/zhengwei125/article/details/53019651)
相关文章
1、linux增加/删除虚拟IP地址-zt----[http://www.51testing.com/html/00/130600-225976.html](http://www.51testing.com/html/00/130600-225976.html)
 在网卡eth1上添加一个虚拟ip 192.168.26.74
1.添加ip
[root@zw-test-db mastermha]#/sbin/ip
 addr add 192.168.26.74/32 dev eth1
2.查看添加的虚拟ip
[root@zw-test-db mastermha]# ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN 
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP qlen 1000
    link/ether 00:50:56:a3:6f:08 brd ff:ff:ff:ff:ff:ff
    inet 192.168.26.233/24 brd 192.168.26.255 scope global eth1
 inet
 192.168.26.74/32 scope global eth1  #这个ip就是我刚新加的
    inet6 fe80::250:56ff:fea3:6f08/64 scope link 
       valid_lft forever preferred_lft forever
3.ping一下看新加的虚拟ip能否ping通
[root@zw-test-db mastermha]# ping 192.168.26.74
PING 192.168.26.74 (192.168.26.74) 56(84) bytes of data.
64 bytes from 192.168.26.74: icmp_seq=1 ttl=64 time=0.028 ms
64 bytes from 192.168.26.74: icmp_seq=2 ttl=64 time=0.029 ms
64 bytes from 192.168.26.74: icmp_seq=3 ttl=64 time=0.020 ms
64 bytes from 192.168.26.74: icmp_seq=4 ttl=64 time=0.023 ms
4.比如要想删除刚添加的虚拟ip就用如下命令：
[root@zw-test-db mastermha]#/sbin/ip addr del 192.168.26.74/32 dev eth1
5. 为了防止下次重启找不到添加的虚拟ip，可以把命令加到/etc/rc.loca里面
/sbin/ip addr add
 192.168.26.74/32 dev eth1
