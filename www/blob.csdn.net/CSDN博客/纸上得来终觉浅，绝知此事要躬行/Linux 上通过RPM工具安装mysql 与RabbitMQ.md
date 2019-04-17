# Linux 上通过RPM工具安装mysql 与RabbitMQ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月28日 17:30:28[boonya](https://me.csdn.net/boonya)阅读数：589










文章来源：[https://www.oschina.net/code/snippet_2426852_49915](https://www.oschina.net/code/snippet_2426852_49915)和[https://www.oschina.net/code/snippet_2426852_49913](https://www.oschina.net/code/snippet_2426852_49913)


## 安装Linux 工具包

rpm安装命令：apt-get install rpm

yum安装命令：apt-get install yum

vim安装命令：apt-get install vim


## ```javarpm安装mysql```

## ```java```

```java
1
```

```java
.下载 http:
```

```java
//dev.mysql.com/downloads/mysql/
```



```java
2
```

```java
.安装
```





```java
2.1
```

```java
.检测是否已经安装了mysql
```



```java
rpm -qa | grep mysql
```





```java
如果已经安装了，将其卸载，如：
```



```java
rpm -e --nodeps  mysql-libs-
```

```java
5.1
```

```java
.
```

```java
71
```

```java
-
```

```java
1
```

```java
.el6.x86_64
```





```java
2.2
```

```java
.安装mysql
```





```java
1
```

```java
、创建一个mysql 文件夹
```



```java
mkdir /usr/local/src/mysql
```





```java
2
```

```java
、进入这个mysql文件夹
```



```java
cd /usr/local/src/mysql
```





```java
3
```

```java
、上传mysql安装包
```



```java
rz -y mysql 安装包
```





```java
4
```

```java
、解压mysql安装包
```



```java
tar -xvf MySQL-
```

```java
5.6
```

```java
.
```

```java
22
```

```java
-
```

```java
1
```

```java
.el6.i686.rpm-bundle.tar
```





```java
5
```

```java
、安装server
```



```java
rpm -ivh MySQL-server-
```

```java
5.6
```

```java
.
```

```java
22
```

```java
-
```

```java
1
```

```java
.el6.i686.rpm
```





```java
#注意:这里会出错...
```





```java
#因为要先安装依赖：
```



```java
yum -y install libaio.so.
```

```java
1
```

```java
libgcc_s.so.
```

```java
1
```

```java
libstdc++.so.
```

```java
6
```





```java
#需要升级 这个 libstdc++-
```

```java
4.4
```

```java
.
```

```java
7
```

```java
-
```

```java
4
```

```java
.el6.x86_64
```



```java
yum  update libstdc++-
```

```java
4.4
```

```java
.
```

```java
7
```

```java
-
```

```java
4
```

```java
.el6.x86_64
```



```java
6
```

```java
、安装中...
```





```java
7
```

```java
、安装client
```



```java
rpm -ivh MySQL-client-
```

```java
5.6
```

```java
.
```

```java
22
```

```java
-
```

```java
1
```

```java
.el6.i686.rpm
```





```java
#需要先安装这个依赖：
```



```java
yum -y install libncurses.so.
```

```java
5
```

```java
libtinfo.so.
```

```java
5
```





```java
8
```

```java
、查询mysq服务运行状态
```



```java
service mysql status
```



```java
#提示 服务未启动。
```





```java
9
```

```java
、启动mysql服务
```



```java
service mysql start
```





```java
10
```

```java
、使用root账号登录mysql
```



```java
# 提示：Access denied
```

```java
for
```

```java
user
```

```java
'root'
```

```java
@
```

```java
'localhost'
```

```java
(using password:NO) 登录失败
```





```java
# 解决办法:在安装mysql server时有句提示：
```



```java
A RADDOM PASSWORD HAS BEEN SET FOR THE MYSQL root USER !You will find that password in
```

```java
'/root/.mysql_secret'
```

```java
.
```



```java
#在这个文件夹下找到 初始设置的随机密码
```



```java
#注意：这个密码是不安全的，所有需要修改初始密码。
```





```java
11
```

```java
、使用随机密码登录mysql账号：
```



```java
mysql -uroot -p
```





```java
12
```

```java
、修改root密码：
```



```java
SET PASSWORD = PASSWORD(
```

```java
'123456'
```

```java
);
```





```java
2.3
```

```java
.系统启动时自动启动mysql服务
```





```java
13
```

```java
、加入到系统服务：
```



```java
chkconfig --add mysql
```





```java
14
```

```java
、自动启动：
```



```java
chkconfig mysql on
```





```java
15
```

```java
、查询列表：
```



```java
chkconfig
```





```java
# 说明：都没关闭（off）时是没有自动启动。
```





```java
2.4
```

```java
.开启远程访问
```



```java
登录：
```



```java
mysql -uroot –p123456
```





```java
设置远程访问（使用root密码）：
```



```java
grant all privileges on *.* to
```

```java
'root'
```

```java
@
```

```java
'%'
```

```java
identified by
```

```java
'123456'
```

```java
;
```



```java
flush privileges;
```





```java
防火墙打开
```

```java
3306
```

```java
端口
```



```java
/sbin/iptables -I INPUT -p tcp --dport
```

```java
3306
```

```java
-j ACCEPT
```



```java
/etc/rc.d/init.d/iptables save
```



```java
/etc/init.d/iptables status
```





```java
# 到这里,恭喜你,在linux系统上就完成了mysql的安装.....
```



## ```java同一linux系统安装两个端口号的 mysql```



```java
安装mysql
```



```java
安装的mysql的percona分支；
```





```java
可以参考文档：http:
```

```java
//www.percona.com/doc/percona-server/5.6/
```







```java
安装包：Percona-Server-
```

```java
5.6
```

```java
.
```

```java
21
```

```java
-
```

```java
70.0
```

```java
-r688-el6-x86_64-bundle.tar
```





```java
首先安装cmake
```



```java
yum -y install cmake
```





```java
命令：
```



```java
cd /usr/local/src/
```



```java
mkdir mysql-percona
```



```java
cd mysql-percona/
```





```java
rz 上传安装包
```



```java
tar 解压安装包
```



```java
tar -xvf Percona-Server-
```

```java
5.6
```

```java
.
```

```java
21
```

```java
-
```

```java
70.0
```

```java
-r688-el6-x86_64-bundle.tar
```





```java
rpm -ivh Percona-Server-shared-
```

```java
56
```

```java
-
```

```java
5.6
```

```java
.
```

```java
21
```

```java
-rel70.
```

```java
0
```

```java
.el6.x86_64.rpm
```



```java
rpm -ivh Percona-Server-client-
```

```java
56
```

```java
-
```

```java
5.6
```

```java
.
```

```java
21
```

```java
-rel70.
```

```java
0
```

```java
.el6.x86_64.rpm
```



```java
rpm -ivh Percona-Server-server-
```

```java
56
```

```java
-
```

```java
5.6
```

```java
.
```

```java
21
```

```java
-rel70.
```

```java
0
```

```java
.el6.x86_64.rpm
```





```java
启动：
```



```java
service mysql start
```





```java
修改root密码：
```



```java
mysqladmin -u root password
```

```java
"root"
```





```java
登录：
```



```java
mysql -uroot -proot
```





```java
设置远程访问（使用root密码）：
```



```java
grant all privileges on *.* to
```

```java
'root'
```

```java
@
```

```java
'%'
```

```java
identified by
```

```java
'root'
```

```java
;
```



```java
flush privileges;
```





```java
防火墙打开
```

```java
3306
```

```java
端口
```



```java
/sbin/iptables -I INPUT -p tcp --dport
```

```java
3306
```

```java
-j ACCEPT
```



```java
/etc/rc.d/init.d/iptables save
```



```java
/etc/init.d/iptables status
```

```java

```



## ```java安装RabbitMQ```





```java
首先安装Erlang
```



```java
添加yum支持
```





```java
cd /usr/local/src/
```



```java
mkdir rabbitmq
```



```java
cd rabbitmq
```





```java
方式一：
```



```java
wget http:
```

```java
//packages.erlang-solutions.com/erlang-solutions-1.0-1.noarch.rpm
```



```java
rpm -Uvh erlang-solutions-
```

```java
1.0
```

```java
-
```

```java
1
```

```java
.noarch.rpm
```





```java
rpm --
```

```java
import
```

```java
http:
```

```java
//packages.erlang-solutions.com/rpm/erlang_solutions.asc
```





```java
sudo yum install erlang
```





```java
方式二：
```



```java
上传esl-erlang_17.
```

```java
3
```

```java
-
```

```java
1
```

```java
~centos~6_amd64.rpm
```



```java
执行 yum install esl-erlang_17.
```

```java
3
```

```java
-
```

```java
1
```

```java
~centos~6_amd64.rpm
```





```java
上传：esl-erlang-compat-R14B-
```

```java
1
```

```java
.el6.noarch.rpm
```



```java
yum install esl-erlang-compat-R14B-
```

```java
1
```

```java
.el6.noarch.rpm
```







```java
开始 安装RabbitMQ
```



```java
上传rabbitmq-server-
```

```java
3.4
```

```java
.
```

```java
1
```

```java
-
```

```java
1
```

```java
.noarch.rpm文件到/usr/local/src/rabbitmq/
```



```java
安装：
```



```java
rpm -ivh rabbitmq-server-
```

```java
3.4
```

```java
.
```

```java
1
```

```java
-
```

```java
1
```

```java
.noarch.rpm
```





```java
启动、停止
```



```java
service rabbitmq-server start
```



```java
service rabbitmq-server stop
```



```java
service rabbitmq-server restart
```



```java
设置开机启动
```



```java
chkconfig rabbitmq-server on
```



```java
设置配置文件
```



```java
cd /etc/rabbitmq
```



```java
cp /usr/share/doc/rabbitmq-server-
```

```java
3.4
```

```java
.
```

```java
1
```

```java
/rabbitmq.config.example
 /etc/rabbitmq/
```





```java
mv rabbitmq.config.example rabbitmq.config
```



```java
开启用户远程访问
```



```java
vi /etc/rabbitmq/rabbitmq.config
```





```java
去掉loopback_user {},前的 百分号
```



```java
#注意要去掉后面的逗号。
```





```java
开启web界面管理工具
```



```java
rabbitmq-plugins enable rabbitmq_management
```



```java
service rabbitmq-server restart
```



```java
防火墙开放
```

```java
15672
```

```java
端口
```



```java
/sbin/iptables -I INPUT -p tcp --dport
```

```java
15672
```

```java
-j ACCEPT
```



```java
/etc/rc.d/init.d/iptables save
```








