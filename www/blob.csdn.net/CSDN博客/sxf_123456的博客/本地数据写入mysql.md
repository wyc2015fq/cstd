# 本地数据写入mysql - sxf_123456的博客 - CSDN博客
2018年09月13日 13:13:57[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：67
```
"""
本地数据写入mysql
"""
import pymysql
def connect():
    """
    连接mysql数据库
    :return:
    """
    conn = pymysql.connect(host="127.0.0.1", port=15506,
                           user="quality", password="sctele@quality",
                           database="qualitycontrol", charset="utf8")
    return conn
def insert_mysql(read_path):
    """
    本地数据插入mysql
    :param read_path:  文件路径
    :return:
    """
    read_file = open(read_path, "r", encoding="UTF-8")
    fields = ["bas_ip", "city_name", "qx_name"]
    table_name = "sat_bas_qx_test"
    conn = connect()
    sql = "INSERT INTO `{table}` ({field}) VALUES ({mark});".format(
        table=table_name,
        field='`' + '`,`'.join(fields) + '`',
        mark=','.join(['%s'] * len(fields))
    )
    value_list = []
    for line in read_file:
        line = line.strip().split(",")
        temp_list = []
        for i in range(len(fields)):
            temp_list.append(line[i])
        value_list.append(temp_list)
        if len(value_list) == 1024:
            with conn.cursor() as cursor:
                cursor.executemany(sql, value_list)
            conn.commit()
            value_list = []
    else:
        with conn.cursor() as cursor:
            cursor.executemany(sql, value_list)
        conn.commit()
    read_file.close()
if __name__ == '__main__':
    read_path = "E:\\公司\\案例数据采集\\数据文档\\sat_bas_qx_test.txt"
    insert_mysql(read_path)
```
