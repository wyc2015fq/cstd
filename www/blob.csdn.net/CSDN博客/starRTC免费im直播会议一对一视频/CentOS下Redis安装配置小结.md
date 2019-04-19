# CentOS下Redis安装配置小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月08日 11:17:01[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：190
Redis是REmote DIctionary Server的缩写。 是一个使用 C 语言写成的，开源的 key-value 非关系型数据库。跟memcached类似，不过数据可以持久化。
Redis的所有数据都保存在内存中，然后不定期的通过异步方式保存到磁盘上(称为“半持久化模式”)；也可以把每一次数据变化都写入到一个append
 only file(aof)里面(称为“全持久化模式”)。
要想成功使用一种产品，我们需要先深入了解它的特性。
## 安装
下载源码：
$ **tar** xzf redis-2.8.8.tar.gz$ **cd** redis-2.8.8$ **make**
make命令执行完成后，会在src目录下生成5个可执行文件，分别是redis-server、redis-cli、redis-benchmark、redis-check-aof、redis-check-dump，它们的作用分别如下：
- redis-server：Redis服务器的daemon启动程序
- redis-cli：Redis命令行操作工具。当然，你也可以用telnet根据其纯文本协议来操作
- redis-benchmark：Redis性能测试工具，测试Redis在你的系统及你的配置下的读写性能
- redis-check-aof：更新日志检查
- redis-check-dump：用于本地数据库检查
安装
**make **PREFIX=**/**usr**/****local ****install**
## 配置
redis的配置文件在你的安装目录里。名为：redis.conf
**mkdir ****/**etc**/**redis
**cp** redis.conf **/**etc**/**redis**/**redis.conf
- redis默认不是用守护进程的，如果需要更改，可把daemonize no改成daemonize yes。
- 如果对redis 默认6379端口不爽的，可以更改port
 6379
- 如果想把数据文件放到一个指定文件夹，更改dir
 /var/lib/redis/（需要自己先创建）（默认是dir ./ 也就是默认放到安装目录下。）
- 客户端闲置多少秒后，断开连接
timeout 300
- 下面的配置二选一
一：SNAPSHOTTING内存快照方式（Save the DB on disk） save * *：保存快照的频率，第一个*表示多长时间，第2个*表示执行多少次写操作。在一定时间内执行一定数量的写操作时，自动保存快照。可设置多个条件。
*#当有一条Keys数据被改变时，900秒刷新到disk一次*
save 900 1*#当有10条Keys数据被改变时，300秒刷新到disk一次*save 300  10*#当有1w条keys数据被改变时，60秒刷新到disk一次*save 60  10000
二：APPEND ONLY MODE （AOF方式）
 appendfsync everysec 每秒同步。这里可以注释掉，打开它下面的选项appendfsync no
appendonlylog如何同步到磁盘（三个选项，分别是每次写都强制调用fsync、每秒启用一次fsync、不调用fsync等待系统自己同步）

是否在每次更新操作后进行日志记录，如果不开启，可能会在断电时导致一段时间内的数据丢失。因为redis本身同步数据文件是按上面save条件来同步的，所以有的数据会在一段时间内只存在于内存中。默认值为no
appendonly no   开启的话每次写操作会记一条log，这会提高数据抗风险能力，但影响效率。
Redis有两种存储方式，默认是snapshot方式，实现方法是定时将内存的快照(snapshot)持久化到硬盘，这种方法缺点是持久化之后如果出现crash则会丢失一段数据。因此在完美主义者的推动下作者增加了aof方式。aof即append only mode，在写入内存数据的同时将操作命令保存到日志文件。
其实aof目的主要是数据可靠性及高可用性，在Redis中有另外一种方法来达到此目的：Replication。由于Redis的高性能，复制基本没有延迟。这样达到了防止单点故障及实现了高可用。
## 运行
redis-server /etc/redis/redis.conf
即可启动redis服务，确认运行了之后，可以用redis-benchmark命令测试看看，还可以通过redis-cli命令实际操作一下，比如：
redis-cli **set** foo barOKredis-cli get foo“bar”
## 关闭
redis-cli shutdown
因为redis是异步写入磁盘的，如果要让内存中的数据马上写入硬盘可以执行如下命令：
redis-cli save
## 艺搜参考
[http://www.cnblogs.com/hb_cattle/archive/2011/10/22/2220907.html](http://www.cnblogs.com/hb_cattle/archive/2011/10/22/2220907.html)
[http://www.cnblogs.com/zhuhongbao/archive/2013/06/04/3117997.html](http://www.cnblogs.com/zhuhongbao/archive/2013/06/04/3117997.html)
