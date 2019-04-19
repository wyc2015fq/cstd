# RabbitMQ消息队列服务 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2018年02月02日 10:32:20[baofenny](https://me.csdn.net/jiaminbao)阅读数：801
                
配置消息队列服务
===============================================
配置时间同步
设置时区
timedatectl set-timezone Asia/Shanghai
#开启NTP服务器同步
#timedatectl  set-ntp yes
配置时间服务器
vim /etc/chrony.conf
查看chrony服务状态
systemctl status chronyd
重启服务
systemctl restart chronyd
查看时间信息
timedatectl
=================================================
参考rabbitmq官网
https://www.rabbitmq.com/install-rpm.html
下载MQ
[root@rabbitmq-server01 soft]# wget https://dl.bintray.com/rabbitmq/all/rabbitmq-server/3.7.2/rabbitmq-server-3.7.2-1.el7.noarch.rpm
安装MQ之前，需要安装Erlang
系统版本：centos7.3
参考配置Erlang源
https://github.com/rabbitmq/erlang-rpm
1、配置yum源
[root@rabbitmq-server01 ~]# 
# In /etc/yum.repos.d/rabbitmq-erlang.repo
[rabbitmq-erlang]
name=rabbitmq-erlang
baseurl=https://dl.bintray.com/rabbitmq/rpm/erlang/20/el/7
gpgcheck=1
gpgkey=https://www.rabbitmq.com/rabbitmq-release-signing-key.asc
repo_gpgcheck=0
enabled=1
[root@rabbitmq-server01 ~]# yum install erlang
RPM安装RabbitMQ
[root@rabbitmq-server01 ~]# rpm -ivh rabbitmq-server/3.7.2/rabbitmq-server-3.7.2-1.el7.noarch.rpm
拷贝RabbitMQ配置文件
[root@rabbitmq-server01 ~]# cp /usr/share/doc/rabbitmq-server-3.7.2/rabbitmq.config.example \ /etc/rabbitmq/rabbitmq.config
开启RabbitMQ守护进程
[root@rabbitmq-server01 ~]# rabbitmq-plugins enable rabbitmq_management
启动rabbitMQ
[root@rabbitmq-server01 ~]# systemctl start rabbitmq-server.service
添加开机启动
[root@rabbitmq-server01 ~]# systemctl enable rabbitmq-server
查看MQ运行状态
[root@rabbitmq-server01 ~]# systemctl status rabbitmq-server.service
放行端口
[root@rabbitmq-server01 ~]# firewall-cmd --zone=public --add-port=5672/tcp --permanent
[root@rabbitmq-server01 ~]# firewall-cmd --add-port=15672/tcp --permanent
[root@rabbitmq-server01 ~]# firewall-cmd --reload
[root@rabbitmq-server01 ~]# systemctl restart rabbitmq-server.service
连接MQ
http://ip:15672
用户名：guest
密  码：guest
问题总结：
Jan 12 15:57:05 test-mq01 rabbitmqctl[15083]: 15:57:05.918 [error] Error when reading /var/lib/rabbitmq/.erlang.cookie: eacces
解决方法：
查看erlang.cookie
[root@rabbitmq-server01 ~]# ll -a /var/lib/rabbitmq
查看erlang.cookie的权限，必须是rabbitmq
[root@rabbitmq-server01 ~]# ll -a
total 8
drwxr-xr-x.  5 rabbitmq rabbitmq   70 Jan 12 16:01 .
drwxr-xr-x. 26 root     root     4096 Jan 12 15:17 ..
drwxr-x---.  2 rabbitmq rabbitmq    6 Jan 12 16:01 config
-r--------.  1 rabbitmq rabbitmq   20 Jan 12 13:00 .erlang.cookie
#####################################################################
集群搭建
1、保证erlang.cookie必须相同
2、修改hosts文件
[root@rabbitmq-server01 ~]# vim  /etc/hosts
10.160.25.60 test-mq01
10.160.25.61 test-mq02
3、逐个启动节点
4、更改节点属性（在MQ2上操作）
//关闭rabbit服务（MQ2）
#rabbitmqctl stop_app
//添加节点
#rabbitmqctl join_cluster rabbit@test-mq01
//开启rabbit服务
#rabbitmqctl start_app
//查看集群信息
#rabbitmqctl cluster_status
假设要把rabbitmq2退出集群
在rabbitmq2上执行
#rabbitmqctl stop_app
#rabbitmqctl reset
#rabbitmqctl start_app
在集群主节点上执行
# rabbitmqctl forget_cluster_node rabbit@rabbitmq2
4   RABBITMQ集群重启
集群重启时，最后一个挂掉的节点应该第一个重启，如果因特殊原因（比如同时断电），而不知道哪个节点最后一个挂掉。可用以下方法重启：
先在一个节点上执行
#rabbitmqctl force_boot
#service rabbitmq-server start
在其他节点上执行
#service rabbitmq-server start
查看cluster状态是否正常（要在所有节点上查询）。
#rabbitmqctl cluster_status
问题总结：
在添加节点join_cluster后提示：
Error: unable to perform an operation on node 'rabbit@test-mq01'
查看提示内容可以看到错误原因
因为MQ2去连接MQ1时，会去连接epmd服务（端口：4369）
解决方法：
1、关闭防火墙
2、开放4369端口

