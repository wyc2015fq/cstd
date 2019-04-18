# ubuntu 15.04 编译安装 mysql 5.5.44 - writeeee的专栏 - CSDN博客
2015年12月26日 14:23:45[writeeee](https://me.csdn.net/writeeee)阅读数：1172
1.安装Mysql
（1）安装编译源码需要的包
sudo apt-get install make cmake gcc g++ bison libncurses5-dev
（2）下载并解压缩
wget [http://cdn.mysql.com/Downloads/MySQL-5.6/mysql-5.6.17.tar.gz](http://cdn.mysql.com/Downloads/MySQL-5.6/mysql-5.6.17.tar.gz)
tar -zxvf mysql-5.6.17.tar.gz
cd mysql-5.6.17
（3）编译安装
编译配置：
cmake . -DCMAKE_INSTALL_PREFIX=/usr/local/mysql -DMYSQL_DATADIR=/usr/local/mysql/data -DSYSCONFDIR=/etc -DWITH_INNOBASE_STORAGE_ENGINE=1
 -DWITH_ARCHIVE_STORAGE_ENGINE=1 -DWITH_BLACKHOLE_STORAGE_ENGINE=1 -DWITH_PARTITION_STORAGE_ENGINE=1 -DWITH_PERFSCHEMA_STORAGE_ENGINE=1 -DWITHOUT_EXAMPLE_STORAGE_ENGINE=1 -DWITHOUT_FEDERATED_STORAGE_ENGINE=1 -DDEFAULT_CHARSET=utf8 -DDEFAULT_COLLATION=utf8_general_ci
 -DWITH_EXTRA_CHARSETS=all -DENABLED_LOCAL_INFILE=1 -DWITH_READLINE=1 -DMYSQL_UNIX_ADDR=/usr/local/mysql/mysql.sock -DMYSQL_TCP_PORT=3306 -DMYSQL_USER=mysql -DCOMPILATION_COMMENT="lq-edition"-DENABLE_DTRACE=1 -DOPTIMIZER_TRACE=1 -DWITH_DEBUG=1
编译：
make
安装：
sudo make install
2.配置MySQL
（1）新建运行Mysql的用户和组
sudo groupadd mysql
sudo useradd -g mysql mysql
（2）设置Mysql安装目录的权限
cd /usr/local/mysql
sudo chown -R mysql:mysql ./
（3）建立配置文件
cp support-files/my-default.cnf /etc/my.cnf
sudo chown mysql:mysql /etc/my.cnf
修改配置文件：
sudo vi /etc/my.cnf
[client]
port = 3306
socket = /usr/local/mysql/data/mysql.sock
[mysqld]
port = 3306
socket = /usr/local/mysql/data/mysql.sock
basedir = /usr/local/mysql
datadir  = /usr/local/mysql/data
（4）初始化数据库
sudo scripts/mysql_install_db --user=mysql --basedir=/usr/local/mysql --datadir=/usr/local/mysql/data/
（5）启动mysql服务
方法1：
直接启动
bin/mysqld_safe &
检查MySQL服务是否启动：
ps -ef |grep mysql
方法2：
通过服务启动
添加服务，复制启动脚本到/etc/init.d目录：
cp support-files/mysql.server /etc/init.d/mysql
设置文本的权限：
sudo chmod 755 /etc/init.d/mysql
启动：
sudo /etc/init.d/mysql start
(关闭mysql服务：sudo /etc/init.d/mysql stop)
或者
sudo service mysql start
(关闭mysql服务：sudo service mysql stop)
检查MySQL服务是否启动：
ps -ef |grep mysql
（6）配置环境变量
为了直接调用mysql，需要将mysql的bin目录加入PATH环境变量。
编辑/etc/profile文件：
sudo vim /etc/profile
在文件最后 添加如下两行：
PATH=$PATH:/usr/local/mysql/bin
export PATH
关闭文件，运行下面的命令，让配置立即生效： 
source /etc/profile
（7）修改root密码（因为默认密码为空）
mysql -h127.0.0.1 -uroot -p
update mysql.user set password=password("mysqldba") where user='root';
flush privileges;
测试一下：
select now(),user(),version();
show databases;
