# Linux下两个不同版本Mysql的安装实战（Mysql5和mysql4） - ljx0305的专栏 - CSDN博客
2009年07月22日 21:26:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1214标签：[mysql																[linux																[数据库																[socket																[server																[tomcat](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[MySql](https://blog.csdn.net/ljx0305/article/category/570123)
问题的产生：在已有的Red Hat Enterprise Linux AS 3.0系统上已经运行了一套web程序，使用Mysql4, tomcat41, 现在又要求安装一套新程序，依旧使用该tomcat41, 但数据库变为mysql5。 
注意事项： 
新的程序需要注意字符集的问题， 
1)具体数据库的权限和分组问题， 
2)mysql5下的具体数据库从windows直接拷贝到Linux下不好使的问题， 
3)以及mysql5需要设置密码的问题， 
4)mysql5在linux下对数据库区分大小写的问题。 
5) mysql在终端进入 
   mysql> 
   时的用户名和密码问题 
此外，在具体安装mysql5的过程中，要注意将mysql5的安装位置（baseDir），数据库的具体存放位置（dataDir），端口号（改为3307）,进行修改，这样才能保证两个不同版本的数据库的同时运行。 
版本号： 
MySQL v4.0.24 
(1)MySQL-server-4.0.24-0.i386.rpm 
(2)MySQL-client-4.0.24-0.i386.rpm 
(3)MySQL-devel-4.0.24-0.i386.rpm 
MySQL 5.0.51a 
mysql-5.0.51a.tar.gz 
下载地址：http://ftp.ntu.edu.tw/pub/MySQL/Downloads/MySQL-5.0/mysql-5.0.51a.tar.gz 
安装MySQL v4.0.24 
#rpm -qa | grep sql 
查询系统是否默认安装了mysql服务器 
然后卸载系统默认安装的mysql 
卸载mysql命令如下： 
#rpm -e --nodeps mysql-3.23.58-1 
安装MySQL服务端： 
#rpm -ivh MySQL-server-4.0.24-0.i386.rpm 
测试服务端是否安装成功： 
#netstat -nat 
查看端口3306是否打开 
然后安装MySQL客户端 
#rpm -ivh MySQL-client-4.0.24-0.i386.rpm 
安装MySQL连接包： 
#rpm -ivh MySQL-devel-4.0.24-0.i386.rpm 
此时Mysql4的各个安装路径如下： 
以我们在Redhat下安装的MySQl4.0.26数据库为例： 
(注意事项：rpm包使用的都是默认的设置，不能更改，以下均为默认设置) 
1.配置文件：/etc/my.cnf 
2.数据库目录：/var/lib/mysql 
3.启动脚本：/etc/rc.d/init.d/mysql 
4.端口3306 
5..socket文件/tmp/mysql.socket 
—————————————————————————————————————— 
下面安装MySQL 5.0.51a 
由于我们安装的MySQL4.0.26的安装包类型是rpm包，所以，需要大家注意的是， 
它使用的都是默认的设置，安装后生成的配置文件和数据库目录等等一系列的配置都是我们不能改变的。 
因此，如果要在同一开发环境下安装两个数据库的话，我们就必须处理以下这些问题： 
1.配置文件安装路径不能相同 
2.数据库目录不能相同 
3.启动脚本不能同名 
4.端口不能相同 
5..socket文件的生成路径不能相同 
依据上面的各种要求：mysql5.0.51a.tar.gz的源码包安装做出以下调整： 
--prefix=/usr/local/mysql5  ### 数据库安装目录 
--localstatedir=/var/lib/mysql5  ### 数据库存放目录 
--with-charset=gbk --with-collation=gbk_chinese_ci --with-extra-charsets=all ### 字符集gbk加载和gbk_chinese_ci加载，可处理中文乱码问题 
其他的设置是对数据库的一下优化，在此就不再赘述。 
参考了一些msyql5的安装文档，在这里感谢各位前辈 
mysql-5.0.51a.tar.gz解压后的安装详解： 
1# cd mysql-5.0.51a 
2# mkdir /usr/local/mysql5 
(这步骤中的诸多参数中，关键的参数已经在前面介绍了，如有不懂，请参看前面的介绍。) 
3# ./configure 
--prefix=/usr/local/mysql5 
--localstatedir=/var/lib/mysql5 --with-comment=Source   
--with-server-suffix=-Community 
--with-mysqld-user=mysql 
--without-debug 
--with-big-tables 
--with-charset=gbk --with-collation=gbk_chinese_ci --with-extra-charsets=all 
--with-pthread 
--enable-static 
--enable-thread-safe-client 
--with-client-ldflags=-all-static 
--with-mysqld-ldflags=-all-static 
--enable-assembler 
--without-innodb 
--without-ndb-debug 
4# make 
5# make install 
6.# useradd mysql //添加 mysql 用户 
7# cd /usr/local/mysql5 
(注意：！！！在安装第二个数据库时候，虽然在./configure后加上了--localstatedir=/var/lib/mysql5但是并未在/var/lib下产生mysql5目录，所以我们要在源码包编译安装完成之后先检查一下是否有这个目录，如果没有的话一定要手动创建一个 命令：＃ mkdir /var/lib/mysql5再执行第八步骤。) 
8# bin/mysql_install_db --user=mysql 
(在确保第七步骤正确完成之后，在执行本步骤时，如果正确的话，在/var/lib/mysql5下将会产生相应的数据库文件。) 
9# chown -R root:mysql . 　　　　　　//设置权限，注意后面有一个 "." 
10# chown -R mysql /var/lib/mysql5 　　//设置 mysql 目录权限 
11# chgrp -R mysql . 　　　　　　　　 //注意后面有一个 "." 
12# cp share/mysql/my-huge.cnf /etc/my5.cnf 
13# cp share/mysql/mysql.server /etc/rc.d/init.d/mysql5 //开机自动启动 mysql。 
14# chmod 755 /etc/rc.d/init.d/mysql5 
15# chkconfig --add mysql5 
16以下是安装第二个数据库时，对启动文件mysql5和配置文件my5.cnf做出的必要修改。 
=================================================================================== 
/etc/rc.d/init.d/mysql5 
修改下面的内容： 
1.datadir=/var/lib/mysql5 
2.conf=/etc/my5.cnf 
3.把"$bindir/mysqld_safe --datadir=$datadir --pid-file=$server_pid_file $other_args >/dev/null 2>&1 &"替换为（双引号中的） 
"$bindir/mysqld_safe --defaults-file=/etc/my5.cnf --datadir=$datadir --pid-file=$server_pid_file $other_args >/dev/null 2>&1 &"（双引号中的） 
=================================================================================== 
/etc/my5.cnf 
修改下面的内容： 
port = 3307 ###修改相关的端口 
socket文件生成路径 
把[client]和[mysqld]中的port号都改成3307, 
socket = /tmp/mysql.sock改成socket = /tmp/mysql5.sock 
[client] 
#password = your_password 
port  = 3307 
socket  = /tmp/mysql5.sock 
# Here follows entries for some specific programs 
# The MySQL server 
[mysqld] 
port  = 3307 
socket  = /tmp/mysql5.sock 
================================================================================== 
17# /etc/rc.d/init.d/mysql5 start 　　　　　　　　　　//启动 MySQL 
18# bin/mysqladmin -u root password "password_for_root" ### 设置数据库JDBC连接的密码 
    注意事项：此密码与从终端客户端登陆数据库的密码是否为同一密码，还存在疑问。 
              设置终端客户端登陆数据库的密码：(默认设置：用户名：root 密码：（空）) 
19# cd /usr/local/mysql5/bin(进入数据库安装目录下执行以下命令) 
20# ./mysql -u root -p (登陆数据库，以mysql>开头均属数据库内的操作,注意不要丢掉分号 
mysql> use mysql; 
mysql> UPDATE user SET Password=PASSWORD('newpassword') where USER='root'; 
mysql> FLUSH PRIVILEGES; 
mysql> exit; 
21# service mysql5 stop 　　　　　　　　　　　　　　//关闭 MySQL 
22tomcat部署文件下的修改： 
修改/usr/tomcat/jetmambo/WEB-INF/classes/jdbc.properties 
1.3306改成3307 
2.jdbc.password=system(注意事项：这个密码就是数据库JDBC连接的密码) 
修改后如下： 
jdbc.driverClassName=com.mysql.jdbc.Driver 
jdbc.url=jdbc:mysql://localhost:3307/timef3_cmd?&useUnicode=true&characterEncoding=gbk 
jdbc.username=root 
jdbc.password=system 
23启动数据库和WEB服务器，验证数据库安装是否成功 
/etc/init.d/mysql restart 
/etc/init.d/mysql5 restart 
注意事项：必须保持两个数据库都开启服务,否则tomcat报SQLException 
/usr/tomcat/bin/catalina.sh run 
补充说明： 
24linux下默认数据库中表名不忽略大小写，做如下设置： 
/etc/my5.cnf 
# The MySQL server 
[mysqld] 
lower_case_table_names = 1 ### 1为忽略大小写 0为不忽略大小写 
port  = 3307 
socket  = /tmp/mysql5.sock 
———————————————————————————————————— 
遗留问题，不知那位大侠可以解答一下： 
在终端进入 
>mysql时 
报： 
ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/lib/mysql/mysql.sock' (2) 
错误。 
---------------------------------------------------------------- 
问题解决了 
http://www.javaeye.com/topic/203986 
非常感谢这篇文章 
./mysql -uroot -p -S /tmp/mysql5.sock 
如果不加参数-S，则在默认目录去找mysql.sock
引用:http://www.javaeye.com/topic/203986
