
# [python] 专题九.Mysql数据库编程基础知识 - 杨秀璋的专栏 - CSDN博客

2016年08月10日 21:59:32[Eastmount](https://me.csdn.net/Eastmount)阅读数：5044所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)[数据库实战开发设计与优化](https://blog.csdn.net/column/details/14842.html)



在Python网络爬虫中，通常是通过TXT纯文本方式存储，其实也是可以存储在数据库中的；同时在WAMP（Windows、Apache、MySQL、PHP或Python）开发网站中，也可以通过Python构建网页的，所以这篇文章主要讲述Python调用MySQL数据库相关编程知识。从以下几个方面进行讲解：
1.配置MySLQ
2.SQL语句基础知识
3.Python操作MySQL基础知识
4.Python调用MySQL示例

## 一. 配置MySQL

首先下载mysql-5.0.96-winx64，安装过程如下图所示。
1.安装MySQL 5.0
![](https://img-blog.csdn.net/20150512004058210)![](https://img-blog.csdn.net/20150512004130127)
![](https://img-blog.csdn.net/20150512004348502)![](https://img-blog.csdn.net/20150512004219533)
2.选择手动配置、服务类型、通用多功能型和安装路径
![](https://img-blog.csdn.net/20150512004628075)![](https://img-blog.csdn.net/20150512004740178)
![](https://img-blog.csdn.net/20150512004807151)![](https://img-blog.csdn.net/20150512004643641)
3.设置数据库访问量连接数为15、端口为3306（代码中设置URL用到）、编码方式为utf-8
![](https://img-blog.csdn.net/20150512004903745)![](https://img-blog.csdn.net/20150512005117097)
![](https://img-blog.csdn.net/20150512005034568)![](https://img-blog.csdn.net/20150512005102601)
4.设置默认超级root用户的用户名和密码，最后安装成功
![](https://img-blog.csdn.net/20150512005402410)![](https://img-blog.csdn.net/20150512005426918)


## 二. SQL语句基础知识
安装MySQL 5.0成功后，进行数据库的简单操作。
1.运行MySQL输入默认用户密码123456
![](https://img-blog.csdn.net/20150512005659517)
2.创建数据库test01和使用数据库(第二次调用直接use database)
create database test01;
![](https://img-blog.csdn.net/20150512005748717)
显示数据库中包含的数据库：show databases;
![](https://img-blog.csdn.net/20160810211616591)
3.创建表student，其中学号为主键
create table student(username varchar(20),password varchar(20),stuid int primary key);
![](https://img-blog.csdn.net/20150512005909322)
4.显示表结构，使用语句desc student
![](https://img-blog.csdn.net/20150512005955171)
5.向学生表中插入数据并显示查询的数据
![](https://img-blog.csdn.net/20150512010232001)
6.删除表：drop table student;
![](https://img-blog.csdn.net/20160810211857845)
7.更新数据
update student set password=’000000’ where stuid=’1’;
![](https://img-blog.csdn.net/20160810212002127)

8.删除数据
Delete from student where username=’eastmount;
![](https://img-blog.csdn.net/20160810212032941)
此时MySQL操作数据库基本讲解结束，你同样可以实现数据库的增删改查、事务、存储过程等操作，建议安装可视化的软件来替代黑框，或使用Navicat for MySQL软件即可。代码如下：
```python
Enter password: ******
mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| test               |
| test01             |
+--------------------+
5 rows in set (0.00 sec)
mysql> use test01;
Database changed
mysql> show tables;
Empty set (0.00 sec)
mysql> create table student(username varchar(20),
    ->                  password varchar(20),
    ->                  stuid int primary key);
Query OK, 0 rows affected (0.33 sec)
mysql> show tables;
+------------------+
| Tables_in_test01 |
+------------------+
| student          |
+------------------+
1 row in set (0.00 sec)
mysql> desc student;
+----------+-------------+------+-----+---------+-------+
| Field    | Type        | Null | Key | Default | Extra |
+----------+-------------+------+-----+---------+-------+
| username | varchar(20) | YES  |     | NULL    |       |
| password | varchar(20) | YES  |     | NULL    |       |
| stuid    | int(11)     | NO   | PRI | NULL    |       |
+----------+-------------+------+-----+---------+-------+
3 rows in set (0.03 sec)
mysql> insert student(username, password, stuid)
    -> values('eastmount','123456',1)
    -> ;
Query OK, 1 row affected (0.05 sec)
mysql> select * from student;
+-----------+----------+-------+
| username  | password | stuid |
+-----------+----------+-------+
| eastmount | 123456   |     1 |
+-----------+----------+-------+
1 row in set (0.00 sec)
mysql> update student set password='000000' where stuid='1';
Query OK, 1 row affected (0.10 sec)
Rows matched: 1  Changed: 1  Warnings: 0
mysql> select * from student;
+-----------+----------+-------+
| username  | password | stuid |
+-----------+----------+-------+
| eastmount | 000000   |     1 |
+-----------+----------+-------+
1 row in set (0.00 sec)
mysql> delete from student where username='eastmount';
Query OK, 1 row affected (0.08 sec)
mysql> select * from student;
Empty set (0.00 sec)
mysql>
```


## 三. Python调用MySQL基础知识


通常的安装方法是使用：pip install mysql 安装Python的MySQL库，但是总会报错。常见错误如：
Microsoft Visual C++ 9.0 is required  (Unable to find vcvarsall.bat)
mysql.c(42) : fatal error C1083: Cannot open include file: 'config-win.h': No such file or directory
这些可能是驱动等问题。
![](https://img-blog.csdn.net/20160810213533665)
**正确安装方法：**
建议下载一个MySQL-python-1.2.3.win-amd64-py2.7.exe文件进行安装。
官网地址：[https://pypi.python.org/pypi/MySQL-python/](https://pypi.python.org/pypi/MySQL-python/)
下载地址：[http://download.csdn.net/detail/eastmount/9598651](http://download.csdn.net/detail/eastmount/9598651)
![](https://img-blog.csdn.net/20160810213558620)

下面我们要详细了解Python数据库API。从Python中访问数据库需要接口程序，接口程序是一个Python模块，它提供数据库客户端库（通常是C语言写成的）的接口供你访问。注意：Python接口程序都一定要遵守Python DB-API规范。
DB-API是一个规范。它定义了一系列必须的对象和数据库存取方式，以便为各种各样的底层数据库系统和多种多样的数据库接口程序提供一致的访问接口。DB-API为不同的数据库提供了一致的访问接口，在不同的数据库之间移植代码成为一件轻松的事情。
下面简单介绍DB-API的使用方法。
**1.****模块属性**
DB-API规范里的以下特性和属性必须提供。一个DB-API兼容模块定义如下所示：
```python
apilevel：模块兼容的DB-API版本号
threadsafety:线程安全级别
paramstyle:支持sql语句参数风格
connect():连接数据库
```
Python调用MsSQL需要导入MySQLdb库，如下：

```python
import MySQLdb
```
**2.connect()函数**
其中主要使用的方法是connect对象。connect()方法生成一个connect对象，用于访问数据库，其参数如下：
```python
user:Username
password:Password
host:Hostname
database:DatabaseName
dsn:Data source name
```
注意并非所有的接口程序都严格按照这种格式，如MySQLdb。
```python
import MySQLdb
conn = MySQLdb.connect(host='localhost', db='test01', user='root', passwd='123456', port=3306, charset='utf8')
```
connect()对象方法如下：
```python
lose():关闭数据库连接，或者关闭游标对象
commit():提交当前事务
rollback()：取消当前事务
cursor()：创建游标或类游标对象
errorhandler(cxn,errcls,errval)：作为已给游标的句柄
```
注意，执行close()方法则上述的连接对象方法不能再使用，否则发生异常。commit()、rollback()、cursor()或许更对于支持事务的数据库更有意义。
数据库事务(Database Transaction) ，是指作为单个逻辑工作单元执行的一系列操作，要么完整地执行，要么完全地不执行。 一旦你完成了数据库连接，关闭了游标对象，然后在执行commit()提交你的操作，然后关闭连接。
**3.游标对象**
上面说了connect()方法用于提供连接数据库的接口，如果要对数据库操作那么还需要使用游标对象。游标对象的属性和方法：
```python
fetchone()：可以看作fetch(取出) one(一个)，也就是得到结果集的下一行(一行)。
fetchmany(size):可以看作fetch(取出)many(多个)，这里的参数是界限，得到结果集的下几行(几行)
fetchall()：顾名思义，取得所有。
execute(sql)：执行数据库操作，参数为sql语句。
close():不需要游标时尽可能的关闭
```
下面通过简单的示例进行讲解。


## 四. Python调用MySQL示例
在前面数据库中我们创建了数据库“test01”和表“student”，同时插入了数据。那么，怎样通过Python来显示呢？
**1.查询所有数据库**
首先，我们查看本地数据库中所包含的数据库名称，通过“show databases”语句。

```python
import MySQLdb
 
try:
    conn=MySQLdb.connect(host='localhost',user='root',passwd='123456',port=3306)
    cur=conn.cursor()
    res = cur.execute('show databases')
    print res
    for data in cur.fetchall():
        print '%s' % data
    cur.close()
    conn.close()
except MySQLdb.Error,e:
     print "Mysql Error %d: %s" % (e.args[0], e.args[1])
```
其中通过链接数据库代码为：
conn=MySQLdb.connect(host='localhost',user='root',passwd='123456',port=3306)
访问root超级用户，其密码为“123456”，端口为“3306”，其结果如下：
![](https://img-blog.csdn.net/20160810214928485)
如果不知道本地数据库的名称，可以通过该方法，先查询数据库中包含哪些数据库，然后再连接该数据库进行相关的操作。
**2.查询表**
下面介绍查询表student中数据，代码如下，代码的具体含义是通过connect()连接数据库，通过conn.cursor()定义游标，然后调用游标的excute(sql)执行数据库操作，此处为查询操作，再通过fetchall()函数获取所有数据。
```python
# coding:utf-8 
import MySQLdb
 
try:
    conn=MySQLdb.connect(host='localhost',user='root',passwd='123456',port=3306, db='test01', charset='utf8')
    cur=conn.cursor()
    res = cur.execute('select * from student')
    print u'表中包含',res,u'条数据\n'
    print u'数据如下:(姓名 密码 序号)'
    for data in cur.fetchall():
        print '%s %s %s' % data
    cur.close()
    conn.close()
except MySQLdb.Error,e:
     print "Mysql Error %d: %s" % (e.args[0], e.args[1])
```
输出结果如图所示：
![](https://img-blog.csdn.net/20160810214940121)
对应的MySQL中的结果是一致的，下图是对应的结果。
![](https://img-blog.csdn.net/20160810214950485)
**3.创建表**
下面这段代码是创建一张教师表，主要是通过commit()提交数据。
```python
# coding:utf-8 
import MySQLdb
 
try:
    conn=MySQLdb.connect(host='localhost',user='root',passwd='123456',port=3306, db='test01', charset='utf8')
    cur=conn.cursor()
    
    #查看表
    print u'插入前包含表:'
    cur.execute('show tables')
    for data in cur.fetchall():
        print '%s' % data
    #插入数据
    sql = '''create table teacher(id int not null primary key auto_increment,
                                name char(30) not null,
                                sex char(20) not null
          )'''
    cur.execute(sql)
    #查看表
    print u'\n插入后包含表:'
    cur.execute('show tables')
    for data in cur.fetchall():
        print '%s' % data
    cur.close()
    conn.commit()
    conn.close()
except MySQLdb.Error,e:
     print "Mysql Error %d: %s" % (e.args[0], e.args[1])
```
输出结果如下所示，插入教师表，包含字段：教师序号（id）、教师名称（name）、教师性别（sex）。
![](https://img-blog.csdn.net/20160810215048685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
插入数据也可以通过execute(sql)方法实现，如：
cur.execute("insert into student values( 'yxz', '111111', '10')")
但插入的新数据通常是通过变量进行赋值，而不是固定的，所以要对这条语句中的值做修改。我们可以做如下修改：

```python
# coding:utf-8 
import MySQLdb
 
try:
    conn=MySQLdb.connect(host='localhost',user='root',passwd='123456',port=3306, db='test01')
    cur=conn.cursor()
    
    #插入数据
    sql = '''insert into student values(%s, %s, %s)'''
    cur.execute(sql, ('yxz','111111', '10'))
    #查看数据
    print u'\n插入数据:'
    cur.execute('select * from student')
    for data in cur.fetchall():
        print '%s %s %s' % data
    cur.close()
    conn.commit()
    conn.close()
except MySQLdb.Error,e:
     print "Mysql Error %d: %s" % (e.args[0], e.args[1])
```
输出结果如下所示：
```python
>>> 
插入数据:
esatmount 123456 1
yangxiuzhang 123456 2
xiaoy 123456 3
yxz 111111 10
>>>
```
同样，对数据库的增删改插都可以进行，请读者自行阅读。
推荐资料：[python使用mysql数据库 - 虫师](http://www.cnblogs.com/fnng/p/3565912.html)
后面我会结合Python爬虫讲述，如何将爬取的内容存储在数据库中，如我CSDN的博客，爬取博客标题、发布时间、阅读量和评论数。

![](https://img-blog.csdn.net/20160810220119444)
MySQL数据库中结果如下图所示：
![](https://img-blog.csdn.net/20160810220214340)
最后希望文章对你有所帮助，如果文章中存在不足或错误的地方，还请海涵~还是那句话，挺享受现在的老师生活，不论科研、项目，还是教学，很充实，加油！
但行好事，莫问前程。
待随满天李桃，再追学友趣事。
(By:Eastmount 2016-08-10 晚上10点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))





