# 详解TCP三次握手四次挥手 - 星爷2401 - CSDN博客
2017年04月19日 11:51:04[星爷2401](https://me.csdn.net/leifukes)阅读数：580

## **学习TCP连接之前，需要学习一下tcpdump命令**
1、抓取网卡数据
`tcpdump -i eth1`
2、监控所有IP：192.168.0.1发出和接收的数据包。
`tcpdump host 192.168.0.1`
3、监控IP：192.168.1.102和IP192.168.1.111以及IP192.168.1.112之间的通讯数据。
`tcpdump host 192.168.1.1 and \ (192.168.1.11  or 192.168.1.112 \) `
4、监控通过eth1网卡，目的地址或原地址是192.168.1.1的数据包
`tcpdump -i eth1 host 192.168.1.1`
5、监控通过eth1网卡，原地址是192.168.1.1的数据包
` tcpdump -i eth1 src host 192.168.1.1`
6、监控通过eth1网卡，目的地址是192.168.1.1的数据包
` tcpdump -i eth1 dst host 192.168.1.1`
7、抓取所有经过eth1，目的或源端口是8088的数据
`tcpdump -i eth1 port 8088`
8、抓取所有经过eth1，源端口是8088的数据
`tcpdump -i eth1 src port 8088`
9、抓取所有经过eth1，目的端口是8088的数据
`tcpdump -i eth1 dst port 8088`
10、同样抓取网络ip
```
tcpdump -i eth1 net 192.168
tcpdump -i eth1 src net 192.168
```
`tcpdump -i eth1 dst net 192.168`
关于tcpdump的flag状态看连接：http://blog.csdn.net/leifukes/article/details/53588026
