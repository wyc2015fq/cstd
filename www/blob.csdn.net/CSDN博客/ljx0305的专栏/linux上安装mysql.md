# linux上安装mysql - ljx0305的专栏 - CSDN博客
2009年07月22日 21:31:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：569标签：[linux																[mysql																[socket](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[MySql](https://blog.csdn.net/ljx0305/article/category/570123)
首先需要建立一个mysql用户，和一个mysql用户组 
groupadd mysql 
useradd -g mysql mysql 
通过源程序包安装MySQL
1.从mysql的官网下载mysql的安装包(.tar.gz文件) 
[http://dev.mysql.com/get/Downloads/MySQL-5.0/mysql-5.0.51a.tar.gz/from/pick](http://dev.mysql.com/get/Downloads/MySQL-5.0/mysql-5.0.51a.tar.gz/from/pick)
2.假设mysql-5.0.51a.tar.gz下载后所在的目录为:/home/mahaibo下。 
现在要把mysql安装在目录:/home/mahaibo/install/mysql 
mysql调用的mysql.sock文件所在目录:/home/mahaibo/install/sock 
mysql调用的my.cnf配置文件目录:/home/mahaibo/install/conf 
mysql的数据目录:/home/mahaibo/install/data 
3.解压mysql-5.0.51a.tar.gz 
- tar -zxvf mysql-5.0.51a.tar.gz   
```java
tar -zxvf mysql-5.0.51a.tar.gz
```
4.配置编译前的准备： 
- ./configure --prefix=/home/mahaibo/install/mysql --localstatedir=/home/mahaibo/install/data --with-unix-socket-path=/home/mahaibo/install/sock   
```java
./configure --prefix=/home/mahaibo/install/mysql --localstatedir=/home/mahaibo/install/data --with-unix-socket-path=/home/mahaibo/install/sock
```
--localstatedir 数据目录参数 
--with-unix-socket-path mysql.sock目录参数 
注：可以通过./configure --help来查看./configure具体有哪些参数。除了--prefix外，像--localstatedir --with-unix-socket-path这些参数都可以在my.cnf里进行具体配置。如果my.cnf里没有配置，那么就以./configure的参数为准，如果在my.cnf里进行了配置，那么my.cnf里的值会覆盖./configure的参数值
如果my.cnf和./configure都没有配置,那么会以默认值。 
数据目录默认值为：如果是通过源程序包安装MySQL，mysql数据目录默认的缺省位置是/usr/local/var,如果通过二进制包安装MySQL，则为/usr/local/mysql/data,如果通过RPM 文件安装，则为/var/lib/mysql 
mysql.sock默认目录:/tmp/mysql.sock
my.cnf默认目录:/etc/my.cnf
5.编译和安装 
- make   
- make install  
```java
make
make install
```
6.在数据目录里生成mysql的数据基本信息和基本表结构等 
- ./scripts/mysql_install_db --user=mysql --datadir=/home/mahaibo/install/data  
```java
./scripts/mysql_install_db --user=mysql --datadir=/home/mahaibo/install/data
```
然后把数据目录设置成mysql用户和用户组
- chown -R mysql:mysql data  
```java
chown -R mysql:mysql data
```
执行完后可以在/home/mahaibo/install/data目录下生成2个目录，一个是mysql,一个是test 
7. 
- mkdir conf  
```java
mkdir conf
```
- mkdir sock  
```java
mkdir sock
```
修改sock目录权限，让mysql用户有可写权限
- chmod 777 sock  
```java
chmod 777 sock
```
8.拷贝my.cnf 
- cp /home/mahaibo/mysql-5.0.51a/support-files/my-medium.cnf /home/mahaibo/install/conf/  
```java
cp /home/mahaibo/mysql-5.0.51a/support-files/my-medium.cnf /home/mahaibo/install/conf/
```
修改名字：mv my-medium.cnf my.cnf 
修改内容：在[client]和[mysqld]部分添加参数：socket          = /home/mahaibo/install/sock/mysql.sock 
9.启动mysql: 
- ./mysqld_safe --defaults-file=/home/mahaibo/install/conf/my.cnf &  
```java
./mysqld_safe --defaults-file=/home/mahaibo/install/conf/my.cnf &
```
可以通过./mysqld_safe --help查看参数。 
如果不加--defaults-file参数，那么默认是去my.cnf的默认目录下去找，即找/etc/my.cnf
可以通过netstat -lnt 查看，如果有3306端口存在，则表示启动成功 
10。登录mysql: 
- mysql -uroot -S/home/mahaibo/install/sock/mysql.sock  
```java
mysql -uroot -S/home/mahaibo/install/sock/mysql.sock
```
mysql 刚开始默认会建立一个root 用户，密码为空。 如果不加参数-S，则在默认目录去找mysql.sock，即/tmp/mysql.sock
11.关闭mysql: 
- mysqladmin shutdown -S/home/mahaibo/install/sock/mysql.sock  
```java
mysqladmin shutdown -S/home/mahaibo/install/sock/mysql.sock
```
如果不加参数-S，则在默认目录去找mysql.sock，即/tmp/mysql.sock
引用 : [http://www.javaeye.com/topic/161165](http://www.javaeye.com/topic/161165)
