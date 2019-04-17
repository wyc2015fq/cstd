# rabbitmq3.6.9+erl18.2.1+centos7.3最终安装版本 - zp704393004的专栏 - CSDN博客





2018年05月02日 17:03:02[原来未知](https://me.csdn.net/zp704393004)阅读数：367








                安装依赖文件：
　　yum install gcc glibc-devel make ncurses-devel openssl-devel xmlto



1)安装GCC GCC-C++ Openssl等模块,安装过就不需要安装了



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- yum -y install make gcc gcc-c++ kernel-devel m4 ncurses-devel openssl-devel  

2)安装ncurses



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- yum -y install ncurses-devel  



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

如果后面在发现rabbitmq有问题：如不能add_user 时，请再进行如下步骤

3)安装erlang环境



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- wget http://erlang.org/download/otp_src_18.2.1.tar.gz  
- tar xvfz otp_src_18.2.1.tar.gz   
- ./configure   
- make install  
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




