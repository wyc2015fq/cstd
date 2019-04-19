# python 时间间隔判断 - sxf_123456的博客 - CSDN博客
2018年01月26日 11:12:18[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1850
**from **datetime **import **datetime,timedelta
**import  **time
t = time.localtime()  *#<class 'time.struct_time'>*now = time.strftime(**'%Y%m%d%H%M%S'**,t) *#str*time.sleep(2)
t2 = time.localtime()
now2 = time.strftime(**'%Y%m%d%H%M%S'**,t2) *#str*start = datetime.fromtimestamp(time.mktime(time.strptime(now,**'%Y%m%d%H%M%S'**))) *#<class 'datetime.datetime'>*end = datetime.fromtimestamp(time.mktime(time.strptime(now2,**'%Y%m%d%H%M%S'**)))  *#<class 'datetime.datetime'>*print(end-start == timedelta(seconds=2) ) *#时间间隔2秒 ,True*start_1 = datetime.now()  *#<class 'datetime.datetime'>*time.sleep(2)
end_1 = datetime.now()  *#<class 'datetime.datetime'>*print(end_1-start_1 < timedelta(seconds=2)) *#False*print(end_1-start_1 == timedelta(seconds=2)) *#False*print(end_1-start_1 > timedelta(seconds=2)) *#True*
