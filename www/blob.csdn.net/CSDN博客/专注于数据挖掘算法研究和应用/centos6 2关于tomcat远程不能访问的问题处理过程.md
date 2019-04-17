# centos6.2关于tomcat远程不能访问的问题处理过程 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月24日 17:19:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5187








问题：局域网内配置好tocmat，服务器本机访问http://ip:8080成功，但远程访问http://ip:8080失败，下面是一步步排查过程：

**1 tomcat禁用IPv6**

关闭网卡IPV6协议

#netstat –anp |grep 8080  //是jsvc程序和tomcat用户在用，且端口在监听状态

#lsof –i:8080 //发现type=IPV6，很奇怪怎么是IPV6监听8080端口

#netstat –tpl //查看端口监听协议

原因是Tomcat在启动的时候，如果检测到系统支持IPV6，默认监听的是IPV6的端口，导致通过IPV4无法访问。使用address=“0.0.0.0”的意思是监听IPV4协议栈下的任何端口。IPV4协议监听任何地址应配置：0.0.0.0。IPV6协议监听任何地址应配置：0:0:0:0:0:0:0:0。

从系统层面关闭IPV6，觉得影响太大，于是先从tomcat应用层面关闭IPV6：

方法一：tomcat目录下配置 bin/Catalina.sh

最后增加：

JAVA_OPTS="$JAVA_OPTS 

-Djava.net.preferIPv4Stack=true 

-Djava.net.preferIPv4Addresses=true "

无效

方法二：tomcat目录下配置conf/server.xml

#netstat -an | grep 8080 //还是ipv6在监听

在端口8080标签内增加address=”0.0.0.0”

无效

2 系统网卡禁用IPv6


从系统层面直接关闭网卡的IPv6协议。

方法一：

#vi /etc/sysconfig/network

增加：NETWORKING_IPV6=no

#service network restart //重启网络

#lsmod |grep ipv6 //仍存在

无效

方法二：

在/etc/modprobe.d/dist.conf结尾添加

alias net-pf-10 off

alias ipv6 off

重启机子

#lsof –i:8080 //type为ipv4了

可是关闭了ipv6关闭了防火墙还是无法远程访问tomcat，崩溃中。。。




**3 防火墙配置**

增加8080端口到防火墙配置中，执行以下操作：

# vi /etc/sysconfig/iptables

增加以下代码:

~~-A RH-Firewall-1-INPUT -m state --state NEW -mtcp -p tcp --dport 8080 -j ACCEPT~~

~~-A INPUT -m state --state NEW -m tcp -p tcp--dport 8080 -j ACCEPT~~

-A INPUT –p tcp –m tcp --dport 8080 –j ACCEPT

//重启防火墙

# service iptables restart

#/etc/init.d/iptables status //查看防火墙状态，已配置8080端口

远程访问依旧失败，究竟原因是什么呢？即使关闭本机防火墙也没办法，各种折腾，已崩溃，耗时一天。




**4 局域网限制端口**

目前的状态是关闭防火墙关闭IPV6，在本机访问tocmat正常，但在局域网无法远程访问。综合分析，可能是局域网服务器区的防火墙对端口作限制，通过telnet也无法返回，最后确认是这个问题。申请局域网防火墙开放服务器的tomcat端口。




从上述一个解决问题过程中，确实发现centos部署tomcat需要关注的两点，一个是tomcat默认绑定了IPv6监听端口；另一个是服务器本机防火墙iptables需要启用8080端口。



