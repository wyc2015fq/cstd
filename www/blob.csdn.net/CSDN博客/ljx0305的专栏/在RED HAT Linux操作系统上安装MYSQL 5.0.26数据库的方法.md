# 在RED HAT Linux操作系统上安装MYSQL 5.0.26数据库的方法 - ljx0305的专栏 - CSDN博客
2009年06月16日 17:35:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1011标签：[mysql																[linux																[数据库																[server																[sql server																[脚本](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[linux下软件安装](https://blog.csdn.net/ljx0305/article/category/440323)
## 在RED HAT Linux操作系统上安装MYSQL 5.0.26数据库的方法
　1、下载MySQL的安装文件
　　安装MySQL需要下面两个文件：
　　MySQL-server-5.0.26-0.i386.rpm
　　MySQL-client-5.0.26-0.i386.rpm
　　下载地址为：[http://dev.mysql.com/downloads/mysql/5.0.html](http://dev.mysql.com/downloads/mysql/5.0.html)，打开此网页，下拉网页找到“Red Hat Enterprise Linux 3 RPM (x86) downloads”项，找到“Server”和“Client programs”项，下载需要的上述两个rpm文件。
　　2、安装MySQL
　　rpm文件是Red Hat公司开发的软件安装包，rpm可让Linux在安装软件包时免除许多复杂的手续。该命令在安装时常用的参数是 –ivh ,其中i表示将安装指定的rmp软件包，V表示安装时的详细信息，h表示在安装期间出现“#”符号来显示目前的安装过程。这个符号将持续到安装完成后才停止。
　　1）安装服务器端
　　在有两个rmp文件的目录下运行如下命令：
　　[root@test1 local]# rpm -ivh MySQL-server-5.0.26-0.i386.rpm
　　显示如下信息。
　　warning: MySQL-server-5.0.26-0.i386.rpm: V3 DSA signature: NOKEY, key ID 5072e1f5
　　Preparing……　　　　　　　########################################### [100%]
　　1:MySQL-server　　　　　########################################### [100%]
　　……（省略显示）
　　/usr/bin/mysqladmin -u root password 'new-password'
　　/usr/bin/mysqladmin -u root -h test1 password 'new-password'
　　……（省略显示）
　　Starting mysqld daemon with databases from /var/lib/mysql
　　如出现如上信息，服务端安装完毕。测试是否成功可运行netstat看Mysql端口是否打开，如打开表示服务已经启动，安装成功。Mysql默认的端口是3306。
　　[root@test1 local]# netstat -nat
　　Active Internet connections (servers and established)
　　Proto Recv-Q Send-Q Local Address　　　　　 Foreign Address　　　　 State
　　tcp　　0　　0 0.0.0.0:3306　　　　 0.0.0.0:*　　　　　 LISTEN
　　上面显示可以看出MySQL服务已经启动。
　　2）安装客户端
　　运行如下命令：
　　[root@test1 local]# rpm -ivh MySQL-client-5.0.26-0.i386.rpm
　　warning: MySQL-client-5.0.26-0.i386.rpm: V3 DSA signature: NOKEY, key ID 5072e1f5
　　Preparing……　　　　########################################### [100%]
　　1:MySQL-client　 ########################################### [100%]
　　显示安装完毕。
　　用下面的命令连接mysql,测试是否成功。
　　登录MySQL
　　登录MySQL的命令是mysql， mysql 的使用语法如下：
　　mysql [-u username] [-h host] [-p[password]] [dbname]
　　username 与 password 分别是 MySQL 的用户名与密码，mysql的初始管理帐号是root，没有密码，注意：这个root用户不是Linux的系统
　　用户。MySQL默认用户是root，由于初始没有密码，第一次进时只需键入mysql即可。
　　[root@test1 local]# mysql
　　Welcome to the MySQL monitor.　Commands end with ; or g.
　　Your MySQL connection id is 1 to server version: 4.0.16-standard
　　Type 'help;' or 'h' for help. Type 'c' to clear the buffer.
　　mysql>
　　出现了“mysql>”提示符，恭喜你，安装成功！
　　增加了密码后的登录格式如下：
　　mysql -u root -p
　　Enter password: (输入密码)
　　其中-u后跟的是用户名，-p要求输入密码，回车后在输入密码处输入密码。
　　注意：这个mysql文件在/usr/bin目录下，与后面讲的启动文件/etc/init.d/mysql不是一个文件。
MySQL的几个重要目录
　　MySQL安装完成后不象SQL Server默认安装在一个目录，它的数据库文件、配置文件和命令文件分别在不同的目录，了解这些目录非常重
　　要，尤其对于Linux的初学者，因为 Linux本身的目录结构就比较复杂，如果搞不清楚MySQL的安装目录那就无从谈起深入学习。
　　下面就介绍一下这几个目录。
　　1、数据库目录
　　/var/lib/mysql/
　　2、配置文件
　　/usr/share/mysql（mysql.server命令及配置文件）
　　3、相关命令
　　/usr/bin(mysqladmin mysqldump等命令)
　　4、启动脚本
　　/etc/rc.d/init.d/（启动脚本文件mysql的目录）
　　修改登录密码
　　MySQL默认没有密码，安装完毕增加密码的重要性是不言而喻的。
　　1、命令
　　usr/bin/mysqladmin -u root password 'new-password'
　　格式：mysqladmin -u用户名 -p旧密码 password 新密码
　　2、例子
　　例1：给root加个密码123456。
　　键入以下命令 ：
　　[root@test1 local]# /usr/bin/mysqladmin -u root password 123456
　　注：因为开始时root没有密码，所以-p旧密码一项就可以省略了。
　　3、测试是否修改成功
　　1）不用密码登录
　　[root@test1 local]# mysql
　　ERROR 1045: Access denied for user: 'root@localhost' (Using password: NO)
　　显示错误，说明密码已经修改。
　　2）用修改后的密码登录
　　[root@test1 local]# mysql -u root -p
　　Enter password: (输入修改后的密码123456)
　　Welcome to the MySQL monitor.　Commands end with ; or g.
　　Your MySQL connection id is 4 to server version: 4.0.16-standard
　　Type 'help;' or 'h' for help. Type 'c' to clear the buffer.
　　mysql>
　　成功！
　　这是通过mysqladmin命令修改口令，也可通过修改库来更改口令。
　　启动与停止
　　1、启动
　　MySQL安装完成后启动文件mysql在/etc/init.d目录下，在需要启动时运行下面命令即可。
　　[root@test1 init.d]# /etc/init.d/mysql start
　　2、停止
　　/usr/bin/mysqladmin -u root -p shutdown
　　3、自动启动
　　1）察看mysql是否在自动启动列表中
　　[root@test1 local]#　/sbin/chkconfig –list
　　2）把MySQL添加到你系统的启动服务组里面去
　　[root@test1 local]#　/sbin/chkconfig　– add　mysql
　　3）把MySQL从启动服务组里面删除。
　　[root@test1 local]#　/sbin/chkconfig　– del　mysql
更改MySQL目录
　　MySQL默认的数据文件存储目录为/var/lib/mysql。假如要把目录移到/home/data下需要进行下面几步：
　　1、home目录下建立data目录
　　cd /home
　　mkdir data
　　2、把MySQL服务进程停掉：
　　mysqladmin -u root -p shutdown
　　3、把/var/lib/mysql整个目录移到/home/data
　　mv /var/lib/mysql　/home/data/
　　这样就把MySQL的数据文件移动到了/home/data/mysql下
　　4、找到my.cnf配置文件
　　如果/etc/目录下没有my.cnf配置文件，请到/usr/share/mysql/下找到*.cnf文件，拷贝其中一个到/etc/并改名为my.cnf)中。命令如下：
　　[root@test1 mysql]# cp /usr/share/mysql/my-medium.cnf　/etc/my.cnf
　　5、编辑MySQL的配置文件/etc/my.cnf
　　为保证MySQL能够正常工作，需要指明mysql.sock文件的产生位置。 修改socket=/var/lib/mysql/mysql.sock一行中等号右边的值
　　为：/home/mysql/mysql.sock 。操作如下：
　　vi　 my.cnf　　　 (用vi工具编辑my.cnf文件，找到下列数据修改之)
　　# The MySQL server
　　[mysqld]
　　port　　　= 3306
　　#socket　 = /var/lib/mysql/mysql.sock（原内容，为了更稳妥用“#”注释此行）
　　socket　 = /home/data/mysql/mysql.sock　　　（加上此行）
　　6、修改MySQL启动脚本/etc/rc.d/init.d/mysql
　　最后，需要修改MySQL启动脚本/etc/rc.d/init.d/mysql，把其中datadir=/var/lib/mysql一行中，等号右边的路径改成你现在的实际存
　　放路径：home/data/mysql。
　　[root@test1 etc]# vi　/etc/rc.d/init.d/mysql
　　#datadir=/var/lib/mysql　　　　（注释此行）
　　datadir=/home/data/mysql　　 （加上此行）
　　7、重新启动MySQL服务
　　/etc/rc.d/init.d/mysql　start
　　或用reboot命令重启Linux
　　如果工作正常移动就成功了，否则对照前面的7步再检查一下。
　　为了在其它电脑上能用root用户登录,需进行以下动作:
　　1、mark@marklinux mark>mysql -h localhost -u root
　　//这样应该可以进入MySQL服务器
　　2、mysql>GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION
　　//赋予任何主机访问数据的权限
　　3、mysql>FLUSH PRIVILEGES
　　//修改生效
　　4、mysql>EXIT
　　//退出MySQL服务器
　　这样就可以在其它任何的主机上以root身份登录啦！
引用于：[http://bbs.bbfish.net/thread-6477-1-1.html](http://bbs.bbfish.net/thread-6477-1-1.html)
