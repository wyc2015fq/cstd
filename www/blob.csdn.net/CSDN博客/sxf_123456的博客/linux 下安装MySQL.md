# linux 下安装MySQL - sxf_123456的博客 - CSDN博客
2017年02月24日 20:56:19[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：218标签：[linux																[mysql](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Centos 安装](https://blog.csdn.net/sxf_123456/article/category/6745709)
1、到mysql官网下载mysql编译好的二进制安装包，在下载页面Select Platform:选项选择linux-generic，然后把页面拉到底部，64位系统下载Linux - Generic (glibc 2.5) (x86, 64-bit)，32位系统下载Linux - Generic (glibc 2.5) (x86, 32-bit)
[](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=1)
[](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=2)
[](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=2)
2、进入安装包所在目录，执行命令：tar -zxvf /usr/mysql-5.7.17-linux-glibc2.5-x86_64.tar.gz
3、复制解压后的mysql目录到系统的本地软件目录:
执行命令：cp -r /usr/mysql-5.7.17-linux-glibc2.5-x86_64 /usr/local/mysql
cd/usr/local/mysql/mysql-5.7.17-linux-glibc2.5-x86_64 
mv  *  /usr/local/mysql/  #将mysql-5.7.17-linux-glibc2.5-x86_64下的所有文件移动到mysql目录下
4、添加系统mysql组和mysql用户：
useradd -r -g mysql mysql #创建mysql用户并添加mysql组里
执行命令：groupadd mysql和useradd -r -g mysql mysql
[](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=5)
[](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=6)
[5、安装数据库：](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=10)
[cd /usr/local/mysql/bin](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=10)
[yum install libaio](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=10)
[./mysql_install_db --user=mysql --basedir=/usr/local/mysql/--datadir=/usr/local/mysql/data](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=10)
[到此数据库安装完毕](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=10)
6、启动mysql服务
启动mysql服务：cd /usr/local/mysql/support-files
./mysql.server start
使用初始密码登录mysql，并修改密码，同时设置root可以远程连接
命令如下：
cat /root/.mysql_secret
cd /usr/local/mysql/bin
./mysql -u root -p
set password =password('新密码');
GRANT ALL PRIVILEGES ON *.* TOroot@"%" IDENTIFIED BY "新密码";
flush privileges;
可能远程连接不了，尝试关闭防火墙，命令如下：
service iptables stop
setenforce 0
7、将mysql设置为开机自启动，并添加软连接
命令如下：
cd /usr/local/mysql/support-files/
cp/usr/local/mysql/support-files/mysql.server /etc/init.d/mysqld
cd /usr/bin
ln -s /usr/local/mysql/bin/mysqlmysql 
添加开机启动：执行命令cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysqld，把启动脚本放到开机初始化目录
启动mysql服务：执行命令service mysqld start
执行命令：ps -ef|grep mysql 看到mysql服务说明启动成功，如图
[](http://jingyan.baidu.com/album/a378c9609eb652b3282830fd.html?picindex=8)
8、编译c连接mysql数据库程序
[root@node1 Desktop]# gcc -o testmysqltest.c -I/usr/local/mysql/include/ -L/usr/local/mysql/lib -lmysqlclient
[root@node1 Desktop]# ./test
