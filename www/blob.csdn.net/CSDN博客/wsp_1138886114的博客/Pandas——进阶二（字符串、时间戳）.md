# Pandas——进阶二（字符串、时间戳） - wsp_1138886114的博客 - CSDN博客





2018年06月22日 11:05:34[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：727










- - - [一、pandas中 字符串 处理（拆分与去除特殊字符）](#一pandas中-字符串-处理拆分与去除特殊字符)
- [二、时间戳](#二时间戳)- - [2.1 python 时间戳](#21-python-时间戳)- [获取当前时间并格式化](#获取当前时间并格式化)
- [推导式：](#推导式)

- [2.2 pandas 时间戳](#22-pandas-时间戳)- [字符串、时间戳—转成时间](#字符串时间戳转成时间)
- [判断时间：NaT](#判断时间nat)
- [创建一组时间日期](#创建一组时间日期)
- [pandas读取csv 处理时间列](#pandas读取csv-处理时间列)
- [pandas读取csv 处理时间戳列–将时间戳转成类别特征](#pandas读取csv-处理时间戳列将时间戳转成类别特征)









### 一、pandas中 字符串 处理（拆分与去除特殊字符）

```
对字符串 进行拆分连接（去空格或者其它符号）
import re 

text ='foo bar  \t good  \tssda' 
strlist = re.split('\s+',text)            #正则匹配任意空字符（去除空格）
输出1：
['foo', 'bar', 'good', 'ssda']

"#".join(strlist)                         #将字符串用"#"连接
输出2：
'foo#bar#good#ssda'

strlist = "#".join(re.split('\s+',text))  #匹配任意空字符并"#"连接
输出3：
'foo#bar#good#ssda'
```
|方法|说明|
|----|----|
|count|返回子串在字符串中的出现次数（非重叠）|
|endswith,startswith|如果字符串以某个后缀结尾（以某个前缀幵头），则返回True|
|join|将字符串用作连接其他字符串序列的分隔符   “#”.join(str1):对字符串用“#”连接|
|index|如果在字符串中找到子串，则返回子串第一个字符所在的位置。如果没有找到，则引发ValueError。|
|find|则返回发现的`第一个字符`所在的位置。如果没有找到，则返回-1|
|rfind|则返回发现的`最后一个字符`所在的位置。如果没有找到，则返回-1|
|replace|用另一个字符串替换指定子串|
|strip, rstrip、Istrip|去除空白符（包括换行符）|
|split|通过指定的分隔符将字符串拆分为一组子串|
|lower(小写),upper（大写）|分别将字母字符转换为小写或大写|
|ljust rjust|用空格（或其他字符）填充字符串的空白侧以返回符合最低宽度的字符串|

### 二、时间戳

```python
from datetime import datetime
from datetime import timedelta
from dateutil.parser import parse
```

##### 2.1 python 时间戳

###### 获取当前时间并格式化

```
now = datetime.now()
stamp = datetime.now()
stamp.strftime("%Y-%m-%d")
输出：
'2018-06-22'
```

###### 推导式：

```
datastrs = ["1/6/2001","12/9/2013"]
[datetime.strptime(x,"%d/%m/%Y") for x in datastrs]
>>>
[datetime.datetime(2001, 6, 1, 0, 0), datetime.datetime(2013, 9, 12, 0, 0)]

parse("2011-03-23")
>>>
datetime.datetime(2011, 3, 23, 0, 0)
```

##### 2.2 pandas 时间戳

```python
from pandas import DataFrame,Series
import pandas as pd
import numpy as np
from datetime import datetime
```

###### 字符串、时间戳—转成时间

```python
datastrs = ["1/6/2001","12/9/2013"]
pd.to_datetime(datastrs)
>>>
DatetimeIndex(['2001-01-06', '2013-12-09'], dtype='datetime64[ns]', freq=None) 
------------------------------------------------------ 
import sys
import datetime

"""
  将878887116 时间戳转换为星期几
"""
# 调用函数 转换时间戳为星期几
weekday = datetime.datetime.fromtimestamp(float(878887116)).isoweekday()
```

###### 判断时间：NaT

```python
idx = pd.to_datetime(datastrs+[None])
>>>
DatetimeIndex(['2001-01-06', '2013-12-09', 'NaT'], dtype='datetime64[ns]', freq=None)

mask = pd.isnull(idx)   #判断时间日期是否为NaT
>>>
array([False, False,  True])

idx[~mask]             #去掉空值
>>>
DatetimeIndex(['2001-01-06', '2013-12-09'], dtype='datetime64[ns]', freq=None)
```

###### 创建一组时间日期

```
dates=[datetime(2011,1,2),datetime(2011,1,5),datetime(2011,1,7),
      datetime(2011,1,8),datetime(2011,1,10),datetime(2011,1,12)]
ts = Series(np.random.randn(6),index=dates)
>>>
2011-01-02    0.374627
2011-01-05   -1.154850
2011-01-07   -0.144595
2011-01-08   -0.331414
2011-01-10    0.859761
2011-01-12   -0.475668
dtype: float64

stamp = ts.index[2]              #通过位置获取索引
>>>
Timestamp('2011-01-07 00:00:00') #通过索引获取值
ts["2011-01"]
>>>
2011-01-02    0.374627
2011-01-05   -1.154850
2011-01-07   -0.144595
2011-01-08   -0.331414
2011-01-10    0.859761
2011-01-12   -0.475668
dtype: float64


longer_ts = Series(np.random.randn(1000),
                   index = pd.date_range("1/1/2000",periods=1000))
longer_ts["2001"]    #只获取2001年的信息
>>>
2001-01-01    0.404751
2001-01-02   -1.290699
2001-01-03    1.522409
2001-01-04   -1.349135
2001-01-05    0.797811
2001-01-06   -0.318972
……
2001-12-28   -1.255813
2001-12-29    0.342102
2001-12-30   -0.683155
2001-12-31   -0.031639
Freq: D, Length: 365, dtype: float64 

index = pd.date_range("1/23/2001","5/23/2001")
>>>
DatetimeIndex(['2001-01-23', '2001-01-24', '2001-01-25', '2001-01-26',
               '2001-01-27', '2001-01-28', '2001-01-29', '2001-01-30',
               '2001-01-31', '2001-02-01',
               ...
               '2001-05-14', '2001-05-15', '2001-05-16', '2001-05-17',
               '2001-05-18', '2001-05-19', '2001-05-20', '2001-05-21',
               '2001-05-22', '2001-05-23'],
              dtype='datetime64[ns]', length=121, freq='D')

index = pd.date_range(start="1/23/2001",end="5/23/2002")  #方法1
index = pd.date_range(end="1/23/2001",periods=20)         #方法2
index = pd.date_range(start="1/23/2001",periods=20)       #方法3
>>>
方法3：
DatetimeIndex(['2001-01-23', '2001-01-24', '2001-01-25', '2001-01-26',
               '2001-01-27', '2001-01-28', '2001-01-29', '2001-01-30',
               '2001-01-31', '2001-02-01', '2001-02-02', '2001-02-03',
               '2001-02-04', '2001-02-05', '2001-02-06', '2001-02-07',
               '2001-02-08', '2001-02-09', '2001-02-10', '2001-02-11'],
              dtype='datetime64[ns]', freq='D')
```

###### pandas读取csv 处理时间列

```python
import datetime
import pandas as pd
import numpy as np
import pickle

#用pandas将时间转为标准格式
#将时间栏合并,并转为标准时间格式
dateparse = lambda dates: pd.datetime.strptime(dates,'%d/%m/%Y %H:%M')
rawdata = pd.read_csv('RealMarketPriceDataPT.csv',
                      parse_dates={'timeline':['date','(UTC)']},
                      date_parser=dateparse)

#定义一个将时间转为数字的函数,s为字符串
def datestr2num(s):
    #toordinal()将时间格式字符串转为数字
    return datetime.datetime.strptime(s,'%Y-%m-%d %H:%M:%S').toordinal()

x = []
y = []
new_date = []

for i in range(rawdata.shape[0]):
    x_convert = int(datestr2num(str(rawdata.ix[i,0])))
    new_date.append(x_convert)
    y_convert = rawdata.ix[i,1].astype(np.float32)
    x.append(x_convert)
    y.append(y_convert)

x = np.array(x).astype(np.float32)

"""
with open('price.pickle','wb') as f:
    pickle.dump((x,y),f)
"""
print(datetime.datetime.fromordinal(new_date[0]),'------>>>>>>',new_date[0])
print(datetime.datetime.fromordinal(new_date[10]),'------>>>>>>',new_date[10])
print(datetime.datetime.fromordinal(new_date[20]),'------>>>>>>',new_date[20])
print(datetime.datetime.fromordinal(new_date[30]),'------>>>>>>',new_date[30])
print(datetime.datetime.fromordinal(new_date[40]),'------>>>>>>',new_date[40])
print(datetime.datetime.fromordinal(new_date[50]),'------>>>>>>',new_date[50])
```

###### pandas读取csv 处理时间戳列–将时间戳转成类别特征

```python
rating_data = sc.textFile(u.data)
rating_data.first()   
#输出："196\t242\t3\t881250949" 

timestamps = rating_data.map(lambda line:line.split("\t"))\
                        .map(lambda fields: int(fields[3]))

def extract_datatime(ts):
    import datatime
    return datatime.datatime.fromtimestamp(ts)
hour_of_day = timestamps.map(lambda ts : extract_datatime(ts).hour)

hour_of_day.take(5) #从原始时间数据，到点钟类别特征的转换
# 输出：【23,3,15,13,13】

def assign_today(hr):
    time_of_day={
        "morning":range(7,12),
        "lunch":range(12,14),
        "afternoon":range(14,18),
        "evening":range(18,23),
        "prenight":range(23,24),
        "night":range(0,7)
    }
    for k,v in time_of_day.items():
        if hr in v:
            return k

time_of_day = hour_of_day.map(lambda hr:assign_today(hr))
time_of_day.take(5)

#输出：【"prenight","night","afternoon"】
```



