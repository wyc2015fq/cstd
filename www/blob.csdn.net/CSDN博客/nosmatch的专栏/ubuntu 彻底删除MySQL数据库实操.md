# ubuntu 彻底删除MySQL数据库实操 - nosmatch的专栏 - CSDN博客
2011年09月28日 21:14:10[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：2838
以下的文章主要介绍的是ubuntu 彻底删除[MySQL](http://database.51cto.com/art/200510/8400.htm)数据库，以及删除MySQL数据库之后对MySQL的重装与删除MySQL的实际操作步骤，如果你对其实际操作感兴趣的话，你就可以浏览以下的文章了。
- sudo apt-get autoremove --purge MySQL-server-5.0  
- sudo apt-get remove MySQL-server  
- sudo apt-get autoremove MySQL-server  
- sudo apt-get remove MySQL-common  
这个很重要上面的其实有一些是多余的。
清理残留数据
- dpkg -l |grep ^rc|awk '{print $2}' |sudo xargs dpkg -P 
安装 MySQL数据库
- sudo apt-get install MySQL-serversudo apt-get install MySQL-clientsudo apt-get install php5-MySQL
安装php5-MySQL数据库 是将php和MySQL连接起来一旦安装完成，MySQL 服务器应该自动启动。您可以在终端提示符后运行以下命令来检查 MySQL 服务器是否正在运行：sudo netstat -tap | grep MySQL
当您运行该命令时，您可以看到类似下面的行：
- tcp 0 0 localhost.localdomain:MySQL *:* LISTEN -  
如果服务器不能正常运行，您可以通过下列命令启动它：
- sudo /etc/init.d/MySQL restart  
进入MySQL
$MySQL -uroot -p 管理员密码
配置 MySQL数据库 的管理员密码：
- sudo MySQLadmin -u root password newpassword 
以上的相关内容就是对ubuntu 彻底删除 MySQL数据库 然后重装 MySQL删除 MySQL的介绍，望你能有所收获。
下面时彻底删除apache2的命令：
1. 删除apache
代码:
$ sudo apt-get --purge remove apache-common
$ sudo apt-get --purge remove apache
2. 如果你安装了apache2确用不了，也删除了。
我是用新立得软件包管理器删除的（当然用apt-get --purge remove命令也可以）。打开新立得后，搜索apache，我是找到apache2，apache2.2-common， apache2-mpm-work，apache2-utils，libapr1，然后右键选择“标记以便彻底删除”。删除他们。
3. （关键一步）找到没有删除掉的配置文件，一并删除
代码:
$ sudo find /etc -name "*apache*" -exec rm -rf {} \;
$ sudo rm -rf /var/www
