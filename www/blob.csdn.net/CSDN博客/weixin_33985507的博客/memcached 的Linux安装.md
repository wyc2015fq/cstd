# memcached 的Linux安装 - weixin_33985507的博客 - CSDN博客
2009年12月21日 14:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
一、memcached 的编译安装
准备:
Memcached下载地址：[http://www.danga.com/memcached/](http://www.danga.com/memcached/)
libevent下载地址: [http://monkey.org/~provos/libevent/](http://monkey.org/~provos/libevent/)
> 
1.分别把memcached和libevent下载回来，放到 /tmp 目录下：
# cd /tmp
# wget http://www.danga.com/memcached/dist/memcached-1.2.0.tar.gz
# wget http://www.monkey.org/~provos/libevent-1.2.tar.gz
2.先安装libevent：
# tar zxvf libevent-1.2.tar.gz
# cd libevent-1.2
# ./configure –prefix=/usr
# make
# make install
3.测试libevent是否安装成功：
# ls -al /usr/lib | grep libevent
lrwxrwxrwx 1 root root 21 11?? 12 17:38 libevent-1.2.so.1 -> libevent-1.2.so.1.0.3
-rwxr-xr-x 1 root root 263546 11?? 12 17:38 libevent-1.2.so.1.0.3
-rw-r–r– 1 root root 454156 11?? 12 17:38 libevent.a
-rwxr-xr-x 1 root root 811 11?? 12 17:38 libevent.la
lrwxrwxrwx 1 root root 21 11?? 12 17:38 libevent.so -> libevent-1.2.so.1.0.3
还不错，都安装上了。
4.安装memcached，同时需要安装中指定libevent的安装位置：
# cd /tmp
# tar zxvf memcached-1.2.0.tar.gz
# cd memcached-1.2.0
# ./configure –with-libevent=/usr
# make
# make install
如果中间出现报错，请仔细检查错误信息，按照错误信息来配置或者增加相应的库或者路径。
安装完成后会把memcached放到 /usr/local/bin/memcached ，
5.测试是否成功安装memcached：
# ls -al /usr/local/bin/mem*
-rwxr-xr-x 1 root root 137986 11?? 12 17:39 /usr/local/bin/memcached
-rwxr-xr-x 1 root root 140179 11?? 12 17:39 /usr/local/bin/memcached-debug 
安装步骤:
1. 下载libevent & memcached 源码包
     libevent-1.3.tar.gz
     memcached-1.2.2.tar.gz
2. 解压缩 libevent-1.3.tar.gz ，编译安装
     # tar zxvf   libevent-1.3.tar.gz
     # cd libevent-1.3  
     # ./configure --prefix=/usr/local/libevent
     # make ; make install
3. 解压缩 memcached-1.2.2.tar.gz, 编译安装
     # tar zxvf memcached-1.2.2.tar.gz  
     # cd memcached-1.2.2  
     # ./configure --prefix=/usr/local/memchached --with-libevent=/usr/local/libevent  
     # make;make install
4. 作软连接，否则运行memcached的时候将找不到libevent模块
     # ln -s /usr/local/libevent/lib/libevent-1.3.so.1 /usr/lib
5.启动memcached
     # /usr/local/memcached/bin/memcached -d -m 256 -p 11211 -u root
OK ！！！ 搞定！ ^^
二、php的memcached模块
# tar vxzf memcache-1.5.tgz
# cd memcache-1.5
# /usr/local/php/bin/phpize
# ./configure --enable-memcache --with-php-config=/usr/local/php/bin/php-config --with-zlib-dir
# make
# make install
安装完后会有类似这样的提示：
Installing shared extensions: /usr/local/php/lib/php/extensions/no-debug-non-zts-20050922/
把这个记住，然后修改php.ini，把
extension_dir = "./"
修改为
extension_dir = "/usr/local/php/lib/php/extensions/no-debug-non-zts-20050922/"
并添加一行
extension=memcache.so
3、测试脚本
自己写一个PHP程序测试一下吧
<?php
     $memcache = new Memcache; //创建一个memcache对象
     $memcache->connect('localhost', 11211) or die ("Could not connect"); //连接Memcached服务器
     $memcache->set('key', 'test'); //设置一个变量到内存中，名称是key 值是test
     $get_value = $memcache->get('key'); //从内存中取出key的值
     echo $get_value;
?>
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/muyannian/archive/2007/11/05/1867725.aspx](http://blog.csdn.net/muyannian/archive/2007/11/05/1867725.aspx)
