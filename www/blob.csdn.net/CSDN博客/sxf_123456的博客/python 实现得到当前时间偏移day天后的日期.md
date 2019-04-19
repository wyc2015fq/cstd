# python 实现得到当前时间偏移day天后的日期 - sxf_123456的博客 - CSDN博客
2017年12月06日 17:51:02[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1353
**import  **datetime
*#以当前时间作为起始点，days=-7向前偏移7天，days=7向后偏移7天*time_now = datetime.datetime.now()
time = (time_now+datetime.timedelta(days=7)).strftime(**"%Y%m%d"**)
print(time)安装dateutil库，注意不是pip install dateutil,而是pip install python-dateutil**import  **datetime
**from **dateutil.relativedelta **import **relativedelta
*#以当前时间作为起始点，days=-7向前偏移7天，days=7向后偏移7天*time_now = datetime.datetime.now()
time = (time_now+datetime.timedelta(days=7)).strftime(**"%Y%m%d"**)
print(time)
*#以当前时间为起始点，偏移一个月*time_1=(time_now+relativedelta(months=-1)).strftime(**"%Y%m%d"**)
print(time_1)
