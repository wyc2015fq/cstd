# python时间处理包：datetime，time - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:34[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1579

# time包
time包基于C语言的库函数(library functions)。Python的解释器通常是用C编写的，Python的一些函数也会直接调用C语言的库函数。
```
import time
print(time.time())   # wall clock time, unit: second
print(time.clock())  # processor clock time, unit: second
```
time.sleep()可以将程序置于休眠状态，直到某时间间隔之后再唤醒程序，让程序继续运行。
```
import time
print('start')
time.sleep(10)     # sleep for 10 secondsprint('wake up')
```
当我们需要定时地查看程序运行状态时，就可以利用该方法。
time包还定义了struct_time对象。该对象实际上是将挂钟时间转换为年、月、日、时、分、秒……等日期信息，存储在该对象的各个属性中(tm_year, tm_mon, tm_mday...)。下面方法可以将挂钟时间转换为struct_time对象:
```
st = time.gmtime()      # 返回struct_time格式的UTC时间
st = time.localtime()   # 返回struct_time格式的当地时间, 当地时区根据系统环境决定。
s  = time.mktime(st)    # 将struct_time格式转换成wall clock time
```
# datetime包
## 简介
datetime包是基于time包的一个高级包， 为我们提供了多一层的便利。datetime可以理解为date和time两个组成部分。date是指年月日构成的日期(相当于日历)，time是指时分秒微秒构成的一天24小时中的具体时间(相当于手表)。你可以将这两个分开管理(datetime.date类，datetime.time类)，也可以将两者合在一起(datetime.datetime类)。
datetime模块定义了下面这几个类：
- datetime.date：表示日期的类。常用的属性有year, month, day；
- datetime.time：表示时间的类。常用的属性有hour, minute, second, microsecond；
- datetime.datetime：表示日期时间。
- datetime.timedelta：表示时间间隔，即两个时间点之间的长度。
- datetime.tzinfo：与时区有关的相关信息。
比如2012年9月3日21时30分，可以用如下方式表达：
```
import datetime
t = datetime.datetime(2012,9,3,21,30)
print(t)
```
所返回的t有如下属性:
hour, minute, second, microsecond
year, month, day, weekday   # weekday表示周几
## 运算
datetime包还定义了时间间隔对象(timedelta)。一个时间点(datetime)加上一个时间间隔(timedelta)可以得到一个新的时间点(datetime)。比如今天的上午3点加上5个小时得到今天的上午8点。同理，两个时间点相减会得到一个时间间隔。
```
import datetime
t      = datetime.datetime(2012,9,3,21,30)
t_next = datetime.datetime(2012,9,5,23,30)
delta1 = datetime.timedelta(seconds = 600)
delta2 = datetime.timedelta(weeks = 3)print(t + delta1)print(t + delta2)
print(t_next - t)
```
在给datetime.timedelta传递参数（如上的seconds和weeks）的时候，还可以是days, hours, milliseconds, microseconds。两个datetime对象还可以进行比较。比如使用上面的t和t_next:
`print(t > t_next)`
## datetime对象与字符串转换
一个方法是用正则表达式来搜索字符串，用格式化读取的方式读取时间信息。
```
from datetime import datetime
format = "output-%Y-%m-%d-%H%M%S.txt" 
str    = "output-1997-12-23-030000.txt" 
t      = datetime.strptime(str, format)
```
- strptime VS strftime
strptime, p = parsing 分[剖]析，分解; 重构，将输入的日期时间字符串转换为指定的格式并读取。
strftime, f = formatting 将时间对象转换为另外一种时间格式
具体的格式写法可参阅官方文档。 如果是Linux系统，也可查阅date命令的手册($man date)，两者相通。
字符串日期时间格式：
|格式字符|意义|
|----|----|
|%a|星期的简写。如 星期三为Web|
|%A|星期的全写。如 星期三为Wednesday|
|%b|月份的简写。如4月份为Apr|
|%B|月份的全写。如4月份为April|
|%c|日期时间的字符串表示。（如： 04/07/10 10:43:39）|
|%d|日在这个月中的天数（是这个月的第几天）|
|%f|微秒（范围[0,999999]）|
|%H|小时（24小时制，[0, 23]）|
|%I|小时（12小时制，[0, 11]）|
|%j|日在年中的天数 [001,366]（是当年的第几天）|
|%m|月份（[01,12]）|
|%M|分钟（[00,59]）|
|%p|AM或者PM|
|%S|秒（范围为[00,61]？|
|%U|周在当年的周数当年的第几周），星期天作为周的第一天|
|%w|今天在这周的天数，范围为[0, 6]，6表示星期天|
|%W|周在当年的周数（是当年的第几周），星期一作为周的第一天|
|%x|日期字符串（如：04/07/10）|
|%X|时间字符串（如：10:43:39）|
|%y|2个数字表示的年份|
|%Y|4个数字表示的年份|
|%z|与utc时间的间隔 （如果是本地时间，返回空字符串）|
|%Z|时区名称（如果是本地时间，返回空字符串）|
|%%|%% => %|
