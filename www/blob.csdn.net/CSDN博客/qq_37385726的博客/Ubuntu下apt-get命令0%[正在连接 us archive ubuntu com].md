# Ubuntu下apt-get命令0%[正在连接 us.archive.ubuntu.com] - qq_37385726的博客 - CSDN博客





2018年09月17日 15:42:45[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：754








这是由于DNS域名解析器没配置好

如果直接去在 /etc/resolv.conf下设置nameserver，在重启后会被覆盖掉，继续变回原来那个。



正确方法是
**1. sudo vim /etc/resolvconf/resolv.conf.d/head **
2. 添加：

**nameserver 202.96.134.133nameserver 8.8.8.8**

其中8.8.8.8的是谷歌提供的DNS



**3.重启虚拟机**





