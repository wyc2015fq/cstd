# Python-GoogleCourse 学习 Day5 - fighting！！！ - CSDN博客
2019年02月25日 11:33:23[dujiahei](https://me.csdn.net/dujiahei)阅读数：23
**目录**
[参考：](#%E5%8F%82%E8%80%83%EF%BC%9A)
[知识点：](#%E7%9F%A5%E8%AF%86%E7%82%B9%EF%BC%9A)
## 参考：
[http://www.runoob.com/python3/python3-file-methods.html](http://www.runoob.com/python3/python3-file-methods.html)
[http://www.runoob.com/python3/python3-date-time.html](http://www.runoob.com/python3/python3-date-time.html)
[https://blog.csdn.net/qq_24753293/article/details/80954840](https://blog.csdn.net/qq_24753293/article/details/80954840)
[https://docs.python.org/3/library/datetime.html](https://docs.python.org/3/library/datetime.html)
## 知识点：
- 文件的读写方式
	- 打开文件方式/open 方法
		- 概念
			Python open() 方法用于打开一个文件，并返回文件对象，在对文件进行处理过程都需要使用到这个函数，如果该文件无法被打开，会抛出 OSError。
			注意点：使用 open() 方法一定要保证关闭文件对象，即调用 close() 方法。
			open() 函数常用形式是接收两个参数：文件名(file)和模式(mode)。
			open(file, mode='r')​
- 语法格式
			open(file, mode='r')
			完整的语法格式为：
			open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)​
- 参数说明
			file: 必需，文件路径（相对或者绝对路径）。
			mode: 可选，文件打开模式
			buffering: 设置缓冲
			encoding: 一般使用utf8
			errors: 报错级别
			newline: 区分换行符
			closefd: 传入的file参数类型
- mode参数
				
![](https://img.mubu.com/document_image/a3a1bc56-801b-4080-ad13-f21455b4569d-2329912.jpg)
- file 对象：file 对象使用 open 函数来创建，下表列出了 file 对象常用的函数
		
![](https://img.mubu.com/document_image/7e31ac78-3245-4630-afe1-42dd74aac58e-2329912.jpg)
![](https://img.mubu.com/document_image/69b4730c-8918-4ce8-adef-67508cfb357a-2329912.jpg)
![](https://img.mubu.com/document_image/aeef4b0f-30bb-4fd5-80d8-426134dfa9ce-2329912.jpg)
- os 模块：os 模块提供了非常丰富的方法用来处理文件和目录。
	
![](https://img.mubu.com/document_image/6f6de514-048c-456b-b0eb-f2e0d5be370a-2329912.jpg)
![](https://img.mubu.com/document_image/d8aa6180-e8b1-4a41-8d52-2741bc997d1b-2329912.jpg)
![](https://img.mubu.com/document_image/ba6fbb16-1569-4657-ac71-15a7c7a95636-2329912.jpg)
![](https://img.mubu.com/document_image/26e9998a-1025-4fc3-b91e-10d32c2b17d4-2329912.jpg)
![](https://img.mubu.com/document_image/04581401-b0da-41ae-9433-d4baeae1d274-2329912.jpg)
- datetime 模块/日期和时间
	- time库与datetime库的区别
		在 Python 文档里，time是归类在Generic Operating System Services中，换句话说， 它提供的功能是更加接近于操作系统层面的。通读文档可知，time 模块是围绕着 Unix Timestamp 进行的。
		该模块主要包括一个类 struct_time，另外其他几个函数及相关常量。 需要注意的是在该模块中的大多数函数是调用了所在平台C library的同名函数， 所以要特别注意有些函数是平台相关的，可能会在不同的平台有不同的效果。另外一点是，由于是基于Unix Timestamp，所以其所能表述的日期范围被限定在 1970 - 2038 之间，如果你写的代码需要处理在前面所述范围之外的日期，那可能需要考虑使用datetime模块更好
		所以 一般情况下我们用datetime库就可以解决大部分问题
- datetime和pandas时间序列分析和处理Timeseries
- datetime库的用法
		datetime模块用于是date和time模块的合集，datetime有两个常量，MAXYEAR和MINYEAR，分别是9999和1.
		datetime模块定义了5个类，分别是
		1.datetime.date：表示日期的类
		2.datetime.datetime：表示日期时间的类
		3.datetime.time：表示时间的类
		from datetime import date
		from datetime import datetime
		from datetime import time
		from datetime import timedelta
		from datetime import tzinfo 
		from datetime import * #不知道用啥 全部导入就可以
		4.datetime.timedelta：表示时间间隔，即两个时间点的间隔
		5.datetime.tzinfo：时区的相关信息
		​
		- date类
			- 参数
				date类有三个参数,datetime.date(year,month,day)，返回year-month-day
- 方法
				1.datetime.date.ctime(),返回格式如 Sun Apr 16 00:00:00 2017
				2.datetime.date.fromtimestamp(timestamp),根据给定的时间戮，返回一个date对象；[datetime.date.today](http://datetime.date.today)()作用相同
				3.datetime.date.isocalendar():返回格式如(year，month，day)的元组,(2017, 15, 6)
				4.datetime.date.isoformat()：返回格式如YYYY-MM-DD
				5.datetime.date.isoweekday()：返回给定日期的星期（0-6）星期一=0，星期日=6 这里表明下python3中是从[1-7]表示的 就是本来是星期几现在显示就是星期几
				6.datetime.date.replace(year,month,day)：替换给定日期，但不改变原日期
				7.datetime.date.strftime(format):把日期时间按照给定的format进行格式化。
				8.datetime.date.timetuple()：返回日期对应的time.struct_time对象
				　　time.struct_time(tm_year=2017, tm_mon=4, tm_mday=15, tm_hour=0, tm_min=0, tm_sec=0, tm_wday=5, tm_yday=105, tm_isdst=-1)
				9.datetime.date.weekday()：返回日期的星期
- time类
			- 参数
				time类有5个参数，datetime.time(hour,minute,second,microsecond,tzoninfo),返回08:29:30
- 方法
				1.datetime.time.replace()
				2.datetime.time.strftime(format):按照format格式返回时间
				3.datetime.time.tzname()：返回时区名字
				4.datetime.time.utcoffset()：返回时区的时间偏移量
- datetime类
			- 参数
				datetime类有很多参数，datetime(year, month, day[, hour[, minute[, second[, microsecond[,tzinfo]]]]])，返回年月日，时分秒
- 方法
				datetime.datetime.ctime()
				datetime.datetime.now().date()：返回当前日期时间的日期部分
				datetime.datetime.now().time()：返回当前日期时间的时间部分
				datetime.datetime.fromtimestamp()
				datetime.datetime.now()：返回当前系统时间
				datetime.datetime.replace()
				datetime.datetime.strftime()：由日期格式转化为字符串格式
				datetime.datetime.strptime():由字符串格式转化为日期格式
- timedelta类
			datetime.datetime.timedelta用于计算两个日期之间的差值。
- python中时间日期格式化符号
		%y 两位数的年份表示（00-99）
		%Y 四位数的年份表示（000-9999）
		%m 月份（01-12）
		%d 月内中的一天（0-31）
		%H 24小时制小时数（0-23）
		%I 12小时制小时数（01-12）
		%M 分钟数（00=59）
		%S 秒（00-59）
		%a 本地简化星期名称
		%A 本地完整星期名称
		%b 本地简化的月份名称
		%B 本地完整的月份名称
		%c 本地相应的日期表示和时间表示
		%j 年内的一天（001-366）
		%p 本地A.M.或P.M.的等价符
		%U 一年中的星期数（00-53）星期天为星期的开始
		%w 星期（0-6），星期天为星期的开始
		%W 一年中的星期数（00-53）星期一为星期的开始
		%x 本地相应的日期表示
		%X 本地相应的时间表示
		%Z 当前时区的名称
		%% %号本身
