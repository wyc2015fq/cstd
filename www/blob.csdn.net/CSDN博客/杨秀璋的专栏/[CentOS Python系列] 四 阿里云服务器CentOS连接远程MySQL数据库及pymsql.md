# [CentOS Python系列] 四.阿里云服务器CentOS连接远程MySQL数据库及pymsql - 杨秀璋的专栏 - CSDN博客





2018年02月22日 10:19:43[Eastmount](https://me.csdn.net/Eastmount)阅读数：918
所属专栏：[数据库实战开发设计与优化](https://blog.csdn.net/column/details/14842.html)[CentOS云服务端之Python爬虫及数据库知识](https://blog.csdn.net/column/details/19699.html)











从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。
- Python基础知识系列：[Pythonj基础知识学习与提升](http://blog.csdn.net/column/details/eastmount-python.html)
- Python网络爬虫系列：[Python爬虫之Selenium+Phantomjs+CasperJS](http://blog.csdn.net/column/details/eastmount-spider.html)
- Python数据分析系列：[知识图谱、web数据挖掘及NLP](http://blog.csdn.net/column/details/eastmount-kgdmnlp.html)



![这里写图片描述](https://img-blog.csdn.net/20180212160809122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161506304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161524640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

随着人工智能和深度学习的风暴来临，Python变得越来越火热，作者也准备从零学习这些知识，写相关文章。本篇文章讲解阿里云服务器CentOS系统连接远程MySQL数据库及配置过程，同时教大家如何编写Python操作MySQL数据库的基础代码，为后面的网络爬虫并存储至服务器打下基础。

文章非常基础，希望这系列文章对您有所帮助，如果有错误或不足之处，还请海涵~

系列文章：
[[CentOS Python系列] 一.阿里云服务器安装部署及第一个Python爬虫代码实现](http://blog.csdn.net/Eastmount/article/details/79321822)
[[CentOS Python系列] 二.pscp上传下载服务器文件及phantomjs安装详解](http://blog.csdn.net/eastmount/article/details/79330430)
[[CentOS Python系列] 三.阿里云MySQL数据库开启配置及SQL语句基础知识](http://blog.csdn.net/eastmount/article/details/79334052)





参考文献：






## 一. 创建新用户




**1.登录root用户，插入新用户**
核心代码：
insert into mysql.user(Host,User,Password) value ("localhost","eastmount",password("123456"));


![](https://img-blog.csdn.net/20180221174410646)
命令如下所示：

```python
mysql> use mysql
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> insert into mysql.user(Host,User,Password) value ("localhost","eastmount",password("123456"));
Query OK, 1 row affected, 3 warnings (0.00 sec)

mysql> flush privileges;
Query OK, 0 rows affected (0.00 sec)

mysql> select host,user,password from user;
+-------------------------+-----------+-------------------------------------------+
| host                    | user      | password                                  |
+-------------------------+-----------+-------------------------------------------+
| localhost               | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| izm5e2qvb8hl5w1gjowpsxz | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| 127.0.0.1               | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| localhost               |           |                                           |
| izm5e2qvb8hl5w1gjowpsxz |           |                                           |
| localhost               | yxz       | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| localhost               | mysql     | *95FE99470B7B7CAF1E150B16ACCA48CDE7925813 |
| 39.107.105.166          | yxz       | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| 39.107.105.166          | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| %                       | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| localhost               | eastmount | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
+-------------------------+-----------+-------------------------------------------+
11 rows in set (0.00 sec)
```

![](https://img-blog.csdn.net/2018022117474762)


通过 select host, user, password from user 查看主机、用户和密码，可以看到 (localhost, eastmount, 123456) 已经插入成功。
此时，用户eastmount是可以登录了，通过语句 "mysql -u eastmount -p 123456"。


![](https://img-blog.csdn.net/20180221175252926)

但是使用该用户创建数据库，报错如下所示，这是需要先进行授权。
mysql> create database douban;
ERROR 1044 (42000): Access denied for user 'eastmount'@'localhost' to database 'douban'




**2.root用户登录再授权**
核心代码：grant all privileges on *.* to eastmount@localhost identified by "eastmount"; 


```
mysql -u root -p
123456

use mysql;

grant all privileges on *.* to eastmount@localhost identified by "eastmount"; 

flush privileges;  

quit;
```

![](https://img-blog.csdn.net/20180221175646720)


但是此时登录会报错，如下所示：
[root@iZ2ze9134z8zlqupc9t6mzZ ~]# mysql -u eastmount -p
Enter password:
ERROR 1045 (28000): Access denied for user 'eastmount'@'localhost' (using password: YES)


![](https://img-blog.csdn.net/20180221180243888)


此时需要关闭服务器修改eastmount登录密码，重启服务即可。




**3.关闭服务器更新登录密码再刷新权限**

核心代码：update user set password=PASSWORD("123456") where user="eastmount";



![](https://img-blog.csdn.net/20180222084841888)



```
[root@iZ2ze9134z8zlqupc9t6mzZ ~]# service mysqld stop
Stopping mysqld:                                           [  OK  ]
[root@iZ2ze9134z8zlqupc9t6mzZ ~]# mysqld_safe --skip-grant-tables &
[1] 30466
[root@iZ2ze9134z8zlqupc9t6mzZ ~]# 180220 23:53:36 mysqld_safe Logging to '/var/log/mysqld.log'.
180220 23:53:36 mysqld_safe Starting mysqld daemon with databases from /var/lib/mysql
mysql -u eastmount -p
Enter password:
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 1
Server version: 5.1.73 Source distribution

Copyright (c) 2000, 2013, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| Eastmount          |
| junyun             |
| mysql              |
| test               |
+--------------------+
5 rows in set (0.00 sec)
```

接下来更新eastmount用户的密码。



![](https://img-blog.csdn.net/20180221181657303)



```
mysql> use mysql;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> update user set password=PASSWORD("123456") where user="eastmount";
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0

mysql> flush privileges;
Query OK, 0 rows affected (0.00 sec)

mysql> quit;
```



**4.重启MySQL服务**
核心代码：service mysqld restart

![](https://img-blog.csdn.net/20180222085038167)


```
[root@iZ2ze9134z8zlqupc9t6mzZ ~]# service mysqld restart
180220 23:57:47 mysqld_safe mysqld from pid file /var/run/mysqld/mysqld.pid ended
Stopping mysqld:                                           [  OK  ]
Starting mysqld:                                           [  OK  ]
[1]+  Done                    mysqld_safe --skip-grant-tables
[root@iZ2ze9134z8zlqupc9t6mzZ ~]#
```




**5.登录成功数据库操作**

![](https://img-blog.csdn.net/20180222085157390)


```
mysql> create database 20180220df;
Query OK, 1 row affected (0.00 sec)

mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| 20180220df         |
| Eastmount          |
| doubanl            |
| junyun             |
| mysql              |
| test               |
+--------------------+
7 rows in set (0.00 sec)


mysql> use 20180220df;
Database changed
mysql> create table student(
    ->     id int not null primary key,
    ->     name varchar(16) not null,
    ->     pwd varchar(20) not null
    -> );
Query OK, 0 rows affected (0.02 sec)

mysql> show tables;
+----------------------+
| Tables_in_20180220df |
+----------------------+
| student              |
+----------------------+
1 row in set (0.00 sec)

mysql> insert into student(id,name,pwd) values(1,'yangxiuzhang','111111');
Query OK, 1 row affected (0.00 sec)

mysql> select * from student;
+----+--------------+--------+
| id | name         | pwd    |
+----+--------------+--------+
|  1 | yangxiuzhang | 111111 |
+----+--------------+--------+
1 row in set (0.00 sec)
```



操作界面如下图所示：



![](https://img-blog.csdn.net/20180222085345183)



![](https://img-blog.csdn.net/2018022208540857)

















## 二. 远程服务器授权及端口开启




但此时连接远程数据库，仍然报错“2003-can't connect to MySQL server on (10060)”。



![](https://img-blog.csdn.net/20180222092905994)



这是需要开启远程服务器并进行公网IP授权，同时开发3306端口号，流程如下：





### （一） CentOS开通MySQL3306端口


1.vi /etc/sysconfig/iptables

2.加入如下代码，核心: -I INPUT -p tcp -m state --state NEW -m tcp --dport 3306-j ACCEPT

```
#Firewall configuration written by system-config-firewall
#Manual customization of this file is not recommanded.
*filter
:INPUT ACCEPT [0:0]
:FORWARD ACCEPT [0:0]
:OUTPUT ACCEPT [0:0]
-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
-A INPUT -p icmp -j ACCEPT
-A INPUT -i lo -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 3306 -j ACCEPT
-A INPUT -j REJECT --reject-with icmp-host-prohibited
-A FORWARD -j REJECT --reject-with icmp-host-prohibited
COMMIT
```

3.保存退出

4.重启防火墙：service iptables restart



![](https://img-blog.csdn.net/20180222093440657)



```
[root@iZ2ze9134z8zlqupc9t6mzZ ~]# service iptables restart
iptables: Setting chains to policy ACCEPT: filter          [  OK  ]
iptables: Flushing firewall rules:                         [  OK  ]
iptables: Unloading modules:                               [  OK  ]
iptables: Applying firewall rules:                         [  OK  ]
```
注意：开放的端口一定要添加到 REJECT 之前。







### (二) 开启远程访问权限


1.mysql -u eastmount -p 123456



![](https://img-blog.csdn.net/20180222094033258)





2.查询主机、用户名和密码：select host,user,password from user;



![](https://img-blog.csdn.net/20180222094120777)





3.授权：grant all privileges on *.* to eastmount@39.107.105.166 identified by "123456" with grant option;

```
mysql> use mysql;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> grant all privileges on *.* to eastmount@39.107.105.166 identified by "123456" with grant option;
Query OK, 0 rows affected (0.00 sec)

mysql> flush privileges;
Query OK, 0 rows affected (0.00 sec)

mysql> select host,user,password from user;
+-------------------------+-----------+-------------------------------------------+
| host                    | user      | password                                  |
+-------------------------+-----------+-------------------------------------------+
| localhost               | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| izm5e2qvb8hl5w1gjowpsxz | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| 127.0.0.1               | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| localhost               |           |                                           |
| izm5e2qvb8hl5w1gjowpsxz |           |                                           |
| localhost               | yxz       | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| localhost               | mysql     | *95FE99470B7B7CAF1E150B16ACCA48CDE7925813 |
| 39.107.105.166          | yxz       | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| 39.107.105.166          | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| %                       | root      | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| localhost               | eastmount | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
| 39.107.105.166          | eastmount | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |
+-------------------------+-----------+-------------------------------------------+
12 rows in set (0.00 sec)
```


可以看到主机host为39.107.105.166，用户名为eastmount，密码为123456的已经添加。


![](https://img-blog.csdn.net/20180222094354299)



但仍然报错“1045 - Access denied for user 'eastmount' (using password: YES)”。



![](https://img-blog.csdn.net/20180222094622846)





4.为eastmount@'%'用户授权

grant all privileges on *.* to eastmount@'%' identified by "123456" with grant option;


flush privileges;


service mysqld restart  



![](https://img-blog.csdn.net/2018022209495887)





此时权限开启成功，同时服务器的3306端口开启。










## 三. 阿里云服务器安全设置




如果现在您仍然无法通过阿里云主机CentOS连接远程MySQL数据库，那您需要设置阿里云的安全组规则，而网上更多的资料忽略了该步骤。下面进行简单叙述：

**第一步：打开阿里云服务器管理控制台页面，点击“管理”按钮**



![](https://img-blog.csdn.net/20180222090551386)





**第二步：在弹出的页面中，选择“本实例安全组”，然后点击“配置规则”**

![](https://img-blog.csdn.net/20180222090733927)





**第三步：在弹出如下界面中点击“添加安全组规则”**

![](https://img-blog.csdn.net/20180222090830666)





**第四步：在“添加安全组规则”界面选择“MySQL（3306）”，其中3306是MySQL数据库的端口号，同样可以设置其他的端口。**

![](https://img-blog.csdn.net/20180222090950884)





**第五步：授权对象填写“0.0.0.0/0”，表示允许任意公网IP登录。**

![](https://img-blog.csdn.net/20180222091124281)


设置成功如下图所示：



![](https://img-blog.csdn.net/20180222091420925)



**第六步：Navicat for MySQL软件登录，输入IP地址、用户名和密码，如下所示。**




![](https://img-blog.csdn.net/20180222091353188)



查看20180220df数据库的student表如下所示：



![](https://img-blog.csdn.net/20180222091449418)





参考阿里云官方文档：[阿里云服务器安全组设置 - 应用案例](https://help.aliyun.com/document_detail/25475.html?spm=5176.2020520101.0.0.433c4df5NCNdUP#allowRemoteAccess)










## 四. Python简单操作数据库




下面简单给出pymsql库操作MySQL数据库的Python代码，这是查询功能。

```python
#!usr/bin/python
#coding: utf-8
#author: yxz
import pymysql

#创建连接
con = pymysql.Connect(host='localhost', user='yxz', passwd='123456', db="Eastmount")
#创建游标
cur = con.cursor()
#执行SQL语句
cur.execute("select * from douban;")
#获取所有执行结果
res = cur.fetchall()
print(res)
#执行语句提交
con.commit()
cur.close()
#关闭连接
con.close()
```

![](https://img-blog.csdn.net/20180222091810619)



如果需要连接远程服务器的Python代码如下所示：

```python
#!usr/bin/python
#coding: utf-8
#author: yxz
import pymysql

#创建连接
con = pymysql.Connect(host='39.107.105.166', user='eastmount', passwd='123456', db="20180220df")
#创建游标
cur = con.cursor()
#执行SQL语句
cur.execute("insert into student(id,name,pwd) values('2','eastmount','20180222')")
cur.execute("select * from student;")
#获取所有执行结果
res = cur.fetchall()
print(res)
#执行语句提交
con.commit()
cur.close()
#关闭连接
con.close()
```

运行结果如下所示：

```python
[root@iZ2ze9134z8zlqupc9t6mzZ eastmount]# python test.py
((1, 'yangxiuzhang', '111111'), (2, 'eastmount', '20180222'))
[root@iZ2ze9134z8zlqupc9t6mzZ eastmount]#
```




总之，希望这篇基础文章对您有所帮助，尤其是刚接触云服务器的新手，如果您是高手，还请多提意见，共同提高。祝大家新年快乐，又一年过去了，娜我们来年一起进步加油。 
( By：Eastmount CSDN 2018-02-22 中午11点[http://blog.csdn.net/Eastmount](http://blog.csdn.net/Eastmount))







