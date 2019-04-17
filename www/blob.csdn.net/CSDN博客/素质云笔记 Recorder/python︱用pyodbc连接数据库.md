# python︱用pyodbc连接数据库 - 素质云笔记/Recorder... - CSDN博客





2017年06月29日 17:44:47[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5421







- 直接连接数据库和创建一个游标（cursor)
- 数据查询（SQL语句为 select …from..where）

## 1、pyodbc连接

```java
import pyodbc

cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER=xxx;DATABASE=xxx;UID=xxx;PWD=xxx')
cursor = cnxn.cursor()

cursor.execute("SELECT  id FROM datatable")
row = cursor.fetchone()
```

#### 其中：
- pyodbc.connect中，SERVER是服务器名称
- cursor.execute为执行语句
- Row这个类，类似于一个元组，但是他们也可以通过字段名进行访问

#### 其中：

execute执行的时候， 

有很多SQL语句用单行来写并不是很方便，所以你也可以使用三引号的字符串来写：

```python
cursor.execute("""
               select user_id, user_name
                 from users
                where last_logon < '2001-01-01'
                  and bill_overdue = 'y'
               """)
```

## 2、row 中的数据抽取

有中文的时候，可以：

```
if row:
    print ' '.join(row)
```

如果要抽取每一个内容，可以定位：

```
for rows in row:
    print row.id
```

#### 简易抽取：

假如你使用的是三引号，那么你也可以这样使用：

```python
deleted = cursor.execute("""
                         delete
                           from products
                          where id <> 'pyodbc'
                         """).rowcount
```

## 3、简易命名

有些数据库（比如SQL Server）在计数时并没有产生列名，这种情况下，你想访问数据就必须使用下标。当然你也可以使用“as”关键字来取个列名

```
row = cursor.execute("select count(*) as user_count from users").fetchone()
print '%s users' % row.user_count
```

## 连接MYSQL

```
import pymysql

# 打开数据库连接
db = pymysql.connect("localhost","testuser","test123","TESTDB" )

# 使用 cursor() 方法创建一个游标对象 cursor
cursor = db.cursor()

# 使用 execute()  方法执行 SQL 查询 
cursor.execute("SELECT VERSION()")

# 使用 fetchone() 方法获取单条数据.
data = cursor.fetchone()

print ("Database version : %s " % data)

# 关闭数据库连接
db.close()
```

连接数据库TESTDB使用的用户名为 “testuser” ，密码为 “test123”,你可以可以自己设定或者直接使用root用户名及其密码，Mysql数据库用户授权请使用Grant命令。

参考：[Python3 MySQL 数据库连接](http://www.runoob.com/python3/python3-mysql.html)

### 主要参考：

[pyodbc的简单使用](https://my.oschina.net/zhengyijie/blog/35587)



