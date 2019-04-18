# 安装mysql5.1(linux red hat 3) - ljx0305的专栏 - CSDN博客
2009年07月22日 21:40:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：647标签：[mysql																[linux																[数据库																[user																[shell																[database](https://so.csdn.net/so/search/s.do?q=database&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[MySql](https://blog.csdn.net/ljx0305/article/category/570123)
首先：
[卸载系统自带的mysql](http://sheng8407-sina-com.javaeye.com/blog/346456)
接着安装：
**linux下mysql(rpm)安装使用手册**
安装Mysql 
1、下载MySQL的安装文件 
安装MySQL需要下面两个文件：MySQL-server-5.1.7-0.i386.rpm
下载地址为：[http://dev.mysql.com/downloads/mysql/5.1.html](http://dev.mysql.com/downloads/mysql/5.1.html) ，打开此网页，下拉网页找到“Linux x86 generic RPM (statically linked against glibc 2.2.5) downloads”项，找到“Server”和“Client programs”项，下载需要的上述两个rpm文件。 
2、安装MySQL 
rpm文件是Red Hat公司开发的软件安装包，rpm可让Linux在安装软件包时免除许多复杂的手续。该命令在安装时常用的参数是 –ivh ,其中i表示将安装指定的rmp软件包，V表示安装时的详细信息，h表示在安装期间出现“#”符号来显示目前的安装过程。这个符号将持续到安装完成后才停止。 
1）安装服务器端 
在有两个rmp文件的目录下运行如下命令： 
[root@test1 local]# rpm -ivh MySQL-server-5.1.7-0.i386.rpm MySQL-client-5.1.7-0.i386.rpm　　显示如下信息。 
warning: MySQL-server-5.1.7-0.i386.rpm
signature: NOKEY, key ID 5072e1f5 
　　 Preparing...　　　　　　　########################################### [100%] 
1:MySQL-server　　　　　########################################### [100%] 
。。。。。。（省略显示） 
/usr/bin/mysqladmin -u root password 'new-password' 
/usr/bin/mysqladmin -u root -h test1 password 'new-password' 
。。。。。。（省略显示） 
Starting mysqld daemon with databases from /var/lib/mysql 
如出现如上信息，服务端安装完毕。测试是否成功可运行netstat看Mysql端口是否打开，如打开表示服务已经启动，安装成功。Mysql默认的端口是3306。 
[root@test1 local]# netstat -nat 
Active Internet connections (servers and established) 
Proto Recv-Q Send-Q Local Address　　　　　 Foreign Address　　　　 State　　　 
tcp　　0　　0 0.0.0.0:3306　　　　 0.0.0.0:*　　　　　 LISTEN　　　 
上面显示可以看出MySQL服务已经启动。 
2）安装客户端 
运行如下命令： 
[root@test1 local]# rpm -ivh MySQL-client-5.1.7-0.i386.rpm
　　 warning: MySQL-client-5.1.7-0.i386.rpm: V3 DSA signature: NOKEY, key ID 5072e1f5 
　　 Preparing...　　　　########################################### [100%] 
1:MySQL-client　 ########################################### [100%] 
显示安装完毕。 
用下面的命令连接mysql,测试是否成功。 
三、登录MySQL
登录MySQL的命令是mysql， mysql 的使用语法如下： 
mysql [-u username] [-h host] [-p[password]] [dbname] 
username 与 password 分别是 MySQL 的用户名与密码，mysql的初始管理帐号是root，没有密码，注意：这个root用户不是Linux的系统用户。MySQL默认用户是root，由于初始没有密码，第一次进时只需键入mysql即可。 
[root@test1 local]# mysql 
Welcome to the MySQL monitor.　Commands end with ; or /g. 
Your MySQL connection id is 1 to server version: 4.0.16-standard 
Type 'help;' or '/h' for help. Type '/c' to clear the buffer. 
mysql> 
出现了“mysql>”提示符，恭喜你，安装成功！ 
增加了密码后的登录格式如下： 
mysql -u root -p 
Enter password: (输入密码) 
其中-u后跟的是用户名，-p要求输入密码，回车后在输入密码处输入密码。
注意：这个mysql文件在/usr/bin目录下，与后面讲的启动文件/etc/init.d/mysql不是一个文件。
四、MySQL的几个重要目录
MySQL 安装完成后不象SQL Server默认安装在一个目录，它的数据库文件、配置文件和命令文件分别在不同的目录，了解这些目录非常重要，尤其对于Linux的初学者，因为 Linux本身的目录结构就比较复杂，如果搞不清楚MySQL的安装目录那就无从谈起深入学习。
下面就介绍一下这几个目录。
1、数据库目录 
/var/lib/mysql/
2、配置文件 
/usr/share/mysql（mysql.server命令及配置文件）
3、相关命令 
/usr/bin(mysqladmin mysqldump等命令)
4、启动脚本 
/etc/rc.d/init.d/（启动脚本文件mysql的目录） 
五、修改登录密码
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
ERROR 1045: Access denied for user: ['root@localhost'](mailto:%20root@localhost) (Using password: NO) 
显示错误，说明密码已经修改。 
2）用修改后的密码登录 
[root@test1 local]# mysql -u root -p 
Enter password: (输入修改后的密码123456) 
Welcome to the MySQL monitor.　Commands end with ; or /g. 
Your MySQL connection id is 4 to server version: 4.0.16-standard 
Type 'help;' or '/h' for help. Type '/c' to clear the buffer. 
mysql> 
成功！ 
这是通过mysqladmin命令修改口令，也可通过修改库来更改口令。
六、启动与停止
1、启动 
MySQL安装完成后启动文件mysql在/etc/init.d目录下，在需要启动时运行下面命令即可。 
[root@test1 init.d]# /etc/init.d/mysql start
2、停止 
/usr/bin/mysqladmin -u root -p shutdown
3、自动启动 
1）察看mysql是否在自动启动列表中 
[root@test1 local]#　/sbin/chkconfig --list 
2）把MySQL添加到你系统的启动服务组里面去 
[root@test1 local]#　/sbin/chkconfig　–- add　mysql 
3）把MySQL从启动服务组里面删除。 
[root@test1 local]#　/sbin/chkconfig　–-del　mysql 
七、更改MySQL目录
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
为保证MySQL能够正常工作，需要指明mysql.sock文件的产生位置。修改socket=/var/lib/mysql/mysql.sock一行中等号右边的值为：/home/mysql/mysql.sock 。操作如下： 
vi　 my.cnf　　　 (用vi工具编辑my.cnf文件，找到下列数据修改之) 
# The MySQL server 
[mysqld] 
port　　　= 3306 
#socket　 = /var/lib/mysql/mysql.sock（原内容，为了更稳妥用“#”注释此行） 
socket　 = /home/data/mysql/mysql.sock　　　（加上此行）
6、修改MySQL启动脚本/etc/rc.d/init.d/mysql 
最后，需要修改MySQL启动脚本/etc/rc.d/init.d/mysql，把其中datadir=/var/lib/mysql一行中，等号右边的路径改成你现在的实际存放路径：home/data/mysql。 
[root@test1 etc]# vi　/etc/rc.d/init.d/mysql 
#datadir=/var/lib/mysql　　　　（注释此行） 
datadir=/home/data/mysql　　 （加上此行）
7、重新启动MySQL服务 
/etc/rc.d/init.d/mysql　start 
或用reboot命令重启Linux 
如果工作正常移动就成功了，否则对照前面的7步再检查一下。
八、MySQL的常用操作
注意：MySQL中每个命令后都要以分号；结尾。
1、显示数据库 
mysql> show databases; 
+----------+ 
| Database | 
+----------+ 
| mysql　　| 
| test　　 | 
+----------+ 
2 rows in set (0.04 sec) 
Mysql刚安装完有两个数据库：mysql和test。mysql库非常重要，它里面有MySQL的系统信息，我们改密码和新增用户，实际上就是用这个库中的相关表进行操作。
2、显示数据库中的表 
mysql> use mysql; （打开库，对每个库进行操作就要打开此库，类似于foxpro ） 
Database changed
mysql> show tables; 
+-----------------+ 
| Tables_in_mysql | 
+-----------------+ 
| columns_priv　　| 
| db　　　　　　　| 
| func　　　　　　| 
| host　　　　　　| 
| tables_priv　　 | 
| user　　　　　　| 
+-----------------+ 
6 rows in set (0.01 sec)
3、显示数据表的结构： 
describe 表名;
4、显示表中的记录： 
select * from 表名; 
例如：显示mysql库中user表中的纪录。所有能对MySQL用户操作的用户都在此表中。 
Select * from user;
5、建库： 
create database 库名; 
例如：创建一个名字位aaa的库 
mysql> create databases aaa; 
6、建表： 
use 库名； 
create table 表名 (字段设定列表)； 
例如：在刚创建的aaa库中建立表name,表中有id(序号，自动增长)，xm（姓名）,xb（性别）,csny（出身年月）四个字段 
use aaa; 
mysql> create table name (id int(3) auto_increment not null primary key, xm char(8),xb char(2),csny date); 
可以用describe命令察看刚建立的表结构。 
mysql> describe name;
+-------+---------+------+-----+---------+----------------+ 
| Field | Type　　| Null | Key | Default | Extra　　　　　| 
+-------+---------+------+-----+---------+----------------+ 
| id　　| int(3)　|　　　| PRI | NULL　　| auto_increment | 
| xm　　| char(8) | YES　|　　 | NULL　　|　　　　　　　　| 
| xb　　| char(2) | YES　|　　 | NULL　　|　　　　　　　　| 
| csny　| date　　| YES　|　　 | NULL　　|　　　　　　　　| 
+-------+---------+------+-----+---------+----------------+
7、增加记录 
例如：增加几条相关纪录。 
mysql> insert into name values('','张三','男','1971-10-01'); 
mysql> insert into name values('','白云','女','1972-05-20'); 
可用select命令来验证结果。 
mysql> select * from name; 
+----+------+------+------------+ 
| id | xm　 | xb　 | csny　　　 | 
+----+------+------+------------+ 
|　1 | 张三 | 男　 | 1971-10-01 | 
|　2 | 白云 | 女　 | 1972-05-20 | 
+----+------+------+------------+
8、修改纪录 
例如：将张三的出生年月改为1971-01-10 
mysql> update name set csny='1971-01-10' where xm='张三';
9、删除纪录 
例如：删除张三的纪录。 
mysql> delete from name where xm='张三';
10、删库和删表 
drop database 库名; 
drop table 表名；
九、增加MySQL用户
格式：grant select on 数据库.* to 用户名@登录主机 identified by "密码" 
例1、增加一个用户user_1密码为123，让他可以在任何主机上登录，并对所有数据库有查询、插入、修改、删除的权限。首先用以root用户连入MySQL，然后键入以下命令：
mysql> grant select,insert,update,delete on *.* to [user_1@"%](mailto:user_1@%22%25) " Identified by "123"; 
例1增加的用户是十分危险的，如果知道了user_1的密码，那么他就可以在网上的任何一台电脑上登录你的MySQL数据库并对你的数据为所欲为了，解决办法见例2。
例 2、增加一个用户user_2密码为123,让此用户只可以在localhost上登录，并可以对数据库aaa进行查询、插入、修改、删除的操作（localhost指本地主机，即MySQL数据库所在的那台主机），这样用户即使用知道user_2的密码，他也无法从网上直接访问数据库，只能通过 MYSQL主机来操作aaa库。
mysql>grant select,insert,update,delete on aaa.* to [user_2@localhost](mailto:user_2@localhost) identified by "123";
用新增的用户如果登录不了MySQL，在登录时用如下命令：
mysql -u user_1 -p　-h 192.168.113.50　（-h后跟的是要登录主机的ip地址）
这里特别提示（使用mysql front链接时）
---开一个可以在远程任意妄为的用户：
grant all privileges on *.* to user_name@'%' identified by 'passwd ' 
--开一个本地的用户：
grant all privileges on *.* to user_name@'localhost ' identified by 'passwd '
# mysql 1130連接錯誤的解決方法
在使用MySQL-Front连接mysql的时候发生的这个错误
ERROR 1130: Host 192.168.88.160 is not allowed to connect to this MySQL server
1。 改表法。可能是你的帐号不允许从远程登陆，只能在localhost。这个时候只要在localhost的那台**电脑**，登入mysql后，更改 “mysql” 数据库里的 “user” 表里的 “host” 项，从”localhost”改称”%”
mysql -u root -p
mysql>use mysql;
mysql>update user set host = ‘%’  where user =’root’;
mysql>flush privileges;
mysql>select ‘host’,'user’ from user where user=’root’;
现在就可以连接了！
2. 授权法。例如，你想myuser使用mypassword从任何主机连接到mysql服务器的话。
GRANT ALL PRIVILEGES ON *.* TO[‘myuser’@'%’](mailto:%E2%80%98myuser%E2%80%99@%27%25%E2%80%99) IDENTIFIED BY ‘mypassword’ WITH GRANT OPTION;
如果你想允许用户myuser从ip为192.168.1.3的主机连接到mysql服务器，并使用mypassword作为密码
GRANT ALL PRIVILEGES ON *.* TO[‘myuser’@'192.168.1.3′](mailto:%E2%80%98myuser%E2%80%99@%27192.168.1.3%E2%80%B2) IDENTIFIED BY ‘mypassword’ WITH

十、备份与恢复
1、备份
例如：将上例创建的aaa库备份到文件back_aaa中
[root@test1 root]# cd　/home/data/mysql　(进入到库目录，本例库已由val/lib/mysql转到/home/data/mysql，见上述第七部分内容) 
[root@test1 mysql]# mysqldump -u root -p --opt aaa > back_aaa
2、恢复
[root@test mysql]# mysql -u root -p ccc < back_aaa
特别提醒：
安装后可能会遇到：Manager of pid-file quit without updating file.
这是因为
              1.原有的数据文件没有删除的
              2.将配置文件的
并则执行下面的命令： 
shell> rm -rf /var/lib/mysql 
然后重新执行下面步骤： 
shell> cd mysql 
shell> scripts/mysql_install_db --user=mysql 
shell> chown -R root . 
shell> chown -R mysql data 
shell> chgrp -R mysql . 
shell> cp support-files/mysql.server /etc/init.d/mysql 
shell> cp support-files/my-medium.cnf /etc/my.cnf 
shell> /etc/init.d/mysql start
Can't connect to local MySQL server through socket '/var/lib/mysql/mysql.soc ： 
1.这是没有配置好my.cnf文件。
2.没有启动mysql服务。
引用:http://www.javaeye.com/topic/369148
