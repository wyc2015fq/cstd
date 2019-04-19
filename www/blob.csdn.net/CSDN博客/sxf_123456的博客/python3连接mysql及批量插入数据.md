# python3连接mysql及批量插入数据 - sxf_123456的博客 - CSDN博客
2018年04月16日 15:04:34[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3180
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2018/4/13 0013 16:33
# @Author  :
# @QQ      : 
# @File    : py_con_mysql.py
# @Software: PyCharm Community Edition
*"""**python connect mysql**"""*import os
import json
import pymysql
def conn_1():
    #mysql_conf = {'host': '10.0.0.0', 'port': 3306, 'user': 'dev', 'password': 'sctele@dev', 'db': 'test', 'charset': 'utf8'}
mysql_conf = json.load(
        open(os.path.abspath(os.path.join(os.path.dirname(__name__),'mysql.json')),'r',encoding='utf-8'))
    connect = pymysql.Connect(**mysql_conf)
    return connect
def conn_2():
    str = """
            {
              "host": "10.0.0.0",
              "port":3307,
              "user": "dev",
              "password": "sctele@dev",
              "db": "test",
              "charset": "utf8"
              }
        """
#mysql_conf = {'host': '10.0.0.0', 'port': 3306, 'user': 'dev', 'password': 'sctele@dev', 'db': 'test', 'charset': 'utf8'}
mysql_conf = json.loads(str, encoding='utf-8')
    try:
        connect = pymysql.Connect(**mysql_conf)
    except Exception as err:
        raise err
    return connect
#批量插入mysql
def many_insert_mysql(cursor):
    fields = ['num1','num2']
    sql = """
    INSERT INTO tt ({fields}) VALUE ({mark});
    """.format(
        fields = '`'+'`,`'.join(fields)+'`',
mark = ','.join(['%s']*len(fields))
    )
    j = 0
rows = []
    for num in range(2):
        sql_=sql%(num,num)
        print(sql_)
        row_1 = []
        row_1.append(num)
        row_1.append(num)
        rows.append(row_1)
        j += 1
if j == 1:
            cursor.executemany(sql,rows)
            conn_2().commit()
            rows = []
            j = 0
if rows:
        cursor.executemany(sql,rows)
        conn_2().commit()
    conn_2().close()
if __name__=='__main__':
    sql = """
            SELECT COUNT(*) FROM test1;
            """
sql_test = """
        INSERT INTO tt (`num1`,`num2`) VALUES (0,1);
    """
#1.连接数据库
cursor_1 = conn_1().cursor()
    with conn_1().cursor() as cursor:
        cursor.execute(sql)
        results = cursor.fetchone()
        print(results[0])
    cursor.close()
    #2.连接数据库
with conn_2().cursor() as cursor:
        cursor.execute(sql)
        result_2 = cursor.fetchone()
        print(result_2[0])
    cursor.close()
    many_insert_mysql(conn_2().cursor())
