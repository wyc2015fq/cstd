# SQLite中的时间日期函数 - weixin_33985507的博客 - CSDN博客
2013年01月25日 09:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
SQLite包含了如下时间/日期函数：
datetime().......................产生日期和时间
date()...........................产生日期
time()...........................产生时间
strftime().......................对以上三个函数产生的日期和时间进行格式化
datetime()的用法是：datetime(日期/时间,修正符,修正符...)
date()和time()的语法与datetime()相同。
在时间/日期函数里可以使用如下格式的字符串作为参数：
YYYY-MM-DD
YYYY-MM-DD HH:MM
YYYY-MM-DD HH:MM:SS
YYYY-MM-DD HH:MM:SS.SSS
HH:MM
HH:MM:SS
HH:MM:SS.SSS
now
其中now是产生现在的时间。
举例（写这个笔记的时间是2006年10月17日晚8点到10点，测试环境：SQLite 2.8.17，WinXP，北京时间）：例1.
select datetime('now'); 
结果：2006-10-17 12:55:54
例2.
select datetime('2006-10-17'); 
结果：2006-10-17 12:00:00
例3.
select datetime('2006-10-17 00:20:00','+1 hour','-12 minute');
结果：2006-10-17 01:08:00
例4.
select date('2006-10-17','+1 day','+1 year');
结果：2007-10-18
例5.
select datetime('now','start of year');
结果：2006-01-01 00:00:00
例6.
select datetime('now','start of month');
结果：2006-10-01 00:00:00
例7.
select datetime('now','start of day');
结果：2006-10-17 00:00:00
例8.
select datetime('now','+10 hour','start of day','+10 hour');
结果：2006-10-17 10:00:00
例9.
select datetime('now','localtime');
结果：2006-10-17 21:21:47
例10.
select datetime('now','+8 hour');
结果：2006-10-17 21:24:45
例3中的+1 hour和-12 minute表示可以在基本时间上（datetime函数的第一个参数）增加或减少一定时间。
例5中的start of year表示一年开始的时间。
从例8可以看出，尽管第2个参数加上了10个小时，但是却被第3个参数“start of day”把时间归零到00:00:00，随后的第4个参数在00:00:00
的基础上把时间增加了10个小时变成了10:00:00。
例9把格林威治时区转换成本地时区。
例10把格林威治时区转换成东八区。
strftime()函数可以把YYYY-MM-DD HH:MM:SS格式的日期字符串转换成其它形式的字符串。
strftime()的语法是strftime(格式, 日期/时间, 修正符, 修正符, ...)
它可以用以下的符号对日期和时间进行格式化：
%d 月份, 01-31
%f 小数形式的秒，SS.SSS
%H 小时, 00-23
%j 算出某一天是该年的第几天，001-366
%m 月份，00-12
%M 分钟, 00-59
%s 从1970年1月1日到现在的秒数
%S 秒, 00-59
%w 星期, 0-6 (0是星期天)
%W 算出某一天属于该年的第几周, 01-53
%Y 年, YYYY
%% 百分号
strftime()的用法举例如下：例11.
select strftime('%Y.%m.%d %H:%M:%S','now','localtime');
结果：2006.10.17 21:41:09
例11用圆点作为日期的分隔附，并把时间转换为当地的时区的时间。
更多关于SQLite日期时间函数方面的内容，可以参考Chris Newman写的《SQLite》（ISBN：0-672-32685-X）中的《Working with Dates and 
Times》一文。
