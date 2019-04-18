# PYTHON-基础-时间日期处理小结 - Nicole的博客 - CSDN博客
2018年09月18日 13:57:34[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：50
**目录**
[涉及对象](#she-ji-dui-xiang)
[1. datetime](#1-datetime)
[2. timestamp](#2-timestamp)
[3. time tuple](#3-time-tuple)
[4. string](#4-string)
[5. date](#5-date)
[datetime基本操作](#datetimeji-ben-cao-zuo_1)
[1. 获取当前datetime](#1.%20%E8%8E%B7%E5%8F%96%E5%BD%93%E5%89%8Ddatetime)
[2. 获取当天date](#2.%20%E8%8E%B7%E5%8F%96%E5%BD%93%E5%A4%A9date)
[3. 获取明天/前N天](#3.%20%E8%8E%B7%E5%8F%96%E6%98%8E%E5%A4%A9%2F%E5%89%8DN%E5%A4%A9)
[4. 获取当天开始和结束时间(00:00:00 23:59:59)](#4.%20%E8%8E%B7%E5%8F%96%E5%BD%93%E5%A4%A9%E5%BC%80%E5%A7%8B%E5%92%8C%E7%BB%93%E6%9D%9F%E6%97%B6%E9%97%B4(00%3A00%3A00%2023%3A59%3A59))
[5. 获取两个datetime的时间差](#5.%20%E8%8E%B7%E5%8F%96%E4%B8%A4%E4%B8%AAdatetime%E7%9A%84%E6%97%B6%E9%97%B4%E5%B7%AE)
[6. 获取本周/本月/上月最后一天](#6.%20%E8%8E%B7%E5%8F%96%E6%9C%AC%E5%91%A8%2F%E6%9C%AC%E6%9C%88%2F%E4%B8%8A%E6%9C%88%E6%9C%80%E5%90%8E%E4%B8%80%E5%A4%A9)
[关系转换](#guan-xi-zhuan-huan_1)
[关系转换例子](#guan-xi-zhuan-huan-li-zi)
[datetime <=> string](#datetime%20%3C%3D%3E%20string)
[datetime <=> timetuple](#datetime%20%3C%3D%3E%20timetuple)
[datetime <=> date](#datetime%20%3C%3D%3E%20date)
[datetime <=> timestamp](#datetime%20%3C%3D%3E%20timestamp)
步骤:
```
1. 掌握几种对象及其关系
2. 了解每类对象的基本操作方法
3. 通过转化关系转化
```
## 涉及对象
### 1. datetime
```
>>> import datetime
>>> now = datetime.datetime.now()
>>> now
datetime.datetime(2015, 1, 12, 23, 9, 12, 946118)
>>> type(now)
<type 'datetime.datetime'>
```
### 2. timestamp
```
>>> import time
>>> time.time()
1421075455.568243
```
### 3. time tuple
```
>>> import time
>>> time.localtime()
time.struct_time(tm_year=2015, tm_mon=1, tm_mday=12, tm_hour=23, tm_min=10, tm_sec=30, tm_wday=0, tm_yday=12, tm_isdst=0)
```
### 4. string
```
>>> import datetime
>>> datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
'2015-01-12 23:13:08'
```
### 5. date
```
>>> import datetime
>>> datetime.datetime.now().date()
datetime.date(2015, 1, 12)
```
## datetime基本操作
### 1. 获取当前datetime
```
>>> import datetime
>>> datetime.datetime.now()
datetime.datetime(2015, 1, 12, 23, 26, 24, 475680)
```
### 2. 获取当天date
```
>>> datetime.date.today()
datetime.date(2015, 1, 12)
```
### 3. 获取明天/前N天
明天
```
>>> datetime.date.today() + datetime.timedelta(days=1)
datetime.date(2015, 1, 13)
```
三天前
```
>>> datetime.datetime.now()
datetime.datetime(2015, 1, 12, 23, 38, 55, 492226)
>>> datetime.datetime.now() - datetime.timedelta(days=3)
datetime.datetime(2015, 1, 9, 23, 38, 57, 59363)
```
### 4. 获取当天开始和结束时间(00:00:00 23:59:59)
```
>>> datetime.datetime.combine(datetime.date.today(), datetime.time.min)
datetime.datetime(2015, 1, 12, 0, 0)
>>> datetime.datetime.combine(datetime.date.today(), datetime.time.max)
datetime.datetime(2015, 1, 12, 23, 59, 59, 999999)
```
### 5. 获取两个datetime的时间差
```
>>> (datetime.datetime(2015,1,13,12,0,0) - datetime.datetime.now()).total_seconds()
44747.768075
```
### 6. 获取本周/本月/上月最后一天
本周
```
>>> today = datetime.date.today()
>>> today
datetime.date(2015, 1, 12)
>>> sunday = today + datetime.timedelta(6 - today.weekday())
>>> sunday
datetime.date(2015, 1, 18)
```
本月
```
>>> import calendar
>>> today = datetime.date.today()
>>> _, last_day_num = calendar.monthrange(today.year, today.month)
>>> last_day = datetime.date(today.year, today.month, last_day_num)
>>> last_day
datetime.date(2015, 1, 31)
```
获取上个月的最后一天(可能跨年)
```
>>> import datetime
>>> today = datetime.date.today()
>>> first = datetime.date(day=1, month=today.month, year=today.year)
>>> lastMonth = first - datetime.timedelta(days=1)
```
## 关系转换
几个关系之间的转化
`Datetime Object / String / timestamp / time tuple`
## 关系转换例子
### datetime <=> string
datetime -> string
```
>>> import datetime
>>> datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
'2015-01-12 23:13:08'
```
string -> datetime
```
>>> import datetime
>>> datetime.datetime.strptime("2014-12-31 18:20:10", "%Y-%m-%d %H:%M:%S")
datetime.datetime(2014, 12, 31, 18, 20, 10)
```
### datetime <=> timetuple
datetime -> timetuple
```
>>> import datetime
>>> datetime.datetime.now().timetuple()
time.struct_time(tm_year=2015, tm_mon=1, tm_mday=12, tm_hour=23, tm_min=17, tm_sec=59, tm_wday=0, tm_yday=12, tm_isdst=-1)
```
timetuple -> datetime
```
timetuple => timestamp => datetime [看后面datetime<=>timestamp]
```
### datetime <=> date
datetime -> date
```
>>> import datetime
>>> datetime.datetime.now().date()
datetime.date(2015, 1, 12)
```
date -> datetime
```
>>> datetime.date.today()
datetime.date(2015, 1, 12)
>>> today = datetime.date.today()
>>> datetime.datetime.combine(today, datetime.time())
datetime.datetime(2015, 1, 12, 0, 0)
>>> datetime.datetime.combine(today, datetime.time.min)
datetime.datetime(2015, 1, 12, 0, 0)
```
### datetime <=> timestamp
datetime -> timestamp
```
>>> now = datetime.datetime.now()
>>> timestamp = time.mktime(now.timetuple())
>>> timestamp
1421077403.0
```
timestamp -> datetime
```
>>> datetime.datetime.fromtimestamp(1421077403.0)
datetime.datetime(2015, 1, 12, 23, 43, 23)
```
版权声明：自由转载-非商用-非衍生-保持署名 | [Creative Commons BY-NC-ND 3.0](http://creativecommons.org/licenses/by-nc-nd/3.0/deed.zh)
