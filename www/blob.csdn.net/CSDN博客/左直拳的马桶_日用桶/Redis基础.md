# Redis基础 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月08日 19:59:23[左直拳](https://me.csdn.net/leftfist)阅读数：2211
**一、Redis简介**
Redis，一种键值对类型的NoSQL数据库。使用起来，貌似非常简单，操作的对象只是key-value，完全没有库、表、字段的概念。当然，key可以分层次，层次之间用：分隔，如gramfather:father:key，这有点类似c#里面的命名空间.父类.子类，但这跟库表不是同一种概念。
对名为name的key进行赋值
127.0.0.1:6379> set name "leftfist" 
OK 
读取名为name的key的值
127.0.0.1:6379> get name 
"leftfist" 
就是这么简单。
value的类型，支持字符、列表(list)、集合、有序集合、哈希表。
优点：快、支持的数据类型丰富、操作简单
缺点：单线程架构，容易有CPU瓶颈，数据都在内存中，以T为单位不现实
在实际应用中，Redis常用作缓存，可以替代memcached，但性能比后者更好，且可以持久化。
二、特性一览
所有数据都在内存中。
五种数据结构：String / Hash / List / Set / Ordered Set。
数据过期时间支持。
不完全的事务支持。
服务端脚本：使用Lua Script编写，类似存储过程的作用。
PubSub：捞过界的消息一对多发布订阅功能，起码Redis-Sentinel使用了它。
持久化：支持定期导出内存的Snapshot 与 记录写操作日志的Append Only File两种模式。
Replication：Master-Slave模式，Master可连接多个只读Slave，暂无专门的Geographic Replication支持。 
三、安装
Redis并不直接支持Windows，但有一个名为微软开放技术团队的组织在开发和维护一个针对６４位windows的变种。听起来感觉怎么这么不靠谱，二手的Redis。所以，要使用Redis，还是抛弃windows，老老实实使用Linux。
Linux  uBuntu下安装Redis:
**１、在终端，输入**
sudo apt-get update
sudo apt-get install redis-server
输入超级管理员口令后，一般即可安装成功。
注意，运行上述命令，请保证网络是通畅的。如果网络环境需要设置代理，尤要注意。
有关代理上网设置，可见拙作：
[http://blog.csdn.net/leftfist/article/details/39677133](http://blog.csdn.net/leftfist/article/details/39677133)
apt-get是一条linux命令，适用于deb包管理式的操作系统，主要用于自动从互联网的软件仓库中搜索、安装、升级、卸载软件或操作系统。相当于node.js的npm，vs.net里的nuget。
**２、检测redis是否运行**
安装后，Redis即自动运行。
在终端，输入
ps -aux|grep redis
其中，ps命令用于列出系统当前运行的进程；grep用于过滤
正常情况下，应该显示类似信息：
redis      942  0.0  0.4  27908  7264 ?        Ssl  14:48   0:10 /usr/bin/redis-server *:6379               
leftfist 19055  0.0  0.0   4832   920 pts/8    S+   18:01   0:00 grep --color=auto redis
**３、打开redis客户端**
在终端，输入
redis-cli
**４、显示redis的详细信息，如版本等**
在redis客户端，输入
info
如果仅仅想看Redis的版本号，可以在终端下直接输入
redis-cli –version 或 redis-cli –v
redis-server –version 或 redis-server -v
**5、罗列key**
在redis客户端，输入
keys *
四、配置Redis
修改/etc/redis/redis.conf。
可以在终端下，输入
sudo vi /etc/redis/redis.conf
如何修改，这是另外一个课题。
