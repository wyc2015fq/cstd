# Memcache与Memcached的区别 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月26日 15:45:00[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：368
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)

Memcache是一个高性能的分布式的内存对象缓存系统，通过在内存里维护一个统一的巨大的hash表，它能够用来存储各种格式的数据，包括图像、视频、文件以及数据库检索的结果等。
Memcached是以守护程序方式运行于一个或多个服务器中，随时会接收客户端的连接和操作。
PHP有两个memcache客户端：php memcache和php memcached。
memcache是完全在PHP框架内开发的，memecached是使用libmemcached的。目前大多数php环境里使用的都是不带d的memcache版本，这个版本出的比较早，是一个原生版本，完全在php框架内开发的。与之对应的带d的memcached是建立在libmemcached的基础上，所以相对来说，memcached版本的功能更全一些，字母d可以理解为daemon。
- Memcache
[http://pecl.php.net/package/memcache](http://pecl.php.net/package/memcache)
[https://php.net/manual/zh/book.memcache.php](https://php.net/manual/zh/book.memcache.php)
- [Memcached](http://zh.wikipedia.org/zh-cn/Memcached)
[http://pecl.php.net/package/memcached](http://pecl.php.net/package/memcached)
[https://php.net/manual/zh/book.memcached.php](https://php.net/manual/zh/book.memcached.php)
Stackoverflow上有比较详细的讨论: [using memcache vs memcached with php](http://stackoverflow.com/questions/1442411/using-memcache-vs-memcached-with-php)和[memcache
 vs memcached](http://stackoverflow.com/questions/1825256/memcache-vs-memcached)
[memcached的方法比memcache多很多](http://www.jb51.net/article/27367.htm)；
下面提供三个地址 大家可以一目了然区分memcache memcached
- [http://memcached.org/](http://memcached.org/) memcached服务端（安装后即可充当memcached服务器 ，对外提供缓存服务）
- [http://pecl.php.net/package/memcache](http://pecl.php.net/package/memcache) memcache客户端之一
- [http://pecl.php.net/package/memcached](http://pecl.php.net/package/memcached) memcache客户端之一
由于服务器端的名字跟客户端的名字相同 所以给很多人带来了疑惑。如此提供地址 一目了然.
memcached没有memcache的connect方法，目前也还不支持长连接。
memcached 支持 Binary Protocol，而 memcache 不支持，意味着 memcached 会有更高的性能。
这里还有一个很清晰表格,[对比](http://code.google.com/p/memcached/wiki/PHPClientComparison)了这两个扩展的具体特性：
安装后查看进程里面是否有memcached服务：ps –aux | grep memcached
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Memcache%E4%B8%8EMemcached%E7%9A%84%E5%8C%BA%E5%88%AB&action=edit&section=1)]艺搜参考
[http://www.jb51.net/article/27366.htm](http://www.jb51.net/article/27366.htm)
[http://blog.linuxeye.com/345.html](http://blog.linuxeye.com/345.html)
[http://www.cnblogs.com/scotoma/archive/2011/02/15/1955573.html](http://www.cnblogs.com/scotoma/archive/2011/02/15/1955573.html)
[http://www.leonzhang.com/2011/06/24/memcached-vs-php-memcache/](http://www.leonzhang.com/2011/06/24/memcached-vs-php-memcache/)
