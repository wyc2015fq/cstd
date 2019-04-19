# python 往mysql中批量插入数据 - sxf_123456的博客 - CSDN博客
2019年03月15日 18:14:23[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：16标签：[python 往mysql中批量插入数据](https://so.csdn.net/so/search/s.do?q=python 往mysql中批量插入数据&t=blog)
个人分类：[python](https://blog.csdn.net/sxf_123456/article/category/7078196)
1、可以批量插入[(value1,value2),(value3,value4)]  列表中是元组类型
2、可以批量插入[[value1,value2],[value3,value4]]  列表中是列表类型
fields = ['a', 'b']
sql = "insert into test2 ({fields}) values ({mark})".format(
    fields='`' + '`,`'.join(fields) + '`',
    mark=','.join(['%s'] * len(fields))
)
temp_list = [[6, 7], [72, 8], [37, 87]]
temp_list = [(1, 2), (2, 3), (3, 4)]
conn = connect()
with conn.cursor() as cur:
    cur.executemany(sql, temp_list)
    conn.commit()
cur.close()
conn.close()
