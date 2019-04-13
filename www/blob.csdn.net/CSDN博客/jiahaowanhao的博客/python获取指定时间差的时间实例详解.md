
# python获取指定时间差的时间实例详解 - jiahaowanhao的博客 - CSDN博客


2018年04月21日 21:13:40[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：204


[python获取指定时间差的时间实例详解](http://cda.pinggu.org/view/25334.html)
在分析数据的时间经常需要截取一定范围时间的数据，比如三天之内，两小时前等等时间要求的数据，因此将该部分经常需要用到的功能模块化，方便以后以后用到的时候复用。在此，也分享给大家。
import time
import sys
reload(sys)
def get_day_of_day(UTC=False, days=0, hours=0, miutes=0, seconds=0):
'''''''
if days>=0,date is larger than today
if days<0,date is less than today
date format = "YYYY-MM-DD"
'''
now = time.time()
timeNew = now + days*24*60*60 + hours*60*60 + miutes*60 + seconds
if UTC :
timeNew = timeNew + time.timezone
t = time.localtime(timeNew)
return time.strftime('%Y-%m-%d %H:%M:%S', t)
\#使用UTC时间 两小时前
t = get_day_of_day(True,0,-2)
print t
\#当地时间 三天前
t = get_day_of_day(False,-3)
print t
\#当地时间 三天后
t = get_day_of_day(False,3)
print t
运行后所得结果：
2016-04-30 20:25:56
2016-05-06 20:25:56
感谢阅读，希望能帮助到大家，谢谢大家对本站的支持！

