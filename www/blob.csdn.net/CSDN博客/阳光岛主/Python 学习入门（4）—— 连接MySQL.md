
# Python 学习入门（4）—— 连接MySQL - 阳光岛主 - CSDN博客

2013年10月26日 17:39:50[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：9271所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



下载[MySQL for Python](http://sourceforge.net/projects/mysql-python/)，最新版[MySQL-python-1.2.4b4.tar.gz](http://sourceforge.net/projects/mysql-python/files/latest/download)

1） 提前安装：mysql_config 环境
否则后面python setup.py build 会提示找不到 “EnvironmentError: mysql_config not found”，安装命令如下：
sudo apt-get install libmysqlclient-dev
sudo apt-get install python-dev（解决fatal error: Python.h: No such file or directory）
CentOS 安装yum install mysql-devel和yum install python-devel（解决error: command 'gcc' failed with exit status 1）

2） 然后，再安装MySQLdb
$ tar zxvf MySQL-python-1.2.2.tar.gz
$ cd MySQL-python-1.2.2
$ sudo python setup.py build
$ sudo python setup.py install

3） 验证成功安装
homer@ubuntu:~/myCode/python$ python
Python 2.7.3 (default, Aug  1 2012, 05:14:39)
[GCC 4.6.3] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>>import MySQLdb
>>>
import MySQLdb没有出错，说明安装成功！

**测试示例：**

```python
import MySQLdb
db = MySQLdb.connect("localhost","myusername","mypassword","mydb" )
cursor = db.cursor()
cursor.execute("SELECT VERSION()")
data = cursor.fetchone()    
print "Database version : %s " % data    
db.close()
```


**python 连接mysql示例：**

```python
####################
# IT-Homer
# 2013-05-10
####################

import MySQLdb

db = MySQLdb.connect(host="localhost", user="root", passwd="abcd1234", db="testDB")
cursor = db.cursor()
cursor.execute("Select * from gameTestDB limit 10")
result = cursor.fetchall()
for row in result:
  #print row
  #print row[0], row[1], row[2]
  #print '%s, %s, %s' % (row[0], row[1], row[2])
  print ', '.join([str(row[0]), str(row[1]), str(row[2])])
cursor.close()

'''
import sys
import MySQLdb
reload(sys)
sys.setdefaultencoding('utf-8')

db = MySQLdb.connect(user='root', passwd='abcd1234', charset='utf8')
cur = db.cursor()
cur.execute('use testDB')
cur.execute('select * from gameTestDB limit 10')
f = file("/home/homer/tmp_mysql.txt", 'w')
for row in cur.fetchall():
  f.write(str(row))
  f.write("\n")
f.close()
cur.close()
'''
```


```python
####################
# IT-Homer
# 2013-05-10
####################

import MySQLdb
# local mysql
# db = MySQLdb.connect(host="localhost", user="root", passwd="abcd1234", db="testDB")
# aws rds mysql
db = MySQLdb.connect(host="ithomer.aliyun.com", user="ithomer", passwd="abcd1234", db="dman")
cursor = db.cursor()
cursor.execute("Select * from score limit 10")
result = cursor.fetchall()
for row in result:
  #print row
  #print row[0], row[1], row[2]
  #print '%s, %s, %s' % (row[0], row[1], row[2])
  print ', '.join([str(row[0]), str(row[1]), str(row[2])])
cursor.close()

'''
import sys
import MySQLdb
reload(sys)
sys.setdefaultencoding('utf-8')

db = MySQLdb.connect(user='root', passwd='abcd1234', charset='utf8')
cur = db.cursor()
cur.execute('use testDB')
cur.execute('select * from gameTestDB limit 10')
f = file("/home/homer/tmp_mysql.txt", 'w')
for row in cur.fetchall():
  f.write(str(row))
  f.write("\n")
f.close()
cur.close()
```

**python 连接mongodb**
1） 安装pymongo
pymongo[下载](https://pypi.python.org/packages/source/p/pymongo/)，最新[pymongo-2.6.tar.gz](https://pypi.python.org/packages/source/p/pymongo/pymongo-2.6.tar.gz)
安装
$ tar zxvf pymongo-2.6.tar.gz
$ cdpymongo-2.6
$ sudo python setup.py build
$ sudo python setup.py install

2）连接mongodb

```python
#!/usr/bin/python
import pymongo
import random
HOST = '172.27.22.21'
PORT = 27017
_DB='test'
_TABLE='testuser'

conn = pymongo.Connection("172.27.22.21", 27017)
db = conn[_DB]  # get db
db.authenticate("yanggang", "123456")
table = db[_TABLE]      # get collection
table.drop()
table.save({"id":1, "name":"homer", "age":18})
for id in range(2,10):
  name = random.choice(['it', 'homer', 'sunboy', 'yanggang'])
  age = random.choice([10, 20, 30, 40, 50, 60])
  table.insert({"id":id, "name":name, "age":age})
cursor = table.find()
for user in cursor:
  print user

'''
conn = pymongo.Connection("172.27.22.21", 27017)
db = conn.test
db.authenticate("yanggang", "123456")
db.testuser.drop()
db.testuser.save({"id":1, "name":"homer", "age":18})
for id in range(2,10):
  name = random.choice(['it', 'homer', 'sunboy', 'yanggang'])
  age = random.choice([10, 20, 30, 40, 50, 60])
  db.testuser.insert({"id":id, "name":name, "age":age})
cursor = db.testuser.find()
for user in cursor:
  print user
'''
```
运行结果
![](https://img-blog.csdn.net/20131030133000109)


python 连接 Redis
1）前往[redis-py](https://github.com/andymccurdy/redis-py)下载发布版本[release](https://github.com/andymccurdy/redis-py/releases)，最新发布版本：[redis-py-2.8.0.zip](https://github.com/andymccurdy/redis-py/archive/2.8.0.tar.gz)
2）解压 redis-py-2.8.0.zip：unzip  redis-py-2.8.0.zip， 安装：sudo python setup.py install
3）验证安装成功：
\# python
>>> import redis
>>>

**redis 设置密码**
a） 修改配置文件
viim  redis.conf
b） 添加一行
requirepass '123456'
c）重启redis服务
/usr/local/bin/redis-server  /etc/redis.conf
d）登陆 redis-cli
$ redis-cli
127.0.0.1:6379>set foo bar
(error) NOAUTH Authentication required.// 设置了密码，操作没有权限
127.0.0.1:6379> help auth// 查看auth命令帮助
AUTH password
summary: Authenticate to the server
since: 1.0.0
group: connection
127.0.0.1:6379> auth '123456'// 输入密码，权限认证
OK
127.0.0.1:6379> set foo bar// 密码权限认证成功后，可以操作
OK
127.0.0.1:6379> get foo
"bar"

**redis-cli 远程连接**
$redis-cli --help
redis-cli 2.8.12
Usage: redis-cli [OPTIONS] [cmd [arg [arg ...]]]
-h <hostname>      Server hostname (default: 127.0.0.1).
-p <port>          Server port (default: 6379).
-s <socket>        Server socket (overrides hostname and port).
-a <password>      Password to use when connecting to the server.

**redis-cli 远程连接命令**
redis-cli -h 123.10.78.100 -p 6379 -a '123456'
注意：为了安全，redis不要用默认端口（6379），强烈推荐使用密码（requirepass 'xxx'），否则很容易被别人访问！


4）简单示例：

```python
>>> import redis
>>> r = redis.StrictRedis(host='localhost', port=6379, db=0)
>>> r.set('foo', 'bar')
True
>>> r.get('foo')
'bar'
```
5）python脚本示例
```python
#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
import redis
_REDIS_HOST = '172.27.9.104'
_REDIS_PORT = 6379
_REDIS_DB = 0

def read_redis():
    r = redis.Redis(host=_REDIS_HOST, port=_REDIS_PORT, db=_REDIS_DB)
    # 删除当前数据库的所有数据
    r.flushdb()             
    r.set('foo', 'bar')
    print(r.get('foo'))         # bar
    r.set('blog', 'ithomer.net')
    r.set('name', 'yanggang')
    # 查询没有key，返回 None
    print(r.get('none123'))     # None
    # 库里有多少key，就多少条数据
    print(r.dbsize())           # 3
    # 列出所有键值
    print(r.keys())             # ['blog', 'foo', 'name']
if __name__ == "__main__":
    read_redis()
```
运行结果：
bar
None
3
['blog', 'foo', 'name']


**参考推荐：**
[Python 連接 MySQL](http://www.blogjava.net/huyi2006/articles/247966.html)
[MySQLdb User's Guide](http://mysql-python.sourceforge.net/MySQLdb.html)
[Python 字符串操作](http://www.cnblogs.com/huangcong/archive/2011/08/29/2158268.html)
[mysql_config not found](http://stackoverflow.com/questions/7475223/mysql-config-not-found-when-installing-mysqldb-python-interface)（stackover flow）

[python 创建mysql数据库](http://hi.baidu.com/lionpanther/item/ead7261c05e6807b7b5f259a)
[python连接mongodb并操作 ](http://blog.chinaunix.net/uid-23078678-id-2554529.html)
[redis-py](https://github.com/andymccurdy/redis-py)（github）



