
# Python数据操作方法封装类实例 - jiahaowanhao的博客 - CSDN博客


2018年07月05日 20:59:19[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：229


[Python数据操作方法封装类实例](http://cda.pinggu.org/view/26011.html)
这篇文章主要介绍了Python数据操作方法封装类,结合具体实例形式分析了Python针对数据库的连接、执行sql语句、删除、关闭等操作技巧,需要的朋友可以参考下
工作中经常会用到数据的插叙、单条数据插入和批量数据插入，以下是本人封装的一个类，推荐给各位：
\#!/usr/bin/env python
\# -*- coding:utf-8 -*-
\# Author:Eric.yue
import logging
import MySQLdb
class _MySQL(object):
def __init__(self,host, port, user, passwd, db):
self.conn = MySQLdb.connect(
host = host,
port = port,
user = user,
passwd = passwd,
db = db,
charset='utf8'
)
def get_cursor(self):
return self.conn.cursor()
def query(self, sql):
cursor = self.get_cursor()
try:
cursor.execute(sql, None)
result = cursor.fetchall()
except Exception, e:
logging.error("mysql query error: %s", e)
return None
finally:
cursor.close()
return result
def execute(self, sql, param=None):
cursor = self.get_cursor()
try:
cursor.execute(sql, param)
self.conn.commit()
affected_row = cursor.rowcount
except Exception, e:
logging.error("mysql execute error: %s", e)
return 0
finally:
cursor.close()
return affected_row
def executemany(self, sql, params=None):
cursor = self.get_cursor()
try:
cursor.executemany(sql, params)
self.conn.commit()
affected_rows = cursor.rowcount
except Exception, e:
logging.error("mysql executemany error: %s", e)
return 0
finally:
cursor.close()
return affected_rows
def close(self):
try:
self.conn.close()
except:
pass
def __del__(self):
self.close()
mysql = _MySQL('127.0.0.1', 3306, 'root', '123456', 'test')
def create_table():
table = """
CREATE TABLE IF NOT EXISTS `watchdog`(
`id` int(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
`name` varchar(100),
`price` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB charset=utf8;
"""
print mysql.execute(table)
def insert_data():
params = [('dog_%d' % i, i) for i in xrange(12)]
sql = "INSERT INTO `watchdog`(`name`,`price`) VALUES(%s,%s);"
print mysql.executemany(sql, params)
if __name__ == '__main__':
create_table()
insert_data()

