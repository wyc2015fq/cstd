# 查看与修改网关，DNS - starRTC免费im直播会议一对一视频 - CSDN博客
2017年12月12日 09:42:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：208
网关是网络中的路由器，作为访问其他网络的接入点。
## 修改ip地址
- 即时生效:
**ifconfig** eth0 192.168.0.20 netmask 255.255.255.0
- 启动生效:
修改/etc/sysconfig/network-scripts/ifcfg-eth0  ，参见：CentOS配置IP小结
## 查看本机的gateway
执行netstat -rn，以0.0.0.0开头的一行的Gateway即为默认网关.
也可以查看/etc/sysconfig/network文件，里面有指定的地址.
或者route -n命令
### 修改default gateway
- 即时生效:
# route add default gw 192.168.0.254
- 启动生效:
修改/etc/sysconfig/network-scripts/ifcfg-eth0
## 查看dns
cat /etc/resolv.conf，内容如下：
nameserver 202.96.69.38
nameserver 202.96.64.38
### 修改dns
修改/etc/resolv.conf 修改后可即时生效，启动同样有效。
## 艺搜参考
[http://blog.sina.com.cn/s/blog_4b4cb06901009hz0.html](http://blog.sina.com.cn/s/blog_4b4cb06901009hz0.html)
[http://blog.csdn.net/donhao/article/details/5682015](http://blog.csdn.net/donhao/article/details/5682015)
