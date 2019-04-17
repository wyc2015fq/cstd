# CentOS7 php 安装 amqp扩展 - zp704393004的专栏 - CSDN博客





2018年04月19日 19:01:22[原来未知](https://me.csdn.net/zp704393004)阅读数：264








继续安装完 rabbitmq后，安装最新 php amqp扩展

[http://www.cnblogs.com/8000cabbage/p/7788575.html](http://www.cnblogs.com/8000cabbage/p/7788575.html)

![](https://images2017.cnblogs.com/blog/811271/201711/811271-20171106110458059-2086419675.png)

参考：[carson](http://blog.csdn.net/a454213722/article/details/51870858)

1.安装rabbitmq-c

[https://github.com/alanxz/rabbitmq-c/releases](https://github.com/alanxz/rabbitmq-c/releases)

![](https://images2017.cnblogs.com/blog/811271/201711/811271-20171106091218747-332037850.png)

安装最新版 //下载的过程时间可能有些长

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
wget -c https://github.com/alanxz/rabbitmq-c/releases/download/v0.8.0/rabbitmq-c-0.8.0.tar.gz

tar zxf rabbitmq-c-0.8.0.tar.gz

cd rabbitmq-c-0.8.0

./configure --prefix=/usr/local/rabbitmq-c-0.8.0

make && make install 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

2安装amqp

官网：[http://pecl.php.net/package/amqp](http://pecl.php.net/package/amqp)

![](https://images2017.cnblogs.com/blog/811271/201711/811271-20171106091751794-916660218.png)

下载最新版



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
wget -c http://pecl.php.net/get/amqp-1.9.3.tgz

tar zxf amqp-1.9.3.tgz

cd amqp-1.9.3

/usr/local/php/bin/phpize

./configure --with-php-config=/usr/local/php/bin/php-config --with-amqp --with-librabbitmq-dir=/usr/local/rabbitmq-c-0.8.0

make && make install
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

![](https://images2017.cnblogs.com/blog/811271/201711/811271-20171106092343153-1511774540.png)

3.php.ini 添加模块

vi /usr/local/php/etc/php.ini

增加

extension = /usr/local/php/lib/php/extensions/no-debug-non-zts-20151012/amqp.so 

systemctl restart php-fpm



查看phpinfo()

![](https://images2017.cnblogs.com/blog/811271/201711/811271-20171106092427763-1213932687.png)
[点击打开链接](https://www.cnblogs.com/8000cabbage/p/7791670.html)


