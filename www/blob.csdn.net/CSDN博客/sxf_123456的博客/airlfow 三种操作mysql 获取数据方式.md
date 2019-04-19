# airlfow 三种操作mysql 获取数据方式 - sxf_123456的博客 - CSDN博客
2019年03月06日 15:31:22[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：31
#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
airflow 操作mysql 数据库获取数据的三种方式
"""
from airflow.hooks.mysql_hook import MySqlHook
from airflow.hooks.dbapi_hook import DbApiHook
def get_DaApi_data():
    """
    airflow中使用 DbApiHook获取mysql数据库数据
    """
    sql_hook = DbApiHook().get_hook(conn_id="mysql_qualitycontrol")
    sql = "select name,capacity,pt_name,area_name from t_mate_cdn_node where isoff=0;"
    result = sql_hook.get_records(sql)
    for row in result:
        print(row)
def get_Mysql_data():
    """
    airflow中使用MySqlHook 获取mysql数据库数据
    """
    sql_hook = MySqlHook().get_hook(conn_id="mysql_qualitycontrol")
    sql = "select name,capacity,pt_name,area_name from t_mate_cdn_node where isoff=0;"
    result = sql_hook.get_records(sql)
    for row in result:
        print(row)
def get_pymysql_data():
    """
    python3 获取mysql数据库数据
    """
    import pymysql
    from contextlib import closing
    conn = pymysql.Connect(host="127.0.0.1", port=15506,
                           user="quality", password="sctele@quality",
                           database="qualitycontrol", charset="utf8")
    sql = "select name,capacity,pt_name,area_name from t_mate_cdn_node where isoff=0;"
    with closing(conn.cursor()) as cur:
        cur.execute(sql)
        result = cur.fetchall()
    for row in result:
        print(row)
if __name__ == '__main__':
    get_DaApi_data()
    get_Mysql_data()
    get_pymysql_data()
