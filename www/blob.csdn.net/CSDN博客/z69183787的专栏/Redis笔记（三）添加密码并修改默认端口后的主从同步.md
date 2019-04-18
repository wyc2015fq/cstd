# Redis笔记（三）添加密码并修改默认端口后的主从同步 - z69183787的专栏 - CSDN博客
2015年03月25日 15:25:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：823
今天发现在Redhat as 5上编译2.6.14会报错，所以下面的演示基于2.4.18，报错信息：
[root@as5 src]# uname -a
Linux as5.cooly.slum 2.6.18-8.el5xen #1 SMP Fri Jan 26 14:42:21 EST 2007 i686 i686 i386 GNU/Linux
[root@as5 src]# make
    LINK redis-server
zmalloc.o: In function `zmalloc_used_memory':
/root/redis-2.6.14/src/zmalloc.c:223: undefined reference to `__sync_add_and_fetch_4'
collect2: ld 返回 1
make: *** [redis-server] 错误 1
[root@as5 src]#
       上一节中redis装在了三个不同的机器上，已经做了主从，也能正常同步，但是没有密码保护，并且使用了默认的端口，下面先说一下本次的主从情况：
master 127.0.0.1 6379  slave1 127.0.0.1 6381 slave2 127.0.0.1 6382
在没有添加密码前：
redis 127.0.0.1:6379> get user:name
"zl"
redis 127.0.0.1:6381> get user:name
"zl"
redis 127.0.0.1:6382> get user:name
"zl"
可见，已经主从同步。
先给master设置密码，redis提供了在线直接修改配置和修改配置文件两种方法，先使用直接修改，调用命令config set requirepass 密码，设置成功后，再执行info命令，发现返回错误，这是因为配置已经生效，访问需要使用密码，执行命令auth 密码，再次执行info命令，已经成功：
 redis 127.0.0.1:6379>config set requirepass #pass123#
OK
redis 127.0.0.1:6379> info
ERR operation not permitted
redis 127.0.0.1:6379> auth #pass123#
OK
redis 127.0.0.1:6379> info
redis_version:2.4.18
redis_git_sha1:00000000
redis_git_dirty:0
在master中新增加一个记录：
redis 127.0.0.1:6379> set user:age 29
OK
redis 127.0.0.1:6379> get user:age
"29"
redis 127.0.0.1:6379>
下面重启并访问slave1和slave2【未重启时还是同步了，我重启slave1后才不会正常同步，不知道2.6会不会有这个问题，明天说明20130719-经测试发现，2.6.14版本也是同样处理的，即，当master使用config set requirepass修改密码后，修改密码之前连接的客户端在没有重启的情况下，仍然可以访问】：
redis 127.0.0.1:6381> keys *
1) "user:name"
slave1中显示已经和master断开：
role:slave
master_host:127.0.0.1
master_port:6379
master_link_status:down
slave2中也没有同步到信息：
redis 127.0.0.1:6382> keys *
1) "user:name"
刚才设置的user:age并没有同步，说明设置的密码已经生效了，此时在slave1中输入  config set masterauth #pass123#
redis 127.0.0.1:6381> config set masterauth #pass123#
OK
redis 127.0.0.1:6381> get user:age
"29"
redis 127.0.0.1:6381>
再访问slave2，也能正常取到值了：
redis 127.0.0.1:6382> get user:age
"29"
redis 127.0.0.1:6382>
         至此，已经能访问添加了密码的master，刚才的配置在重启服务后会丢失，如果永久保存，需要修改配置文件对应的参数，如config set requirepass #pass123#只需要找到redis.conf中的requirepass，把注释去掉，并修改值为#pass123#，即可。
