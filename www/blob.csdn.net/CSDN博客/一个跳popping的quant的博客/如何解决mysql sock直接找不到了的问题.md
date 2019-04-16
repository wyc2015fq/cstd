# 如何解决mysql.sock直接找不到了的问题 - 一个跳popping的quant的博客 - CSDN博客





2017年10月21日 15:47:19[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：17199








今天为了修改一个在Linux服务器上的项目，就把之前的项目直接删除了，然后又把新的项目上传了上去，通过tomcat发布后，发现通过外网不能访问，而且也没有找到tomcat的进程，于是就看了一下日志，发现是连接mysql的时候出现了问题。于是测试了一下mysql发现没法登录，出现了Can’t connect to local MySQL server through socket ‘/var/lib/mysql/mysql.sock的问题。 

  先说一下mysql.sock的作用吧。 

  首先mysql的连接方式有两种： 

  1、通过TCP/IP的连接 

  2、通过本地socket的连接，对mysql.sock来说，其作用是程序与mysqlserver处于同一台机器，发起本地连接时可用。在连接时你无须定义连接host的具体IP，只要为空或localhost就可以。
之后就google了一下，网上的方法都是通过查看/etc/my.cnf文件，查看mysql.sock的位置，然后建立软连接，将找到的mysql.sock的位置与配置文件中所说的位置连接起来。但是我遇到的问题是直接找不到了！！！！！通过find的命令全盘搜索也找不到。

于是又找到了一篇文章，[http://www.weiruoyu.cn/?p=673](http://www.weiruoyu.cn/?p=673) ，不过文章提到的是没有切换用户，这一点无所谓，我一直用的是root用户，然后，按照上面说的：

```
$/usr/bin/mysql_install_db     //重建授权表

$/usr/bin/mysqld_safe &

$/usr/local/bin/mysql                //测试

mysq>bye;
```

重新建立了新的授权表，重新生成新的 /var/lib/mysql/mysql.sock 和 hostname.pid，然后直接通过mysql命令登录，修改密码：

```
mysql> use mysql;
mysql> update user set password=password("你的新密码") where user="root";
mysql> flush privileges;
mysql> quit
```

重新启动一下服务，就可以了！！！！！




