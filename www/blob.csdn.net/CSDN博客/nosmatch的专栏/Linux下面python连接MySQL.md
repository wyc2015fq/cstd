# Linux下面python连接MySQL - nosmatch的专栏 - CSDN博客
2011年07月14日 20:38:07[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：995标签：[python																[linux																[数据库																[user																[table																[sql](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[脚本](https://blog.csdn.net/HDUTigerkin/article/category/845589)
MySQLdb模块使用相当简单，只要import MySQLdb环境即可使用。帮助可见：
>>> help ('MySQLdb')
>>> help ('MySQLdb.cursors')
1、导入模块
import MySQLdb
2、连接数据库
conn =   MySQLdb.Connection('host', 'user', 'password', 'dbname')
含义是：
引用
host：Mysql主机
user：连接使用的用户名
password：连接使用的用户名密码
dbname：默认打开的数据库
若该用户可打开其他数据库，可切换数据库：
conn.select_db('database name')
※若打开数据库信息不对，或失败，会触发OperationalError，类似：
引用
>>> conn=MySQLdb.Connection('localhost','linuxing','redflag','proftpd')
Traceback (most recent call last):
 File "<stdin>", line 1, in ?
 File "build/bdist.linux-i686/egg/MySQLdb/__init__.py", line 74, in Connect
 File "build/bdist.linux-i686/egg/MySQLdb/connections.py", line 170, in __init__
_mysql_exceptions.OperationalError: (1045, "Access denied for user ['linuxing'@'localhost'](mailto:' rel=) (using password: YES)")
请执行处理异常，并确认参数后，再尝试。
3、获取数据库指针
因该模块底层其实是调用C API的，所以，需要先得到当前指向数据库的指针：
cur =   conn.cursor()
4、获取数据库信息（select）
先使用指针对象执行SQL语句：
cur.execute('select * from tables')
其返回值为SQL语句得到的行数，如：2L，表示2行。
然后，可以从该对象的fetchone或fetchall方法得到行信息。
5、获取行信息
指针对象的fetchone()方法，是每次得到一行的tuple返回值：
引用
>>> row=cur.fetchone()
>>> print row
('user1', '52c69e3a57331081823331c4e69d3f2e', 1000L, 1000L, '/home/FTP/user1', '')
指针对象的fetchall()方法，是得到一组tuple，其内容为由行信息组成的tuple值：
引用
>>> cur.scroll(0,'absolute')
>>> row=cur.fetchall()
>>> print row
(('user1', '52c69e3a57331081823331c4e69d3f2e', 1000L, 1000L, '/home/FTP/user1', ''), ('user2', '7e58d63b60197ceb55a1c487989a3720', 1000L, 1000L, '/home/FTP/user2', None))
6、移动指针
当使用fetchone()方法是，指针是会发生移动的。所以，若不重置指针，那么使用fetchall的信息将只会包含指针后面的行内容。
手动移动指针使用：
cur.scroll(int,parm)
含义为：
引用
int：移动的行数，整数；在相对模式下，正数向下移动，负值表示向上移动。
parm：移动的模式，默认是relative，相对模式；可接受absoulte，绝对模式。
※实际上，我觉得应该尽量避免移动指针；而应从select语句得到全部结果后，交给python处理tuple，比较方便。但我们应该明白该指针的含义。
7、修改数据
修改数据，包括插入、更新、删除。它们都是使用指针对象的execute()方法执行：
cur.execute("insert  into table (row1, row2) values ('111', '222')")
cur.execute("update  table set   row1 = 'test'  where  row2 = 'row2' ")
cur.execute("delete from  table  where row1 = 'row1' ")
因单引号“'”用于SQL语句中的标识，所以，python中的字符串需使用双引号括住。
此外，也可以使用python的“格式化字符串”写法，简化命令，例如：
cur.execute("update  table set   row1 = '%s'  where  row2 = '%s' " %('value1','value2'))
※请注意，'%s'的单引号是SQL语句的间隔符，'value1'的单引号是python的字符串间隔符，其含义是不同的。是否需要间隔符，以及使用双引号还是单引号作为间隔，需根据其含义决定。例如，还有：
cur.execute("update FTPUSERS set passwd=%s where userid='%s' " %("md5('123')",'user2'))
这里，paswd=%s是因SQL的md5()函数是不需要单引号间隔的；"md5('123')"是python的字符串中含有单引号，所以用双引号括住。
8、提交修改
一般情况下，MySQLdb模块会自动提交修改。但我们在更新数据后，手动运行一次：
conn.commit()
