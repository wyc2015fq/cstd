# mysql修改初始密码.... - nosmatch的专栏 - CSDN博客
2011年07月21日 17:12:54[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：10159
                 在向大家详细介绍Linux mysql之前，首先让大家了解下Linux mysql，然后全面介绍Linux mysql，希望对大家有用。
1. Linux mysql安装：
$ yum install mysql-server
2. Linux mysql修改root密码：
$ mysqladmin -u root password your_new_passwd
3. 启动Linux mysql服务
$ /etc/init.d/mysqld start
4. 添加为系统服务并随之自动启动：
$ chkconfig --level 2345 mysqld on
5. 修改防火墙，开启3306端口，使得可以进行远程访问：
System->Administration->Firewall->Other ports，添加3306端口，一定要把tcp和udp都加上！
其本质是修改了/etc/sysconfig/iptables文件，也可直接进行修改，按照如下格式：
-A INPUT -m state --state NEW -m tcp -p tcp --dport 3306 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p udp --dport 3306 -j ACCEPT
6. mysql默认的root用户是不能远程访问的，需要为Linux mysql中添加一个可以远程访问的用户，首先以root用户登录Linux mysql，然后增加一个用户：
GRANT ALL ON *.* TO your_username@'your_host_name_or_ip_address' IDENTIFIED BY 'your_password'
这里的ALL表示，所有任何权限（包括增删改等），*.*表示任何数据库中的任何表，也可指定成Linux mysql中的某个数据库甚至某个表，该用户登录后只能对该数据库进行刚刚赋予的操作。your_host_name_or_ip_address是指规定你只能在通过该ip地址远程访问，如果指定任意地址均可访问，则可用通配符%代替。
比如：GRANT insert ON test.* IDENTIFIED BY 'test'它的含义是，用户可在任意IP地址通过用户名test，密码test登录，登录后，只能对test数据库进行insert操作。
其本质是在Linux mysql中默认包含了一个Linux mysql的数据库，其中有一个user表，上述GRANT命令其实就是往这个表中添加了一行记录。也可以直接修改这个表达到同样的效果，不过较为麻烦。注意添加密码，可以使用PASSWORD()函数。
7. 远程登录，以在linux下为例（window下可用Mysql GUI Tools):
$ mysql -u test -h 192.168.1.111 -p#然后输入密码即可
8. 解决数据库表插入中文显示乱码问题：
1)修改/etc/my.cnf文件，找到[mysqld]，在后面加入以下行：
default-character-set = utf8
同时新建一项，名为[client]，在之后插入同上的语句。重启Linux mysql服务。
2) 对于创建数据库的sql脚本，需要加入以下语句：
drop database if exists TEST;
create database TEST default character set utf8;
这样，Linux mysql默认的字符集被设为UTF-8,所创建表的字符集也变为UTF-8，并且客户端也将以UTF-8显示。
简介Linux MySQL重要目录与登录密码
特别值得一提的是Linux MySQL有很多值得学习的地方，这里我们主要介绍Linux MySQL，包括介绍Linux MySQL各种等方面。Linux MySQL安装完成后不象SQL Server默认安装在一个目录，它的数据库文件、配置文件和命令文件分别在不同的目录，了解这些目录非常重要，尤其对于Linux的初学者，因为 Linux本身的目录结构就比较复杂，如果搞不清楚Linux MySQL的安装目录那就无从谈起深入学习。
下面就介绍一下这几个目录。
Linux MySQL的几个重要目录
1、数据库目录
/var/lib/mysql/
2、配置文件
/usr/share /mysql（mysql.server命令及配置文件）
3、相关命令
/usr/bin（mysqladmin mysqldump等命令）
4、启动脚本
/etc/rc.d/init.d/（启动脚本文件mysql的目录）
Linux MySQL修改登录密码
Linux MySQL默认没有密码，安装完毕增加密码的重要性是不言而喻的。
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
Welcome to the MySQL monitor. Commands end with ; or \g.
Your MySQL connection id is 4 to server version: 4.0.16-standard
Type 'help;' or '\h' for help. Type '\c' to clear the buffer.
mysql>
成功！ 这是通过mysqladmin命令修改口令，也可通过修改库来更改口令。
Linux MySQL启动与停止
1、启动
MySQL安装完成后启动文件mysql在/etc/init.d目录下，在需要启动时运行下面命令即可。
[root@test1 init.d]# /etc/init.d/mysql start 另外安装程序把MySQL安装成了service，所以在任何目录下都可以用：# service mysql start启动Linux MySQL。
2、停止
/usr/bin/mysqladmin -u root -p shutdown 或者：# service mysql stop
3、自动启动
1）察看mysql是否在自动启动列表中
[root@test1 local]# /sbin/chkconfig –list
2）把Linux MySQL添加到你系统的启动服务组里面去
[root@test1 local]# /sbin/chkconfig – add mysql
3）把Linux MySQL从启动服务组里面删除。
[root@test1 local]# /sbin/chkconfig – del mysql
