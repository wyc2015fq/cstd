# Ubuntu下mysql配置 - gauss的专栏 - CSDN博客
2015年04月16日 13:49:36[gauss](https://me.csdn.net/mathlmx)阅读数：244
个人分类：[数据库](https://blog.csdn.net/mathlmx/article/category/2641909)

在ubuntu下面安装ubuntu最简便的方法是:
*** sudo apt-get install mysql-server              sudo apt-get install mysql-client***
apt-get的安装文件缓存在/var/cache/apt/achrive中，可备份；
mysql安装成功后，默认的安装地址在/usr/local/mysql中，而配置文件已经默认的移动到/etc/mysql/my.cnf
mysql默认的root账号是没有密码的，并且默认不被远程访问；
1）开启远程访问
修改my.cnf文件，将
***  bind-address=127.0.0.1***
修改为本机的网络地址，如：
***bind-address=192.168.0.120***
这个时候如果启动/usr/local/mysql/bin/mysqld_safe --user=mysql         (mysql用户和mysql组是安装程序自动建立的，如果没有，则需要手动建立)，会显示：
***warning:world-writable config file /etc/mysql/my.cnf is ignored***
必须手动修改权限：
*** sudo chown mysql:mysql /etc/mysql/my.cnf***
***              sudo 644 /etc/mysql/my.cnf***
然后正常启动mysql，启动mysql以后，在本地利用root登录：
*** mysql>grant all on *.* to ***[***new_user_name@'%'***](mailto:new_user_name@' rel=)*** identified by 'passwd'***
如：
***mysql>grant all on *.* to ***[***corey@'%'***](mailto:corey@' rel=)*** identified by '111111111'***
***            mysql>grant all on *.* to ***[***corey@'192.168.0.100'***](mailto:corey@' rel=)*** identified by '111111111'***
然后在远程机器的客户端可正常访问；
