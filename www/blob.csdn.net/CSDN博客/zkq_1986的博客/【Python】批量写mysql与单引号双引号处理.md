# 【Python】批量写mysql与单引号双引号处理 - zkq_1986的博客 - CSDN博客





2018年08月29日 14:24:40[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：451








```
# coding:utf-8

"""
寻找参考文献异常的记录
"""

from util.DBUtil import MySQL
import pymysql

mysql = MySQL()

def run():
    mysql.query("select pmid,section, reference_ids, paragraph,pmc from full_texts")
    result = mysql.show()
    cnt = 0
    total_cnt = 0
    sql = "insert into full_texts_abnormal_ref(pmid,pmc, section, reference_ids, paragraph) values"
    for row in result:
        pmid = row[0]
        section = row[1]
        reference_ids = row[2]
        paragraph = row[3]
        pmc = row[4]
        reference_ids_list = eval(reference_ids)

        if len(reference_ids_list) > 0:
            for ref in reference_ids_list:
                es = ref.split('.')
                if len(es) == 2:
                    if cnt > 0:
                        sql += ' ,'
                    sql += "('{}','{}','{}','{}','{}')".format(pmid, pmc, pymysql.escape_string(section), pymysql.escape_string(reference_ids),
                                                               pymysql.escape_string(paragraph))
                    # print(sql)
                    cnt += 1
                    if cnt % 10000 == 0:
                        mysql.query(sql)
                        mysql.commit()
                        sql = "insert into full_texts_abnormal_ref(pmid,pmc, section, reference_ids, paragraph) values"
                        cnt = 0
                    break
        total_cnt += 1
        if total_cnt % 1000 == 0:
            print('processed :', total_cnt)
    if cnt > 0:
        mysql.query(sql)
        mysql.commit()
    print('total cnt: ', cnt)

if __name__ == '__main__':
    run()
```

MySQL类代码：
```
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

    def __init__(self, host='localhost', user='root', passwd='root', db='plos_one'):

        """MySQL Database initialization """
        try:
            self.conn = pymysql.connect(host, user, passwd, db,charset="utf8")
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





