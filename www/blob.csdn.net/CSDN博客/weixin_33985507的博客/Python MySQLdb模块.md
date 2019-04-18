# Python MySQLdb模块 - weixin_33985507的博客 - CSDN博客
2016年11月01日 18:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### 安装
```bash
系统：Ubuntu 14.04
MySQLdb
$ pip install Mysql-python
-> 报错：EnvironmentError: mysql_config not found （安装libmysqlclient-dev）
$ apt-get install libmysqlclient-dev
$ pip install Mysql-python
如果是MAC系统，则需要先安装mysql服务。
```
### 使用
```python
# coding=utf-8
import MySQLdb
conn = MySQLdb.connect(host='127.0.0.1', user='zheng', db='satezheng', passwd='satezheng')
cur = conn.cursor()
# 查
# 执行命令，并不会直接打印,可以使用fetchall来读取。
reCount1 = cur.execute('show tables;')
print reCount1      # 输出一个数字，是SQL影响的行数
# 循环输出结果，以元祖的方式输出
for data in cur.fetchall():
    print data
# 插入一条数据
# 执行增删改等命令时，可用如下方法。
sql1 = 'insert into Products(prod_id, prod_name) values(%s, %s);'
params = ('00002', 'GLA')
reCount2 = cur.execute(sql1, params)        #为了防止SQL注入,可以使用该方法
conn.commit()   #提交
# 批量插入
li = [
    ('000004', 'TYA'),
    ('000005', 'TUH')
]
sql1 = 'insert into Products(prod_id, prod_name) values(%s, %s);'
# 批量使用executemany
reCount23= cur.executemany(sql1, li)        #为了防止SQL注入,可以使用该方法
conn.commit()   #提交
cur.close()
conn.close()
```
上边查看数据时，返回是元祖形式，有时看起来并不明显，我们可以选择用字典的方式，将列名一起输出。
```python
import MySQLdb
conn = MySQLdb.connect(host='127.0.0.1', user='zheng', db='satezheng', passwd='satezheng')
# 使用字典形式返回数据
cur = conn.cursor(cursorclass = MySQLdb.cursors.DictCursor)
# 执行命令，并不会直接打印,可以使用fetchall来读取。
reCount1 = cur.execute('select * from Products')
print reCount1      # 输出一个数字，是SQL影响的行数
for data in cur.fetchall():
    print data
cur.close()
conn.close()
# 结果==》
5
{'prod_name': 'hilife', 'prod_id': '00001'}
{'prod_name': 'GLA', 'prod_id': '00002'}
{'prod_name': 'GTL', 'prod_id': '00003'}
{'prod_name': 'TYA', 'prod_id': '000004'}
{'prod_name': 'TUH', 'prod_id': '000005'}
```
`cursor`读取执行结果的方法有：
- `fetchall()` 把所有的都读出来
- `fetchmany(n)` 输出指定数量n的行
- `fetchone` 只返回第一条结果
