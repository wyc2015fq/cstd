# 环境安装系列 - Linux下 MYSQL安装和基本管理 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月18日 16:11:35[initphp](https://me.csdn.net/initphp)阅读数：1125








可以去这个网址下载mysql包：http://dev.mysql.com/downloads/file.php?id=454649




- 安装linux一些必须的包：
`yum install gcc gcc-c++ gcc-g77 autoconf automake zlib* fiex* libxml* ncurses-devel libmcrypt* libtool-ltdl-devel* libevent`- 安装mysql，mysql安装包需要自己下载，安装目录 /usr/local/

```
sudo tar xzvf mysql-5.5.41.tar.gz
cd mysql-5.5.41 
./configure --prefix=/usr/local/mysql.5.5.41 --enable-thread-safe-client --enable-assembler --with-big-tables --with-client-ldflags=-all-static --with-mysqld-ldflags=-all-static --with-charset=utf8 --with-collation=utf8_general_ci --with-extra-charsets=complex 
make 
make install
```
- 创建mysql用户组等信息

```
sudo  groupadd mysql 
sudo  useradd -g mysql -s /sbin/nologin mysql 
sudo ./scripts/mysql_install_db --user=mysql --datadir=/usr/local/mysql.5.5.41/var 
sudo  chown -R root /usr/local/mysql.5.5.41 
sudo  chown -R mysql:mysql /usr/local/mysql.5.5.41/var
```
- 管理MYSQL

```
#启动mysql：
sudo  /usr/local/mysql.5.5.41/share/mysql/mysql.server  start
#重启mysql
sudo  /usr/local/mysql.5.5.41/share/mysql/mysql.server  restart
#停止mysql
sudo  /usr/local/mysql.5.5.41/share/mysql/mysql.server  stop
```









