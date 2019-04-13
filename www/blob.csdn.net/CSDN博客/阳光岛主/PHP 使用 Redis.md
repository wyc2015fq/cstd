
# PHP 使用 Redis - 阳光岛主 - CSDN博客

2015年07月25日 11:19:16[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：3987个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)



**Redis 驱动安装**
开始在 PHP 中使用 Redis 前， 需要确保已经安装了 redis 服务及 PHP redis 驱动，且你的机器上能正常使用 PHP。
本文示例代码，请见**[米扑博客](http://blog.mimvp.com)**：[PHP 使用 Redis](http://blog.mimvp.com/2015/07/php-use-redis/)

**PHP redis 驱动下载地址**
[https://github.com/nicolasff/phpredis](https://github.com/nicolasff/phpredis)
**下载源码或zip压缩包**
[https://github.com/phpredis/phpredis/archive/develop.zip](https://github.com/phpredis/phpredis/archive/develop.zip)
**PHP安装redis扩展**
which phpize
/usr/local/bin/phpize
./configure  –with-php-config=/usr/local/bin/php-config
make && make install
**错误处理：**
homer@ubuntu:~$**/usr/local/bin/phpize**
Configuring for:
PHP Api Version:         20121113
Zend Module Api No:      20121212
Zend Extension Api No:   220121212
Cannot find autoconf. Please check your autoconf installation and the
$PHP_AUTOCONF environment variable. Then, rerun this script.
**解决方法：**
**下载 m4-1.4.9.tar.gz**
[http://ftp.gnu.org/gnu/m4/m4-1.4.9.tar.gz](http://ftp.gnu.org/gnu/m4/m4-1.4.9.tar.gz)
**下载 autoconf-2.62.tar.gz**
[http://ftp.gnu.org/gnu/autoconf/autoconf-2.62.tar.gz](http://ftp.gnu.org/gnu/autoconf/autoconf-2.62.tar.gz)
**安装步骤**（root权限）
\# tar -zvxf m4-1.4.9.tar.gz
\# cd m4-1.4.9/
\# ./configure && make && make install
\# cd ../
\# tar -zvxf autoconf-2.62.tar.gz
\# cd autoconf-2.62/
\# ./configure && make && make install
**yum 安装方法**
yum install m4
yum install autoconf
**修改php.ini文件**
可以通过创建 phpinfo.php 文件，输入<?php echo phpinfo(); ?>来查看 php.ini 配置文件路径
![php-use-redis-01](http://blog.mimvp.com/wp-content/uploads/2015/07/php-use-redis-01.png)
vim  /etc/php5/apache2/php.ini
查找生成的 redis.so 文件，find命令：
find / -name “redis.so”
/usr/local/lib/php/extensions/no-debug-non-zts-20121212/redis.so
/home/homer/Downloads/tool-server/phpredis-develop/.libs/redis.so
/home/homer/Downloads/tool-server/phpredis-develop/modules/redis.so
增加如下内容:
extension=/usr/local/lib/php/extensions/no-debug-non-zts-20121212/redis.so
安装完成后重启 php-fpm 或 apache
/etc/init.d/apache2 restart**或**
/etc/init.d/php-fpm restart**或**
/etc/init.d/httpd restart
查看phpinfo信息，就能看到redis扩展
![php-use-redis-02](http://blog.mimvp.com/wp-content/uploads/2015/07/php-use-redis-02.png)
**启动Redis后台服务**
/usr/local/bin/redis-server   /etc/redis/redis_6379.conf   &
**Redis 示例**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|<?php
|$redis|=|new|Redis();
|$redis|->connect(|'127.0.0.1'|, 6379);|// 连接本地Redis服务
|echo|'redis is running : '|.|$redis|->ping() .|"<br>"|;|// 查看服务是否运行
|// 数据存取
|$redis|->set(|"foo"|,|"bar"|);|// 存值
|$redis|->expire(|"foo"|, 10);|// 设置超时
|$foo_value|=|$redis|->get(|"foo"|);|// 取值
|echo|"foo : "|.|$foo_value|.|"<br>"|;
|echo|"<br><br>"|;
|// list列表
|$redis|->lpush(|"mimvp"|,|"blog"|);
|$redis|->lpush(|"mimvp"|,|"forum"|);
|$redis|->lpush(|"mimvp"|,|"proxy"|);
|$list_value|=|$redis|->lrange(|"mimvp"|, 0, 2);
|print_r(|$list_value|);
|echo|"<br><br>"|;

|// keys 示例和超时验证
|$i|= 0;
|for|(|$i|=0;|$i|<20;|$i|++) {
|echo|" ---- "|.|$i|.|" ---- "|.|"<br>"|;
|$l_list|=|$redis|->keys(|"*"|);
|print_r(|$l_list|);
|echo|"<br><br>"|;
|sleep(1);
|}
|?>
|

**运行结果：**
|1

|

|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|redis is running : +PONG
|foo : bar

|Array ( [0] => proxy [1] => forum [2] => blog )
|---- 0 ----
|Array ( [0] => foo [1] => mimvp )
|---- 1 ----
|Array ( [0] => foo [1] => mimvp )
|---- 2 ----
|Array ( [0] => foo [1] => mimvp )
|---- 3 ----
|Array ( [0] => foo [1] => mimvp )
|---- 4 ----
|Array ( [0] => foo [1] => mimvp )
|---- 5 ----
|Array ( [0] => foo [1] => mimvp )
|---- 6 ----
|Array ( [0] => foo [1] => mimvp )
|---- 7 ----
|Array ( [0] => foo [1] => mimvp )
|---- 8 ----
|Array ( [0] => foo [1] => mimvp )
|---- 9 ----
|Array ( [0] => foo [1] => mimvp )
|---- 10 ----
|Array ( [0] => mimvp )
|---- 11 ----
|Array ( [0] => mimvp )
|---- 12 ----
|Array ( [0] => mimvp )
|---- 13 ----
|Array ( [0] => mimvp )
|---- 14 ----
|Array ( [0] => mimvp )
|---- 15 ----
|Array ( [0] => mimvp )
|---- 16 ----
|Array ( [0] => mimvp )
|---- 17 ----
|Array ( [0] => mimvp )
|---- 18 ----
|Array ( [0] => mimvp )
|---- 19 ----
|Array ( [0] => mimvp )
|



