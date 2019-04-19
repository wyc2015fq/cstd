# redis Sentinel部署 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年04月02日 15:57:26[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：22
sentinel 系统用于管理多个 Redis 服务器（instance）
执行以下三个任务：
- 监控（Monitoring）： Sentinel 会不断地检查你的主服务器和从服务器是否运作正常。
- 提醒（Notification）： 当被监控的某个 Redis 服务器出现问题时， Sentinel 可以通过 API 向管理员或者其他应用程序发送通知。
- 自动故障迁移（Automatic failover）： 当一个主服务器不能正常工作时， Sentinel 会开始一次自动故障迁移操作， 它会将失效主服务器的其中一个从服务器升级为新的主服务器， 并让失效主服务器的其他从服务器改为复制新的主服务器； 当客户端试图连接失效的主服务器时， 集群也会向客户端返回新主服务器的地址， 使得集群可以使用新主服务器代替失效服务器。
 Redis Sentinel 释出为一个单独的可执行文件 redis-sentinel ， 但实际上它只是一个运行在特殊模式下的 Redis 服务器， 你可以在启动一个普通 Redis 服务器时通过给定 --sentinel 选项来启动 Redis Sentinel 。
启动 Sentinel
对于 redis-sentinel 程序， 你可以用以下命令来启动 Sentinel 系统：
redis-sentinel /path/to/sentinel.conf
对于 redis-server 程序， 你可以用以下命令来启动一个运行在 Sentinel 模式下的 Redis 服务器：
redis-server /path/to/sentinel.conf --sentinel
两种方法都可以启动一个 Sentinel 实例。
启动 Sentinel 实例必须指定相应的配置文件， 系统会使用配置文件来保存 Sentinel 的当前状态， 并在 Sentinel 重启时通过载入配置文件来进行状态还原。
配置 Sentinel
Redis 源码中包含了一个名为 sentinel.conf 的文件， 这个文件是一个带有详细注释的 Sentinel 配置文件示例。
运行一个 Sentinel 所需的最少配置如下所示：
sentinel monitor mymaster 127.0.0.1 6379 2 sentinel down-after-milliseconds mymaster 60000 sentinel failover-timeout mymaster 180000 sentinel parallel-syncs mymaster 1
第一行配置指示 Sentinel 去监视一个名为 mymaster 的主服务器， 这个主服务器的 IP 地址为 127.0.0.1 ， 端口号为 6379 ， 而将这个主服务器判断为失效至少需要 2 个 Sentinel 同意 （只要同意 Sentinel 的数量不达标，自动故障迁移就不会执行）。
其他选项的基本格式如下：
sentinel <选项的名字> <主服务器的名字> <选项的值>
[http://redisdoc.com/topic/sentinel.html](http://redisdoc.com/topic/sentinel.html)
[https://blog.csdn.net/men_wen/article/details/72724406](https://blog.csdn.net/men_wen/article/details/72724406)
分别有3个Sentinel节点，1个主节点，2个从节点组成一个Redis Sentinel。
role IP port
master 127.0.0.1 6379
slave1 127.0.0.1 6380
slave2 127.0.0.1 6381
Sentinel1 127.0.0.1 26379
Sentinel2 127.0.0.1 26380
Sentinel3 127.0.0.1 26381
---------------------
主节点
配置：
port 6379
daemonize yes
logfile "6379.log"
dbfilename "dump-6379.rdb"
dir "/var/redis/data/"
1
2
3
4
5
启动主节点
➜ sudo redis-server redis-6379.conf
---------------------
启动两个从节点
配置（两个从节点配置相同，除了文件名有区分）
port 6380
daemonize yes
logfile "6380.log"
dbfilename "dump-6380.rdb"
dir "/var/redis/data/"
slaveof 127.0.0.1 6379 // 从属主节点
1
2
3
4
5
6
启动两个从节点
➜ sudo redis-server redis-6380.conf
➜ sudo redis-server redis-6381.conf
---------------------
确认主从关系
redis-cli -h 127.0.0.1 -p 6379 INFO replication # Replication role:master connected_slaves:2
- 从节点视角（6380端口）
➜ redis-cli -h 127.0.0.1 -p 6380 INFO replication # Replication role:slave
部署Sentinel节点
以26379为例。
配置
// Sentinel节点的端口
port 26379
dir /var/redis/data/
logfile "26379.log"
// 当前Sentinel节点监控 127.0.0.1:6379 这个主节点
// 2代表判断主节点失败至少需要2个Sentinel节点节点同意
// mymaster是主节点的别名 初次配置时主数据库的地址和端口号，当主数据库发生变化时，哨兵会自动更新这个配置
sentinel monitor mymaster 127.0.0.1 6379 2
//每个Sentinel节点都要定期PING命令来判断Redis数据节点和其余Sentinel节点是否可达，如果超过30000毫秒且没有回复，则判定不可达
sentinel down-after-milliseconds mymaster 30000
//当Sentinel节点集合对主节点故障判定达成一致时，Sentinel领导者节点会做故障转移操作，选出新的主节点，原来的从节点会向新的主节点发起复制操作，限制每次向新的主节点发起复制操作的从节点个数为1
sentinel parallel-syncs mymaster 1
//故障转移超时时间为180000毫秒
sentinel failover-timeout mymaster 180000
---------------------
启动redis-sentinel sentinel-26379.conf &
验证
redis-cli -h 127.0.0.1 -p 26379 INFO Sentinel
