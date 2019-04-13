
# Hive日期函数 - anlcy - 博客园






# [Hive日期函数](https://www.cnblogs.com/camilla/p/8618628.html)
无论做什么数据，都离不开日期函数的使用。
这里转载一下Hive的日期函数的使用，写的相当完整。
日期函数UNIX时间戳转日期函数: from_unixtime语法:from_unixtime(bigint unixtime[, string format])
返回值: string
说明: 转化UNIX时间戳（从1970-01-01 00:00:00 UTC到指定时间的秒数）到当前时区的时间格式
举例：
hive> select from_unixtime(1323308943,'yyyyMMdd') from dual;
20111208
获取当前UNIX时间戳函数: unix_timestamp语法:   unix_timestamp()
返回值:   bigint
说明: 获得当前时区的UNIX时间戳
举例：
hive>   select unix_timestamp() from dual;
1323309615
日期转UNIX时间戳函数: unix_timestamp语法:unix_timestamp(string date)
返回值:   bigint
说明: 转换格式为“yyyy-MM-dd HH:mm:ss“的日期到UNIX时间戳。如果转化失败，则返回0。
举例：
hive>   select unix_timestamp('2011-12-07 13:01:03') from dual;
1323234063
指定格式日期转UNIX时间戳函数: unix_timestamp语法:   unix_timestamp(string date, string pattern)
返回值:   bigint
说明: 转换pattern格式的日期到UNIX时间戳。如果转化失败，则返回0。
举例：
hive>   select unix_timestamp('20111207 13:01:03','yyyyMMdd HH:mm:ss') from dual;
1323234063
日期时间转日期函数: to_date语法:   to_date(string timestamp)
返回值:   string
说明: 返回日期时间字段中的日期部分。
举例：
hive>   select to_date('2011-12-08 10:03:01') from dual;
2011-12-08
日期转年函数: year语法:   year(string date)
返回值: int
说明: 返回日期中的年。
举例：
hive>   select year('2011-12-08 10:03:01') from dual;
2011
hive>   select year('2012-12-08') from dual;
2012
日期转月函数: month语法: month   (string date)
返回值: int
说明: 返回日期中的月份。
举例：
hive>   select month('2011-12-08 10:03:01') from dual;
12
hive>   select month('2011-08-08') from dual;
8
日期转天函数: day语法: day   (string date)
返回值: int
说明: 返回日期中的天。
举例：
hive>   select day('2011-12-08 10:03:01') from dual;
8
hive>   select day('2011-12-24') from dual;
24
日期转小时函数: hour语法: hour   (string date)
返回值: int
说明: 返回日期中的小时。
举例：
hive>   select hour('2011-12-08 10:03:01') from dual;
10
日期转分钟函数: minute语法: minute   (string date)
返回值: int
说明: 返回日期中的分钟。
举例：
hive>   select minute('2011-12-08 10:03:01') from dual;
3
日期转秒函数: second语法: second   (string date)
返回值: int
说明: 返回日期中的秒。
举例：
hive>   select second('2011-12-08 10:03:01') from dual;
1
日期转周函数: weekofyear语法:   weekofyear (string date)
返回值: int
说明: 返回日期在当前的周数。
举例：
hive>   select weekofyear('2011-12-08 10:03:01') from dual;
49
日期比较函数: datediff语法:   datediff(string enddate, string startdate)
返回值: int
说明: 返回结束日期减去开始日期的天数。
举例：
hive>   select datediff('2012-12-08','2012-05-09') from dual;
213
日期增加函数: date_add语法:   date_add(string startdate, int days)
返回值: string
说明: 返回开始日期startdate增加days天后的日期。
举例：
hive>   select date_add('2012-12-08',10) from dual;
2012-12-18
日期减少函数: date_sub语法:   date_sub (string startdate, int days)
返回值: string
说明: 返回开始日期startdate减少days天后的日期。
举例：
hive>   select date_sub('2012-12-08',10) from dual;
2012-11-28





