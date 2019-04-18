# Python下连接SQL - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:02:14[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：968
个人分类：[Python](https://blog.csdn.net/puqutogether/article/category/2595125)









在Python中，读取的文件有时候不是本地文件，需要从数据库上读取，这个时候我们就需要把Python和SQL数据库连接，对其中的数据down下来，数据处理分析之后，再把数据上传上去。



首先，我们需要一个Python中做SQL的模块：mysqldb，这个在我的百度云上有，安装就可以使用了。




然后需要用到的mysqldb的函数以及事例如下：





MySQLdb模块是python连接mysql数据库的一个模块，在操作mysql数据库是经常使用，在连接数据库时connect是最常用的一种方法，这个方法有好多参数，总结了一下，主要有一下几种：

connect()方法用于连接数据库，返回一个数据库连接对象。如果要连接一个位于www.gyyx.com服务器上名为mysql的MySQL数据库，连接串可以这样写：

db = MySQLdb.connect(host="www.gyyx.com",user="user",passwd="xxx",db="mysql" )

connect()的参数列表如下：

host，连接的数据库服务器主机名，默认为本地主机(localhost)。

user，连接数据库的用户名，默认为当前用户。

passwd，连接密码，没有默认值。

db，连接的数据库名，没有默认值。

conv，将文字映射到Python类型的字典。默认为MySQLdb.converters.conversions

cursorclass，cursor()使用的种类，默认值为MySQLdb.cursors.Cursor。

compress，启用协议压缩功能。

named_pipe，在windows中，与一个命名管道相连接。

init_command，一旦连接建立，就为数据库服务器指定一条语句来运行。

read_default_file，使用指定的MySQL配置文件。

read_default_group，读取的默认组。

unix_socket，在unix中，连接使用的套接字，默认使用TCP。

port，指定数据库服务器的连接端口，默认是3306。

连接对象的db.close()方法可关闭数据库连接，并释放相关资源。

连接对象的db.cursor([cursorClass])方法返回一个指针对象，用于访问和操作数据库中的数据。

连接对象的db.begin()方法用于开始一个事务，如果数据库的AUTOCOMMIT已经开启就关闭它，直到事务调用commit()和rollback()结束。

连接对象的db.commit()和db.rollback()方法分别表示事务提交和回退。

指针对象的cursor.close()方法关闭指针并释放相关资源。

指针对象的cursor.execute(query[,parameters])方法执行数据库查询。

指针对象的cursor.fetchall()可取出指针结果集中的所有行，返回的结果集一个元组(tuples)。

指针对象的cursor.fetchmany([size=cursor.arraysize])从查询结果集中取出多行，我们可利用可选的参数指定取出的行数。

指针对象的cursor.fetchone()从查询结果集中返回下一行。

指针对象的cursor.arraysize属性指定由cursor.fetchmany()方法返回行的数目，影响fetchall()的性能，默认值为1。

指针对象的cursor.rowcount属性指出上次查询或更新所发生行数。-1表示还没开始查询或没有查询到数据。






```python
# -*- coding: utf-8 -*-    
#mysqldb   
import time, MySQLdb   
  
#连接   
conn=MySQLdb.connect(host="localhost",user="root",passwd="root",db="test",charset="utf8") 
cursor = conn.cursor()   

#删除表
sql = "drop table if exists user"
cursor.execute(sql)

#创建
sql = "create table if not exists user(name varchar(128) primary key, created int(10))"
cursor.execute(sql)

#写入   
sql = "insert into user(name,created) values(%s,%s)"  
param = ("aaa",int(time.time()))   
n = cursor.execute(sql,param)   
print 'insert',n   
  
#写入多行   
sql = "insert into user(name,created) values(%s,%s)"  
param = (("bbb",int(time.time())), ("ccc",33), ("ddd",44) )
n = cursor.executemany(sql,param)   
print 'insertmany',n   

#更新   
sql = "update user set name=%s where name='aaa'"  
param = ("zzz")   
n = cursor.execute(sql,param)   
print 'update',n   
  
#查询   
n = cursor.execute("select * from user")   
for row in cursor.fetchall():   
    print row
    for r in row:   
        print r   
  
#删除   
sql = "delete from user where name=%s"  
param =("bbb")   
n = cursor.execute(sql,param)   
print 'delete',n   

#查询   
n = cursor.execute("select * from user")   
print cursor.fetchall()   

cursor.close()   
  
#提交   
conn.commit()
#关闭   
conn.close()
```





**1.引入MySQLdb库**



import MySQLdb 

**2.和数据库建立连接**

conn=MySQLdb.connect(host="localhost",user="root",passwd="sa",db="mytable",charset="utf8") 

提供的connect方法用来和数据库建立连接,接收数个参数,返回连接对象. 



比较常用的参数包括 

host:数据库主机名.默认是用本地主机. 

user:数据库登陆名.默认是当前用户. 

passwd:数据库登陆的秘密.默认为空. 

db:要使用的数据库名.没有默认值. 

port:MySQL服务使用的TCP端口.默认是3306.

charset:数据库编码.

更多关于参数的信息可以查这里 

http://mysql-python.sourceforge.net/MySQLdb.html 


然后,这个连接对象也提供了对事务操作的支持,标准的方法 

commit() 提交 

rollback() 回滚 


3.执行sql语句和接收返回值 

cursor=conn.cursor() 

n=cursor.execute(sql,param) 
首先,我们用使用连接对象获得一个cursor对象,接下来,我们会使用cursor提供的方法来进行工作.这些方法包括两大类:1.执行命令,2.接收返回值 


cursor用来执行命令的方法: 

callproc(self, procname, args):用来执行存储过程,接收的参数为存储过程名和参数列表,返回值为受影响的行数 
execute(self, query, args):执行单条sql语句,接收的参数为sql语句本身和使用的参数列表,返回值为受影响的行数 

executemany(self, query, args):执行单条sql语句,但是重复执行参数列表里的参数,返回值为受影响的行数 

nextset(self):移动到下一个结果集 


cursor用来接收返回值的方法: 
fetchall(self):接收全部的返回结果行. 

fetchmany(self, size=None):接收size条返回结果行.如果size的值大于返回的结果行的数量,则会返回cursor.arraysize条数据. 

fetchone(self):返回一条结果行. 

scroll(self, value, mode='relative'):移动指针到某一行.如果mode='relative',则表示从当前所在行移动value条,如果mode='absolute',则表示从结果集的第一行移动value条. 


下面的代码是一个完整的例子. 


```python
#使用sql语句,这里要接收的参数都用%s占位符.要注意的是,无论你要插入的数据是什么类型,占位符永远都要用%s 
sql="insert into cdinfo values(%s,%s,%s,%s,%s)" 
#param应该为tuple或者list 
param=(title,singer,imgurl,url,alpha) 
#执行,如果成功,n的值为1 
n=cursor.execute(sql,param) 

#再来执行一个查询的操作 
cursor.execute("select * from cdinfo") 
#我们使用了fetchall这个方法.这样,cds里保存的将会是查询返回的全部结果.每条结果都是一个tuple类型的数据,这些tuple组成了一个tuple 
cds=cursor.fetchall() 
#因为是tuple,所以可以这样使用结果集 
print cds[0][3] 
#或者直接显示出来,看看结果集的真实样子 
print cds
```



MySQLdb呢，其实和Python内置的sqlite3的使用方法基本相同。MySQLdb默认情况下，查询结果行都是返回tuple，访问的时候不是很方便，必须按照0，1这样读取。

结果就像这样：

(u'ccc', 33L)

(u'ddd', 44L)

(u'zzz', 1340790602L)


以前使用sqllite3的时候，可以修改过Connection对象的row_factory属性，以便使用sqlite3.Row，这样结果集中的数据行就是字典形式的，可以用字段名访问，那么MySQLdb中是不是也有这样的方法呢，经过在网上搜索发现，MySQLdb中有DictCursor，要做到这点也很简单，那就是建立数据库连接是传递cusorclass参数，或者在获取Cursor对象时传递cusorclass参数即可：

conn=MySQLdb.connect(host="localhost",user="root",passwd="root",db="test",charset="utf8",cursorclass=MySQLdb.cursors.DictCursor)  

cursor = conn.cursor()  

或者

conn=MySQLdb.connect(host="localhost",user="root",passwd="root",db="test",charset="utf8")  

cursor = conn.cursor(cursorclass=MySQLdb.cursors.DictCursor)  

返回结果就是这样：

{'name': u'ccc', 'created': 33L}

{'name': u'ddd', 'created': 44L}

{'name': u'zzz', 'created': 1340790602L}



#如果需要批量的插入数据,就这样做 

sql="insert into cdinfo values(0,%s,%s,%s,%s,%s)" 

#每个值的集合为一个tuple,整个参数集组成一个tuple,或者list 

param=((title,singer,imgurl,url,alpha),(title2,singer2,imgurl2,url2,alpha2)) 

#使用executemany方法来批量的插入数据.这真是一个很酷的方法! 

n=cursor.executemany(sql,param) 

**4.关闭数据库连接**

需要分别的关闭指针对象和连接对象.他们有名字相同的方法 

cursor.close() 

conn.close() 


四步完成,基本的数据库操作就是这样了.下面是两个有用的连接 

MySQLdb用户指南: http://mysql-python.sourceforge.net/MySQLdb.html 

MySQLdb文档: http://mysql-python.sourceforge.net/MySQLdb-1.2.2/public/MySQLdb-module.html

**5 编码（防止乱码）**

需要注意的点：

**1 Python文件设置编码 utf-8 （文件前面加上 #encoding=utf-8)**
**    2 MySQL****数据库charset=utf-8**
**    3 Python****连接MySQL是加上参数 charset=utf8**
**    4 ****设置Python的默认编码为 utf-8 (sys.setdefaultencoding(utf-8)**





**[python]**[view
 plain](http://blog.csdn.net/vincent_czz/article/details/7697039#)[copy](http://blog.csdn.net/vincent_czz/article/details/7697039#)

- #encoding=utf-8 
- **import** sys   
- **import** MySQLdb   
- 
- reload(sys)   
- sys.setdefaultencoding('utf-8')   
- 
- db=MySQLdb.connect(user='root',charset='utf8')   





注：MySQL的配置文件设置也必须配置成utf8


设置 MySQL 的 my.cnf 文件，在 [client]/[mysqld]部分都设置默认的字符集（通常在/etc/mysql/my.cnf)： 

[client]

default-character-set = utf8

[mysqld]

default-character-set = utf8




