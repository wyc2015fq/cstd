# 【Python】 时间 日历 - YZXnuaa的博客 - CSDN博客
2018年02月05日 10:31:35[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：89
#!/usr/bin/python
# -*- coding: UTF-8 -*-
import datetime
i = datetime.datetime.now()
print ("当前的日期和时间是 %s" % i)
print ("ISO格式的日期和时间是 %s" % i.isoformat() )
print ("当前的年份是 %s" %i.year)
print ("当前的月份是 %s" %i.month)
print ("当前的日期是  %s" %i.day)
print ("dd/mm/yyyy 格式是  %s/%s/%s" % (i.day, i.month, i.year) )
print ("当前小时是 %s" %i.hour)
print ("当前分钟是 %s" %i.minute)
print ("当前秒是  %s" %i.second)
#!/usr/bin/python
# -*- coding: UTF-8 -*-
import calendar
cal = calendar.month(2018, 2)
print ("以下输出2016年1月份的日历:")
print (cal)
当前的日期和时间是 2018-02-05 10:24:48.263873
ISO格式的日期和时间是 2018-02-05T10:24:48.263873
当前的年份是 2018
当前的月份是 2
当前的日期是  5
dd/mm/yyyy 格式是  5/2/2018
当前小时是 10
当前分钟是 24
当前秒是  48
以下输出2016年1月份的日历:
   February 2018
Mo Tu We Th Fr Sa Su
          1  2  3  4
 5  6  7  8  9 10 11
12 13 14 15 16 17 18
19 20 21 22 23 24 25
26 27 28
