# centos部署Redis和Java开发(Jedis) - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月19日 10:01:06[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2265








centos部署Redis和Java开发(Jedis)

1、部署Redis

1）下载：http://redis.io/download   

2）解压编译：

$ tar xzf redis-3.2.3.tar.gz

$ cd redis-3.2.3

$ make

3）Redis构成：

Redis 由四个可执行文件：redis-benchmark、redis-cli、redis-server、redis-stat 这四个文件，加上一个redis.conf就构成了整个redis的最终可用包。它们的作用如下：

src/redis-server：Redis服务器的daemon启动程序

src/redis-cli：Redis命令行操作工具。当然，你也可以用telnet根据其纯文本协议来操作

src/redis-benchmark：Redis性能测试工具，测试Redis在你的系统及你的配置下的读写性能

src/redis-stat：Redis状态检测工具，可以检测Redis当前状态参数及延迟状况

redis.conf文件：daemonize参数设置为yes，使redis后台运行。

4）服务端启动：

$ src/redis-server redis.conf &

5）客户端连接：

$ src/redis-cli

redis> set foo bar

OK

redis> get foo

"bar"




2、Jedis开发

1）Jedis下载：http://download.csdn.net/detail/fjssharpsword/9607256

2）引入Jedis-2.6.0.jar包

      主要代码：

               Jedis jd = new Jedis("127.0.0.1",6379);
jd.set("foo", "bar");

jd.close();




3、Redis主要研究点：重点要研究redis的分布锁，使进程间可以共享互斥访问资源。



