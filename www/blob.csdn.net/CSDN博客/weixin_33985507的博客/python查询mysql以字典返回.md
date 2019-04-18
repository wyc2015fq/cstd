# python查询mysql以字典返回 - weixin_33985507的博客 - CSDN博客
2018年09月27日 14:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13

```python
# *_*coding:utf-8 *_*
import pymysql
conn = pymysql.connect(host='192.168.33.10', user='root', passwd='root', db='spider', charset='utf8')
cur = conn.cursor(cursor=pymysql.cursors.DictCursor)
sql = "select * from topic order by id asc "
cur.execute(sql)  # 返回受影响的行数
res = cur.fetchall()  # 返回数据,返回的是tuple类型
for item in res:
    print(item['id'])
cur.close()
conn.close()
```
