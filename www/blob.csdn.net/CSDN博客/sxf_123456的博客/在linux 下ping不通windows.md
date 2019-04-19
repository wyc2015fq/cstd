# 在linux 下ping不通windows - sxf_123456的博客 - CSDN博客
2017年11月11日 02:29:40[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：761
1、首先，分别在liunx和window,使用ifconfig和ipconfig命令，看看是不是在同一网段，在同一网段
2、
在关闭linux下的防火墙。输入命令如下：
chkconfig iptables off/on  (永久性关闭和开启防火墙)， service iptables stop/start (暂时性关闭或开启防火墙)。
 也可以开启防火墙时设置端口号：
修改/etc/sysconfig/iptables 文件，添加以下内容： 
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT 
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT
