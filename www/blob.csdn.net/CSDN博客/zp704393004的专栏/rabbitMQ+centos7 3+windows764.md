# rabbitMQ+centos7.3+windows764 - zp704393004的专栏 - CSDN博客





2018年04月17日 10:29:30[原来未知](https://me.csdn.net/zp704393004)阅读数：34
个人分类：[学习记录](https://blog.csdn.net/zp704393004/article/category/7595660)









参考网址：https://blog.csdn.net/qq_34021712/article/details/72567786

1.安装Erlang环境





1)安装GCC GCC-C++ Openssl等模块,安装过就不需要安装了



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- yum -y install make gcc gcc-c++ kernel-devel m4 ncurses-devel openssl-devel  

2)安装ncurses



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- yum -y install ncurses-devel  

3)安装erlang环境



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- wget http://erlang.org/download/otp_src_18.2.1.tar.gz  
- tar xvfz otp_src_18.2.1.tar.gz   
- ./configure   
- make install  





2.安装RabbitMQ 



1)下载rabbitmq-server-3.6.9.tar.xz



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- wget http://www.rabbitmq.com/releases/rabbitmq-server/v3.6.9/rabbitmq-server-generic-unix-3.6.9.tar.xz      

2)对于下载xz包进行解压，首先先下载xz压缩工具：



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- yum install xz  

3)对rabbitmq包进行解压：



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- xz -d xz -d rabbitmq-server-generic-unix-3.6.9.tar.xz  
- tar -xvf rabbitmq-server-generic-unix-3.6.9.tar  

4)随后移动至/usr/local/下 改名rabbitmq：



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- cp -r rabbitmq_server-3.6.9 /usr/local/rabbitmq  

5)这种下载的方式解压后直接可以使用，无需再编译安装； 



**[html]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- 进入到rabbit文件内，其命令文件存在于sbin文件夹下，因此需要将sbin文件夹的路径添加到PATH中：修改/etc/profile  
- export PATH=/usr/local/rabbitmq/sbin:$PATH     
- 执行source /etc/profile使得PATH路径更新，rabbitMQ安装成功。  

6)随后启用MQ管理方式：



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- rabbitmq-plugins enable rabbitmq_management   #启动后台管理  
- rabbitmq-server -detached   #后台运行rabbitmq  

7)设置端口号，可供外部访问：



**[plain]**[view plain](https://blog.csdn.net/qq_34021712/article/details/72567786#)[copy](https://blog.csdn.net/qq_34021712/article/details/72567786#)

- iptables -I INPUT -p tcp --dport 15672 -j ACCEPT  

8)添加用户和权限
默认网页guest用户是不允许访问的，需要增加一个用户修改一下权限，代码如下：
添加用户:rabbitmqctl add_user admin admin

添加权限:rabbitmqctl set_permissions -p "/" admin ".*" ".*" ".*"

修改用户角色:rabbitmqctl set_user_tags admin administrator

然后就可以远程访问了，然后可直接配置用户权限等信息。



