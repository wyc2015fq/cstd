
# Linux环境Nginx安装与调试以及PHP安装 - 无界 - CSDN博客

置顶2015年08月17日 15:06:15[21aspnet](https://me.csdn.net/21aspnet)阅读数：16115


linux版本：64位CentOS 6.4
Nginx版本：nginx1.8.0
php版本：php5.5.28
**1.编译安装Nginx**
官网：[http://wiki.nginx.org/Install](http://wiki.nginx.org/Install)
下载
\#wget[http://nginx.org/download/nginx-1.8.0.tar.gz](http://nginx.org/download/nginx-1.8.0.tar.gz)
\#tar -zvxf nginx-1.8.0.tar.gz
\#cd ./nginx-1.8.0
\#./configure --prefix=/usr/local/nginx
\#make
\#make install
安装Nginx时报错
错误提示：./configure: error: the HTTP rewrite module requires the PCRE library.
安装pcre-devel解决问题
\# yum -y install pcre-devel
错误提示：./configure: error: the HTTP cache module requires md5 functions
from OpenSSL library. You can either disable the module by using
--without-http-cache option, or install the OpenSSL library into the system,
or build the OpenSSL library statically from the source with nginx by using
--with-http_ssl_module --with-openssl=<path> options.
解决办法：
\# yum -y install openssl openssl-devel
或者**一并安装**：
\# yum -y install pcre-devel openssl openssl-devel
**启动nginx**
\# /usr/local/nginx/sbin/nginx -c /usr/local/nginx/conf/nginx.conf
安装成功查看nginx进程
\#ps aux | grep nginx
![](https://img-my.csdn.net/uploads/201301/26/1359192313_3345.jpg)
测试页面，直接输入服务器ip
![](https://img-my.csdn.net/uploads/201301/26/1359191862_3989.jpg)
其实nginx默认的web工作目录在/usr/local/nginx/html
可以修改nginx的配置文件nginx.conf改变到其他路径
**防火墙配置**
注意如果你希望在本地机器例如xp访问虚拟机的网页，如果是centos6需要修改防火墙启动80端口
\#cd /etc/sysconfig
修改iptables文件，或者直接用vim编辑
\#vim /etc/sysconfig/iptables
添加下面一行，打开防火墙80端口:
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
重启防火墙
\# /etc/init.d/iptables restart
查看CentOS防火墙信息：/etc/init.d/iptables status
关闭CentOS防火墙服务：/etc/init.d/iptables stop
**停止nginx**
查询nginx主进程号
\#ps -ef | grep nginx
从容停止Nginx：
kill -QUIT 主进程号
快速停止Nginx：
kill -TERM 主进程号
强制停止Nginx：
pkill -9 nginx
**2.安装php**
去[www.php.net](http://www.php.net/)找下载最新的版本
[http://www.php.net/downloads.php](http://www.php.net/downloads.php)
**下载解压**
\#wget[http://cn2.php.net/get/php-5.5.28.tar.gz/from/this/mirror](http://cn2.php.net/get/php-5.5.28.tar.gz/from/this/mirror)
\#tar zxvf php-5.5.28.tar.gz
\#cd php-5.5.28

**配置**
\#./configure --prefix=/usr/local/php \
--with-curl \
--with-freetype-dir \
--with-gd \
--with-gettext \
--with-iconv-dir \
--with-kerberos \
--with-libdir=lib64 \
--with-libxml-dir \
--with-mysql \
--with-mysqli \
--with-openssl \
--with-pcre-regex \
--with-pdo-mysql \
--with-pdo-sqlite \
--with-pear \
--with-png-dir \
--with-xmlrpc \
--with-xsl \
--with-zlib \
--enable-fpm \
--enable-bcmath \
--enable-libxml \
--enable-inline-optimization \
--enable-gd-native-ttf \
--enable-mbregex \
--enable-mbstring \
--enable-opcache \
--enable-pcntl \
--enable-shmop \
--enable-soap \
--enable-sockets \
--enable-sysvsem \
--enable-xml \
--enable-zip

如果需要--with-jpeg-dir \可以按下面的装很多yum
如果需要GD库还要
./configure --prefix=/usr/local/php --with-mysql --enable-fpm--enable-opcache--with-freetype-dir --with-jpeg-dir --with-png-dir --with-gd --with-zlib --with-libxml-dir --enable-xml --disable-debug --enable-safe-mode --enable-mbstring
如果配置错误，需要安装需要的模块，直接yum**一并安装**依赖库
\#yum -y install libjpeg libjpeg-devel libpng libpng-devel freetype freetype-devel libxml2 libxml2-devel mysql pcre-devel

配置成功：
![](https://img-blog.csdn.net/20140126110224734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**编译安装**
配置成功后继续完成编译安装php
\#make && make install
![](https://img-blog.csdn.net/20140422112820281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
php-5.4.11已经支持**fastcgi**了，不用在打补丁了。
1.在编译安装时加上'--enable-fpm'，注意修改php/etc/php-fpm.conf 去掉pid前的注释；
2.复制/sapi/fpm/init.d.php-fpm /etc/init.d/php-fpm还可以做成服务
复制php-fpm文件到php安装目录
\#cp -R ./sapi/fpm/php-fpm.conf /usr/local/php/etc/php-fpm.conf
**拷贝php.ini文件**
安装目录有2个文件：php.ini-development和php.ini-production
注意php.ini-production拥有较高的安全性设定，则适合上线当产品使用
\#cp php.ini-development /usr/local/php/lib/php.ini
或者
\# cp php.ini-production /usr/local/php/lib/php.ini
如果操作了这一步以后使用phpinfo()就会看到Loaded Configuration File：
![](https://img-blog.csdn.net/20130917184258328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**php-fpm启动**
拷贝启用文件
\#cp -R ./sapi/fpm/php-fpm /etc/init.d/php-fpm
**启动**
\#/etc/init.d/php-fpm
php安装成功查看进程
\#ps aux|grep php
![](https://img-blog.csdn.net/20140422132054406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**重启**
\#killall php-fpm
\#/etc/init.d/php-fpm

关于php-fpm配置参考：[http://blog.csdn.net/unix21/article/details/8743246](http://blog.csdn.net/unix21/article/details/8743246)
**php-fpm源码目录**：php-5.5.10\sapi\fpm

**配置nginx支持PHP**
nginx的配置文件nginx.conf在
\# cd /usr/local/nginx/conf

nginx默认web文件夹在
/usr/local/nginx/html
如果需要nginx支持PHP需要修改nginx.conf，加入如下语句：
location ~ \.php$ {
root html;
fastcgi_pass 127.0.0.1:9000;
fastcgi_index index.php;
include fastcgi_params;
fastcgi_param SCRIPT_FILENAME /usr/local/nginx/html$fastcgi_script_name;
}本机的web文件夹在/usr/www，需要修改fastcgi_param SCRIPT_FILENAME指向对应目录即可：
所以

```python
location ~ \.php$ {
root html;
fastcgi_pass 127.0.0.1:9000;
fastcgi_index index.php;
include fastcgi_params;
fastcgi_param SCRIPT_FILENAME /usr/www$fastcgi_script_name;
}
```
注意这样只能解析php，不能解析/usr/www下的html文件，是因为需要改配置
```python
location / {
            #root   html;
			root /usr/www;
            index  index.html index.htm;
        }
```
**重启nginx**
\# /usr/local/nginx/sbin/nginx -s reload
测试页面info.php
```python
<?php
phpinfo();
?>
```
上传至/usr/www目录，成功运行
![](https://img-blog.csdn.net/20150810141357784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**错误描述：**
PHP Warning:  phpinfo(): It is not safe to rely on the system's timezone settings. You are *required* to use the date.timezone setting or the date_default_timezone_set() function. In case you used any of those methods and you are still getting this warning, you most likely misspelled the timezone identifier. We selected 'UTC' for '8.0/no DST' instead in C:\info.php on line 4
解决办法：
打开php.ini 配置文件
找到
[Date]
; Defines the default timezone used by the date functions
; http://php.net/date.timezone
;date.timezone =
修改为：
[Date]
; Defines the default timezone used by the date functions
; http://php.net/date.timezone
date.timezone =PRC或者UTC
然后记得重启Nginx!
另外，还有下面两种办法（不建议）
1、在页头使用date_default_timezone_set()设置 date_default_timezone_set('PRC'); //东八时区 echo date('Y-m-d H:i:s');
2、在页头使用 ini_set('date.timezone','Asia/Shanghai');
对于中国，可选值：Asia/Chongqing ，Asia/Shanghai ，Asia/Urumqi （依次为重庆，上海，乌鲁木齐）港台地区可用：Asia/Macao ，Asia/Hong_Kong ，Asia/Taipei （依次为澳门，香港，台北），还有新加坡：Asia/Singapore，老的PRC也行。
PHP所支持的时区列表可参见：http://www.php.net/manual/zh/timezones.php
===================================
**开启PHP的opcace**
php.ini配置opcace
;opcache
zend_extension = /usr/local/php/lib/php/extensions/no-debug-non-zts-20121212/opcache.so
opcache.memory_consumption=128
opcache.interned_strings_buffer=8
opcache.max_accelerated_files=4000
opcache.revalidate_freq=60
opcache.fast_shutdown=1
opcache.enable_cli=1
opcache.enable=1
![](https://img-blog.csdn.net/20130918183426921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果嫌这个不直观可以装个PHP文件查看，地址在[https://gist.github.com/ck-on/4959032](https://gist.github.com/ck-on/4959032)，
把上面的代码保存为一个php文件放到你的网站目录下面打开就可以看到：
![](https://img-blog.csdn.net/20130918184043421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Opcache参考：
[新一代 PHP 加速插件 Zend Opcache](http://www.laogui.com/Zend-Opcache)
[Linux平滑编译升级php至5.5.0](http://levi.cg.am/?p=2987)
**3.升级php**
假设之前安装过老版本的php例如php5.4.8，需要安装最新的版本只要用之前的安装步骤安装最新的版本即可.
**4.跟踪Nginx和PHP**
设置Nginx和PHP都是单子进程
**修改nginx.conf**
worker_processes  1;
**修改php-fpm.conf**
pm = static
pm.max_children = 1
\#ps aux
![](https://img-my.csdn.net/uploads/201303/31/1364725077_1779.jpg)
**strace/ltrace**
strace或ltrace跟踪都可以查看应用程序正在运行过程中发起的系统函数调用和动态库函数调用，二者大同小异。
\#strace
-p 进程id   通过进程号跟踪指定进程
-o  文件名 跟踪信息输出到指定文件
-f   跟踪其通过fork调用产生的子进程
-t  输出每一个系统调用的发起时间
-T 输出每一个系统调用的消耗时间
退出Ctrl+C
1).Nginx master process
![](https://img-my.csdn.net/uploads/201303/31/1364724836_4607.jpg)
2).Nginx worker process
![](https://img-my.csdn.net/uploads/201303/31/1364724845_2810.jpg)
3) php-fpm master process
![](https://img-my.csdn.net/uploads/201303/31/1364724851_9110.jpg)
4) php-fpm pool
![](https://img-my.csdn.net/uploads/201303/31/1364724858_1951.jpg)

**pstack**
pstack查看系统调用和函数调用关系，可以看到当前Nginx内部在执行什么函数。
其实，pstack不过是gdb实现的一个shell脚本而已。
![](https://img-blog.csdn.net/20130815084946078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**5.调试Nginx**
由于gdb需要gcc的时候加上-g参数，这样生成的文件才能使用gdb调试，因此我们要对源码做一下小改动
## 修改auto/cc/conf文件
ngx_compile_opt="-c"
变为
ngx_compile_opt="-c -g"
## 执行configure
./configure --prefix=/usr/local/nginx
另一个值得关注的编译选项是-O0，如果在gdb内打印变量时提示"<value optimized out>"或gdb提示显示的当前正执行的代码行与源码匹配不上而让人感觉莫名其妙，那么，这则多半是因为gcc的优化导致，我们可以加上-O0选项来强制禁用gcc的编译优化。除了可以通过编辑objs/Makefile文件，把这两个选项直接加在CFLAGS变量里以外，还有另外几种方法也可以达到同样的效果。
1． 在进行configure配置时，按如下方式执行。
[root@localhost nginx-1.2.0]\#CFLAGS="-g -O0"./configure --prefix=/usr/local/nginx
2． 在执行make时，按如下方式执行。
[root@localhost nginx-1.2.0]\# makeCFLAGS="-g -O0"
直接修改objs/Makefile文件和上面提到的第2种方法是在我们已经执行configure之后进行的，如果之前已经执行过make，那么还需刷新所有源文件的时间戳，以便重新编译每一个Nginx源文件。
修改nginx的conf配置文件为单进程
```python
﻿﻿﻿﻿worker_processes  1;
daemon off; 
master_process  off;
```
**查看宏定义**
注意：Nginx中有大量的宏，如果不做特殊处理是看不到这些宏定义的：
![](https://img-blog.csdn.net/20130815111259375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
必须编译的时候使用ggdb3才可以查看定义。
## 修改auto/cc/conf文件
ngx_compile_opt="-c"
变为
ngx_compile_opt="-c -ggdb3"
或者
\#makeCFLAGS="-ggdb3 -O0"
![](https://img-blog.csdn.net/20130815111421875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
\#info macro NGX_OK
\#macro expand NGX_OK
查看ngx_core.h
![](https://img-blog.csdn.net/20130815112138578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**启动nginx**
\# /usr/local/nginx/sbin/nginx -c /usr/local/nginx/conf/nginx.conf
\#ps -efH | grep nginx
\#gdb -p 进程id     或者(gdb) attach进程id  一样的
利用set args 命令就可以修改发送给程序的参数，而使用show args 命令就可以查看其缺省参数的列表。
(gdb)set args -c /usr/local/nginx/conf/nginx.conf
(gdb) show args
使用l（list）命令查看main 函数，这个main函数是在src/core/nginx.c里面，是nginx的入口函数
(gdb) l \#列出源文件，只列出10行，请使用l- 和 l 或者l +行号 或者 l +函数名 进行定位
(gdb) break 215 \#在哪行打断点, 可以break  main
break命令（可以简写为b）可以用来在调试的程序中设置断点，该命令有如下四种形式：
break line-number 使程序恰好在执行给定行之前停止。
break function-name 使程序恰好在进入指定的函数之前停止。
break line-or-function if conditio如果condition（条件）是真，程序到达指定行或函数时停止。
break routine-name 在指定例程的入口处设置断点
如果该程序是由很多原文件构成的，你可以在各个原文件中设置断点，而不是在当前的原文件中设置
断点，其方法如下：
(gdb) break filename:line-number
(gdb) break filename:function-name
要想设置一个条件断点，可以利用break if命令，如下所示：
(gdb) break line-or-function if expr
例：
(gdb) break 46 if testsize==100
从断点继续运行：countinue命令break命令（可以简写为b）可以用来在调试的程序中设置断点，该命令有如下四种形式：
break line-number 使程序恰好在执行给定行之前停止。
break function-name 使程序恰好在进入指定的函数之前停止。
break line-or-function if conditio如果condition（条件）是真，程序到达指定行或函数时停止。
break routine-name 在指定例程的入口处设置断点
如果该程序是由很多原文件构成的，你可以在各个原文件中设置断点，而不是在当前的原文件中设置
断点，其方法如下：
(gdb) break filename:line-number
(gdb) break filename:function-name
要想设置一个条件断点，可以利用break if命令，如下所示：
(gdb) break line-or-function if expr
例：
(gdb) break 46 if testsize==100
从断点继续运行：countinue命令
断点可以多个
(gdb)info break**列出所有断点**
删除指定的某个断点：
(gdb) delete breakpoint 1
该命令将会删除编号为1的断点，如果不带编号参数，将删除所有的断点
禁止使用某个断点
(gdb) disable breakpoint 1
该命令将禁止断点1,同时断点信息的(Enb)域将变为n
允许使用某个断点
(gdb) enable breakpoint 1
该命令将允许断点1,同时断点信息的(Enb)域将变为y
![](https://img-blog.csdn.net/20130728161756671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
(gdb) continue 从断点开始继续执行直到下一个断点
(gdb) r \#run，从main函数开始重新运行程序，因为之前已经执行了，所以要想从main开始调试要r，这样就会结束之前的进程
(gdb) l   列出代码
(gdb) s 下一步，会列出当前所在的文件代码行数
(gdb)finish 结束执行当前函数，显示其返回值（如果有的话）
(gdb)bt**打印栈帧，可以看出当前文件名以及函数**
(gdb) p *ptr   打印指针内容
(gdb)info args
打印出当前函数的参数名及其值。
(gdb)info locals
打印出当前函数中所有局部变量及其值。
(gdb)info catch
打印出当前的函数中的异常处理信息。
查看数组
int *array = (int *) malloc (len * sizeof (int));
(gdb)p *array@len
@的左边是数组的首地址的值，也就是变量array所指向的内容，右边则是数据的长度，其保存在变量len中
(gdb)disp * 结构体变量查看结构体信息
(gdb)p * 结构体变量查看结构体信息
其实 print *a@5   指针对象都可以看到
**查看内存**
(gdb)x
forward-search <regexp>
search <regexp>
向前面搜索。
reverse-search <regexp>
**查看void指针**
ngx_http_init_connection(ngx_connection_t *c)
查看p *c
![](https://img-blog.csdn.net/20130816135941578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果要直接查看data
p *c->data
提示Attempt to dereference a generic pointer.
因为空指针类型必须强制转换！
查看代码
```python
ngx_http_connection_t  *hc;
c->data = hc;
```
所以强制转换即可
![](https://img-blog.csdn.net/20130816171116640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
空指针对象转换技巧：
*(类型*)(空指针对象)

**查看socket**
\#lsof -p  进程id
**监控epoll FD**
\#ps aux|grep nginx
\#sudo ls -alh /proc/Nginx的进程id/fd
![](https://img-blog.csdn.net/20130728175027234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**使用tcpdump查看数据包**
\#tcpdump -i eth0 port 80
![](https://img-blog.csdn.net/20130911112314406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5peDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**压力测试**
\#ab -c l -n 10000 http://127.0.0.1/index.html

**参考文献**：
[nginx模块_使用gdb调试nginx源码](http://www.cnblogs.com/yjf512/archive/2012/05/10/2494635.html)
[绑定Nginx到gdb](http://book.51cto.com/art/201305/395377.htm)
[Nginx模块开发入门](http://www.162cm.com/p/ngx_ext.html)
[gdb调试nginx的helloworld](http://haoningabc.iteye.com/blog/1176287)
[gdb中查看数组各项的值](http://blog.csdn.net/azr22005/article/details/6656872)
[gdb查看运行时数据](http://blog.sina.com.cn/s/blog_677166430100tegm.html)
[GDB 格式化结构体输出](http://blog.csdn.net/unix21/article/details/9991925)[GDB使用总结](http://blog.csdn.net/unix21/article/details/9628933)

