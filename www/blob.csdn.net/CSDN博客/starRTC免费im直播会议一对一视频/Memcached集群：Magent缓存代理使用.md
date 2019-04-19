# Memcached集群：Magent缓存代理使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:21:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：423
## 安装依赖库
yum install libevent-devel libevent
magent安装参见[Memcached代理软件magent安装小结](http://192.168.1.100/elesos_com/index.php?title=Memcached%E4%BB%A3%E7%90%86%E8%BD%AF%E4%BB%B6magent%E5%AE%89%E8%A3%85%E5%B0%8F%E7%BB%93)
magent -u root -n 51200 -l 127.0.0.1 -p 12000 -s 127.0.0.1:11211 -s 127.0.0.1:11212 -b 127.0.0.1:11213
- 1、分别在11211、11212、11213端口启动3个Memcached进程，在12000端口开启magent代理程序；
- 2、11211、11212端口为主Memcached，11213端口为备份Memcached；
防止单点现象，对缓存代理也可以做备份，客户端连接到缓存代理服务器，缓存代理服务器连接多个缓存服务器，如:
- Magent代理服务器：2台，分别为 192.168.1.2:12000、192.168.1.3:12000
- Memcached主服务器：3台，分别为 192.168.1.4:11211、192.168.1.5:11211、192.168.1.6:11211
- Memcached备服务器：2台，分别为 192.168.1.7:11211、192.168.1.8:11211
在 192.168.1.4、192.168.1.5、192.168.1.6、192.168.1.7、192.168.1.8 上分别编译安装并运行Memcached
在 192.168.1.2、192.168.1.3 上分别 编译安装 magent
## magent命令
-h this message
  -u uid
  -g gid
  -p port, default is 11211. **(**0 to disable tcp support**)**-s ip:port, **set** memcached server **ip** and port
  -b ip:port, **set** backup memcached server **ip** and port
  -l**ip**, **local****bind****ip** address, default is 0.0.0.0
  -n number, **set** max connections, default is 4096最大连接数
  -D**do** not go to background
  -k use ketama key allocation algorithm
  -f**file**, unix socket path to listen on. default is off
  -i number, max keep alive connections **for** one memcached server, default is 20-v verbose
在 192.168.1.2、192.168.1.3 上分别运行 magent(注意修改-l后的IP)：
magent -u root -n 51200 -l 192.168.1.2 -p 12000 -s 192.168.1.4:11211 -s 192.168.1.5:11211 -s 
192.168.1.6:11211 -b 192.168.1.7:11211 -b 192.168.1.8:11211
以前，我们用PHP连接多台Memcached服务器，做分布式缓存时，参考代码如下：
$memcache=**new** Memcache;$memcache->addServer('localhost',11211);$memcache->addServer('localhost',11212);$memcache->addServer('localhost',11213);for($i=0;$i<1000;$i++){$memcache->set($i,$i,0,1000);}
现在，连接的主机不是Memcached服务器了，而是 Magent代理服务器，给 addServer()方法传参时，传入的是Magent主机IP
与端口！测试代码如下：
$mem=**new** \Memcache();$host='192.168.1.2';$port='12000 ';$mem->connect($host,$port);$key1='elesos1';$value1='1';$mem->add($key1,$value1);$key2='elesos2';$value2='2';$mem->add($key2,$value2);
说明：
- 1、PHP连接magent，把缓存key1交给magent，magent根据自身的配置参数，再加上一定的哈希算法，会计算出key1存在3台
主Memcached服务器的某一台上，然后以同样的算法，将key1也在2台备用的Memcached服务器中的某一台上，再存一份数据。
即，主服务器是分布式存储的，同时，从服务器也是分布式存储的；
- 2、在PHP获取缓存数据key1时，magent一旦得知数据所存的那台主Memcached服务器挂掉了，它就会转向从备用的Memcached
服务器中获取数据。注意：服务器的定位选择算法跟存的时候是一样的。
- 3、有个缺陷，当 down 掉的那台主Memcached服务器重新恢复正常后，Memcahed里是没有数据的，即数据全部丢失，但此时
备用的Memcached服务器又不会将数据同步到主服务器。
- 4、可通过Memcached管理软件MemAdmin查看数据分布情况。
magent与memcached 是可以混搭的，不必死板的一个magent s-memcached s-memcached b-memcached
magent也可以连magent的。
Memcached重启复活后无数据，因此magent取得的将是空值，尽管备份Memcached还有数据。可采用定时维护服务器，恢复memcached。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Memcached%E9%9B%86%E7%BE%A4%E7%B3%BB%E5%88%971%EF%BC%9AMagent%E7%BC%93%E5%AD%98%E4%BB%A3%E7%90%86%E4%BD%BF%E7%94%A8&action=edit&section=3)]艺搜参考
[http://blog.snsgou.com/post-800.html](http://blog.snsgou.com/post-800.html)
[http://zyan.cc/post/393/](http://zyan.cc/post/393/)
[http://www.cnblogs.com/happyday56/p/3461113.html](http://www.cnblogs.com/happyday56/p/3461113.html)
