# 黑猴子的家：Redis RDB 转 AOF 错误操作案例 - weixin_33985507的博客 - CSDN博客
2018年10月17日 12:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**首先备份，再操作**
```
[root@hadoop102 myredis]# cp appendonly.aof appendonly.aof20181010bak
[root@hadoop102 myredis]# cp dump.rdb dump.rdb20181010bak
```
（1）关闭AOF
```
[root@hadoop102 myredis]# redis-cli shutdown
[root@hadoop102 myredis]# vim redis.conf
appendonly no
```
（2）启动redis，写入数据
```
[root@hadoop102 myredis]# redis-server redis.conf
[root@hadoop102 myredis]# redis-cli
127.0.0.1:6379> set k1 v1
ok
127.0.0.1:6379> set k2 v2
ok
```
（3）启动AOF，此时RDB是有数据的
```
127.0.0.1:6379> shutdown
//启动AOF
[root@hadoop102 myredis]# vim redis.conf
appendonly yes
```
（4）启动redis，发现没有数据
dump.rdb文件，此时还是有数据的，redis启动了AOF后，无脑加载AOF
AOF没有数据，所以Redis显示没有数据
```
[root@hadoop102 myredis]# redis-server redis.conf
[root@hadoop102 myredis]# redis-cli
127.0.0.1:6379> key *
```
（5）发现redis没有数据，这个异象，多数人会选择关闭redis
```
//就是这步导致RDB，dump.rdb文件彻底没数据的
//shutdown 默认会进行 镜像fork 操作，此时redis没有数据，进行fork，会把dump.rdb文件清空
127.0.0.1:6379> shutdown
```
（6）此时的正确做法是
```
// shutdown 带 nosave  参数，不fork镜像，不写入RDB
127.0.0.1:6379> shutdown  nosave
```
