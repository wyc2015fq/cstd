# Redis:主从复制 - 零度的博客专栏 - CSDN博客
2016年09月22日 17:40:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：271标签：[redis																[主从复制](https://so.csdn.net/so/search/s.do?q=主从复制&t=blog)](https://so.csdn.net/so/search/s.do?q=redis&t=blog)
个人分类：[Redis](https://blog.csdn.net/zmx729618/article/category/6430276)
### 1、主从复制       
       单台redis会遇到单点故障的问题，为了解决redis的高可用，主从复制应该是学习的第二个步骤。本文想达到的目的是，一个master带一个slave，而slave又带了一个slave，这样的好处是，当master故障后，直接把slaver1改为master，其他的配置不需要修改，服务又可以正常使用。
![](https://img-blog.csdn.net/20130718185519515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3Nlcmdzdw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
       作为master的redis配置文件不需要修改，此处只修改slave1和slave12的配置文件，对于slave1只需在redis.conf中找到# slaveof <masterip> <masterport>并修改为 slaveof192.168.1.18 6379 然后保存即可，而slave12则修改为slaveof 192.168.10.10 6379 ，这样就可以了，启动slave1和slave12试试吧。
在192.168.1.18上执行redis-cli，使用info命令，在# Replication处能看到
role:master
connected_slaves:1
slave0: 192.168.10.10,6379,online
在192.168.10.10上的redis-cli中可以看到：
# Replication
role:slave
master_host: 192.168.1.18
master_port:6379
master_link_status:up
master_last_io_seconds_ago:7
master_sync_in_progress:0
slave_priority:100
slave_read_only:1
connected_slaves:1
slave0: 192.168.12.12,6379,online
在192.168.12.12上可以看到：
# Replication
role:slave
master_host: 192.168.10.10
master_port:6379
master_link_status:up
master_last_io_seconds_ago:3
master_sync_in_progress:0
slave_priority:100
slave_read_only:1
connected_slaves:0
     此时我们想实现的目标已经实现，但是发现在两个slave上无法插入数据，当master故障后，需要在slave1上首先执行slaveof no one命令，再次执行info Replication时，它已经转为master了，此时slave12不受影响。
redis 192.168.10.10:6379> slaveof no one
OK
redis 192.168.10.10:6379> infoReplication
# Replication
role:master
connected_slaves:1
slave0:127.0.0.1,6382,online
redis 192.168.10.10:6379>
        至此，已经完成了主从复制，细心的同学可能已经发现，只需配置master的地址和端口，就可以作为slave存在，这样数据就被别人盗取了。
### 2、主从同步添加密码及修改端口
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
