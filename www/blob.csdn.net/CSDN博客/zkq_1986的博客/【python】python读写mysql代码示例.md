# 【python】python读写mysql代码示例 - zkq_1986的博客 - CSDN博客





2019年01月23日 07:24:32[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：44
个人分类：[程序设计语言																[数据库](https://blog.csdn.net/zkq_1986/article/category/6476638)](https://blog.csdn.net/zkq_1986/article/category/6534515)








```python
# -*- coding:utf-8 -*-

import pymysql
import sys

__all__ = ['MySQL']


class MySQL(object):
    '''
    MySQL
    '''
    conn = ''
    cursor = ''

    def __init__(self, host='localhost', user='root', passwd='root', db='weibo_history', charset='utf8'):

        """MySQL Database initialization """
        try:
            self.conn = pymysql.connect(host, user, passwd, db)
        except Exception:
            sys.exit()

        self.cursor = self.conn.cursor()

    def query(self, sql):
        """  Execute SQL statement """
        return self.cursor.execute(sql)

    def commit(self):
        """ Return the results after executing SQL statement """
        return self.conn.commit()

    def show(self):
        """ Return the results after executing SQL statement """
        return self.cursor.fetchall()

    def __del__(self):
        """ Terminate the connection """
        self.conn.close()
        self.cursor.close()


# test
if __name__ == '__main__':
    mysql = MySQL()
    mysql.query("select user_id from user where has_relation = '0' limit 1")
    result = mysql.show()
    for row in result:
        userid = row[0]
        print(userid[0])
        sql = "UPDATE user SET has_relation = '1' WHERE user_id = '%s'" %userid
        print(sql)
        mysql.query(sql)
```






