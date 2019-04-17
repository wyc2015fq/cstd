# Redis cluster 集群 安装 配置 详解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月30日 11:56:45[boonya](https://me.csdn.net/boonya)阅读数：471标签：[linux																[redis																[yum																[cluster](https://so.csdn.net/so/search/s.do?q=cluster&t=blog)
个人分类：[Memcache/Redis](https://blog.csdn.net/boonya/article/category/5904837)




Redis 集群是一个提供在多个Redis间节点间共享数据的程序集.redis3.0以前，只支持主从同步的，如果主的挂了，写入就成问题了。3.0出来后就可以很好帮我们解决这个问题。
目前redis 3.0还不稳定，如果要用在生产环境中，要慎重。



### **一.redis服务器说明**



```python
192.168.10.219 6379  
    192.168.10.219 6380  
    192.168.10.219 6381  
      
    192.168.10.220 6382  
    192.168.10.220 6383  
    192.168.10.220 6384
```
**要让集群正常运作至少需要三个主节点，不过在刚开始试用集群功能时， 强烈建议使用六个节点： 其中三个为主节点， 而其余三个则是各个主节点的从节点。所有用二台机器，开6个redis进程，模拟6台机器。**


### **二.安装ruby,rubygems**


```python
# yum -y install gcc openssl-devel libyaml-devel libffi-devel readline-devel zlib-devel gdbm-devel ncurses-devel gcc-c++ automake autoconf

# yum -y install ruby rubygems   //安装ruby rubygems

//换源
# gem source -l
# gem source --remove http://rubygems.org/
# gem sources -a http://ruby.taobao.org/
# gem source -l

# gem install redis --version 3.0.0  //安装gem_redis
Successfully installed redis-3.0.0
1 gem installed
Installing ri documentation for redis-3.0.0...
Installing RDoc documentation for redis-3.0.0...
```



以前写过一篇ruby on rails的文章，请参考：[**centos ruby on rails apache mysql 安装配置详解**](http://blog.51yip.com/server/1493.html)

**喜欢手动编译的人，可以参考这篇文章，安装ruby.**

### **三.安装redis3.0.0**



**1，安装redis**




```python
# wget http://download.redis.io/releases/redis-3.0.0.tar.gz
# tar -xvzf redis-3.0.0.tar.gz
# cd redis-3.0.0
# make && make install
# cd src
# cp redis-trib.rb /usr/local/bin  

# mkdir /etc/redis
# mkdir /var/log/redis
```



**上述操作要先在二台机器上面都操作好。**

**2,配置redis**


```python
[root@slave2 redis-3.0.0]#  vim redis.conf   //解压的根目录，有redis.conf,做以下修改
port 6379
pidfile /var/run/redis-6379.pid
dbfilename dump-6379.rdb
appendfilename "appendonly-6379.aof"
cluster-config-file nodes-6379.conf
cluster-enabled yes
cluster-node-timeout 5000
appendonly yes
```


**3，copy配置文件，并修改端口**


```python
# cp redis.conf /etc/redis/redis-6379.conf
# cp redis.conf /etc/redis/redis-6380.conf
# cp redis.conf /etc/redis/redis-6381.conf

# scp redis.conf 192.168.10.220:/etc/redis/redis-6382.conf
# scp redis.conf 192.168.10.220:/etc/redis/redis-6383.conf
# scp redis.conf 192.168.10.220:/etc/redis/redis-6384.conf

# sed -i "s/6379/6380/g" /etc/redis/redis-6380.conf
# sed -i "s/6379/6381/g" /etc/redis/redis-6381.conf

# sed -i "s/6379/6382/g" /etc/redis/redis-6382.conf
# sed -i "s/6379/6383/g" /etc/redis/redis-6383.conf
# sed -i "s/6379/6384/g" /etc/redis/redis-6384.conf
```


**将配置文件分别copy到二台机器上，并替换端口：**


```python
# cat redis-6380.conf |awk '{if($0 !~ /^$/ && $0 !~ /#/) {print $0}}' |grep 6380
pidfile /var/run/redis-6380.pid
port 6380
dbfilename dump-6380.rdb
appendfilename "appendonly-6380.aof"
cluster-config-file nodes-6380.conf
```


**有5处修改端口的地方**

**4，启动并查看redis**


```python
# redis-server /etc/redis/redis-6379.conf > /var/log/redis/redis-6379.log 2>&1 &
# redis-server /etc/redis/redis-6380.conf > /var/log/redis/redis-6380.log 2>&1 &
# redis-server /etc/redis/redis-6381.conf > /var/log/redis/redis-6381.log 2>&1 &

# redis-server /etc/redis/redis-6382.conf > /var/log/redis/redis-6382.log 2>&1 &
# redis-server /etc/redis/redis-6383.conf > /var/log/redis/redis-6383.log 2>&1 &
# redis-server /etc/redis/redis-6384.conf > /var/log/redis/redis-6384.log 2>&1 &

# netstat -tpnl |grep redis
tcp        0      0 0.0.0.0:6379                0.0.0.0:*                   LISTEN      7298/redis-server *
tcp        0      0 0.0.0.0:6380                0.0.0.0:*                   LISTEN      7299/redis-server *
tcp        0      0 0.0.0.0:6381                0.0.0.0:*                   LISTEN      7304/redis-server *
tcp        0      0 0.0.0.0:16379               0.0.0.0:*                   LISTEN      7298/redis-server *
tcp        0      0 0.0.0.0:16380               0.0.0.0:*                   LISTEN      7299/redis-server *
tcp        0      0 0.0.0.0:16381               0.0.0.0:*                   LISTEN      7304/redis-server *
tcp        0      0 :::6379                     :::*                        LISTEN      7298/redis-server *
tcp        0      0 :::6380                     :::*                        LISTEN      7299/redis-server *
tcp        0      0 :::6381                     :::*                        LISTEN      7304/redis-server *
tcp        0      0 :::16379                    :::*                        LISTEN      7298/redis-server *
tcp        0      0 :::16380                    :::*                        LISTEN      7299/redis-server *
tcp        0      0 :::16381                    :::*                        LISTEN      7304/redis-server *

[root@slave2 redis]# ll /etc/redis/
总用量 156
-rw-r--r-- 1 root root     0 4月  30 23:54 appendonly-6379.aof
-rw-r--r-- 1 root root     0 5月   1 00:08 appendonly-6380.aof
-rw-r--r-- 1 root root     0 5月   1 00:08 appendonly-6381.aof
-rw-r--r-- 1 root root    18 5月   1 00:08 dump-6379.rdb
-rw-r--r-- 1 root root    18 5月   1 00:08 dump-6380.rdb
-rw-r--r-- 1 root root    18 5月   1 00:08 dump-6381.rdb
-rw-r--r-- 1 root root   763 5月   1 00:08 nodes-6379.conf
-rw-r--r-- 1 root root   763 5月   1 00:08 nodes-6380.conf
-rw-r--r-- 1 root root   763 5月   1 00:08 nodes-6381.conf
-rw-r--r-- 1 root root 41412 4月  30 23:30 redis-6379.conf
-rw-r--r-- 1 root root 41412 4月  30 23:39 redis-6380.conf
-rw-r--r-- 1 root root 41412 4月  30 23:39 redis-6381.conf
```


所有节点都启动成功，并不代表，他们就是集群了。

### **四.创建集群，并查看**



**1，创建redis集群**



```python
# redis-trib.rb create --replicas 1 192.168.10.219:6379 192.168.10.219:6380 192.168.10.219:6381 192.168.10.220:6382 192.168.10.220:6383 192.168.10.220:6384
```



**2，查看redis集群状态**


```python
[root@slave2 redis]# redis-trib.rb check 192.168.10.219:6379
Connecting to node 192.168.10.219:6379: OK
Connecting to node 192.168.10.220:6384: OK
Connecting to node 192.168.10.219:6381: OK
Connecting to node 192.168.10.220:6383: OK
Connecting to node 192.168.10.220:6382: OK
Connecting to node 192.168.10.219:6380: OK
>>> Performing Cluster Check (using node 192.168.10.219:6379)
M: d40d9a367c24784b0336c7b80fb4c87337e2cba6 192.168.10.219:6379
 slots:5461-10922 (5462 slots) master
 1 additional replica(s)
S: 5f00f163d0c0a540ea99daf004f55588a802327b 192.168.10.220:6384
 slots: (0 slots) slave
 replicates d40d9a367c24784b0336c7b80fb4c87337e2cba6
S: b3b1a848987b5a87a06888e126d5c9b16f871ff5 192.168.10.219:6381
 slots: (0 slots) slave
 replicates d2eb5a8a77f87888792428aed4692dfb907e7a1d
M: d2eb5a8a77f87888792428aed4692dfb907e7a1d 192.168.10.220:6383
 slots:10923-16383 (5461 slots) master
 1 additional replica(s)
M: a8eafe8b19d6a28c034917da13a43ce1230fe870 192.168.10.220:6382
 slots:0-5460 (5461 slots) master
 1 additional replica(s)
S: a87d207204a53ab599bf7f6ffb9d679d0eef4f25 192.168.10.219:6380
 slots: (0 slots) slave
 replicates a8eafe8b19d6a28c034917da13a43ce1230fe870
[OK] All nodes agree about slots configuration.
>>> Check for open slots...
>>> Check slots coverage...
[OK] All 16384 slots covered.
```


到这儿，redis集群就配置成功了

**五，测试redis集群**


```python
# redis-cli -c -p 6379 -h 192.168.10.219   //在219登录
192.168.10.219:6379> set tank tank1    //设置测试值
-> Redirected to slot [4407] located at 192.168.10.220:6382
OK   //直接转向到220 6382端口，数据存到了220 6382，不过有点奇怪
192.168.10.220:6382> get tank   //可以取到值
"tank1"

[root@manage redis]# redis-cli -c -p 6383 -h 192.168.10.220  //220机器，6383端口
192.168.10.220:6383> get tank
-> Redirected to slot [4407] located at 192.168.10.220:6382  //直接转向220 6382端口
"tank1"

[root@manage redis]# ps aux |grep redis //查看进程
root      7310  0.2  0.9 137436  9724 pts/0    Sl   Apr30   0:09 redis-server *:6382 [cluster]
root      7311  0.2  0.9 137436  9776 pts/0    Sl   Apr30   0:09 redis-server *:6383 [cluster]
root      7316  0.2  0.9 137436  9944 pts/0    Sl   Apr30   0:10 redis-server *:6384 [cluster]
root      7478  0.0  0.0 103256   812 pts/0    S+   00:56   0:00 grep redis

[root@manage redis]# kill -9 7310  //关闭220 6382端口，看数据会不会丢失

[root@slave2 redis]# redis-trib.rb check 192.168.10.219:6379  //查看集群情况，任何接口都行
Connecting to node 192.168.10.219:6379: OK
Connecting to node 192.168.10.220:6384: OK
Connecting to node 192.168.10.219:6381: OK
Connecting to node 192.168.10.220:6383: OK
Connecting to node 192.168.10.219:6380: OK
>>> Performing Cluster Check (using node 192.168.10.219:6379)
M: d40d9a367c24784b0336c7b80fb4c87337e2cba6 192.168.10.219:6379
 slots:5461-10922 (5462 slots) master
 1 additional replica(s)
S: 5f00f163d0c0a540ea99daf004f55588a802327b 192.168.10.220:6384
 slots: (0 slots) slave
 replicates d40d9a367c24784b0336c7b80fb4c87337e2cba6
S: b3b1a848987b5a87a06888e126d5c9b16f871ff5 192.168.10.219:6381
 slots: (0 slots) slave
 replicates d2eb5a8a77f87888792428aed4692dfb907e7a1d
M: d2eb5a8a77f87888792428aed4692dfb907e7a1d 192.168.10.220:6383
 slots:10923-16383 (5461 slots) master
 1 additional replica(s)
M: a87d207204a53ab599bf7f6ffb9d679d0eef4f25 192.168.10.219:6380
 slots:0-5460 (5461 slots) master
 0 additional replica(s)
[OK] All nodes agree about slots configuration.
>>> Check for open slots...
>>> Check slots coverage...
[OK] All 16384 slots covered.  //变成，三主二从了

[root@slave2 redis]# redis-cli -c -p 6379 -h 192.168.10.219    //219登录
192.168.10.219:6379> get tank
-> Redirected to slot [4407] located at 192.168.10.219:6380   //数据被转到了6380节点了
"tank1"
```


经测试，集群是可用的。

**作者:海底苍鹰,地址:[http://blog.51yip.com/nosql/1725.html](http://blog.51yip.com/nosql/1725.html)**](https://so.csdn.net/so/search/s.do?q=yum&t=blog)](https://so.csdn.net/so/search/s.do?q=redis&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




