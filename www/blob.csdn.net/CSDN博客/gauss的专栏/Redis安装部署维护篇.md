# Redis安装部署维护篇 - gauss的专栏 - CSDN博客
2014年10月18日 00:22:31[gauss](https://me.csdn.net/mathlmx)阅读数：446
                
 Redis是个高性能的key-value数据库，它的key具有丰富的数据结构：string，hash，list set和sorted set。作为NOSQL，比起memcache之类，不仅仅key数据结构丰富，而且具有持久化的功能，并且能够支持主从复制，很方便构建集群。redis高性能很大程度上源于它是个内存型数据库，它的高性能表现在：set操作11w/s，get操作8.1w/s，与其他类型数据库性能差异，可以而参考：[http://timyang.net/data/mcdb-tt-redis/](http://timyang.net/data/mcdb-tt-redis/) 
  。为了进一步加深对redis的理解总结，我打算写个redis系列的博客。这里主要谈谈redis安装部署及运维维护。
1、下载安装
```
[root@xsf003 tool]# wget -c http://redis.googlecode.com/files/redis-2.4.17.tar.gz    #下载
[root@xsf003 tool]# tar -zxvf redis-2.4.17.tar.gz   #解压
[root@xsf003 tool]# cd redis-2.4.17
[root@xsf003 redis-2.4.17]# make  #编译
[root@xsf003 redis-2.4.17]# make install #安装
```
安装完毕，常用工具会自动拷贝到/user/loca/bin目录下。做为服务器，我们常常还需要把redis设置成开机自启动，源码包中有个很好用的脚本，执行脚步根据提示输入即可。
```
[root@xsf003 redis-2.4.17]# cd utils/
[root@xsf003 utils]# ./install_server.sh 
Welcome to the redis service installer
This script will help you easily set up a running redis server
Please select the redis port for this instance: [6379] 
Selecting default: 6379
Please select the redis config file name [/etc/redis/6379.conf] 
Selected default - /etc/redis/6379.conf
Please select the redis log file name [/var/log/redis_6379.log] 
Selected default - /var/log/redis_6379.log
Please select the data directory for this instance [/var/lib/redis/6379] 
Selected default - /var/lib/redis/6379
Please select the redis executable path [/usr/local/bin/redis-server] 
Copied /tmp/6379.conf => /etc/init.d/redis_6379
Installing service...
Successfully added to chkconfig!
Successfully added to runlevels 345!
Starting Redis server...
Installation successful!
```
     注意执行install_server.sh，需要先进入utils目录，不然脚本会报错，提示找不到相应文件。安装完服务，redis自动启动，可以用ps命令查看到相关信息：
```
[root@xsf003 utils]# ps -ef | grep redis
root      4554     1  0 10:55 ?        00:00:02 /usr/local/bin/redis-server /etc/redis/6379.conf
root      4564  2808  0 10:59 pts/0    00:00:00 grep redis
```
2、手动启动关闭服务
```
[root@xsf003 utils]# /etc/init.d/redis_6379 stop   #关闭
[root@xsf003 utils]# /etc/init.d/redis_6379 start  #启动
```
        3、通过客户端命令行工具连接redis服务查看redis相关信息
a)连接
```
[root@xsf003 utils]# redis-cli 
redis 127.0.0.1:6379>
```
b）其他指令
```
redis 127.0.0.1:6379> info  #查看server版本内存使用连接等信息
redis 127.0.0.1:6379> client list  #获取客户连接列表
redis 127.0.0.1:6379> client kill 127.0.0.1:33441 #终止某个客户端连接
redis 127.0.0.1:6379> dbsize #当前保存key的数量
redis 127.0.0.1:6379> save #立即保存数据到硬盘
redis 127.0.0.1:6379> bgsave #异步保存数据到硬盘
redis 127.0.0.1:6379> flushdb #当前库中移除所有key
redis 127.0.0.1:6379> flushall #移除所有key从所有库中
redis 127.0.0.1:6379> lastsave #获取上次成功保存到硬盘的unix时间戳
redis 127.0.0.1:6379> monitor #实时监测服务器接收到的请求
redis 127.0.0.1:6379> slowlog len #查询慢查询日志条数
(integer) 3 
redis 127.0.0.1:6379> slowlog get #返回所有的慢查询日志，最大值取决于slowlog-max-len配置
redis 127.0.0.1:6379> slowlog get 2 #打印两条慢查询日志
redis 127.0.0.1:6379> slowlog reset #清空慢查询日志信息
```
通过以上操作，单台服务器基本跑起来了，不过后面的路还很长很长。。。。
参考文章：
http://redis.io/topics/introduction
http://timyang.net/data/mcdb-tt-redis/
http://redis.io/commands#server
http://code.google.com/p/redis/
