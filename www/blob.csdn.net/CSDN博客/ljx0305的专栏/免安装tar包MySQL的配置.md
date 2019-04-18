# 免安装tar包MySQL的配置 - ljx0305的专栏 - CSDN博客
2009年07月22日 21:15:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：670标签：[mysql																[perl																[数据库																[include																[user																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=perl&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[MySql](https://blog.csdn.net/ljx0305/article/category/570123)
# 免安装tar包MySQL的配置
这两天安装MYSQL遇到很多问题，找了很多材料，把总结的东西写出来看看
1、首先选择合适的 mysql 版本。 去 [http://www.mysql.com](http://www.mysql.com/)
下最新的版本 mysql
同时一个版本分有不同的分发形式，有的以 rpm 包，有的以 tar 包，而有的以源代码的方式发布。rpm包安装方便，但是不够灵活；源代码很灵活却需要编译，所以我下载了 tar 包来安装，因为它做了优化编译。 一般tar包直接解压就可以使用了，但是想启动和停止方便还需要修改一些链接这里就都写出来咯！
2、安装mysql。 
rpm包的安装不在赘述，源代码和tar包最好安装在 /usr/local/mysql 中，其方法是源代码 configure 时候加入 ――prefix=/usr/loal/mysql，而 tar 包直接解压到 /usr/local/mysql 即可。 
3、安装后配置。 
在 mysql 的目录里(安装后)找到 /bin/mysql_install_db 并运行，tar 包在 mysql 中的 脚本s 目录下，这样数据库就安装了： 
$ cd /usr/local/mysql 
$ ./bin/mysql_install_db （初始化数据库）
添加组 mysql 和用户 mysql，把其初始目录设置到 mysql 的安装目录里，并将 mysql 设置成 mysql 文件夹的所有者，这样可以防止不能连接 MySql 服务器错误： 
$ useradd -M -o -r -d /usr/local/mysql -s /bin/bash -c "mysql user" -u 27 mysql 
$ cd /usr/local/ 
$ chmod 750 mysql -R 
$ chgrp mysql mysql -R 
$ chown mysql mysql -R 
执行 mysql 目录里 /bin/mysql_safe，tar包有个 shell 文件叫 safe_mysql 也可以，这样 mysql 的守护进程就开始运行： 
$ /usr/local/mysql/bin/safe_mysqld & 
查看网络的3306端口情况，来判断 mysql 是否正常的运行了： 
$ netstat -atln 
如果没有正常运行，到 data 目录下查看日志文件查找错误。 
为了方便期间，我们建立几个链接文件到 /sbin中，以免每次要到 mysql 中运行程序： 
$ ln -s /usr/local/mysql/bin/mysql /sbin/mysql 
$ ln -s /usr/local/mysql/bin/mysqladmin /sbin/mysqladmin 
将守护进程脚本复制到初始化目录中，并加入到自动启动列表中： 
$ cd mysql  
$ cp support-files/mysql.server /etc/rc.d/init.d/mysqld 
$修改mysqld中的 pid_file的目录！当然，目录随意最好是和已经启动的PID目录一致
$ chkconfig ――add mysqld 
$ chkconfig ――level 345 mysqld on 
验证上述操作的正确性，运行： 
$ /etc/rc.d/init.d/mysqld restart 
或者: 
$ service mysqld restart 
4、mysql的安全设置 
运行下面指令，清除空密码帐户： 
$ mysqladmin -uroot password "youpassword" #设置root帐户的密码 
$ mysql -uroot -p 
mysql>use mysql; 
mysql>delete from user where password=""; #删除用于本机匿名连接的空密码帐号 
mysql>flush privileges; 
mysql>quit 
最后来个测试，不过先切换到 mysql 下 msql-benmak 目录： 
$  perl run-all-tests ――user=root ――password="new" 
为了写程序访问 mysql 数据库，应在 /usr/include 和 /usr/lib 中建立链接到 mysql 的 include 和 lib 目录中，这样只要在程序中加入 #include 就可以引用 mysql 的头文件。 
ln -s /usr/local/mysql/lib/mysql /usr/lib/mysql 
ln -s /usr/local/mysql/include/mysql /usr/include/mysql 
其中的路径及文件、文件夹名根据你的情况进行相应修改，例如我用的 tar 包中 include 目录下没有 mysql 这个文件夹，只有很多的头文件，所以我的是这样写的： 
ln -s /usr/local/mysql/lib /usr/lib/mysql 
ln -s /usr/local/mysql/include /usr/include/mysql 
但无论如何只要它的头文件能正常的引用就可以了。 
有的是可以灵活变通的，不考虑其他的因素的时候，你的mysql完全可以安装到root目录中，但是一般大家都放到 /usr/local/mysql 这个目录中。 
4、perl模块的安装 
很多的网页还是用 perl，为了能用 perl 语言访问 mysql，同时能运行 mysql 附带的测试程序，我安装了一些 perl 的模块。 
对此，说法不一，其实你不安装业无可厚非，有的用 php 作 web 就不需要了，像我只是写应用程序访问 mysql 也是不需要安装的，但是我安装了，只是为了能测试 mysql。 
我安装的模块有： 
Data-Dumper-2.121 
Data-ShowTable-3.3 
DBD-mysql-2.9003 
DBI-1.42 
Msql-Mysql-modules-1.2219 
Net-MySQL-0.08 
mysql 手册推荐 Data-Dumper + Data-ShowTable + DBI + Msql-Mysql-modules，这样DBI就安装成功了。但是 mysql 网站上推荐的 DBI 模块中只有 DBI + Net-MySQL + DBD-mysql 三个模块，我对 perl 不熟悉，也不知道这样的 pm 文件到底用处是什么，但是有一点是肯定的，安装这样模块是没有任何的坏处的。 
安装过程中 Data-ShowTable 724行代码出现错误，明显的少了两个 ">"，DBD-mysql编译也出现错误，原因是路径不对，没有找到文件 mysql-config ，把你的路径加入进去编译就可以。 
其他的编译都按照 readme 上去做就可以了。 
5、管理工具 
安装完毕后，你就可以在数据库中加入自己的东西了。 
还用命令行？反正我是头大的，我觉得只要你懂就可以了，至于实际的应用就可以用一些 GUI 好点的工具，毕竟程序员的天职就是让计算机更贴近人。要不他们干嘛的？ 
至于mysql管理工具我推荐mysqlcc，十分好用，虽然有些地方用起来不舒服，不过有兴趣的话你自己改一下也可以，毕竟这是开源的优势嘛。 
在 /usr/bin 中建立一个软连接： 
ln -s /usr/local/mysqlcc/mysqlcc /usr/bin/mysqlcc 
那么，Use it anywhere！ 
6、添加远程用户 
为了能让远程的用户访问数据库，还要添加远程用户。 
$ mysql -p 
Enter password:***** 
mysql>use mysql 
mysql>grant all privileges on *.* to test@"192.168.0.%" identified by "test" with grant option; 
mysql>quit; 
这样就建立了一个超级用户 test，可以在本地局域网的任何地方连接，权限很大但密码很弱智，我们不想这样，但是此时，用户的权限已经开始生效了。 
如果你手工修改授权表，要： 
mysql>FLUSH PRIVILEGES; 
或者 
$ mysqladmin flush-privileges -p 
此外，全局权限的改变和口令改变在下一次客户连接时生效，表和列权限在客户的下一次请求时生效，数据库权限改变在下一个 USE db_name 命令生效。 
用 mysqlcc 修改 test 用户的权限，由于我不想让这个用户破坏我的数据，所以在 user 表中。
引用:http://www.linuxdiyf.com/bbs/thread-107248-1-1.html
