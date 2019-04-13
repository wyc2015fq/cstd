
# NFS搭建 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月18日 17:25:55[Jlins](https://me.csdn.net/dyllove98)阅读数：1063


服务器端：
yum
 -y install nfs*rpcbind
vi /etc/exports
/home/willserver 192.168.1.100(rw,sync,no_root_squash)
service rpcbind restart
service nfs restart
setenforce 0

客户端：
mkdir /mnt/testclient
yum -y install nfs*
mount -t nfs 192.168.1.100:/home/testserver /mnt/testclient/
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

