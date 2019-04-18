# linux下redis安装配置及redis常用命令 - gauss的专栏 - CSDN博客
2015年06月22日 00:21:55[gauss](https://me.csdn.net/mathlmx)阅读数：256
个人分类：[分布式与缓存设计](https://blog.csdn.net/mathlmx/article/category/3168687)
**一、下载redis**
下载地址：[http://code.google.com/p/redis/downloads/list](http://code.google.com/p/redis/downloads/list)
2013年12月7号，我下载的是最新版本：redis-2.6.14.tar.gz
另附上csdn上该版本的资源：
[http://download.csdn.net/detail/wantianwen/6677973](http://download.csdn.net/detail/wantianwen/6677973)
使用root安装
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- su  
将该下载包拷贝到/opt
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- cp redis-2.6.14.tar.gz /opt  
**二、安装**
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- cd /opt  
- tar zxvf redis-2.6.14.tar.gz  
- cd redis-2.6.14  
- make && make install  
安装好后查看下redis的可执行文件已经在此目录下：
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- ls /usr/local/bin/redis*  
- 
- /usr/local/bin/redis-benchmark  
- /usr/local/bin/redis-cli  
- /usr/local/bin/redis-check-aof  
- /usr/local/bin/redis-server  
- /usr/local/bin/redis-check-dump  
**三、配置redis**
redis的配置文件启动时需要用到：
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- cp redis.conf /etc/  
然后编辑redis.conf配置文件（/etc/redis.conf），按需求做出适当调整，比如：
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- vi /etc/redis.conf  
- daemonize yes #默认为on。yes为转为守护进程，否则启动时会每隔5秒输出一行监控信息  
- save 900 1 #900秒内如果有一个key发生变化时，则将数据写入进镜像  
- maxmemory 256000000 #分配256M内存  
将6379端口在防火墙中开放：
rhel系列
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- vi /etc/sysconfig/iptables#加入一行，需要具备其修改权限  
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 6379 -j ACCEPT
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- /etc/init.d/iptables restart  
查看防火墙状态：
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- /etc/init.d/iptables status  
debian系列
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- ufw allow 6379  
查看防火墙状态
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- ufw status  
- 6379                       ALLOW       Anywhere  
创建redis的日志文件夹：
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- mkdir -p /var/log/redis/log  
启动redis并指定redis的日志文件：
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- /usr/local/bin/redis-server /etc/redis.conf 1>/var/log/redis/infolog.log 2>/var/log/redis/errlog.log &  
加入自启动：
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- vi /etc/rc.local#加入  
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- /usr/local/bin/redis-server /etc/redis.conf 1>/var/log/redis/infolog.log 2>/var/log/redis/errlog.log &  
**四、服务器优化（根据实际情况判定是否需要优化）**
如果内存情况比较紧张的话，需要设定内核参数：
echo 1 > /proc/sys/vm/overcommit_memory
这里说一下这个配置的含义：
/proc/sys/vm/overcommit_memory
该文件指定了内核针对内存分配的策略，其值可以是0、1、2。
0， 表示内核将检查是否有足够的可用内存供应用进程使用；如果有足够的可用内存，内存申请允许；否则，内存申请失败，并把错误返回给应用进程。
1， 表示内核允许分配所有的物理内存，而不管当前的内存状态如何。
2， 表示内核允许分配超过所有物理内存和交换空间总和的内存
**五、测试redis**
客户端连接
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- redis-cli  
- redis 127.0.0.1:6379> set redisKey value  
- OK  
- redis 127.0.0.1:6379> get redisKey  
- "value"  
- redis 127.0.0.1:6379> del redisKey  
- (integer) 1  
- redis 127.0.0.1:6379> exists key  
- (integer) 0  
keys *
取出当前匹配的所有key
> exists larry
(integer) 0
当前的key是否存在
del lv
删除当前key
expire
设置过期时间
> expire larry 10
(integer) 1
> move larry ad4
(integer) 1
移动larry键值对到ad4数据库
> persist lv
(integer) 1
移除当前key的过期时间
randomkey
随机返回一个key
rename
重命名key
type
返回值的数据类型
type testlist
list
> ping
PONG
测试连接是否还在
>echo name
"larry"
打印
> select ad4databank
OK
数据库切换
> quit
退出连接
> dbsize
(integer) 12
当前数据库中key的数量
> info
服务器基本信息
monitor
实时转储收到的请求
config get
获取服务器的参数配置
flushdb
清空当前数据库
flushall
清除所有数
**六、遇到问题**
我用java操作redis时，报过这样的错误：
(error) MISCONF Redis is configured to save RDB snapshots, but is currently not able to persist on disk. Commands that may modify the data set are disabled. Please check Redis logs for details about the error.
解决：
**[plain]**[view plain](http://blog.csdn.net/wantianwen/article/details/17199095#)[copy](http://blog.csdn.net/wantianwen/article/details/17199095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/102448/fork)
- redis-cli  
- config set stop-writes-on-bgsave-error no  
