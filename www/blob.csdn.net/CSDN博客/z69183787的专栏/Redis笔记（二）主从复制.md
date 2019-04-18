# Redis笔记（二）主从复制 - z69183787的专栏 - CSDN博客
2015年03月25日 15:14:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：495
个人分类：[Redis-基础](https://blog.csdn.net/z69183787/article/category/2923783)

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
        至此，已经完成了主从复制，细心的网友可能已经发现，只需配置master的地址和端口，就可以作为slave存在，这样数据就被别人盗取了！！！
目前是的，在下一节会解决这个问题。
