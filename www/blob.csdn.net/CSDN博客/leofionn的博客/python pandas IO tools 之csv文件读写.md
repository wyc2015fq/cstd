
# python pandas IO tools 之csv文件读写 - leofionn的博客 - CSDN博客


2018年04月17日 15:15:35[leofionn](https://me.csdn.net/qq_36142114)阅读数：187


# 源地址：https://blog.csdn.net/ly_ysys629/article/details/55107237

# python pandas IO tools 之csv文件读写
英文原文：[pandas IO tools](http://pandas.pydata.org/pandas-docs/stable/io.html#io-read-csv-table)
读取csv文件：pd.read_csv(),写入csv文件:pd.to_csv()
pandas还可以读取一下文件：
read_csv,
read_excel,
read_hdf,
read_sql,
read_json,
read_msgpack (experimental),
read_html,
read_gbq (experimental),
read_stata,
read_sas,
read_clipboard,
read_pickle;
相应的写入：
to_csv,
to_excel,
to_hdf,
to_sql,
to_json,
to_msgpack (experimental),
to_html,
to_gbq (experimental),
to_stata,
to_clipboard,
to_pickle.
### 常用参数的读取csv文件
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
obj=pd.read_csv(
```
```python
'f:/ceshi.csv'
```
```python
)
```
```python
print
```
```python
obj
```
```python
print
```
```python
type(obj)
```
```python
print
```
```python
obj.dtypes
```
```python
Unnamed: 0  c1  c2  c3
0          a   0   5  10
1          b   1   6  11
2          c   2   7  12
3          d   3   8  13
4          e   4   9  14
<class 'pandas.core.frame.DataFrame'>
Unnamed: 0    object
c1             int64
c2             int64
c3             int64
dtype: object
```
ceshi.csv为有列索引没有行索引的数据，read_csv会自动加上行索引，即使原数据集有行索引。
read_csv读取的数据类型为Dataframe，obj.dtypes可以查看每列的数据类型
```python
obj_2=pd.read_csv(
```
```python
'f:/ceshi.csv'
```
```python
,header=
```
```python
None
```
```python
,names=range(
```
```python
2
```
```python
,
```
```python
5
```
```python
))
```
```python
print
```
```python
obj_2
```
```python
2   3   4
0  c1  c2  c3
1   0   5  10
2   1   6  11
3   2   7  12
4   3   8  13
5   4   9  14
```
header=None时，即指明原始文件数据没有列索引，这样read_csv为自动加上列索引，除非你给定列索引的名字。
```python
obj_2=pd.read_csv(
```
```python
'f:/ceshi.csv'
```
```python
,header=
```
```python
0
```
```python
,names=range(
```
```python
2
```
```python
,
```
```python
5
```
```python
))
```
```python
print
```
```python
obj_2
```
```python
2  3   4
0  0  5  10
1  1  6  11
2  2  7  12
3  3  8  13
4  4  9  14
```
header=0，表示文件第0行（即第一行，python，索引从0开始）为列索引，这样加names会替换原来的列索引。
```python
obj_2=pd.read_csv(
```
```python
'f:/ceshi.csv'
```
```python
,index_col=
```
```python
0
```
```python
)
```
```python
print
```
```python
obj_2
```
```python
c1  c2  c3
a   0   5  10
b   1   6  11
c   2   7  12
d   3   8  13
e   4   9  14
```
```python
obj_2=pd.read_csv(
```
```python
'f:/ceshi.csv'
```
```python
,index_col=[
```
```python
0
```
```python
,
```
```python
2
```
```python
])
```
```python
print
```
```python
obj_2
```
```python
c1  c3
  c2        
a 5    0  10
b 6    1  11
c 7    2  12
d 8    3  13
e 9    4  14
```

index_col为指定数据中那一列作为Dataframe的行索引，也可以可指定多列，形成层次索引，默认为None,即不指定行索引，这样系统会自动加上行索引（0-）
```python
obj_2=pd.read_csv(
```
```python
'f:/ceshi.csv'
```
```python
,index_col=
```
```python
0
```
```python
,usecols=[
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
])
```
```python
print
```
```python
obj_2
```

```python
c1  c2  c3
a   0   5  10
b   1   6  11
c   2   7  12
d   3   8  13
e   4   9  14
```

```python
obj_2=pd.read_csv(
```
```python
'f:/ceshi.csv'
```
```python
,index_col=
```
```python
0
```
```python
,usecols=[
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
])
```
```python
print
```
```python
obj_2
```
```python
c2  c3
c1        
0    5  10
1    6  11
2    7  12
3    8  13
4    9  14
```
usecols:可以指定原数据集中，所使用的列。在本例中，共有4列，当usecols=[0,1,2,3]时，即选中所有列，之后令第一列为行索引，当usecols=[1,2,3]时，即从第二列开始，之后令原始数据集的第二列为行索引。
```python
obj_2=pd.read_csv(
```
```python
'f:/ceshi.csv'
```
```python
,index_col=
```
```python
0
```
```python
,nrows=
```
```python
3
```
```python
)
```
```python
print
```
```python
obj_2
```
```python
c1  c2  c3
a   0   5  10
b   1   6  11
c   2   7  12
```
nrows：可以给出从原始数据集中的所读取的行数，目前只能从第一行开始到nrows行。
### datetime handing 数据中日期处理
```python
obj_3=pd.read_csv(
```
```python
'f:/ceshi_date.csv'
```
```python
,index_col=
```
```python
0
```
```python
,)
```
```python
print
```
```python
obj_3
```
```python
print
```
```python
type(obj_3.index)
```
```python
A  B  C
date                      
20090101           a  2  3
20090102           b  3  4
20090103           c  4  5
<class 'pandas.indexes.numeric.Int64Index'>
```
```python
obj_3=pd.read_csv(
```
```python
'f:/ceshi_date.csv'
```
```python
,index_col=
```
```python
0
```
```python
,parse_dates=
```
```python
True
```
```python
)
```
```python
print
```
```python
obj_3
```
```python
print
```
```python
type(obj_3.index)
```
```python
A  B  C
date                      
2009-01-01         a  2  3
2009-01-02         b  3  4
2009-01-03         c  4  5
<class 'pandas.tseries.index.DatetimeIndex'>
```
parse_dates=True:可令字符串解析成时间格式。
```python
data=
```
```python
'date,value,cat\n1/6/2000,5,a\n2/6/2000,10,b\n3/6/2000,15,c'
```
```python
print
```
```python
data
```
```python
date,value,cat
1/6/2000,5,a
2/6/2000,10,b
3/6/2000,15,c
```
```python
from
```
```python
StringIO
```
```python
import
```
```python
StringIO
```
```python
print
```
```python
pd.read_csv(StringIO(data),parse_dates=[
```
```python
0
```
```python
],index_col=
```
```python
0
```
```python
)
```
```python
value cat
date                 
2000-01-06      5   a
2000-02-06     10   b
2000-03-06     15   c
```
```python
print
```
```python
pd.read_csv(StringIO(data),parse_dates=[
```
```python
0
```
```python
],index_col=
```
```python
0
```
```python
,dayfirst=
```
```python
True
```
```python
)
```
```python
value cat
date                 
2000-06-01      5   a
2000-06-02     10   b
2000-06-03     15   c
```
US常用时间格式：MM/DD/YYYY，dayfirst=True：可将其改为DD/MM/YYYY
### 分隔符和阈值
```python
tem=
```
```python
'id|level|category\npatient1|123,000|x\npatient2|23,000|y\npatient3|1,234,018|z'
```
```python
print
```
```python
tem
```
```python
id|level|category
patient1|123,000|x
patient2|23,000|y
patient3|1,234,018|z
```
```python
print
```
```python
pd.read_csv(StringIO(tem),sep=
```
```python
'|'
```
```python
)
```
```python
id      level category
0  patient1    123,000        x
1  patient2     23,000        y
2  patient3  1,234,018        z
```
```python
print
```
```python
pd.read_csv(StringIO(tem),sep=
```
```python
'|'
```
```python
,thousands=
```
```python
','
```
```python
)
```
```python
id    level category
0  patient1   123000        x
1  patient2    23000        y
2  patient3  1234018        z
```


