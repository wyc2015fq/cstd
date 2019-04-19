# 源码安装Memcached服务器及其2种PHP客户端 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月26日 15:44:33[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：510
本文所用源码包皆为当时最新stable稳定版。
安装memcached服务器
先安装[libevent](http://libevent.org/), 最新版为2.0.21
tar -zxvf libevent-2.0.21-stable.tar.gz 
.**/**configure
*# make**# make install*
再安装[Memcached](http://memcached.org/)， 最新版为1.4.17
.**/**configure 
**make****make  ****install**
查看版本
.**/**memcached -h
memcached 1.4.17
启动Memcached服务器端的命令为(其中192.168.220.137为我的IP)：
# **/**usr**/**local**/**bin**/**memcached -d -m 10 -u root -l 192.168.220.137 -p 11211-c 256 -P **/**tmp**/**memcached.pid
memcached -d -m 512 -u root -p 11211 -c 1024 -P /tmp/memcached.pid
默认启动：
memcached -d -p 11211 -u memcached -m 64 -c 1024 -P /var/run/memcached/memcached.pid
其中：
- -d选项是启动一个守护进程，
- -m是分配给Memcache使用的内存数量，单位是MB，我这里是10MB，
- -u是运行Memcache的用户，我这里是root，
- -l是监听的服务器IP地址，如果有多个地址的话，我这里指定了服务器的IP地址192.168.0.200，
- -p是设置Memcache监听的端口，我这里设置了12000，最好是1024以上的端口，
- -c选项是最大运行的并发连接数，默认是1024，我这里设置了256，按照你服务器的负载量来设定，
- -P是设置保存Memcache的pid文件，我这里是保存在 /tmp/memcached.pid，
启动时如果出现如下异常：
error while loading shared libraries: libevent-2.0.so.5: cannot open shared object file: No such file or directory
可vim /etc/ld.so.conf加上/usr/local/lib后ldconfig
## 安装客户端memcache
目前[最新稳定版是2.2.7](http://pecl.php.net/package/memcache)
**cd** memcache-2.2.7
**whereis** phpize找到其路径然后执行：
*# /usr/bin/phpize*
.**/**configure --with-php-config=**/**usr**/**bin**/**php-config
**make***# make install*
在php.ini(/etc目录下)文件中添加一行
extension=memcache.so
## 安装客户端memcached
memcached的1.2.4及以上增加了CAS(Check and Set)协议,用于同一key的多进程并发处理问题。这种情况其实跟数据库很像，如果同时有几个进程对同一个表的同一数据进行更新的话，那会不会打架呢。数据库里面可以锁定整张表，也可以锁定表里面的一行，其实memcached加入的CAS与此差不多。
php的扩展memcache，不支持cas，所以我们要装memcached扩展，memcached扩展是基于libmemcached，所以要先安装libmemcached。
有一点不要搞混了,服务端的memcached包和php的扩展memcached包是不一样的,注意下,目前服务端的memcached官网最新版是1.4.17,而php的扩展memcached包最新版是2.1.0.
Memcached 与Memcache的区别参见[Memcache与Memcached的区别](http://172.30.24.252/elesos_com/index.php?title=Memcache%E4%B8%8EMemcached%E7%9A%84%E5%8C%BA%E5%88%AB)
### libmemcached的安装
当前最新版是1.0.18
**cd** libmemcached-1.0.18
.**/**configure --prefix=**/**usr**/**local**/**libmemcached --with-memcached**make ****&& ****make****install**
先检查phpize和php-config工具有没有安装，如果没有直接yum install php-devel就行了... 找到安装位置
*#which phpize**#which php-config*
我的都是在/usr/bin/... 目录下 phpize是用来编译安装扩展库用的...
### 安装memcached客户端
[最新版是2.1.0](http://pecl.php.net/package/memcached)
**cd** memcached-2.1.0 
*#/usr/bin/phpize*
注意libmemcached路径的指定跟前面的安装目录一致.  
*#./configure --prefix=/usr/local/phpmemcached --enable-memcached --with-php-config=/usr/bin/php-config *--with-libmemcached-dir=**/**usr**/**local**/**libmemcached
*#make  && make install*
为了以防混淆，这里指定php的memcached扩展的安装路径为/usr/local/phpmemcached
修改php的配置文件php.ini，加上模块extension=memcached.so
看看extension_dir指定的模块路径下是否有memcached.so文件.
extension_dir一般是/usr/lib/php/modules/..... 如果没有看下是否在lib64下，ln -s做下软链接.如：
**ln **-s **/**usr**/**lib64**/**php**/**modules  **/**usr**/**lib**/**php**/**modules
但最好的做法是在 /etc/php.d/目录下添加一个文件：memcached.ini
**vi ****/**etc**/**php.d**/**memcached.ini
写入：extension=memcached.so
加这个文件要注意文件权限。否则，phpinfo中additional .ini files parsed 中没有这个文件时，证明权限不对，PHP不能打开这个文件。
重启web服务.
.**/**nginx -s reload
service php-fpm restart
在phpinfo中可以看到已经有上面2个客户端扩展了。
## 艺搜参考
[http://blog.csdn.net/lg_lin/article/details/9632273](http://blog.csdn.net/lg_lin/article/details/9632273)
[http://iceeggplant.blog.51cto.com/1446843/819576](http://iceeggplant.blog.51cto.com/1446843/819576)
[http://bardo.iteye.com/blog/914110](http://bardo.iteye.com/blog/914110)
