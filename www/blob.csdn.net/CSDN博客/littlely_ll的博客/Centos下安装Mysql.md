# Centos下安装Mysql - littlely_ll的博客 - CSDN博客





2017年06月22日 21:12:11[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：149标签：[mysql																[centos](https://so.csdn.net/so/search/s.do?q=centos&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[mysql](https://blog.csdn.net/littlely_ll/article/category/6929640)








一、二进制免编译包安装  参考：[http://www.apelearn.com/bbs/forum.php?mod=viewthread&tid=10105&highlight=mysql5.7](http://www.apelearn.com/bbs/forum.php?mod=viewthread&tid=10105&highlight=mysql5.7)

1、下载地址： [http://mirrors.sohu.com/mysql/MySQL-5.7/](http://mirrors.sohu.com/mysql/MySQL-5.7/)    文件名还glibc的为免编译的二进制安装包 

     文件:  mysql-5.7.13-linux-glibc2.5-x86_64.tar.gz    mysql-5.6.30-linux-glibc2.5-x86_64.tar.gz 

2、官网下载：[http://dev.mysql.com/downloads/mysql/](http://dev.mysql.com/downloads/mysql/)

    Select Platform:  选择 –>  linux - Generic   然后选择 (mysql-5.7.13-linux-glibc2.5-x86_64.tar.gz)  

Linux - Generic (glibc 2.5) (x86, 64-bit), Compressed TAR Archive   5.7.13  610.2M 

Download 

(mysql-5.7.13-linux-glibc2.5-x86_64.tar.gz) 

正文：
二：卸载旧版本 

使用下面的命令检查是否安装有MySQL Server

rpm -qa | grep mysql 

显示： mysql-libs-5.1.71-1.el6.x86_64 

有的话通过下面的命令来卸载掉
rpm -e mysql-libs-5.1.71-1.el6.x86_64   //普通删除模式 

rpm -e –nodeps mysql-libs-5.1.71-1.el6.x86_64    // 强力删除模式，如果使用上面命令删除时，提示有依赖的其它文件，则用该命令可以对其进行强力删除 

或者 yum remove mysql-libs-5.1.71-1.el6.x86_64    //yum卸载 

三、安装
*下载包*

cd /usr/local/src

wget   [http://mirrors.sohu.com/mysql/MySQL-5.7/mysql-5.7.12-linux-glibc2.5-x86_64.tar.gz](http://mirrors.sohu.com/mysql/MySQL-5.7/mysql-5.7.12-linux-glibc2.5-x86_64.tar.gz)

```
http://mirrors.sohu.com/mysql/MySQL-5.7/mysql-5.7.13-linux-glibc2.5-x86_64.tar.gz
```

*解压 *

tar  zxvf mysql-5.7.12-linux-glibc2.5-x86_64.tar.gz 

mv  mysql-5.7.12-linux-glibc2.5-x86_64  /usr/local/mysql

*初始化*

     // mysql_install_db 已经不再推荐使用了，建议改成 mysqld –initialize 完成实例初始化。

使用下面的命令查看是否有mysql用户及用户组 

cat /etc/passwd 查看用户列表 

cat /etc/group 查看用户组列表 

如果没有就创建
useradd -M -s /sbin/nologin  mysql            //-M　不建立使用者目录 

mkdir -p /data/mysql 

chown mysql /data/mysql 

cd /usr/local/mysql 

./bin/mysqld  –initialize –user=mysql –datadir=/data/mysql   //旧方法：    ./scripts/mysql_install_db –user=mysql –datadir=/data/mysql  

注意，这一步最后一行会有一个提示 

[Note] A temporary password is generated for root@localhost: B*s1i(*,kXwg 

最后面的字符串为root密码。 

./bin/mysql_ssl_rsa_setup –datadir=/data/mysql
*拷贝配置文件和启动脚本*

注：在启动MySQL服务时，会按照一定次序搜索my.cnf，先在/etc目录下找，找不到则会搜索”$basedir/my.cnf”，在本例中就是 /usr/local/mysql/my.cnf，这是新版MySQL的配置文件的默认位置！

cp support-files/my-default.cnf  /etc/my.cnf 

vim /etc/my.cnf //编辑或者修改

[mysqld]       // 在这里修改 

basedir = /usr/local/mysql 

datadir = /data/mysql 

port = 3306 

socket = /tmp/mysql.sock     //注意路径不能出错，否则启动出错 上次少了/
cp support-files/mysql.server /etc/init.d/mysqld 

vi /etc/init.d/mysqld   //编辑或者修改 

basedir=/usr/local/mysql 

datadir=/data/mysql
*启动服务*

/etc/init.d/mysqld start

启动出错的话可以查看日志：less /data/mysql/localhost.localdomain.err

加入开机启动 

chkconfig –add mysqld 

chkconfig mysqld on 

service mysqld start
*设置root密码*

使用初始化密码登录 

/usr/local/mysql/bin/mysql -uroot -p’B*s1i(*,kXwg’  //进入后直接设置密码 

mysql>set password = password(‘mypass’);   //注意分号一定要设置一下新密码 

退出来，再使用新的密码登录就可以了
还有一种情况，就是不知道初始化密码 

vi /etc/my.cnf 

在[mysqld]下面增加一行 

skip-grant-tables 

重启  /etc/init.d/mysqld restart
/usr/local/mysql/bin/mysql -uroot  

mysql> update user set authentication_string=password(‘123333’) where user=’root’; 

退出来后，更改my.cnf，去掉刚加的 skip-grant-tables 

重启 /etc/init.d/mysqld restart
此时就可以使用新的密码了。















