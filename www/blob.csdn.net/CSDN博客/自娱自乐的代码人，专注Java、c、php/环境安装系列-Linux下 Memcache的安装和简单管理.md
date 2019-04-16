# 环境安装系列 - Linux下 Memcache的安装和简单管理 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月19日 10:44:29[initphp](https://me.csdn.net/initphp)阅读数：1204







- 需要准备的安装包：

> 
1.memcache包：memcached-1.4.0.tar.gz

2.php扩展包：memcache-2.2.5.tgz


[下载包](http://initphp.qiniudn.com/memcache%E5%AE%89%E8%A3%85%E5%8C%85.zip)


- 
将memcache安装到/usr/local目录下面：


```
#版本可能不同，选择自己相应的版本
sudo  tar zxvf memcached-1.4.21.tar.gz 
cd memcached-1.4.21
sudo  ./configure --prefix=/usr/local/memcache  --with-libevent 
sudo  make
sudo  make install
```
- 管理memcache：

```
#启动memcache
cd  /usr/local/memcache/bin/
./memcached -d -m 1024 -u root -l 127.0.0.1 -p 11211 -c 4096 -P /usr/local/memcache/memcached.pid

#查看memcache
ps  -ef  |  grep  memcached
 
#停止memcache进程
sudo  kill   21039
```
- memcahce启动项命令含义



```
-d 选项是启动一个守护进程，
-m 是分配给Memcache使用的内存数量，单位是MB，这里是10MB
-u 是运行Memcache的用户，这里是root
-l 是监听的服务器IP地址，如果有多个地址的话，这里指定了服务器的IP地址127.0.0.1
-p 是设置Memcache监听的端口，这里设置了11211，最好是1024以上的端口
-c 选项是最大运行的并发连接数，默认是1024，这里设置了4096，按照你服务器的负载量来设定
-P 是设置保存Memcache的pid文件
```


- 扩展包安装

```
#根据你自己php的安装目录
sudo  tar xzvf memcache-2.2.7.tgz
cd memcache-2.2.7 
sudo  /usr/local/php.5.4.35/bin/phpize 
sudo ./configure --enable-memcache --with-php-config=/usr/local/php.5.4.35/bin/php-config  --with-zlib-dir 
sudo  make
sudo  make install
```
- 修改php.ini文件

```
sudo  vim /usr/local/php.5.4.35/lib/php.ini 
#加上：extension=memcache.so
sudo  kill  -USR2 `cat /usr/local/php.5.4.35/var/run/php-fpm.pid` #重启
```





