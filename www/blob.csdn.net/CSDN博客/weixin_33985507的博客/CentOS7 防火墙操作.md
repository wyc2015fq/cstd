# CentOS7 防火墙操作 - weixin_33985507的博客 - CSDN博客
2018年10月16日 22:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
CentOS 7 (firewall)防火墙已经改为了firewall,现整理一下相关命令如下:
firewall-cmd --list-ports #查看已开放端口
firewall-cmd --zone=public --add-port=80/tcp --permanent #开启端口,重载后生效
命令含义：
--zone #作用域
--add-port=80/tcp #添加端口，格式为：端口/通讯协议
--permanent #永久生效，没有此参数重启后失效
firewall-cmd --reload #重启防火墙,比如添加规则之后，需要执行此操作
firewall-cmd --state #查看防火墙状态，是否是running
systemctl stop firewalld.service #停止firewall
systemctl disable firewalld.service #禁止firewall开机启动
firewall-cmd --get-zones #列出支持的zone
firewall-cmd --get-services #列出支持的服务
firewall-cmd --query-service ftp #查看ftp服务是否支持，返回yes或者no
firewall-cmd --add-service=ftp #临时开放ftp服务
firewall-cmd --add-service=ftp --permanent #永久开放ftp服务
firewall-cmd --remove-service=ftp --permanent #永久移除ftp服务
iptables -L -n #查看规则
ps: 修改操作完成后执行firewall-cmd --reload 生效.
