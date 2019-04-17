# ubuntu12.04LTS升级到ubuntu14.04LTS时不能连接外部网络的问题 - V__KING__的专栏 - CSDN博客





2014年08月27日 16:04:29[v__king__](https://me.csdn.net/V__KING__)阅读数：713








问题：ubuntu12.04LTS升级到ubuntu14.04LTS时不能连接外部网络的问题，打开firefox或者ping www.baidu.com都不能上网了。

解决：

1)执行：sudo /etc/init.d/networking stop 


2）设置固定ip

sudo vi /etc/network/interfaces

将里边的内容改为


```
auto lo
iface lo inet loopback
auto eth0
iface eth0 inet static
address 192.168.1.110
netmask 255.255.255.0
network 192.168.1.0
broadcast 192.168.1.255
gateway 192.168.1.1
```




3)设置dns服务器

sudo gedit /etc/resolv.conf

添加以下的记录,格式是

nameserver dns的ip地址

3) sudo /etc/init.d/networking start如果还不能连接，重启系统即可。









