
# Linux -- CentOS7修改防护墙端口 - 致林 - 博客园






# [Linux -- CentOS7修改防护墙端口](https://www.cnblogs.com/bincoding/p/6067089.html)
CentOS升级到7之后，发现无法使用iptables控制Linuxs的端口，原因是Centos 7使用firewalld代替了原来的iptables。下面记录如何使用firewalld开放Linux端口：
开启端口
firewall-cmd --zone=public --add-port=80/tcp --permanent
命令含义：
--zone \#作用域
--add-port=80/tcp  \#添加端口，格式为：端口/通讯协议
--permanent  \#永久生效，没有此参数重启后失效
重启防火墙
firewall-cmd --reload
例如安装Nagios后，要开放5666端口与服务器连接，命令如下：
[root@centos7-1 ~]\# firewall-cmd --add-port=5666/tcp　即时打开，这里也可以是一个端口范围，如1000-2000/tcp
success
[root@centos7-1 ~]\# firewall-cmd --permanent --add-port=5666/tcp　写入配置文件
success
[root@centos7-1 ~]\# firewall-cmd --reload　重启防火墙
success
[root@centos7-1 ~]\#





