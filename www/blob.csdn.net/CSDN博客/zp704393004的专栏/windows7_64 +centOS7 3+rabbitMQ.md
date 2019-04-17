# windows7_64 +centOS7.3+rabbitMQ - zp704393004的专栏 - CSDN博客





2018年04月16日 18:34:44[原来未知](https://me.csdn.net/zp704393004)阅读数：49标签：[RabbitMQ](https://so.csdn.net/so/search/s.do?q=RabbitMQ&t=blog)
个人分类：[学习记录](https://blog.csdn.net/zp704393004/article/category/7595660)









参考网址【略有不同】：

https://www.linuxidc.com/Linux/2016-03/129557.htm




RabbitMQ是流行的开源消息队列系统，是AMQP（Advanced Message Queuing Protocol高级消息队列协议）的标准实现，用erlang语言开发。RabbitMQ据说具有良好的性能和时效性，同时还能够非常好的支持集群和负载部署，非常适合在较大规模的分布式系统中使用，具体特性还在验证中，待检验。由于项目需要，安装并配置了RabbitMQ，服务器操作系统是[CentOS](https://www.linuxidc.com/topicnews.aspx?tid=14)7。具体步骤如下：

　　安装依赖文件：

　　yum install gcc glibc-devel make ncurses-devel openssl-devel xmlto

　　1.Erlang安装配置

　　下载安装包，地址http://www.erlang.org/downloads，我选择的是otp_src_18.2.1.tar.gz。

　　然后解压文件：

[root@iZ25e3bt9a6Z rabbitmq]# tar -xzvf otp_src_18.2.1.tar.gz
[root@iZ25e3bt9a6Z rabbitmq]# cd otp_src_18.2.1/

　　配置安装路径编译代码：

[root@iZ25e3bt9a6Z otp_src_18.3]# ./configure --prefix=/opt/erlang

　　执行编译结果：

[root@iZ25e3bt9a6Z otp_src_18.3]# make && make install

　　完成后进入/opt/erlang查看执行结果

[root@iZ25e3bt9a6Z rabbitmq]# cd /opt/erlang/
[root@iZ25e3bt9a6Z erlang]# erl
Erlang/OTP 18 [erts-7.3] [source] [64-bit] [smp:8:8] [async-threads:10] [hipe] [kernel-poll:false]

Eshell V7.3  (abort with ^G)
1>

　　当出现以上信息时表示安装完成。然后输入’halt().’退出即可。

　　然后在配置Erlang环境变量,vi /etc/profile文件，增加下面的环境变量:

#set erlang environment
export PATH=$PATH:/opt/erlang/bin


　　source  /etc/profile使得文件生效

　　在安装Erlang过程中，可能会遇到以下问题，一般都是因为系统中缺少相应的包引起的，缺少什么包直接yum安装即可。

　　2.下载安装RabbitMq

[root@iZ25e3bt9a6Z rabbitmq]# wget http://www.rabbitmq.com/releases/rabbitmq-server/v3.6.9/rabbitmq-server-generic-unix-3.6.9.tar.xz

　　解压文件

[root@iZ25e3bt9a6Z rabbitmq]# xz -d rabbitmq-server-generic-unix-3.6.9.tar.xz
[root@iZ25e3bt9a6Z rabbitmq]# tar -xvf rabbitmq-server-generic-unix-3.6.9.tar　【这里直接复制到了/opt/里面】

　　解压后进入文件夹/opt发现多了个文件夹rabbitmq-server-generic-unix-3.6.9，重命名为rabbitmq以便记忆。

　　然后在配置rabbitmq环境变量,vi /etc/profile文件，增加下面的环境变量:

#set rabbitmq environment
export PATH=$PATH:/opt/rabbitmq/sbin[这里使用的export PATH=/opt/rabbitmq/sbin:$PATH   ]

　　source  /etc/profile使得文件生效

　　3.RabbitMQ服务启动关闭

　　以上就已经完成了RabbitMq的安装，怎么启动服务呢？

　　启动服务：

[root@iZ25e3bt9a6Z rabbitmq]# cd sbin/


[root@iZ25e3bt9a6Z sbin]# ./rabbitmq-server【这里有区别】

如果成功，将出现

RabbitMQ 3.6.1. Copyright (C) 2007-2016 Pivotal Software, Inc.
  ##  ##      Licensed under the MPL.  See http://www.rabbitmq.com/
  ##  ##
  ##########  Logs: /opt/rabbitmq/var/log/rabbitmq/rabbit@localhost.log
  ######  ##        /opt/rabbitmq/var/log/rabbitmq/rabbit@localhost-sasl.log
  ##########
              Starting broker... completed with 0 plugins.


　　查看服务状态：

[root@iZ25e3bt9a6Z sbin]# ./rabbitmqctl status
Status of node rabbit@iZ25e3bt9a6Z ...
[{pid,11849},
 {running_applications,
    [{rabbitmq_management,"RabbitMQ Management Console","3.6.1"},
      {rabbitmq_management_agent,"RabbitMQ Management Agent","3.6.1"},
      {rabbitmq_web_dispatch,"RabbitMQ Web Dispatcher","3.6.1"},
      {webmachine,"webmachine","1.10.3"},
      {amqp_client,"RabbitMQ AMQP Client","3.6.1"},
      {mochiweb,"MochiMedia Web Server","2.13.0"},
      {syntax_tools,"Syntax tools","1.7"},
      {ssl,"Erlang/OTP SSL application","7.3"},
      {public_key,"Public key infrastructure","1.1.1"},
      {asn1,"The Erlang ASN1 compiler version 4.0.2","4.0.2"},
      {crypto,"CRYPTO","3.6.3"},
      {compiler,"ERTS  CXC 138 10","6.0.3"},
      {inets,"INETS  CXC 138 49","6.2"},
      {rabbit,"RabbitMQ","3.6.1"},
      {mnesia,"MNESIA  CXC 138 12","4.13.3"},
      {rabbit_common,[],"3.6.1"},
      {xmerl,"XML parser","1.3.10"},
      {os_mon,"CPO  CXC 138 46","2.4"},
      {ranch,"Socket acceptor pool for TCP protocols.","1.2.1"},
      {sasl,"SASL  CXC 138 11","2.7"},
      {stdlib,"ERTS  CXC 138 10","2.8"},
      {kernel,"ERTS  CXC 138 10","4.2"}]},
 {os,{unix,linux}},
 {erlang_version,
    "Erlang/OTP 18 [erts-7.3] [source] [64-bit] [smp:8:8] [async-threads:64] [hipe] [kernel-poll:true]\n"},
 {memory,
    [{total,64111264},
      {connection_readers,0},
      {connection_writers,0},
      {connection_channels,0},
      {connection_other,2808},
      {queue_procs,2808},
      {queue_slave_procs,0},
      {plugins,367288},
      {other_proc,19041296},
      {mnesia,61720},
      {mgmt_db,158696},
      {msg_index,47120},
      {other_ets,1372440},
      {binary,128216},
      {code,27368230},
      {atom,992409},
      {other_system,14568233}]},
 {alarms,[]},
 {listeners,[{clustering,25672,"::"},{amqp,5672,"::"}]},
 {vm_memory_high_watermark,0.4},
 {vm_memory_limit,6556241100},
 {disk_free_limit,50000000},
 {disk_free,37431123968},
 {file_descriptors,
    [{total_limit,65435},
      {total_used,2},
      {sockets_limit,58889},
      {sockets_used,0}]},
 {processes,[{limit,1048576},{used,204}]},
 {run_queue,0},
 {uptime,412681},
 {kernel,{net_ticktime,60}}]

　　关闭服务：

[root@iZ25e3bt9a6Z sbin]# ./rabbitmqctl stop


Stopping and halting node rabbit@iZ25e3bt9a6Z ...

在第二天又出现启动失败，boot fail，这个应该是erlang安装问题，重新安装erlang。


　　4. 配置网页插件

　　首先创建目录，否则可能报错：

mkdir /etc/rabbitmq


　　然后启用插件：

./rabbitmq-plugins enable rabbitmq_management

　　配置linux 端口 15672 网页管理  5672 AMQP端口
　　然后访问http://localhost:15672即可 

　　默认用户guest 密码guest

![](https://www.linuxidc.com/upload/2016_03/160328210925611.png)

　　5. 远程访问配置

　　默认网页是不允许访问的，需要增加一个用户修改一下权限，代码如下：

　　添加用户:rabbitmqctl add_user hxb hxb

　　添加权限:rabbitmqctl set_permissions -p "/" hxb ".*" ".*" ".*"

      修改用户角色rabbitmqctl set_user_tags hxb administrator

　　然后就可以远程访问了，然后可直接配置用户权限等信息。



　　6. rabbitmq常用命令

　　add_user        <UserName> <Password>

　　delete_user    <UserName>

　　change_password <UserName> <NewPassword>

　　list_users

　　add_vhost    <VHostPath>

　　delete_vhost <VHostPath>

　　list_vhostsset_permissions  [-p <VHostPath>] <UserName> <Regexp> <Regexp> <Regexp>

　　clear_permissions [-p <VHostPath>] <UserName>

　　list_permissions  [-p <VHostPath>]

　　list_user_permissions <UserName>

　　list_queues    [-p <VHostPath>] [<QueueInfoItem> ...]

　　list_exchanges [-p <VHostPath>] [<ExchangeInfoItem> ...]

　　list_bindings  [-p <VHostPath>]

　　list_connections [<ConnectionInfoItem> ...]




