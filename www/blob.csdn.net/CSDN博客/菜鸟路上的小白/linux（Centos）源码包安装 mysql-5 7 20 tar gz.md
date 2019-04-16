# linux（Centos）源码包安装 mysql-5.7.20.tar.gz - 菜鸟路上的小白 - CSDN博客





2018年03月16日 10:54:35[somnus_小凯](https://me.csdn.net/u012486840)阅读数：926










1.依赖包安装:

yum -y install gcc gcc-c++ cmake ncurses-devel autoconf perl perl-devel

2.创建mysql安装目录以及数据存放目录

mkdir -p /usr/local/mysql/data

3.创建用户、用户组

groupadd mysql
 useradd -r -g mysql mysql

4.开始安装

tar -zxvf mysql-5.7.20.tar.gz


进入 cd mysql-5.7.20

# cmake . -DCMAKE_INSTALL_PREFIX=/usr/local/mysql \
-DINSTALL_DATADIR=/usr/local/mysql/data \
-DDEFAULT_CHARSET=utf8 \
-DDEFAULT_COLLATION=utf8_general_ci \
-DEXTRA_CHARSETS=all \


-DENABLED_LOCAL_INFILE=1

注意：


- CMake Error at cmake/boost.cmake:81 (MESSAGE):
- You can download it with -DDOWNLOAD_BOOST=1 -DWITH_BOOST=
- 
- 
- This CMake script will look for boost in . If it is not there,
- it will download and unpack it (in that directory) for you.
- 
- 
- If you are inside a firewall, you may need to use an http proxy:
- 
- 
- export http_proxy=http://example.com:80
- 
- 
- Call Stack (most recent call first):
- cmake/boost.cmake:238 (COULD_NOT_FIND_BOOST)
- CMakeLists.txt:455 (INCLUDE)
- 
- 
- 
- 
- -- Configuring incomplete, errors occurred!
- See also "/root/mysqlsoftware/mysql-5.7.20/CMakeFiles/CMakeOutput.log".
- See also "/root/mysqlsoftware/mysql-5.7.20/CMakeFiles/CMakeError.log".

如果出错缺少boots请先安装

解决办法是：
1.在/usr/local下创建一个名为boost的文件夹
mkdir -p /usr/local/boost
2.进入这个新创建的文件夹然后下载boost
    wget http://www.sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz[](http://www.sourceforge.net/projects/boost/files/boost/1.59.0/boost_1_59_0.tar.gz)3.解压
    tar -xvzf boost_1_59_0.tar.gz
4.继续cmake，添加上红色部分
cmake . -DCMAKE_INSTALL_PREFIX=/usr/local/mysql \
-DDEFAULT_CHARSET=utf8 \
-DDEFAULT_COLLATION=utf8_general_ci \
-DENABLED_LOCAL_INFILE=ON \
-DWITH_INNOBASE_STORAGE_ENGINE=1 \
-DWITH_FEDERATED_STORAGE_ENGINE=1 \
-DWITH_BLACKHOLE_STORAGE_ENGINE=1 \
-DWITHOUT_EXAMPLE_STORAGE_ENGINE=1 \
-DWITH_PARTITION_STORAGE_ENGINE=1 \
-DWITH_PERFSCHEMA_STORAGE_ENGINE=1 \
-DCOMPILATION_COMMENT='Mysqlma' \
-DWITH_READLINE=ON \
-DWITH_BOOST=/usr/local/boost \
-DSYSCONFDIR=/data/mysqldata/3306 \
-DMYSQL_UNIX_ADDR=/data/mysqldata/3306/mysql.sock
以上执行完在mysql目录下执行make（以上操作时间比较漫长，安装大约20分钟左右）

![](https://blog.csdn.net/img-blog.csdn.net/20180316095211196?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI0ODY4NDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

检查系统是否已经有mysql用户，如果没有则创建


![](https://blog.csdn.net/img-blog.csdn.net/20180316104531534?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI0ODY4NDA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


mysql 初始化数据库






