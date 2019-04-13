
# Linux（centOS）手动安装Apache+MySQL+PHP+Memcached+Nginx原创无错版 - 无界 - CSDN博客

置顶2011年07月03日 13:21:22[21aspnet](https://me.csdn.net/21aspnet)阅读数：16497


最后更新时间：2012.3.21
**===================**
**第一步：Apache安装(已更新到2.2.22)**
**===================**
下载 去[http://www.apache.org](http://www.apache.org)自己找新的
[http://httpd.apache.org/](http://httpd.apache.org/)
国内有很多镜像点例如"人人"和"163"
[http://labs.renren.com/](http://labs.renren.com/)
注意要找httpd，因为apache下有很多项目，其中httpd就是web服务器
[http://labs.renren.com/](http://labs.renren.com/)
英文文档：[http://httpd.apache.org/docs/2.2/](http://httpd.apache.org/docs/2.2/)
中文文档：[http://www.php100.com/manual/apache2/index.html](http://www.php100.com/manual/apache2/index.html)
-------------------------------------------------
\#wget[http://labs.renren.com/apache-mirror/httpd/httpd-2.2.22.tar.gz](http://labs.renren.com/apache-mirror/httpd/httpd-2.2.22.tar.gz)
\#tar zxvf httpd-2.2.22.tar.gz
\#cd ./httpd-2.2.22
\#./configure --prefix=/usr/local/apache2/ --enable-proxy --enable-ssl --enable-cgi --enable-rewrite --enable-so --enable-module=so
\#make
\#make install
说明：如果make不成功，那么是configure不成，例如你缺少ssl组件，可以去掉
--enable-ssl
-----------------------------------------------
如果**出现“No recognized SSL/TLS toolkit detected”错误**
\#yum -y install openssl openssl-devel
------------------------
如果Make: command not found
看看是不是make没装
\# rpm -qa|grep make
如果没有信息那么说明make没装
\#yum install make 即可
-------------------
手动启动
\# /usr/local/apache2/bin/apachectl start
重启
\# /usr/local/apache2/bin/apachectl restart
如果报错说：
> Could not reliably determine the server’s fully qualified domain name, using 127.0.1.1 for ServerName
需要修改/usr/local/apache2/conf/httpd.conf在
\#serverName[www.example.com:80](http://www.example.com)
这个位置修改为：
serverName localhost:80
--------------------------
YUM自动安装的启动
\#service httpd restart
输入这个会提示出错httpd未被被识别的服务
这个请见本博客另一篇介绍
-------------------------
![](http://hi.csdn.net/attachment/149772_1309670191QQ40.gif)
这就说明安装成功了。
自己在浏览器输入：[http://localhost/](http://localhost/)
![](http://hi.csdn.net/attachment/149772_1309677085cFnU.jpg)

这样就说明成功了！
------------------------------------------
**防火墙配置**
注意如果你希望在本地机器例如xp访问虚拟机的网页，如果是**centos6**需要修改防火墙启动**80端口**
/etc/sysconfig

**[root@centos6 ~]\# vim /etc/sysconfig/iptables**
添加下面一行，打开防火墙80端口:
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
重启防火墙\#/etc/init.d/iptables restart
查看CentOS防火墙信息：/etc/init.d/iptables status
关闭CentOS防火墙服务：/etc/init.d/iptables stop
------------------------
**开机加入****自动****启动服务**
编辑rc.local文件
\#vi /etc/rc.d/rc.local
加入如下启动命令
/usr/sbin/apachectl start
/etc/rc.d/init.d/mysqld start
/etc/rc.d/init.d/smb start
/usr/local/subversion/bin/svnserve -d
注意：我的这个路径是yum安装这些服务得到的路径，填写时要根据自己的安装路径，找不到可以借助whereis命令，主要是找到apachectl 、mysqld 、smb 这三个启动文件。
------------------------------------------
增加权限和文件格式转换（不然apache会报没权限执行文件）\#chmod +x   文件\#dos2unix   文件----------------------------------------
**ab测试**
\#ab -n 1000  -c  10[http://localhost/](http://localhost/)
------------------------------------------
**卸载apache**
如果是源码安装
\#rm   -rf   你的apache安装路径
同样作用于其他
========================
httpd启用虚拟主机
\#Include conf/extra/httpd-vhosts.conf
httpd-vhosts.conf
<VirtualHost *:80>
ServerAdmin webmaster@dummy-host.example.com
DocumentRoot "/www/"
<Directory />
\#拒绝所有请求
\#    Deny from all
\#是否显示列表
\#    Options Indexes FollowSymLinks
\#是否启用rewrite
\#    AllowOverride all
========================
**YUM方式安装的，这样卸载**：*\#yum remove*httpd
否则要用
\#rpm -qa|grep httpd
一个个删除
\# rpm -e  ......

**===================**
**第二步 Mysql安装**
**===================**
第一种方法：使用yum自动安装
\#yum -y install mysql-server
----------------------------------------
第二种：下载源码编译安装
\#wgethttp://mysql.cdpa.nsysu.edu.tw/Downloads/MySQL-5.1/mysql-5.1.57.tar.gz
\#tar zxvf mysql-5.1.57.tar.gz
\#cd mysql-5.1.57
\#./configure --prefix=/usr/local/mysql --with-mysqld-user=mysql --with-charset=gbk --bindir=/usr/local/bin --sbindir=/usr/local/sbin --with-plugins=innobase --with-partition
\#make
\#make install
\#cd /usr/local/mysql
\#mysql_install_db
\#chown -R mysql:mysql *
\#cp /usr/local/mysql/share/mysql/mysql.server /etc/init.d/mysql
\#/etc/rc.d/mysql start (加入到/etc/rc.local 以便系统重启后自动启动mysql)
修改/etc/rc.local
说明：请修改mysql配置文件my.cnf中wait_timeout = 600（有些程序执行时间较长，mysql超时时间如果太短的话，容易发生mysql server has go away的错误）
注意事项：
安装完成后，需要拷贝一份my.cnf到ect目录下，使用support-files目录中的一个作为模板。在这个目录中有4个模板文件，是根据不同机器的内存定制的。
\#cp support-files/my-medium.cnf /etc/my.cnf

**===================**
**第三步 PHP安装(已更新到5.4.8版)**
**===================**
去[www.php.net](http://www.php.net)找最新的版本
5.4安装
\#wget[http://cn2.php.net/get/php-5.4.8.tar.gz/from/this/mirror](http://cn2.php.net/get/php-5.4.8.tar.gz/from/this/mirror)
\#tar zxvf  php-5.4.8.tar.gz
\#cd php-5.4.8
\#./configure --prefix=/usr/local/php --with-apxs2=/usr/local/apache2/bin/apxs --with-mysql-dir=/usr/local/mysql
如果失败 可能原因有各种各样
-------------------------------
如果提示：Cannot find MySQL header files under /date/mysql.
这个是缺少了 mysql-devel 安装包，用
yum -y install mysql-devel 即可解决！
\#yum -y install mysql-devel
-------------------------------
如果提示：xml2-config not found. Please check your libxml2 installation.
需要安装libxml2
\#yum install*libxml2*-devel
-------------------------------
如果提示：Try adding --with-zlib-dir=<DIR>试着添加一下 zlib的路径，如下
\#./configure --prefix=/usr/local/php --with-apxs2=/usr/local/apache2/bin/apxs --with-mysql-dir=/usr/local/mysql --with-zlib-dir=/usr/local/zlib
------------------------------
完全成功才可以编译安装
\#make
\#make install
------------------------------------------------------
安装失败如果提示：Cannot load /usr/local/apache2/modules/libphp5.so into server:
原因：是Linux有一个SELinux保护模式引起的。
解决办法：
1关闭SELINUX的方法:
vi /etc/selinux/config 将SELINUX=enforcing 改成SELINUX=disabled 需要重启
这个方法可能会对服务器带来风险。
2不关闭SELINUX的方法:
\# setenforce 0
\# chcon -c -v -R -u system_u -r object_r -t textrel_shlib_t /usr/local/apache/modules/libphp5.so
\# service httpd restart
\# setenforce 1
--------------------------------------
复制php.ini配置文件
参考此文[http://blog.csdn.net/21aspnet/article/details/7001344](http://blog.csdn.net/21aspnet/article/details/7001344)
注意php新版本此文件名有变更
\#cp php.ini-production /usr/local/php/lib/php.ini
------------------------------
修改apache的配置文件httpd.conf
增加
AddType application/x-httpd-php .php
修改www的默认文件地址DocumentRoot
注意还要修改Directoty的路径和上面的一样
重启httpd
测试php文件
phpinfo()
---------------------------------
执行php文件
\#/usr/local/php/bin/php    xx.php

**加速器：**
[PHP安装eAccelerator](http://blog.csdn.net/21aspnet/article/details/8194030)


**PHP安装扩展的两种类型**：

### [1](http://blog.csdn.net/21aspnet/article/details/8203447)
### .
### [PHP安装扩展mcrypt以及相关依赖项 【PHP安装PECL扩展的方法】 ](http://blog.csdn.net/21aspnet/article/details/8203447)
### [2.PHP安装与使用VLD查看opcode代码【PHP安装第三方扩展的方法】 ](http://blog.csdn.net/21aspnet/article/details/7002644)



**===================**
**第四步 Memcached安装**
**===================**
可以参考[http://timke.blog.163.com/blog/static/1015873062009111094715315/](http://timke.blog.163.com/blog/static/1015873062009111094715315/)
说明必须**先安装libevent**
官网[http://libevent.org/](http://libevent.org/)找最新的版本
\#wget[https://github.com/downloads/libevent/libevent/libevent-2.0.16-stable.tar.gz](https://github.com/downloads/libevent/libevent/libevent-1.4.13-stable.tar.gz)
\#tar xzvf libevent-2.0.16-stable.tar.gz
\#cd libevent-2.0.16-stable
\#./configure --prefix=/usr
\#make
\#make install
--------------------------------------------
**第二步 安装memcached**
\# wget[http://memcached.googlecode.com/files/memcached-1.4.9.tar.gz](http://memcached.googlecode.com/files/memcached-1.4.9.tar.gz)
\#tar xzvf memcached-1.4.9.tar.gz
\#cd memcached-1.4.9
\#./configure --prefix=/usr/local/memcached --with-libevent=/usr
注意这里选择libevent的位置即可  例如你的是在–with-libevent=/usr/local/libevent/
\#make
\#make install
启动
\#/usr/local/memcached/bin/memcached -d -m 100 -uroot -l 0.0.0.0 -p 11211 -c 512 -P /usr/local/memcached/memcached.pid
查看详情
\#ps aux|grep mem
输出pid
\#cat /usr/local/memcached/memcached.pid
查看内存使用
\#top -n 1 |grep Mem
-------------------------------------------
**第三步  安装memcached的php扩展memcache**
\#wget[http://pecl.php.net/get/memcache-2.2.6.tgz](http://pecl.php.net/get/memcache-2.2.6.tgz)
\#tar vxzf memcache-2.2.6.tgz
\#cd memcache-2.2.6
\#/usr/local/php/bin/phpize
此处出错可以参考[http://blog.csdn.net/21aspnet/article/details/7001182](http://blog.csdn.net/21aspnet/article/details/7001182)
\#./configure --enable-memcache --with-php-config=/usr/local/php/bin/php-config --with-zlib-dir
或者
\#./configure --enable-memcache --with-php-config=/usr/local/php/bin/php-config --with-zlib-dir=/usr
\#make
\#make install
\#vi /usr/local/php/lib/php.ini
php.ini添加
extension=memcache.so
<完>
-------------------
重启
\# /usr/local/apache2/bin/apachectl restart
---------------
测试
```python
$memcache = new Memcache; //创建一个memcache对象   
$memcache->connect('localhost', 11211) or die ("Could not connect"); //连接Memcached服务器   
$memcache->set('key', 'test'); //设置一个变量到内存中，名称是key 值是test   
$get_value = $memcache->get('key'); //从内存中取出key的值   
echo $get_value;
```

--------------
或者手动指定
extension_dir = "/usr/local/php/lib/php/extensions/no-debug-non-zts-20060613/"
```python
class MemCache {
    var $mem = null;
    
    private static $isLibLoad = false;
    //put your code here
    public function Com_MemCache(){
    	if(!self::$isLibLoad){
    		dl("memcache.so");
    		self::$isLibLoad = true;
    	}
       	
        $this->mem = new Memcache();
        $this->mem->connect("127.0.0.1", 11211);
    }
    public function set($key,$value,$flag = null,$expire = 5000){
        $this->mem->set($key, $value, $flag, $expire);
    }
    public function get($key){
        return $this->mem->get($key);
    }
    function __destruct(){
        $this->mem->close();
    }
}
```
调用
```python
$cache = new MemCache();
           $mm = $cache ->get("A");
           if($mm==null){
             $cache->set('A', date('Y-m-j').' '.date('G:i:s'),null,15);
           }
echo $mm;
```

因为MemCache.php有 dl("memcache.so");所以在php.ini中不需要extension=memcache.so了，但是前一句extension_dir是要的。
**===================**
**Nginx****安装**
**===================**
官网：[http://wiki.nginx.org/Install](http://wiki.nginx.org/Install)
下载
\#wget[http://nginx.org/download/nginx-1.2.6.tar.gz](http://nginx.org/download/nginx-1.2.6.tar.gz)
\#./configure --prefix=/usr/local/nginx
\#make
\#make install
安装Nginx时报错
./configure: error: the HTTP rewrite module requires the PCRE library.
安装pcre-devel解决问题
yum -y install pcre-devel
错误提示：./configure: error: the HTTP cache module requires md5 functions
from OpenSSL library. You can either disable the module by using
--without-http-cache option, or install the OpenSSL library into the system,
or build the OpenSSL library statically from the source with nginx by using
--with-http_ssl_module --with-openssl=<path> options.
解决办法：
yum -y install openssl openssl-devel
总结：
yum -y install pcre-devel openssl openssl-devel
./configure --prefix=/usr/local/nginx
make
make install
**启动nginx**
\#/usr/local/nginx/sbin/nginx -c /usr/local/nginx/conf/nginx.conf
**安装成功的页面**
![](https://img-my.csdn.net/uploads/201301/23/1358940294_7640.jpg)
**停止操作**
停止操作是通过向nginx进程发送信号（什么是信号请参阅linux文 章）来进行的
步骤1：查询nginx主进程号
ps -ef | grep nginx
在进程列表里 面找master进程，它的编号就是主进程号了。
步骤2：发送信号
从容停止Nginx：
kill -QUIT 主进程号
快速停止Nginx：
kill -TERM 主进程号
强制停止Nginx：
pkill -9 nginx
另外， 若在nginx.conf配置了pid文件存放路径则该文件存放的就是Nginx主进程号，如果没指定则放在nginx的logs目录下。有了pid文 件，我们就不用先查询Nginx的主进程号，而直接向Nginx发送信号了，命令如下：
kill -信号类型 '/usr/nginx/logs/nginx.pid'
**平滑重启**
如果更改了配置就要重启Nginx，要先关闭Nginx再打开？不是的，可以向Nginx 发送信号，平滑重启。
平滑重启命令：
kill -HUP 住进称号或进程号文件路径
或者使用
/usr/nginx/sbin/nginx -s reload

注意，修改了配置文件后最好先检查一下修改过的配置文件是否正 确，以免重启后Nginx出现错误影响服务器稳定运行。判断Nginx配置是否正确命令如下：
nginx -t -c /usr/nginx/conf/nginx.conf
或者
/usr/nginx/sbin/nginx -t

**平滑升级**
如果服务器正在运行的Nginx要进行升级、添加或删除模块时，我们需 要停掉服务器并做相应修改，这样服务器就要在一段时间内停止服务，Nginx可以在不停机的情况下进行各种升级动作而不影响服务器运行。
步骤1：
如 果升级Nginx程序，先用新程序替换旧程序文件，编译安装的话新程序直接编译到Nginx安装目录中。
步 骤2：执行命令
kill -USR2 旧版程序的主进程号或进程文件名
此时旧的Nginx主进程将会把自己的进程文件改名为.oldbin，然后执行新版 Nginx。新旧Nginx会同市运行，共同处理请求。
这时要逐步停止旧版 Nginx，输入命令：
kill -WINCH 旧版主进程号
慢慢旧的工作进程就都会随着任务执行完毕而退出，新版的Nginx的工作进程会逐渐取代旧版 工作进程。
此 时，我们可以决定使用新版还是恢复到旧版。
不重载配置启动新/旧工作进程
kill -HUP 旧/新版主进程号
从容关闭旧/新进程
kill -QUIT 旧/新主进程号
如果此时报错，提示还有进程没有结束就用下面命令先关闭旧/新工作进程，再关闭主进程号：
kill -TERM 旧/新工作进程号
这样下来，如果要恢复到旧版本，只需要上面的几个步 骤都是操作新版主进程号，如果要用新版本就上面的几个步骤都操作旧版主进程号就行了。
上面就是Nginx的一些基本的操作，希望以后Nginx能有更好的方法来处理这些操作， 最好是Nginx的命令而不是向Nginx进程发送系统信号。


