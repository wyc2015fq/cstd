
# 安装MySql+连接数据库+读取数据并存储成dataframe（python3.6） - OraYang的博客 - CSDN博客

2017年10月11日 20:11:43[OraYang](https://me.csdn.net/u010665216)阅读数：427所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



## 环境安装
### 安装依赖环境
```python
$ sudo apt
```
```python
-get
```
```python
install python
```
```python
-pip
```
```python
python
```
```python
-dev
```
```python
libmysqlclient
```
```python
-dev
```
### 安装MySqldb
```python
$
```
```python
pip install mysqlclient
```
```python
#python3.6
```
```python
$
```
```python
pip install
```
```python
MySQL
```
```python
-python
```
```python
#python 2.7
```
## 连接数据库+读取数据+存储
```python
#!/usr/bin/python
```
```python
# -*- coding: utf-8 -*-
```
```python
import
```
```python
MySQLdb
```
```python
import
```
```python
pandas
```
```python
as
```
```python
pd
conn = MySQLdb.connect(host=
```
```python
'*.*.*.*'
```
```python
,user=
```
```python
'root'
```
```python
,password=
```
```python
'123456'
```
```python
,db =
```
```python
'**'
```
```python
,port=
```
```python
3306
```
```python
,charset=
```
```python
'utf8'
```
```python
)
cur = conn.cursor()
```
```python
#cur.execute
```
```python
cur.execute(
```
```python
'select* from table_name'
```
```python
)
```
```python
#sql
```
```python
rows = cur.fetchall()
data = pd.DataFrame.from_records(list(rows))
```
```python
#convert tuple to list to DataFrame
```
```python
#data = pd.DataFrame(rows)
```
```python
data.to_csv(
```
```python
'device.csv'
```
```python
,index =
```
```python
False
```
```python
)
conn.close()
```

