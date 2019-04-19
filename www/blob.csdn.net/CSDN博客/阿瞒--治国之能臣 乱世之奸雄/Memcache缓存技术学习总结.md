# Memcache缓存技术学习总结 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2012年11月15日 23:01:52[baofenny](https://me.csdn.net/jiaminbao)阅读数：4423
Memcached是一个高发性能的分布式对象缓存系统。目前全世界不少人使用这个缓存项目来构建自己
大负载的网站，来分担数据库的压力，通过在内在里维护一个统一的巨大的的hash表，它能够用来
存储各种格式的数据，包括图像、视频、文件以及数据库检索的结果等，简单的说就是将数据调用
到内存中，然后从内在中读取，从而大大提高读取速度。
memcache是一个高效的分布式的内在对象缓存系统，她可以支持把各种php的数据（array对象、基本数据
类型）放入到它管理的内在中.
Memcached最佳实践：将从数据库查询的数据放入Memcached
使用方法：
 一、telnet------>>>memchached缓存系统；
 crud
 二、php程序(memcache扩展、memcached扩展、使用socket访问)----->>>memcached缓存系统；
使用memcache扩展（一堆函数）来操作memcached操作系统
 1.在php.ini中配置extension
 放入到php/ext/目录
 2.打开extension
 这里启用memcache的扩展
 extension=php_memcache.dll
 3.编程测试
 memcache除了资源类型外的其它类型都可以储存；
 $mem = new Memcache();
 //$mem->connect("localhost","11211");//立即向localhost的11211端口发出请求；
 //上面链接不写，可以创建下面多个memcached服务链接；
 $mem->addserver("192.168.1.200","11211");
 $mem->addserver("192.168.1.201","11211");
 $mem->addserver("192.168.1.202","11211");
 //这里使用负载均衡算法来选一个memcached使用；
 $mem->add("name","jmb",1,100);
使用socket套接字符串操作memcached缓存系统;（memcached-client.php）
浅谈memcached机制：
1.是c/s结构的，使用简单文本通讯；
2.基于libevent事件处理;
3.是内存缓存；
4.基于客户端的分布式；
Memcached的细节：
1.生命周期，是在add数据的时候指定的；
2.如何配置，将session数据自动保存到memcached服务器中; 
步骤：
1.在php.ini文件中
把session数据放入memcache
session save_hander = memcache
这里指定放到哪个memcached服务器
session save_path = "tcp//127.0.0.1:11211"
小结：我们只需要在php.ini中作一个配置，然后再使用session时候，还是老规矩，只是这个时候，
我们的session数据，已经放到memcached中
如何使用memcached的因素：
1.如果是一个小网站，pv值不大，就不考虑使用memcached
2.变化频繁，查询频繁，但是不一定写入数据库（适合memcached）（用户在线状态）
3.变化频繁，一变化就要入库（比如股票，金融）不适合memcached
4.变化不频繁，查询频繁，不管入不入库，都比较适合memcache，（新浪的新闻频道）
