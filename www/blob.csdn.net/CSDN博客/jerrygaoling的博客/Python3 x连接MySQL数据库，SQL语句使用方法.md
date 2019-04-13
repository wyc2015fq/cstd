
# Python3.x连接MySQL数据库，SQL语句使用方法 - jerrygaoling的博客 - CSDN博客


2018年05月08日 22:10:01[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：319


# Python3.x连接MySQL数据库
## 使用的Python包
连接MySQL数据库使用的包为`pymysql`
使用pip导入此包，在cmd下输入一下语句：
```python
pip install pymysql
```
![这里写图片描述](https://img-blog.csdn.net/20180508220819455?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 打开数据库连接
编写连接数据库的方法，连接数据库使用`pymysql`包中的`connect()`方法
```python
import
```
```python
pymysql
```
```python
global
```
```python
conn
```
```python
# 打开数据库连接（ip/数据库用户名/登录密码/数据库名/编码格式）
```
```python
conn = pymysql.connect(
```
```python
"localhost"
```
```python
,
```
```python
"root"
```
```python
,
```
```python
"root"
```
```python
,
```
```python
"Database"
```
```python
,charset=
```
```python
'utf8'
```
```python
)
```
## 使用SQL语句查询数据
编写查询方法，使用`pymysql`包中的`execute()`方法
首先创建一个游标对象，用于扫描数据库。
```python
global
```
```python
cursor
```
```python
# 使用 cursor() 方法创建一个游标对象 cursor
```
```python
cursor = conn.cursor()
```
使用`execute()`方法执行SQL查询语句：
```python
# 使用 execute()  方法执行 SQL 查询
```
```python
cursor.execute(sql)
```
## 获取查询结果
返回结果只要使用`fetchone()`，`fetchall()`两个方法
前者会返回单条数据，后者为返回所有查询数据，并以list方式存储
在使用此方法获取查询数据时，需使用`try`,`catch`语句包裹查询方法。
```python
def
```
```python
selectRecord
```
```python
()
```
```python
:
```
```python
# 使用 fetchone() 方法获取单条数据.
```
```python
#data = cursor.fetchone()
```
```python
try
```
```python
:
        rs=cursor.fetchall()
```
```python
except
```
```python
:
        rs=false
        print(
```
```python
"查询失败！！"
```
```python
)
```
```python
return
```
```python
rs
```
## 其他方法
在`pymysql`中还存在其他的返回值操作，例如获取返回数据的数据量`rowcount`
```python
def
```
```python
rowcont
```
```python
()
```
```python
:
```
```python
return
```
```python
cursor.rowcount
```
此方法将返回所查询到的所有数据条数。
## 关闭数据库
在所有的数据库连接操作之后，都需要关闭数据库，以免出现持续占用的现象。
使用`close()`方法来关闭数据库。
```python
def
```
```python
myclose
```
```python
()
```
```python
:
```
```python
try
```
```python
:
        conn.close()
```
```python
except
```
```python
:
        print(
```
```python
"关闭连接失败！！"
```
```python
)
```
## SQL语句的使用细节
在编写SQL语句中，所有的变量需要使用`%`来转换
例如：需要添加一个模糊搜索中的`%`时，需要输入`%%`来转换
输入字符串时，需要用`%s`来表示，同时在SQL语句后，使用`%（“字符串”）`来表述输入值
```python
sql=
```
```python
"SELECT * FROM school WHERE Unit LIKE '%s%%' AND Cseedate LIKE '%s%%' AND Csymptoms LIKE '%%%s%%'"
```
```python
%(
```
```python
"昆明理工大学"
```
```python
,
```
```python
"2018-05-08"
```
```python
,
```
```python
"无线通信团队"
```
```python
)
```
其最终输入给数据库的查询语句为：
```python
SELECT * FROM school WHERE Unit LIKE
```
```python
'昆明理工大学%'
```
```python
AND Cseedate LIKE
```
```python
'2018-05-08%'
```
```python
AND Csymptoms LIKE
```
```python
'%无线通信团队%'
```

