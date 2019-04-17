# Linux 下使用Mysql迁移存储数据 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年10月10日 09:42:03[boonya](https://me.csdn.net/boonya)阅读数：2324








                
本文出自逆流的鱼yuiop：[http://blog.csdn.net/hejjunlin/article/details/52768613](http://blog.csdn.net/hejjunlin/article/details/52768613)
前言：数据库每天的数据不断增多，自动删除机制总体风险太大，想保留更多历史性的数据供查询，于是从小的hbase换到大的hbase上，势在必行。今天记录下这次数据仓库迁移。看下Agenda:
- 彻底卸载MySQL
- 安装MySQL
- MySQL数据库的初始化及相关配置
- 存储数据目录修改
- 客户端连接又来问题(中间过程)
- 小总结

## 彻底卸载MySQL

**yum方式安装的MySQL**



```
yum remove mysql mysql-server mysql-libs compat-mysql51 
rm -rf /var/lib/mysql
rm /etc/my.cnf
```


查看是否还有mysql相关：




`rpm -qa|grep mysql`


**rpm方式安装的MySQL**

查看系统中是否以rpm包安装的mysql：



```
[root@itv-h5-web-online001-sjhl ~]# rpm -qa | grep -i mysql
MySQL-server-5.6.17-1.el6.i686
MySQL-client-5.6.17-1.el6.i686
```

卸载mysql



```
[root@itv-h5-web-online001-sjhl ~]# rpm -e MySQL-server-5.6.17-1.el6.i686
[root@itv-h5-web-online001-sjhl ~]# rpm -e MySQL-client-5.6.17-1.el6.i686
```

删除mysql服务



```
[root@itv-h5-web-online001-sjhl ~]# chkconfig --list | grep -i mysql
[root@itv-h5-web-online001-sjhl ~]# chkconfig --del mysql
```

删除分散mysql文件夹



```
[root@itv-h5-web-online001-sjhl ~]# whereis mysql 或者 find / -name mysql
mysql: /usr/lib/mysql /usr/share/mysql
```

## 安装MySQL

通过yum来进行mysql的安装

我是通过yum的方式来进行mysql的数据库安装，首先我们可以输入 yum list | grep mysql 命令来查看yum上提供的mysql数据库可下载的版本：


`[root@itv-h5-web-online001-sjhl ~]# yum list | grep mysql`


就可以得到yum服务器上mysql数据库的可下载版本信息： 


![这里写图片描述](https://img-blog.csdn.net/20161009182914806)


然后我们可以通过输入


`yum install -y mysql-server mysql mysql-devel `


命令将mysql mysql-server mysql-devel都安装好(注意:安装mysql时我们并不是安装了mysql客户端就相当于安装好了mysql数据库了，我们还需要安装mysql-server服务端才行)


`[root@itv-h5-web-online001-sjhl ~]# yum install -y mysql-server mysql mysql-deve`


在等待了一番时间后，yum会帮我们选择好安装mysql数据库所需要的软件以及其它附属的一些软件 


![这里写图片描述](https://img-blog.csdn.net/20161009171117187)

我们发现，通过yum方式安装mysql数据库省去了很多没必要的麻烦，当出现下面的结果时，就代表mysql数据库差不多安装成功了。 


![这里写图片描述](https://img-blog.csdn.net/20161009183216701)

此时我们可以通过如下命令，查看刚安装好的mysql-server的版本


`[root@itv-h5-web-online001-sjhl ~]# rpm -qi mysql-server`


我们安装的mysql-server并不是最新版本，如果你想尝试最新版本，那就去mysql官网下载rpm包安装（不过还是不建议安装最新的，可能有各种未知的问题），至此我们的mysql数据库已经安装完成了。


![这里写图片描述](https://img-blog.csdn.net/20161009171717822)

## MySQL数据库的初始化及相关配置

我们在安装完mysql数据库以后，会发现会多出一个mysqld的服务，这个数据库服务，我们通过输入 service mysqld start 命令就可以启动我们的mysql服务。

注意：如果我们是第一次启动mysql服务，mysql服务器首先会进行初始化的配置，如：


`[root@itv-h5-web-online001-sjhl ~]# service mysqld start`


![这里写图片描述](https://img-blog.csdn.net/20161009184047296)


然而问题来了：Starting mysqld : [Failed] 

然后看了下log是说：Do you already have another mysqld server running on port: 3306? 

是说是不是有mysqld 占用了3306端口，明明已卸载了。。

接着看下端口的状态： 

检查mysql 的端口号



```
[root@itv-h5-web-online001-sjhl ~]# netstat -a -t -n|grep 3306
[root@itv-h5-web-online001-sjhl ~]# netstat -nltp|grep mysql
```



把服务停掉：


`[root@itv-h5-web-online001-sjhl ~]# service mysqld stop`


接着查下是否有mysqld进程在


`[root@itv-h5-web-online001-sjhl ~]# ps -ef|grep mysqld`


![这里写图片描述](https://img-blog.csdn.net/20161009183724771)

果然在占了3306端口，只有kill这个，重新start，就应该没问题了。 

最后用命令：


`[root@itv-h5-web-online001-sjhl ~]# /etc/inint.d/mysqld start`


来启动mysql服务，这时就正常了。

mysql数据库安装完以后只会有一个root管理员账号，但是此时的root账号还并没有为其设置密码，在第一次启动mysql服务时，会进行数据库的一些初始化工作，在输出的一大串信息中，我们看到有这样一行信息 ：/usr/bin/mysqladmin -u root password ‘new-password’　　// 为root账号设置密码

所以我们可以通过 该命令来给我们的root账号设置密码(注意：这个root账号是mysql的root账号，非Linux的root账号)


`[root@itv-h5-web-online001-sjhl ~]# mysqladmin -u root password '123456'`


此时我们就可以通过 mysql -u root -p 命令来登录我们的mysql数据库

## 存储数据目录修改

由于默认mysql装的分区盘空间很小，需要修改对应存储数据文件路径，找到/etc/mycnf中，用vim打开，对应修改，datadir就是要修改的目标，socket本想一快改了的，发现改了后，不能登录mysql了，应该还有一配置文件未改，于是暂时放弃了。

![这里写图片描述](https://img-blog.csdn.net/20161009173916194)

用命令


`/etc/init.d/mysqld restart`


进行重启mysql服务，即可生效（ps:一定要事先把mysql服务先停掉，再修改以上文件）

## 客户端连接又来问题

当以为上面大功告吉的了的时候，用mysql可视化工具支连接，发现又报如下错误： 


![这里写图片描述](https://img-blog.csdn.net/20161009184253813)


原因是未对远程客户端口进行授权，接着开始授权：

![这里写图片描述](https://img-blog.csdn.net/20161009173939163)

**以下是一些可选方案：**

例如，你想myuser使用mypassword从任何主机连接到mysql服务器的话。



```
GRANT ALL PRIVILEGES ON *.* TO 'myuser'@'%' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;

FLUSH   PRIVILEGES;
```

如果你想允许用户myuser从ip为192.168.1.6的主机连接到mysql服务器，并使用mypassword作为密码



```
GRANT ALL PRIVILEGES ON *.* TO 'myuser'@'192.168.1.6' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;

FLUSH   PRIVILEGES;
```

如果你想允许用户myuser从ip为192.168.1.6的主机连接到mysql服务器的dk数据库，并使用mypassword作为密码



```
GRANT ALL PRIVILEGES ON dk.* TO 'myuser'@'192.168.1.6' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;

FLUSH   PRIVILEGES;
```

**这下才是大功告成！！！！**

## 小总结

最后总结三条：
- 先在目标机器上安装好环境。如mysql
- 进行数据配置，并且测试能通过访问
- 数据迁移，原有机器上打包copy到新机器上。

附上一些常用MySQL启动、停止、重启常用命令：

**启动**

1、使用 service 启动：



```
service mysqld start (5.0版本是mysqld)
service mysql start (5.5.7版本是mysql)
```

2、使用 mysqld 脚本启动：


`/etc/inint.d/mysqld start`
3、使用 safe_mysqld 启动：


`safe_mysqld&`
**停止**

1、使用 service 启动：


`service mysqld stop`
2、使用 mysqld 脚本启动：


`/etc/inint.d/mysqld stop`
**重启**

1、使用 service 启动：



```
service mysqld restart 
service mysql restart (5.5.7版本命令)
```



2、使用 mysqld 脚本启动：


`/etc/init.d/mysqld restart`







