# 通过PHP扩展phpredis操作redis - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月08日 13:27:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：193
我们使用phpredis，这个扩展能让你用PHP操作redis。
源码下载：
phpize
.**/**configure
**make ****&& ****make ****install**
在php.ini 文件中，添加配置：
**[**redis**]**extension=redis.so
查找extension dir方法：在phpinfo信息中搜extension_dir，我的位于/usr/lib64/php/modules
service php-fpm restart
通过phpinfo可以看到为2.2.5版
**<?php**$redis=**new** redis();$result=$redis->connect('127.0.0.1',6379);var_dump($result);echo $redis->get('mykey');
## 艺搜参考
[http://redis.io/clients](http://redis.io/clients)
[https://github.com/nicolasff/phpredis](https://github.com/nicolasff/phpredis)
[redis php 实例一](http://blog.51yip.com/cache/1439.html)
[http://blog.csdn.net/love__coder/article/details/8691679](http://blog.csdn.net/love__coder/article/details/8691679)
