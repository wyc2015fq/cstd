# CentOS7中使用iptables - 菜鸟路上的小白 - CSDN博客





2016年11月14日 17:39:19[somnus_小凯](https://me.csdn.net/u012486840)阅读数：22628
个人分类：[linux](https://blog.csdn.net/u012486840/article/category/6382672)









## 1、关闭firewall：


```
systemctl stop firewalld.service #停止firewall

systemctl disable firewalld.service #禁止firewall开机启动
```


## 2、安装iptables防火墙


```
#安装
yum install iptables-services
```



编辑防火墙配置文件


```
vi /etc/sysconfig/iptables
```



```
# sample configuration for iptables service
# you can edit this manually or use system-config-firewall
# please do not ask us to add additional ports/services to this default configuration
*filter
:INPUT ACCEPT [0:0]
:FORWARD ACCEPT [0:0]
:OUTPUT ACCEPT [0:0]
-A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
-A INPUT -p icmp -j ACCEPT
-A INPUT -i lo -j ACCEPT

-A INPUT -p tcp -m state --state NEW -m tcp --dport 22 -j ACCEPT
-A INPUT -p tcp -m state --state NEW -m tcp --dport 21 -j ACCEPT
-A INPUT -p tcp -m state --state NEW -m tcp --dport 20 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80  -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 3306 -j ACCEPT
-A INPUT -j REJECT --reject-with icmp-host-prohibited
-A FORWARD -j REJECT --reject-with icmp-host-prohibited
COMMIT
```



```
:wq! #保存退出
```



```
systemctl restart iptables.service #最后重启防火墙使配置生效

systemctl enable iptables.service #设置防火墙开机启动
```


## 3、关闭SELINUX


```
vi /etc/selinux/config
```



```
#SELINUX=enforcing #注释掉
#SELINUXTYPE=targeted #注释掉
SELINUX=disabled #增加
```



```
:wq! #保存退出
```


`setenforce 0 #使配置立即生效`



