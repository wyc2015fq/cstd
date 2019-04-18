# RedHat Enterprise Linux 4下安装Apache2.2.3+Php1.5+Mysql5.0.24+GD - ljx0305的专栏 - CSDN博客
2009年06月16日 17:40:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1576标签：[mysql																[apache																[php																[redhat																[linux																[perl](https://so.csdn.net/so/search/s.do?q=perl&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=redhat&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[linux下软件安装](https://blog.csdn.net/ljx0305/article/category/440323)
查看是否安装Apache
rpm -qa |grep httpd
rpm -e httpd
请使用whereis mysql看看MySQL的路径 
rpm -qa |grep httpd
rpm -e httpd
rpm安装mysql:
[root@xy998 root]# rpm -ivh MySQL-server-5.0.24-0.i386.rpm
[root@xy998 root]# rpm -ivh MySQL-client-5.0.24-0.i386-.rpm
[root@xy998 root]# /usr/bin/mysqladmin -u root -p password 'cnadmysql'
[root@xy998 root]# /etc/rc.d/init.d/mysql start
tar安装mysql
请你使用如下命令来决定那个版本
# rpm -qa | grep glibc
glibc-kernheaders-2.4-8.10
glibc-common-2.3.2-11.9
glibc-2.3.2-11.9
glibc-devel-2.3.2-11.9
如果出现以上信息，请选择mysql-max-5.0.24-linux-i686-glibc23.tar.gz版本
[root@xy998 root]# tar -zvxf mysql-max-5.0.24-linux-i686-glibc23.tar.gz
[root@xy998 root]# mkdir -p /usr/local/mysql （省略也可）
[root@xy998 root]# cp -r mysql-max-5.0.24-linux-i686-glibc23 /usr/local/mysql
[root@xy998 root]# cp /usr/local/mysql/support-files/my-medium.cnf /etc/my.cnf
添加mysql用户及用户组
[root@xy998 root]# groupadd mysql
[root@xy998 root]# useradd -g mysql mysql
修改mysql目录权限
[root@xy998 root]# chown -R root /usr/local/mysql
[root@xy998 root]# chgrp -R mysql /usr/local/mysql
[root@xy998 root]# chown -R mysql /usr/local/mysql/data
生成mysql系统数据库
[root@xy998 root]# /usr/local/mysql/scripts/mysql_install_db --user=mysql&
启动mysql服务
[root@xy998 root]# /usr/local/mysql/bin/mysqld_safe --user=mysql&
如出现 Starting mysqld daemon with databases from /usr/local/mysql/data
代表正常启动mysql服务了, 按Ctrl + C 跳出
修改 mysql 的 root 密码
[root@xy998 root]# /usr/local/mysql/bin/mysqladmin -u root -p password "123456"
[root@xy998 root]# cp support-files/mysql.server /etc/rc.d/init.d/mysqld
[root@xy998 root]# chmod 700 /etc/rc.d/init.d/mysqld
[root@xy998 root]# chkconfig --add mysqld
[root@xy998 root]# chkconfig --level 345 mysqld on
[root@xy998 root]# service mysqld start
[root@xy998 root]# ln -s /usr/local/mysql/bin/mysql /usr/bin/mysql
安装zlib-1.2.3
[root@xy998 root]# tar -zxvf zlib-1.2.3.tar.gz
[root@xy998 root]# cd zlib-1.2.3
[root@xy998 zlib-1.2.3]# ./configure --prefix=/usr/local/zlib2
[root@xy998 zlib-1.2.3]# make
[root@xy998 zlib-1.2.3]# make install
安装XPM 
#rpm -ivhxpm-3.4k-2.i386.rpm
安装libxml 
[root@xy998 root]# tar -zxf libxml2-2.6.16.tar.gz 
[root@xy998 root]# cd libxml2-2.6.16 
[root@xy998 libxml2-2.6.16 ]# ./configure (xml默认安装就可以,不要指定路径了,因为安装时php可能找不到它,PHP5只支持libxml2-
2.5.10以上版本) 
[root@xy998 libxml2-2.6.16 ]# make 
[root@xy998 libxml2-2.6.16 ]# make install 
安装jpegsrc.v6b
[root@xy998 root]# tar -zxvf jpegsrc.v6b.tar.gz
[root@xy998 root]# cd jpeg-6b
[root@xy998 jpeg-6b]# ./configure --prefix=/usr/local/jpeg6
[root@xy998 jpeg-6b]# make
[root@xy998 jpeg-6b]# mkdir /usr/local/jpeg6 
[root@xy998 jpeg-6b]# mkdir /usr/local/jpeg6/include 
[root@xy998 jpeg-6b]# mkdir /usr/local/jpeg6/lib
[root@xy998 jpeg-6b]# mkdir /usr/local/jpeg6/bin 
[root@xy998 jpeg-6b]# mkdir /usr/local/jpeg6/man 
[root@xy998 jpeg-6b]# mkdir /usr/local/jpeg6/man/man1 
[root@xy998 jpeg-6b]# make install-lib 
[root@xy998 jpeg-6b]# make install
安装libpng-1.2.8-config
[root@xy998 root]# tar -zxvf libpng-1.2.8-config.tar.gz
[root@xy998 root]# cd libpng-1.2.8-config
[root@xy998 libpng-1.2.8]# ./configure --prefix=/usr/local/libpng2
[root@xy998 libpng-1.2.8]# make test
[root@xy998 libpng-1.2.8]# make install
或者安装libpng-1.2.8
[root@xy998 root]# tar zxf libpng-1.2.8.tar.gz
[root@xy998 root]# cd libpng-1.2.8
(这个不用configure，有现存的makefile文件，在scripts下面，直接copy到外面来就行了)
[root@xy998 root]# cp scripts/makefile.std makefile
#[root@xy998 root]# cp scripts/makefile.linux makefile
make
make install
安装freetype
[root@xy998 root]# tar -zxf freetype-2.1.9.tar.gz
[root@xy998 root]# cd freetype-2.1.9
[root@xy998 freetype-2.1.9]# ./configure --prefix=/usr/local/freetype2
[root@xy998 freetype-2.1.9]# make
[root@xy998 freetype-2.1.9]# make install
安装GD库
[root@xy998 root]# tar -zxvf gd-2.0.33.tar.gz
[root@xy998 root]# cd gd-2.0.33
[root@xy998 gd-2.0.33]# ./configure --prefix=/usr/local/gd2 --with-zlib=/usr/local/zlib2/ 
--with-jpeg=/usr/local/jpeg6/ --with-freetype=/usr/local/freetype2/ 
--with-png=/usr/local/libpng2/
[root@xy998 gd-2.0.33]# make
[root@xy998 gd-2.0.33]# make install
安装Apache
[root@xy998 root]# tar zxvf httpd-2.2.3.tar.gz 
[root@xy998 root]# cd httpd-2.2.3
[root@xy998 httpd-2.2.3]# ./configure --prefix=/usr/local/apache2 --enable-module=so --enable-mods-shared=all
[root@xy998 httpd-2.2.3]# make
[root@xy998 httpd-2.2.3]# make install
[root@xy998 httpd-2.2.3]# /usr/local/apache2/bin/apachectl start 启动apache
安装PHP5
[root@xy998 root]# tar -zxvf php-5.1.5.tar.gz
[root@xy998 root]# cd php-5.1.5
[root@xy998 php-5.1.5]# ./configure --prefix=/usr/local/php5 --with-apxs2=/usr/local/apache2/bin/apxs 
--with-jpeg-dir=/usr/local/jpeg6/ --with-gd=/usr/local/gd2/ 
--with-freetype-dir=/usr/local/freetype2/ --enable-trace-vars 
--with-zlib-dir=/usr/local/zlib2/ --with-mysql=/usr/local/mysql/ 
[root@xy998 php-5.1.5]# make 
[root@xy998 php-5.1.5]# make install 
cp php.ini-dist /usr/local/php5/lib/php.ini
后期修改配置文件 vi /usr/local/apache2/conf/httpd.conf 
ADDType application/x-httpd-php .php
ADDType application/x-httpd-php-source .phps
把上面这两行加进去，就可以让apache执行php文件，否则，打开网页时会成为下载。
AddType application/x-tar .tgz 下加一行 
#LoadModule php5_module modules/libphp5.so 
AddType application/x-httpd-php .php 
如果你搜索其它地方没有以下这行 LoadModule php5_module modules/libphp5.so 
请把上面的#号去掉 
还有找到 
DirectoryIndex index.html index.html.var 
在后面加 index.php 让它把index.php做为默认页 
找到 # don't use Group #-1 on these systems! 
把下面的用户名和组改为 User apache Group apache (原来好像是nobody) 
再找 #ServerName 把#去掉,后面的IP改成你的IP. 
找到 DocumentRoot "/usr/local/apache2/htdocs" 
把/usr/local/apache2/htdocs改为你存放网页文件的路径
为了让中文网页没乱码找到 AddDefaultCharset iso8859-1 
把后面的iso8859-1改为gb2312 或者是干脆off 
# /usr/local/apache2/bin/apachectl start 
用ZendOptimizer加速PHP 
#tar zxvf ZendOptimizer-2.5.3-linux-glibc21-i386.tar.gz 
#cd ZendOptimizer-2.5.3-linux-glibc21-i386 
#./install.sh 
安装的时候注意输入正确的安装apache的路径等内容! 
安装完毕,再看看phpinfo页面的zend选项就发现了!! 
如果你以前的apache没有删除,而你又想使用 
一切正常的话，你可以看到一连串的信息出现，告诉你Apache是如何被安装的。如果你使用RedHat Linux 的话，
从/usr/local/apache/bin/apachectl 到 /etc/rc.d/init.d/httpd 建立一个符号连接：
# ln -s /usr/local/apache/bin/apachectl /etc/rc.d/init.d/httpd
# /etc/init.d/httpd restart 重启Apache
常见问题
1.当你看到mysql有很多版本，如：
mysql-max-5.0.24-linux-i686-glibc23.tar.gz和
mysql-max-5.0.24-linux-i686.tar.gz
这俩个到底选哪个呢，请你使用如下命令来决定
# rpm -qa | grep glibc
glibc-kernheaders-2.4-8.10
glibc-common-2.3.2-11.9
glibc-2.3.2-11.9
glibc-devel-2.3.2-11.9
如果出现以上信息，请选择mysql-max-5.0.24-linux-i686-glibc23.tar.gz版本
2：编译php时出现
./configure: /usr/local/apache2/bin/apxs: /replace/with/path/to/perl/interpreter:
bad interpreter: No such file or directory
这是找不到perl解释器的缘故。
修改/usr/local/apache2/bin/apxs文件中：
/replace/with/path/to/perl/interpreter
把他替换成perl所在的路径如/opt/ActivePerl-5.8/bin/perl,
(如果你下载的是active perl5.8的rpm，他默认安装路径是/opt/ActivePerl-5.8/bin/perl)
3.安装了perl后，发现所有的文本编辑器都不能用了。
运行vi或者gedit时候，提示找不到libperl.so
到perl的安装目录下找一个libperl.so放到/usr/lib目录下就解决问题了
我的libperl.so在/opt/ActivePerl-5.8/lib/5.8.8/i686-linux-thread-multi-CORE/目录下。
如果不知道libperl.so在什么地方，可以用
# locate libperl.so
查找，或者用find命令
4.当我使用mysql-standard-5.0.24，编译php时出现mysql错误：
checking for mSQL support... no
checking for MSSQL support via FreeTDS... no
checking for MySQL support... yes
checking for specified location of the MySQL UNIX socket... no
checking for MySQL UNIX socket location... /tmp/mysql.sock
checking for mysql_close in -lmysqlclient... no
checking for mysql_error in -lmysqlclient... no
configure: error: mysql configure failed. Please check config.log for more information.
网查找说：安装PHP的时候没有指定一下mysql的安装目录。但是我已经指定了，所以这个说法是错误的,其实主要原因是 mysql-level没有装，
也就是mysql的版本不对，应该换
mysql-max-5.0.24版本（包含所有mysql相关内容的软件包）
5.编译php时出现
configure: error: freetype2 not found! 
没有安装freetype-level
configure: error: libpng.(a|so) not found. 
没有安装libpng-devel
6忘了mysql的root口令怎么办
# /usr/local/mysql/support-files/mysql.server stop
# mysqld_safe --skip-grant-tables & 
# mysqladmin -u user password 'newpassword'
# mysqladmin flush-privileges
**编译安装php5.2.0时出错**1.错误信息...................如下
checking for mcrypt support... no 
checking for mhash support... no 
checking whether to include mime_magic support... no 
checking for MING support... no 
checking for mSQL support... no 
checking for MSSQL support via FreeTDS... no 
checking for MySQL support... yes 
checking for specified location of the MySQL UNIX socket... /tmp/mysql.sock 
checking for MySQL UNIX socket location... /tmp/mysql.sock 
checking for mysql_close in -lmysqlclient... no 
checking for mysql_error in -lmysqlclient... no 
configure: error: mysql configure failed. Please check config.log for more information.
2.解决方法:
如果是自己选择GZ包安装的MYSQL。这里要加路径。
加的方法为--with-mysql-dir=/user/local/mysql
切记不可写为：--with-mysql=/user/local/mysql
httpd-2.0.X.tar.gz 版本最好是下载最新的啦,下载地址:[http://www.apache.org](http://www.apache.org/)
MySQL-client-4.0.20-0.i386.rpm 
MySQL-server-4.0.20-0.i386.rpm(如果你打算升级mysql的话请下载,下载地址:http: //www.mysql.org 当然也是下载最新的版本最好了,由于我不打算安装mysql,就省略了,安装的时候的步骤也是先安装mysql!) 
php-5.0.X.tar.gz 下载地址:[http://www.php.net](http://www.php.net/)
ZendOptimizer-2.5.3-linux-glibc21-i386.tar.gz 
zend的最新版安装是好像有点问题,就下载这个版本就可以,zend用来加速php,你可以选择不安装!下载地址:http: //www.zend.com 
gd-2.0.28.tar.gz 这个软件的下载地址,php推荐的是:[http://www.boutell.com/gd/](http://www.boutell.com/gd/) 但是由于某些原因,开发者不支持gif图像的创建,这有点不太方便,所以,我下载了个支持gif图像的,也就是打了gif补丁的:http: //www.rime.com.au/gd/ 
libxml2-2.X.X.tar.gz 下载地址: 
zlib-1.X.X.tar.gz
jpegsrc.v6b.tar.gz 下载地址:[ftp://ftp.uu.net/graphics/jpeg/](ftp://ftp.uu.net/graphics/jpeg/)
libpng-1.2.5.tar.gz 下载地址:[http://www.libpng.org/pub/png/libpng.html](http://www.libpng.org/pub/png/libpng.html)
freetype2-X.X.tar.gz 下载地址:[http://www.fretype.org](http://www.fretype.org/)
xpm-3.4k-2.i386.rpm
引用于:[http://qzone.qq.com/blog/16985214-1204871210](http://qzone.qq.com/blog/16985214-1204871210)
