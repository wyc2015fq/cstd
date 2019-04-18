# python time模块 - nosmatch的专栏 - CSDN博客
2012年07月10日 11:13:34[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：2753标签：[python																[struct																[import																[calendar																[windows																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=calendar&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
Python模块学习 ---- time 日期时间处理
2009-08-07 14:29
在应用程序的开发过程中，难免要跟日期、时间处理打交道。如：记录一个复杂算法的执行时间；网络通信中数据包的延迟等等。Python中提供了time, datetime calendar等模块来处理时间日期，今天对time模块中最常用的几个函数作一个介绍。
## time.time
　　time.time()函数返回从1970年1月1日以来的秒数，这是一个浮点数。
## time.sleep
　　可以通过调用time.sleep来挂起当前的进程。time.sleep接收一个浮点型参数，表示进程挂起的时间。
## time.clock
　　在windows操作系统上，time.clock() 返回第一次调用该方法到现在的秒数，其精确度高于1微秒。可以使用该函数来记录程序执行的时间。下面是一个简单的例子：
- import time  
- 
- print time.clock() #1
- time.sleep(2)  
- print time.clock() #2
- time.sleep(3)  
- print time.clock() #3
- 
- #---- result
- #3.91111160776e-06
- #1.99919151736
- #4.99922364435
import time print time.clock() #1 time.sleep(2) print time.clock() #2 time.sleep(3) print time.clock() #3 #---- result #3.91111160776e-06 #1.99919151736 #4.99922364435
## time.gmtime
　　该函数原型为：time.gmtime([sec])，可选的参数sec表示从1970-1-1以来的秒数。其默认值为 time.time()，函数返回time.struct_time类型的对象。（struct_time是在time模块中定义的表示时间的对象），下 面是一个简单的例子：
- import time  
- 
- print time.gmtime()  #获取当前时间的struct_time对象
- print time.gmtime(time.time() - 24 * 60 * 60)  #获取昨天这个时间的struct_time对象
- 
- #---- result
- #time.struct_time(tm_year=2009, tm_mon=6, tm_mday=23, tm_hour=15, tm_min=16, tm_sec=3, tm_wday=1, tm_yday=174, tm_isdst=0)
- #time.struct_time(tm_year=2009, tm_mon=6, tm_mday=22, tm_hour=15, tm_min=16, tm_sec=3, tm_wday=0, tm_yday=173, tm_isdst=0)
import time print time.gmtime() #获取当前时间的struct_time对象 print time.gmtime(time.time() - 24 * 60 * 60) #获取昨天这个时间的struct_time对象 #---- result #time.struct_time(tm_year=2009, tm_mon=6, tm_mday=23, tm_hour=15, tm_min=16, tm_sec=3, tm_wday=1, tm_yday=174, tm_isdst=0)
 #time.struct_time(tm_year=2009, tm_mon=6, tm_mday=22, tm_hour=15, tm_min=16, tm_sec=3, tm_wday=0, tm_yday=173, tm_isdst=0)
## time.localtime
　　time.localtime与time.gmtime非常类似，也返回一个struct_time对象，可以把它看作是gmtime()的本地化版本。
## time.mktime
　　time.mktime执行与gmtime(), localtime()相反的操作，它接收struct_time对象作为参数，返回用秒数来表示时间的浮点数。例如:
- import time  
- 
- #下面两个函数返回相同（或相近）的结果
- print time.mktime(time.localtime())  
- print time.time()  
import time #下面两个函数返回相同（或相近）的结果 print time.mktime(time.localtime()) print time.time()
## time.strftime
　　time.strftime将日期转换为字符串表示，它的函数原型为：time.strftime(format[, t])。参数format是格式字符串（格式字符串的知识可以参考：[time.strftime](http://docs.python.org/library/time.html#time.strftime)），可选的参数t是一个struct_time对象。下面的例子将struct_time对象转换为字符串表示：
- import time  
- 
- print time.strftime('%Y-%m-%d %H:%M:%S', time.gmtime())  
- print time.strftime('Weekday: %w; Day of the yesr: %j')  
- 
- #---- result
- #2009-06-23 15:30:53
- #Weekday: 2; Day of the yesr: 174
import time print time.strftime('%Y-%m-%d %H:%M:%S', time.gmtime()) print time.strftime('Weekday: %w; Day of the yesr: %j') #---- result #2009-06-23 15:30:53 #Weekday: 2; Day of the yesr: 174
## time.strptime
　　按指定格式解析一个表示时间的字符串，返回struct_time对象。该函数原型为：time.strptime(string, format)，两个参数都是字符串，下面是一个简单的例子，演示将一个字符串解析为一个struct_time对象：
- import time  
- 
- print time.strptime('2009-06-23 15:30:53', '%Y-%m-%d %H:%M:%S')  
- 
- #---- result
- #time.struct_time(tm_year=2009, tm_mon=6, tm_mday=23, tm_hour=15, tm_min=30, tm_sec=53, tm_wday=1, tm_yday=174, tm_isdst=-1)
import time print time.strptime('2009-06-23 15:30:53', '%Y-%m-%d %H:%M:%S') #---- result #time.struct_time(tm_year=2009, tm_mon=6, tm_mday=23, tm_hour=15, tm_min=30, tm_sec=53, tm_wday=1, tm_yday=174, tm_isdst=-1)
