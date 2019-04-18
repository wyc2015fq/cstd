# Centos环境下mysql源码编译安装 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年08月15日 10:52:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：244













yum -y install gcc gcc-c++ ncurses-devel perl





##从mysql5.5起，mysql源码安装开始使用cmake了，设置源码编译配置脚本。

wget --no-check-certificate [https://cmake.org/files/v2.8/cmake-2.8.12.tar.gz](https://cmake.org/files/v2.8/cmake-2.8.12.tar.gz)

tar -zxvf cmake-2.8.12.tar.gz

cd cmake-2.8.12

./bootstrap  && make && make install






mkdir -p /usr/local/mysql





tar -zxvf mysql-5.6.37.tar.gz

cd mysql-5.6.37

/usr/local/bin/cmake . -DCMAKE_INSTALL_PREFIX=/usr/local/mysql/ -DMYSQL_DATADIR=/data2/mysqldb -DWITH_INNOBASE_STORAGE_ENGINE=1
 -DMYSQL_TCP_PORT=3308 -DMYSQL_UNIX_ADDR=/usr/local/mysql/data/mysql.sock -DMYSQL_USER=mysql -DWITH_DEBUG=0

rm CMakeCache.txt   #试验删不删除的影响，试过没删除也没影响

make

make install






mkdir -p /data2/mysqldb

chown -R mysql:mysql /usr/local/mysql

chown -R mysql:mysql /data2/mysqldb






cp /usr/local/mysql/support-files/my-default.cnf /etc/my.cnf

chown mysql:mysql /etc/my.cnf

##my.cnf重新配置参数，编译阶段的参数没写到这个配置文件去。

#文件配置

vim /etc/my.cnf

basedir =/usr/local/mysql

datadir =/data2/mysqldb

port =3308

# server_id = .....

socket =/usr/local/mysql/data/mysql.sock




[mysqld_safe]

log-error=/usr/local/mysql/log/mysqld.log

pid-file=/usr/local/mysql/run/mysqld/mysqld.pid




default-storage-engine=MyISAM

server-id       = 1

innodb_buffer_pool_size = 21M

innodb_additional_mem_pool_size = 2M

innodb_log_file_size = 5M

innodb_log_buffer_size = 8M

innodb_flush_log_at_trx_commit = 1

innodb_lock_wait_timeout = 50






#Because the MySQL server, mysqld, must access the data directory when it runs later, you should either run mysql_install_db from the same system account
 that will be used for running mysqld

/usr/local/mysql/scripts/mysql_install_db --user=mysql --basedir=/usr/local/mysql --datadir=/data2/mysqldb




##指定配置文件

##/usr/local/mysql/scripts/mysql_install_db --user=mysql --basedir=/usr/local/mysql --datadir=/data2/mysqldb --defaults-file=/usr/local/mysql/my.cnf






mkdir -p /usr/local/mysql/log

chown mysql:mysql /usr/local/mysql/log/

mkdir -p /usr/local/mysql/run/mysqld/

chown -R mysql:mysql /usr/local/mysql/run/mysqld/

touch /usr/local/mysql/log/mysqld.log

chown mysql:mysql /usr/local/mysql/log/mysqld.log









#To start a second server, provide different --socket and --port option values, and pass a --datadir=dir_name option to mysqld_safe so that the server uses a different data directory.

mysqld_safe --socket=file_name --port=port_number --defaults-file=/usr/local/mysql/my.cnf










#安全模式启动

/usr/local/mysql/bin/mysqld_safe --socket=/usr/local/mysql/data/mysql.sock --port=3308 --defaults-file=/usr/local/mysql/my.cnf






#开启mysql服务

/usr/local/mysql/support-files/mysql.server start







#密码为空，如果能登陆上，则安装成功

/usr/local/mysql/bin/mysql -u root -p




或自定义带sock文件

/usr/local/mysql/bin/mysql -uroot -p -S /usr/local/mysql/data/mysql.sock





后续步骤：


cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysqld-lastest   

vim /etc/profile   

*MYSQL_PATH=/usr/local/mysql/bin:/usr/local/mysql/lib*

* export MYSQL_PATH*




source /etc/profile

##添加到系统运行级别

chkconfig --level 35 mysqld on

##查看端口占用

netstat -antlp | grep 3308




#修改密码

mysqladmin -u root password '123456'








/usr/local/mysql/bin/mysqld_safe --skip-grant-tables




问题：   

ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/tmp/mysql.sock' (2)   

解决：   

新建一个链接或在mysql中加入-S参数，直接指出mysql.sock位置。   

ln -s /usr/local/mysql/data/mysql.sock /tmp/mysql.sock   

/usr/local/mysql/bin/mysql -u root -S /usr/local/mysql/data/mysql.sock







![](https://img-blog.csdn.net/20170815105911437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




