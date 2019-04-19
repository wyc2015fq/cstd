# ubuntu14.04设置静态ip - xqhrs232的专栏 - CSDN博客
2016年10月19日 00:52:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：182
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.cnblogs.com/vincedotnet/p/4013099.html](http://www.cnblogs.com/vincedotnet/p/4013099.html)

1. 找到文件并作如下修改：
sudo vim /etc/network/interfaces
修改如下部分：
auto eth0
iface eth0 inet static
address 192.168.0.117
gateway 192.168.0.1 #这个地址你要确认下 网关是不是这个地址
netmask 255.255.255.0
network 192.168.0.0
broadcast 192.168.0.255
2. 修改dns解析
因为以前是dhcp解析，所以会自动分配dns服务器地址
而一旦设置为静态ip后就没有自动获取到的dns服务器了
要自己设置一个
sudo vim /etc/resolv.conf
写上一个公网的DNS
nameserver 202.96.128.86
（注意：8.8.8.8是谷歌的DNS服务器，但是解析速度慢，还是找到一个国内的dns来用）
3. 重启网卡：
sudo /etc/init.d/network restart
