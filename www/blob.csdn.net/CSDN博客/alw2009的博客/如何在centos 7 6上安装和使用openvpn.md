# 如何在centos 7.6上安装和使用openvpn. - alw2009的博客 - CSDN博客





2019年01月02日 21:35:07[traveler_zero](https://me.csdn.net/alw2009)阅读数：127








0.准备

epel是什么可以参考：[https://fedoraproject.org/wiki/EPEL](https://fedoraproject.org/wiki/EPEL)
`sudo yum install -y epel-release`
1. 安装
`sudo yum install -y openvpn`
2. 将配置文件拷贝到 /etc/openvpn/client/config

3. 启动守护进程openvpn

```
cd /etc/openvpn/client/config
openvpn --config shibs.ovpn --daemon
```





