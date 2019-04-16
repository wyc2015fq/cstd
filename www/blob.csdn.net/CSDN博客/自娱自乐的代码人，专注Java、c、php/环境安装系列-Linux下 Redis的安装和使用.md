# 环境安装系列 - Linux下 Redis的安装和使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年02月03日 12:56:35[initphp](https://me.csdn.net/initphp)阅读数：718
个人分类：[环境安装系列](https://blog.csdn.net/initphp/article/category/2755407)








- 主要准备的安装包

> 
1. PHP扩展包：redis-2.2.5.tgz   [官网](http://pecl.php.net/package/redis)

2. Redis包：redis最新版本redis-2.8.19.tar.gz [官网](http://redis.io/)


[下载包](http://initphp.qiniudn.com/redis.zip)




- 将redis安装到/usr/local/目录下面

> 
将压缩包解压到 /usr/local/目录下面。

执行命令：

```cpp
sudo make
sudo make install
```
redis的配置文件 redis.conf 在当前目录下，修改配置文件：
```
#将no改为yes,让redis后台运行
daemonize no
```
安装完成后，redis的启动脚本会放置在/usr/loca/bin/ 目录下面。

启动redis：
`/usr/local/bin/redis-server /usr/local/redis-2.8.19/redis.conf`通过命令查看是否启动成功：`ps -ef | grep redis`
停止redis命令：



```cpp
/usr/local/bin/redis-cli shutdown
```




- redis.conf的命令详解

> 


```
#是否作为守护进程运行
daemonize yes
#如以后台进程运行，则需指定一个pid，默认为/var/run/redis.pid
pidfile redis.pid
#绑定主机IP，默认值为127.0.0.1
#bind 127.0.0.1
#Redis默认监听端口
port 6379
#客户端闲置多少秒后，断开连接，默认为300（秒）
timeout 300
#日志记录等级，有4个可选值，debug，verbose（默认值），notice，warning
loglevel verbose
#指定日志输出的文件名，默认值为stdout，也可设为/dev/null屏蔽日志
logfile stdout
#可用数据库数，默认值为16，默认数据库为0
databases 16
#保存数据到disk的策略
#当有一条Keys数据被改变是，900秒刷新到disk一次
save 900 1
#当有10条Keys数据被改变时，300秒刷新到disk一次
save 300 10
#当有1w条keys数据被改变时，60秒刷新到disk一次
save 60 10000
#当dump .rdb数据库的时候是否压缩数据对象
rdbcompression yes
#本地数据库文件名，默认值为dump.rdb
dbfilename dump.rdb
#本地数据库存放路径,默认值为 ./
dir ./
```



- redis扩展安装

> 


```
sudo  tar xzvf redis-2.2.5.tgz
cd redis-2.2.5
sudo  /usr/local/php.5.4.35/bin/phpize
sudo ./configure --with-php-config=/usr/local/php.5.4.35/bin/php-config
sudo  make
sudo  make install
```


修改php.ini：





```
sudo  vim /usr/local/php.5.4.35/lib/php.ini
#加上：extension=redis.so
sudo  kill  -USR2 `cat /usr/local/php.5.4.35/var/run/php-fpm.pid` #重启
```


phpinfo:



![](https://img-blog.csdn.net/20150203130500194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









> 






> 






