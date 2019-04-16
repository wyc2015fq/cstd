# 服务器数据库系列 - Memcache在linux下安装 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月25日 17:06:05[initphp](https://me.csdn.net/initphp)阅读数：873








**Memcache安装和使用**

## 安装Memcache

tar zxvf memcached-1.4.0.tar.gz

cd memcached-1.4.0

./configure --with-libevent=/usr/local/lib

make

make install

## 安装PHP Memcache 扩展

tar xzvf memcache-2.2.5.tgz

cd memcache-2.2.5

/usr/local/php.5.3.3/bin/phpize

./configure--enable-memcache--with-php-config=/usr/local/web/php.5.3.3/bin/php-config --with-zlib-dir

make

make install

## 修改php.ini配置文件

vi /usr/local/web/php.5.3.3/lib/php.ini

加上：extension=memcache.so

重启php-fpm：

kill -INT `cat /usr/local/web/php.5.3.3/var/run/php-fpm.pid` 关闭

/usr/local/web/php.5.3.3/sbin/php-fpm 启动

## 使用Memcache

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

memcached -d -m 1024 -u root -l 127.0.0.1 -p 11211 -c 128 -P /tmp/memcached.pid



