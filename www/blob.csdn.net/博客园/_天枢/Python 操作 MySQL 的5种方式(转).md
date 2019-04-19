# Python 操作 MySQL 的5种方式(转) - _天枢 - 博客园
## [Python 操作 MySQL 的5种方式(转)](https://www.cnblogs.com/yhleng/p/9798907.html)
2018-10-16 16:41 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9798907)
# Python 操作 MySQL 的5种方式
不管你是做数据分析，还是网络爬虫，Web 开发、亦或是机器学习，你都离不开要和数据库打交道，而 MySQL 又是最流行的一种数据库，这篇文章介绍 Python 操作 MySQL 的5种方式，你可以在实际开发过程中根据实际情况合理选择。
### 1、MySQLdb
MySQLdb又叫MySQL-python ，是 Python 连接 MySQL 最流行的一个驱动，很多框架都也是基于此库进行开发，遗憾的是它只支持 Python2.x，而且安装的时候有很多前置条件，因为它是基于C开发的库，在 Windows 平台安装非常不友好，经常出现失败的情况，现在基本不推荐使用，取代的是它的衍生版本。
```
# 前置条件
sudo apt-get install python-dev libmysqlclient-dev # Ubuntu
sudo yum install python-devel mysql-devel # Red Hat / CentOS
# 安装
pip install MySQL-python
Windows 直接通过下载 exe 文件安装
```
```
#!/usr/bin/python
import MySQLdb
db = MySQLdb.connect(
     host="localhost",    # 主机名
     user="root",         # 用户名
     passwd="pythontab.com",  # 密码
     db="testdb")        # 数据库名称
# 查询前，必须先获取游标
cur = db.cursor()
# 执行的都是原生SQL语句
cur.execute("SELECT * FROM mytable")
for row in cur.fetchall():
    print(row[0])
db.close()
```
### 2、mysqlclient
由于 MySQL-python(MySQLdb) 年久失修，后来出现了它的 Fork 版本 mysqlclient，完全兼容 MySQLdb，同时支持 Python3.x，是 Django ORM的依赖工具，如果你想使用原生 SQL 来操作数据库，那么推荐此驱动。安装方式和 MySQLdb 是一样的，Windows 可以在 https://www.lfd.uci.edu/~gohlke/pythonlibs/#mysqlclient 网站找到 对应版本的 whl 包下载安装。
```
pip install mysqlclient
```
### 3、PyMySQL
PyMySQL 是纯 Python 实现的驱动，速度上比不上 MySQLdb，最大的特点可能就是它的安装方式没那么繁琐，同时也兼容 MySQL-python
```
pip install PyMySQL
# 为了兼容mysqldb，只需要加入
pymysql.install_as_MySQLdb()
```
例子:
```
import pymysql
conn = pymysql.connect(host='127.0.0.1', user='root', passwd="pythontab.com", db='testdb')
cur = conn.cursor()
cur.execute("SELECT Host,User FROM user")
for r in cur:
    print(r)
cur.close()
conn.close()
```
### 4、peewee
写原生 SQL 的过程非常繁琐，代码重复，没有面向对象思维，继而诞生了很多封装 wrapper 包和 ORM 框架，ORM 是 Python 对象与数据库关系表的一种映射关系，有了 ORM 你不再需要写 SQL 语句。提高了写代码的速度，同时兼容多种数据库系统，如sqlite, mysql、postgresql，付出的代价可能就是性能上的一些损失。如果你对 Django 自带的 ORM 熟悉的话，那么 peewee的学习成本几乎为零。它是 Python 中是最流行的 ORM 框架。
安装
```
pip install peewee
```
例子：
```
import peewee
from peewee import *
db = MySQLDatabase('testdb', user='root', passwd='pythontab.com')
class Book(peewee.Model):
    author = peewee.CharField()
    title = peewee.TextField()
    class Meta:
        database = db
Book.create_table()
book = Book(author="pythontab", title='pythontab is good website')
book.save()
for book in Book.filter(author="pythontab"):
    print(book.title)
```
官方文档：http://docs.peewee-orm.com/en/latest/peewee/installation.html
### 5、SQLAlchemy
如果想找一种既支持原生 SQL，又支持 ORM 的工具，那么 SQLAlchemy 是最好的选择，它非常接近 Java 中的 Hibernate 框架。
```
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy_declarative import Address, Base, Person
class Address(Base):
    __tablename__ = 'address'
    id = Column(Integer, primary_key=True)
    street_name = Column(String(250))
engine = create_engine('sqlite:///sqlalchemy_example.db')
Base.metadata.bind = engine
DBSession = sessionmaker(bind=engine)
session = DBSession()
# Insert a Person in the person table
new_person = Person(name='new person')
session.add(new_person)
session.commit()
```

