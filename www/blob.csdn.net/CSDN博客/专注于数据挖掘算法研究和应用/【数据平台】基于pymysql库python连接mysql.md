# 【数据平台】基于pymysql库python连接mysql - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月14日 15:03:24[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：218
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、场景：python连接mysql的API使用pymysql库

      https://pypi.python.org/pypi/PyMySQL/

     安装：pip install pymysql 即可




2、代码示例：



```python
#!/usr/bin/env python
#   --coding = utf-8
#   Author Jason.F
import pymysql

#创建链接对象
conn = pymysql.connect(host='XX', port=3306, user='XX', passwd='XX', db='XX')
#创建游标
cursor = conn.cursor()
#执行sql，更新单条数据，并返回受影响行数
cursor.execute("select user_uid, course_uid, created_at from study_log limit 10")
watched_list = cursor.fetchall()
print (list(watched_list))
#关闭游标
cursor.close()
#关闭连接
conn.close()
```





