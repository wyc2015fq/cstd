# DB2 日期和时间的函数应用总结 - fanyun的博客 - CSDN博客
2016年11月02日 14:20:25[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：487
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
DAYNAME 返回一个大小写混合的字符串，对于参数的日部分，用星期表示这一天的名称（例如，Friday）。 
DAYOFWEEK 返回参数中的星期几，用范围在 1-7 的整数值表示，其中 1 代表星期日。 
DAYOFWEEK_ISO 返回参数中的星期几，用范围在 1-7 的整数值表示，其中 1 代表星期一。 
DAYOFYEAR 返回参数中一年中的第几天，用范围在 1-366 的整数值表示。 
DAYS 返回日期的整数表示。 
JULIAN_DAY 返回从公元前 4712 年 1 月 1 日（儒略日历的开始日期）到参数中指定日期值之间的天数，用整数值表示。 
MIDNIGHT_SECONDS 返回午夜和参数中指定的时间值之间的秒数，用范围在 0 到 86400 之间的整数值表示。 
MONTHNAME 对于参数的月部分的月份，返回一个大小写混合的字符串（例如，January）。 
TIMESTAMP_ISO 根据日期、时间或时间戳记参数而返回一个时间戳记值。 
TIMESTAMP_FORMAT 从已使用字符模板解释的字符串返回时间戳记。 
TIMESTAMPDIFF 根据两个时间戳记之间的时差，返回由第一个参数定义的类型表示的估计时差。 
TO_CHAR 返回已用字符模板进行格式化的时间戳记的字符表示。TO_CHAR 是 VARCHAR_FORMAT 的同义词。 
TO_DATE 从已使用字符模板解释过的字符串返回时间戳记。TO_DATE 是 TIMESTAMP_FORMAT 的同义词。 
WEEK 返回参数中一年的第几周，用范围在 1-54 的整数值表示。以星期日作为一周的开始。 
WEEK_ISO 返回参数中一年的第几周，用范围在 1-53 的整数值表示。 
要使当前时间或当前时间戳记调整到 GMT/CUT，则把当前的时间或时间戳记减去当前时区寄存器： 
current time - current timezone 
current timestamp - current timezone 
给定了日期、时间或时间戳记，则使用适当的函数可以单独抽取出（如果适用的话）年、月、日、时、分、秒及微秒各部分： 
YEAR (current timestamp) 
MONTH (current timestamp) 
DAY (current timestamp) 
HOUR (current timestamp) 
MINUTE (current timestamp) 
SECOND (current timestamp) 
MICROSECOND (current timestamp) 
因为没有更好的术语，所以您还可以使用英语来执行日期和时间计算： 
current date + 1 YEAR 
current date + 3 YEARS + 2 MONTHS + 15 DAYS 
current time + 5 HOURS - 3 MINUTES + 10 SECONDS 
从时间戳记单独抽取出日期和时间也非常简单： 
DATE (current timestamp) 
TIME (current timestamp) 
而以下示例描述了如何获得微秒部分归零的当前时间戳记： 
CURRENT TIMESTAMP - MICROSECOND (current timestamp) MICROSECONDS 
如果想将日期或时间值与其它文本相衔接，那么需要先将该值转换成字符串。为此，只要使用 CHAR() 函数： 
char(current date) 
char(current time) 
char(current date + 12 hours) 
要将字符串转换成日期或时间值，可以使用： 
TIMESTAMP ('2002-10-20-12.00.00.000000') 
TIMESTAMP ('2002-10-20 12:00:00') 
DATE ('2002-10-20') 
DATE ('10/20/2002') 
TIME ('12:00:00') 
TIME ('12.00.00') 
TIMESTAMP()、DATE() 和 TIME() 函数接受更多种格式。上面几种格式只是示例，我将把它作为一个练习，让读者自己去发现其它格式。 
有时，您需要知道两个时间戳记之间的时差。为此，DB2 提供了一个名为 TIMESTAMPDIFF() 的内置函数。但该函数返回的是近似值，因为它不考虑闰年，而且假设每个月只有 30 天。以下示例描述了如何得到两个日期的近似时差：
timestampdiff (<n>, char( 
timestamp('2002-11-30-00.00.00')- 
timestamp('2002-11-08-00.00.00'))) 
对于 <n>，可以使用以下各值来替代，以指出结果的时间单位： 
1 = 秒的小数部分 
2 = 秒 
4 = 分 
8 = 时 
16 = 天 
32 = 周 
64 = 月 
128 = 季度 
256 = 年 
当日期很接近时使用 timestampdiff() 比日期相差很大时精确。如果需要进行更精确的计算，可以使用以下方法来确定时差（按秒计）： 
(DAYS(t1) - DAYS(t2)) * 86400 + 
(MIDNIGHT_SECONDS(t1) - MIDNIGHT_SECONDS(t2)) 
为方便起见，还可以对上面的方法创建 SQL 用户定义的函数： 
CREATE FUNCTION secondsdiff(t1 TIMESTAMP, t2 TIMESTAMP) 
RETURNS INT 
RETURN ( 
(DAYS(t1) - DAYS(t2)) * 86400 + 
(MIDNIGHT_SECONDS(t1) - MIDNIGHT_SECONDS(t2)) 
) 
@ 
如果需要确定给定年份是否是闰年，以下是一个很有用的 SQL 函数，您可以创建它来确定给定年份的天数： 
CREATE FUNCTION daysinyear(yr INT) 
RETURNS INT 
RETURN (CASE (mod(yr, 400)) WHEN 0 THEN 366 ELSE 
CASE (mod(yr, 4)) WHEN 0 THEN 
CASE (mod(yr, 100)) WHEN 0 THEN 365 ELSE 366 END 
ELSE 365 END 
END)@ 
最后，以下是一张用于日期操作的内置函数表。它旨在帮助您快速确定可能满足您要求的函数，但未提供完整的参考。有关这些函数的更多信息，请参考 SQL 参考大全。 
SQL 日期和时间函数 
DAYNAME 返回一个大小写混合的字符串，对于参数的日部分，用星期表示这一天的名称（例如，Friday）。 
DAYOFWEEK 返回参数中的星期几，用范围在 1-7 的整数值表示，其中 1 代表星期日。 
DAYOFWEEK_ISO 返回参数中的星期几，用范围在 1-7 的整数值表示，其中 1 代表星期一。 
DAYOFYEAR 返回参数中一年中的第几天，用范围在 1-366 的整数值表示。 
DAYS 返回日期的整数表示。 
JULIAN_DAY 返回从公元前 4712 年 1 月 1 日（儒略日历的开始日期）到参数中指定日期值之间的天数，用整数值表示。 
MIDNIGHT_SECONDS 返回午夜和参数中指定的时间值之间的秒数，用范围在 0 到 86400 之间的整数值表示。 
MONTHNAME 对于参数的月部分的月份，返回一个大小写混合的字符串（例如，January）。 
TIMESTAMP_ISO 根据日期、时间或时间戳记参数而返回一个时间戳记值。 
TIMESTAMP_FORMAT 从已使用字符模板解释的字符串返回时间戳记。 
TIMESTAMPDIFF 根据两个时间戳记之间的时差，返回由第一个参数定义的类型表示的估计时差。 
TO_CHAR 返回已用字符模板进行格式化的时间戳记的字符表示。TO_CHAR 是 VARCHAR_FORMAT 的同义词。 
TO_DATE 从已使用字符模板解释过的字符串返回时间戳记。TO_DATE 是 TIMESTAMP_FORMAT 的同义词。 
WEEK 返回参数中一年的第几周，用范围在 1-54 的整数值表示。以星期日作为一周的开始。 
WEEK_ISO 返回参数中一年的第几周，用范围在 1-53 的整数值表示。 
使用示例：
```
Code highlighting produced by Actipro CodeHighlighter (freeware)http://www.CodeHighlighter.com/-->--获取当前日期：
select current date from sysibm.sysdummy1; 
values current date;
--获取当前日期 
select current time from sysibm.sysdummy1; 
values current time; 
--获取当前时间戳 
select current timestamp from sysibm.sysdummy1; 
values current timestamp; 
--要使当前时间或当前时间戳记调整到 GMT/CUT，则把当前的时间或时间戳记减去当前时区寄存器：
values current time -current timezone; 
values current timestamp -current timezone; 
--获取当前年份
values year(current timestamp);
--获取当前月 
values month(current timestamp);
--获取当前日 
values day(current timestamp);
--获取当前时 
values hour(current timestamp);
--获取分钟 
values minute(current timestamp);
--获取秒 
values second(current timestamp);
--获取毫秒 
values microsecond(current timestamp); 
--从时间戳记单独抽取出日期和时间
values date(current timestamp); 
values VARCHAR_FORMAT(current TIMESTAMP,'yyyy-mm-dd'); 
values char(current date); 
values time(current timestamp); 
--执行日期和时间的计算
values current date+1 year;    
values current date+3 years+2 months +15 days; 
values current time +5 hours -3 minutes +10 seconds; 
--计算两个日期之间的天数
values days(current date)- days(date('2010-02-20')); 
--时间和日期换成字符串
values char(current date); 
values char(current time); 
--要将字符串转换成日期或时间值
values timestamp('2010-03-09-22.43.00.000000'); 
values timestamp('2010-03-09 22:44:36'); 
values date('2010-03-09'); 
values date('03/09/2010'); 
values time('22:45:27'); 
values time('22.45.27'); 
--计算两个时间戳记之间的时差：
--秒的小数部分为单位 
values timestampdiff(1,char(current timestamp - timestamp('2010-01-01-00.00.00'))); 
--秒为单位 
values timestampdiff(2,char(current timestamp - timestamp('2010-01-01-00.00.00'))); 
--分为单位 
values timestampdiff(4,char(current timestamp - timestamp('2010-01-01-00.00.00'))); 
--小时为单位 
values timestampdiff(8,char(current timestamp - timestamp('2010-01-01-00.00.00'))); 
--天为单位 
values timestampdiff(16,char(current timestamp - timestamp('2010-01-01-00.00.00'))); 
--周为单位 
values timestampdiff(32,char(current timestamp - timestamp('2010-01-01-00.00.00'))); 
--月为单位 
values timestampdiff(64,char(current timestamp - timestamp('2010-01-01-00.00.00'))); 
--季度为单位 
values timestampdiff(128,char(current timestamp - timestamp('2010-01-01-00.00.00'))); 
--年为单位 
values timestampdiff(256,char(current timestamp - timestamp('2010-01-01-00.00.00')));
```
