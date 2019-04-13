
# Python 学习入门（10）—— 时间 - 阳光岛主 - CSDN博客

2013年11月20日 22:43:43[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4444所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



Python格式化日期时间的函数为datetime.datetime.strftime()；由字符串转为日期型的函数为：datetime.datetime.strptime()，两个函数都涉及日期时间的格式化字符串，列举如下：
%a     Abbreviated weekday name
%A     Full weekday name
%b     Abbreviated month name
%B     Full month name
%c     Date and time representation appropriate for locale
%dDay of month as decimal number (01 - 31)
%HHour in 24-hour format (00 - 23)
%I     Hour in 12-hour format (01 - 12)
%j     Day of year as decimal number (001 - 366)
%mMonth as decimal number (01 - 12)
%MMinute as decimal number (00 - 59)
%p     Current locale's A.M./P.M. indicator for 12-hour clock
%SSecond as decimal number (00 - 59)
%U     Week of year as decimal number, with Sunday as first day of week (00 - 51)
%w     Weekday as decimal number (0 - 6; Sunday is 0)
%W     Week of year as decimal number, with Monday as first day of week (00 - 51)
%x     Date representation for current locale
%X     Time representation for current locale
%y     Year without century, as decimal number (00 - 99)
%YYear with century, as decimal number
%z, %Z     Time-zone name or abbreviation; no characters if time zone is unknown
%%     Percent sign

**示例：**

```python
﻿#!/usr/bin/python
# it-homer in 2013
import sys
reload(sys)
sys.setdefaultencoding("utf-8")
import datetime
def format_time():
  t = datetime.datetime.now()
  print(t)       # 2013-11-20 09:36:51.198680
  t = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
  print(t)       # 2013-11-20 09:39:16
  t = datetime.datetime.now().strftime('%b-%d-%y %H:%M:%S')
  print(t)       # Nov-20-13 09:36:51
  t = datetime.datetime.now().strftime('%b-%d-%Y %H:%M:%S')
  print(t)       # Nov-20-2013 09:36:51
  # weekday
  t = datetime.datetime.now().strftime('%a %A %U %W %w')
  print(t)      # Wed Wednesday 46 46 3 
  # month
  t = datetime.datetime.now().strftime('%b %B')
  print(t)       # Nov November
  # day
  t = datetime.datetime.now().strftime('%d %j')
  print(t)       # 20 324
  # date and time for locale
  t = datetime.datetime.now().strftime('%c')
  print(t)       # Wed Nov 20 10:15:49 2013
  # hour
  t = datetime.datetime.now().strftime('%H %l')
  print(t)       # 10 10
  # A.M/P.M
  t = datetime.datetime.now().strftime('%p')
  print(t)       # AM
  t = datetime.datetime.now().strftime('%x')
  print(t)       # 11/20/13
  t = datetime.datetime.now().strftime('%X')
  print(t)       # 10:23:36
  t = datetime.datetime.now().strftime('%x %X')
  print(t)       # 11/20/13 10:24:47
  t = datetime.datetime.now().strftime('%z')
  print(t)       #
  t = datetime.datetime.now().strftime('%Z')
  print(t)       # 

  # 字符串转换成datetime
  t = datetime.datetime.strptime('Nov-20-13 09:42', '%b-%d-%y %H:%M')
  print(t)      # 2013-11-20 09:42:00
  t = datetime.datetime(2013, 11, 20, 9, 42)
  print(t)      # 2013-11-20 09:42:00
  # datetime转换成字符串
  t = datetime.datetime.now().strftime('%b-%d-%y %H:%M:%S')
  print(t)      # Nov-20-13 10:26:40

if __name__ == "__main__":
  format_time()
```

**运行结果：**2013-11-20 10:29:26.456640
2013-11-20 10:29:26
Nov-20-13 10:29:26
Nov-20-2013 10:29:26
Wed Wednesday 46 46 3
Nov November
20 324
Wed Nov 20 10:29:26 2013
10 10
AM
11/20/13
10:29:26
11/20/13 10:29:26

2013-11-20 09:42:00
2013-11-20 09:42:00
Nov-20-13 10:29:26


**timestamp <--> datetime**

```python
def main():
    createtime = int(time.time())       # createtime = 1393316647
    basefname2 = datetime.datetime.fromtimestamp(createtime).strftime('%Y-%m-%d %H:%M:%S')
    print("timestamp -> datetime: %d : %s" % (createtime, basefname2))
    
    nowtime = datetime.datetime.now()
    future = nowtime + datetime.timedelta(minutes = 5)
    print("datetime add: %s : %s" % (nowtime, future))
    
    ts = time.mktime(future.timetuple())
    print("datetime -> timestamp: %s : %d" % (future, ts))
```
运行结果：
timestamp -> datetime: 1393317675 : 2014-02-25 16:41:15
datetime add: 2014-02-25 16:41:15.900701 : 2014-02-25 16:46:15.900701
datetime -> timestamp: 2014-02-25 16:46:15.900701 : 1393317975

**timestamp 转 datetime**
"createtime":1389151309
basefname2= datetime.datetime.fromtimestamp(createtime).strftime('%Y-%m-%d %H:%M:%S')
结果：
"createtime2":"2014-01-08 11:21:49"
参考：[stackoverflow](http://stackoverflow.com/questions/3682748/converting-unix-timestamp-string-to-readable-date-in-python)，

**完整示例：**

```python
def convet_cdate(ctime):
    cday = 0
    chour = 0
    cminite = 0
    csecond = 0
    
#     ctime = '15秒前'
#     ctime = '10分钟前'
#     ctime = '2小时14分钟前'
#     ctime = '1天24分钟前'
#     ctime = '1天10小时24分钟前'
#     ctime = '1天10小时24分钟15秒前'
    ctime = '29分钟前'
#     ctime = '23小时46分钟前'
    
    if '天' in ctime:
        cday = int(ctime.split('天')[0])
        ctime = ctime.split('天')[1]
    if '小时' in ctime:
        chour = int(ctime.split('小时')[0])
        ctime = ctime.split('小时')[1]
    if '分钟' in ctime:
        cminite = int(ctime.split('分钟')[0])
        ctime = ctime.split('分钟')[1]
    if '秒' in ctime:
        csecond = int(ctime.split('秒')[0])
        
    elapse_time = cday * 24 * 3600 + chour * 3600 + cminite * 60 + csecond
    
    print("cday: " + str(cday) + "; chour: " + str(chour) + ", cminite: " + str(cminite) + "; csecond: " + str(csecond))
    print("elapse_time: " + str(elapse_time))
    
    ctime_now = int(time.time())
    print("ctime_now: " + str(ctime_now))
    print("elapse_now: " + str(ctime_now - elapse_time))
    
    # timestamp -> datetime
    result = datetime.datetime.fromtimestamp(ctime_now - elapse_time).strftime('%Y-%m-%d %H:%M:%S') 
    print("result: " + str(result))
```


