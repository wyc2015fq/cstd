# DB2函数使用总结 - fanyun的博客 - CSDN博客
2017年01月25日 09:13:01[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：555
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
** 函数名：**
** 函数解释                               函数举例**
 AVG()
 返回一组数值的平均值.　SELECTAVG(SALARY)FROMBSEMPMS;
 CORR(),CORRELATION()
 返回一对数值的关系系数.
 SELECT CORRELATION(SALARY,BONUS)FROM   BSEMPMS;
 COUNT()
 返回一组行或值的个数.SELECTCOUNT(*)FROMBSEMPMS;
 COVAR(),COVARIANCE()
 返回一对数值的协方差.
 　　SELECTCOVAR(SALARY,BONUS)FROMBSEMPMS;
 MAX()
 返回一组数值中的最大值.
 　　SELECTMAX(SALARY)FROMBSEMPMS;
 MIN()
 返回一组数值中的最小值.
 　　SELECTMIN(SALARY)FROMBSEMPMS;
 STDDEV()
 返回一组数值的标准偏差.
 　　SELECTSTDDEV(SALARY)FROMBSEMPMS;
 SUM()
 返回一组数据的和.
 　　SELECTSUM(SALARY)FROMBSEMPMS;
 VAR(),VARIANCE()
 返回一组数值的方差.
 　　SELECTVARIANCE(SALARY)FROMBSEMPMS;
 ABS(),ABSVAL()
 返回参数的绝对值.
 　　SELECTABS(-3.4)FROMBSEMPMS;
 ACOS()
 返回参数的反余弦值.
 　　SELECTACOS(0.9)FROMBSEMPMS;
 ASCII()
 返回整数参数最左边的字符的ASCII码.　　SELECTASCII('R')FROMBSEMPMS;
 ASIN()
 返回用弧度表示的角度的参数的反正弦函数.　　SELECTASIN(0.9)FROMBSEMPMS;
 ATAN()
 返回参数的反正切值,该参数用弧度表示的角度的参数.　　SELECTATAN(0.9)FROMBSEMPMS;
 ATAN2()
 返回用弧度表示的角度的X和Y坐标的反正切值.　　SELECTATAN2(0.5,0.9)FROMBSEMPMS;
 BIGINT()
 返回整型常量中的数字或字符串的64位整数表示
 SELECTBIGINT(EMP_NO)FROMBSEMPMS;
 CEILING() OR CEIL()
 返回比参数大或等于参数的最小的整数值
 CHAR()
 返回日期时间型,字符串,整数,十进制或双精度浮点数的字符串表示. SELECTCHAR(SALARY,',')FROMBSEMPMS;
 CHR()
 返回具有由参数指定的ASCII码的字符
 SELECTCHAR(167)FROMBSEMPMS;
 CONCAT()
 返回两个字符串的连接.
 SELECTCONCAT(EMP_NO,EMP_NAM)FROMBSEMPMS;
 YEAR()
 返回数值的年部分.SELECTYEAR('2003/01/02')FROMBSEMPMS;
 VARCHAR()
 返回字符串,日期型,图形串的可变长度的字符串表示
 SELECTVARCHAR(EMP_NAM,50)FROMBSEMPMS;
 UCASE() OR UPPER()
 返回字符串的大写
 TRUNCATE() OR TRUNC()
 从表达式小数点右边的位置开始截断并返回该数值.
 SELECTTRUNCATE(345.6789,2)FROMBSEMPMS;
 TIME()
 返回一个数值中的时间
 SELECTTIME('2001-03-19.12.30.123456')FROMBSEMPMS;
 SUBSTR(EXP1,EXP2)
 返回EXP1串自EXP2处开始的子串.
 SQRT()
 返回该参数的平方根.SELECTSQRT(36)FROMBSEMPMS;
 SPACE()
 返回由参数指定的长度,包含空格在内的字符串.SELECTSPACE(10)FROMBSEMPMS;
 SECOND()
 返回一个数值的秒部分.SELECTSECOND('18:34:32')FROMBSEMPMS;
 RTRIM()
 删除字符串尾部的空格.
 　　SELECTRTRIM('COMMENT')FROMBSEMPMS;
 ROUND(EXP1,EXP2)
 返回EXP1小数点右边的第EXP2位置处开始的四舍五入值.
 　　SELECTROUND(2345.6789,2)FROMBSEMPMS
 REPLACE(EXP1,EXP2,EXP3)
 用EXP3替代EXP1中所有的EXP2
 SELECTCHAR(REPLACE('ROMANDD','NDD','CCB'),10)
 FROMBSEMPMS;
 REPEAT(EXP1,EXP2)
 返回EXP1重复EXP2次后的字符串.
 SELECTCHAR(REPEAT('REPEAT',3),21)FROMBSEMPMS;
 REAL()
 返回一个数值的单精度浮点数表示.
 　　SELECTREAL(10)FROMBSEMPMS;
 RAND()
 返回0和1之间的随机浮点数.SELECTRAND()FROMBSEMPMS;
 POWER(EXP1,EXP2)
 返回EXP1的EXP2次幂.SELECTPOWER(2,5)FROMBSEMPMS;
 POSSTR(EXP1,EXP2)
 返回EXP2在EXP1中的位置.SELECT('ABCDEFGH','D')FROMBSEMPMS;
 NULLIF(EXP1,EXP2)
 如果EXP1=EXP2,则为NULL,否则为EXP1
 NODENUMBER()
 返回行的分区号.SELECTNODENUMBER(EMP_NO)FROMBSEMPMS;
 MONTH()
 返回一个数值的月部分.SELECTMONTH('2003/10/20')FROMBSEMPMS;
 MOD(EXP1,EXP2)
 返回EXP1除以EXP2的余数.SELECTMOD(20,8)FROMBSEMPMS
 MINUTE()
 返回一个数值的分钟部分.
 　　SELECTMINUTE('18:34:23')FROMBSEMPMS;
 LTRIM()
 删除字符串前面的空格.SELECTLTRIM('CDDD')FROMBSEMPMS;
 HOUR()
 返回一个数值的小时部分.
 　　SELECTHOUR('18:34:23')FROMBSEMPMS;
 DOUBLE()
 如果参数是一个数字表达式,返回与其相对应的浮点数,如果参数是字符串表达式,则返回该数的字符串表达式.
 　　SELECTDOUBLE('5678')FROMBSEMPMS;
 EXP()
 返回参数的指数函数.SELECTEXP(2)FROMBSEMPMS;
 FLOAT()
 返回一个数的浮点表示.SELECTFLOAT(789)FROMBSEMPMS;
 FLOOR()
 返回小于或等于参数的最大整数.SLECTFLOOR(88.93)FROMBSEMPMS;
 HEX()
 返回一个表示为字符串的值的16进制表示.
 SELECTHEX(16)FROMBSEMPMS;
 DAYNAME
 返回一个大小写混合的字符串，对于参数的日部分，用星期表示这一天的名称（例如，Friday）。
 DAYOFWEEK
 返回参数中的星期几，用范围在1-7 的整数值表示，其中1 代表星期日。
 DAYOFWEEK_ISO
 返回参数中的星期几，用范围在1-7 的整数值表示，其中1 代表星期一。
 DAYOFYEAR
 返回参数中一年中的第几天，用范围在1-366的整数值表示。
 DAYS
 返回日期的整数表示。
 JULIAN_DAY
 返回从公元前4712 年1 月1 日（儒略日历的开始日期）到参数中指定日期值之间的天数，用整数值表示。
 MIDNIGHT_SECONDS
 返回午夜和参数中指定的时间值之间的秒数，用范围在0 到86400 之间的整数值表示。
 MONTHNAME
 对于参数的月部分的月份，返回一个大小写混合的字符串（例如，January）。
 TIMESTAMP_ISO
 根据日期、时间或时间戳记参数而返回一个时间戳记值。
 TIMESTAMP_FORMAT
 从已使用字符模板解释的字符串返回时间戳记。
 TIMESTAMPDIFF
 根据两个时间戳记之间的时差，返回由第一个参数定义的类型表示的估计时差。
 TO_CHAR
 返回已用字符模板进行格式化的时间戳记的字符表示。TO_CHAR 是VARCHAR_FORMAT 的同义词。
 TO_DATE
 从已使用字符模板解释过的字符串返回时间戳记。TO_DATE 是TIMESTAMP_FORMAT 的同义词。
 WEEK
  返回参数中一年的第几周，用范围在1-54 的整数值表示。以星期日作为一周的开始。
 WEEK_ISO
  返回参数中一年的第几周，用范围在1-53 的整数值表示。
     要使当前时间或当前时间戳记调整到GMT/CUT，则把当前的时间或时间戳记减去当前时区寄存器：
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
 current date + 3 YEARS + 2 MONTHS +15 DAYS 
 current time + 5 HOURS - 3 MINUTES +10 SECONDS
 从时间戳记单独抽取出日期和时间也非常简单： 
 DATE (current timestamp) 
 TIME (current timestamp)
 而以下示例描述了如何获得微秒部分归零的当前时间戳记： 
 CURRENT TIMESTAMP - MICROSECOND(current timestamp) MICROSECONDS 
 如果想将日期或时间值与其它文本相衔接，那么需要先将该值转换成字符串。为此，只要使用CHAR() 函数：
 char(current date) 
 char(current time) 
 char(current date + 12 hours) 
 要将字符串转换成日期或时间值，可以使用： 
 TIMESTAMP('2002-10-20-12.00.00.000000') 
 TIMESTAMP ('2002-10-20 12:00:00') 
 DATE ('2002-10-20') 
 DATE ('10/20/2002') 
 TIME ('12:00:00') 
 TIME ('12.00.00') 
 TIMESTAMP()、DATE() 和TIME() 函数接受更多种格式。上面几种格式只是示例，我将把它作为一个练习，让读者自己去发现其它格式。
 有时，您需要知道两个时间戳记之间的时差。为此，DB2 提供了一个名为TIMESTAMPDIFF() 的内置函数。但该函数返回的是近似值，因为它不考虑闰年，而且假设每个月只有30 天。以下示例描述了如何得到两个日期的近似时差：
 timestampdiff (<n>, char( 
 timestamp('2002-11-30-00.00.00')- 
 timestamp('2002-11-08-00.00.00'))) 
 对于<n>，可以使用以下各值来替代，以指出结果的时间单位： 
 1 = 秒的小数部分  
 2 = 秒  
 4 = 分  
 8 = 时  
 16 = 天  
 32 = 周  
 64 = 月  
 128 = 季度  
 256 = 年  
 当日期很接近时使用timestampdiff()比日期相差很大时精确。如果需要进行更精确的计算，可以使用以下方法来确定时差（按秒计）：
 (DAYS(t1) - DAYS(t2)) * 86400+   
 (MIDNIGHT_SECONDS(t1) -MIDNIGHT_SECONDS(t2)) 
 为方便起见，还可以对上面的方法创建SQL 用户定义的函数： 
 CREATE FUNCTION secondsdiff(t1TIMESTAMP, t2 TIMESTAMP) 
 RETURNS INT 
 RETURN ( 
 (DAYS(t1) - DAYS(t2)) * 86400+   
 (MIDNIGHT_SECONDS(t1) - MIDNIGHT_SECONDS(t2))
 ) 
 @  
    如果需要确定给定年份是否是闰年，以下是一个很有用的SQL 函数，您可以创建它来确定给定年份的天数：
 CREATE FUNCTION daysinyear(yr INT) 
 RETURNS INT 
 RETURN (CASE (mod(yr, 400)) WHEN 0THEN 366 ELSE  
         CASE(mod(yr, 4))   WHEN 0 THEN  
         CASE(mod(yr, 100)) WHEN 0 THEN 365 ELSE 366 END  
         ELSE365 END 
 END)@ 
       最后，以下是一张用于日期操作的内置函数表。它旨在帮助您快速确定可能满足您要求的函数，但未提供完整的参考。有关这些函数的更多信息，请参考SQL 参考大全。
     SQL 日期和时间函数 
 DAYNAME 返回一个大小写混合的字符串，对于参数的日部分，用星期表示这一天的名称（例如，Friday）。  
 DAYOFWEEK 返回参数中的星期几，用范围在1-7 的整数值表示，其中1 代表星期日。  
 DAYOFWEEK_ISO 返回参数中的星期几，用范围在1-7 的整数值表示，其中1 代表星期一。  
 DAYOFYEAR 返回参数中一年中的第几天，用范围在1-366 的整数值表示。  
 DAYS 返回日期的整数表示。  
 JULIAN_DAY 返回从公元前4712 年1 月1 日（儒略日历的开始日期）到参数中指定日期值之间的天数，用整数值表示。  
 MIDNIGHT_SECONDS 返回午夜和参数中指定的时间值之间的秒数，用范围在0 到86400 之间的整数值表示。  
 MONTHNAME 对于参数的月部分的月份，返回一个大小写混合的字符串（例如，January）。  
 TIMESTAMP_ISO 根据日期、时间或时间戳记参数而返回一个时间戳记值。  
 TIMESTAMP_FORMAT 从已使用字符模板解释的字符串返回时间戳记。  
 TIMESTAMPDIFF 根据两个时间戳记之间的时差，返回由第一个参数定义的类型表示的估计时差。  
 TO_CHAR 返回已用字符模板进行格式化的时间戳记的字符表示。TO_CHAR 是VARCHAR_FORMAT 的同义词。  
 TO_DATE 从已使用字符模板解释过的字符串返回时间戳记。TO_DATE 是TIMESTAMP_FORMAT 的同义词。  
 WEEK 返回参数中一年的第几周，用范围在1-54 的整数值表示。以星期日作为一周的开始。  
 WEEK_ISO 返回参数中一年的第几周，用范围在1-53 的整数值表示。
 db2日期操作函数
 db2日期操作函数
 当前时间:
 CURRENT DATE
 CURRENT TIME
 CURRENT TIMESTAMP
 获取时间的年、月、日、时、分、秒及微秒各部分:
 YEAR (current timestamp)
 MONTH (current timestamp)
 DAY (current timestamp)
 HOUR (current timestamp)
 MINUTE (current timestamp)
 SECOND (current timestamp)
 MICROSECOND (current timestamp)
 时间的加减:
 current date + 1 YEAR
 current date + 3 YEARS + 2 MONTHS +15 DAYS
 current time + 5 HOURS - 3 MINUTES +10 SECONDS
 要计算两个日期之间的天数，您可以对日期作减法：
 days (current date) - days(date('1999-10-22'))
 时间转换为字符串:
 char(current date)
 字符串转换成时间:
 TIMESTAMP()、DATE() 和TIME(), 支持的字符串格式需要参考帮助, 没有oracle的to_date灵活
 Data('2005-01-01')
 日期和时间的一些函数:
 DAYNAME 返回一个大小写混合的字符串，对于参数的日部分，用星期表示这一天的名称（例如，Friday）。
 DAYOFWEEK 返回参数中的星期几，用范围在1-7 的整数值表示，其中1 代表星期日。
 DAYOFWEEK_ISO 返回参数中的星期几，用范围在1-7 的整数值表示，其中1 代表星期一。
 DAYOFYEAR 返回参数中一年中的第几天，用范围在1-366 的整数值表示。
 DAYS 返回日期的整数表示。
 JULIAN_DAY 返回从公元前4712 年1 月1 日（儒略日历的开始日期）到参数中指定日期值之间的天数，用整数值表示。
 MIDNIGHT_SECONDS 返回午夜和参数中指定的时间值之间的秒数，用范围在0 到86400 之间的整数值表示。
 MONTHNAME 对于参数的月部分的月份，返回一个大小写混合的字符串（例如，January）。
 TIMESTAMP_ISO 根据日期、时间或时间戳记参数而返回一个时间戳记值。
 TIMESTAMP_FORMAT 从已使用字符模板解释的字符串返回时间戳记。
 TIMESTAMPDIFF 根据两个时间戳记之间的时差，返回由第一个参数定义的类型表示的估计时差。
 TO_CHAR 返回已用字符模板进行格式化的时间戳记的字符表示。TO_CHAR 是VARCHAR_FORMAT 的同义词。
 TO_DATE 从已使用字符模板解释过的字符串返回时间戳记。TO_DATE 是TIMESTAMP_FORMAT 的同义词。
 WEEK 返回参数中一年的第几周，用范围在1-54 的整数值表示。以星期日作为一周的开始。
 WEEK_ISO 返回参数中一年的第几周，用范围在1-53 的整数值表示。
 因为没有更好的术语，所以您还可以使用英语来执行日期和时间计算：
  current date + 1 YEAR
 current date + 3 YEARS + 2 MONTHS +15 DAYS
 current time + 5 HOURS - 3 MINUTES +10 SECONDS
    要计算两个日期之间的天数，您可以对日期作减法，如下所示：
    days (current date)- days (date('1999-10-22'))
   而以下示例描述了如何获得微秒部分归零的当前时间戳记：
   CURRENT TIMESTAMP -MICROSECOND (current timestamp) MICROSECONDS
   如果想将日期或时间值与其它文本相衔接，那么需要先将该值转换成字符串。为此，只要使用CHAR() 函数： 
 char(current date)
 char(current time)
 char(current date + 12 hours)   
 要将字符串转换成日期或时间值，可以使用： 
 TIMESTAMP('2002-10-20-12.00.00.000000')
 TIMESTAMP ('2002-10-20 12:00:00')
 DATE ('2002-10-20')
 DATE ('10/20/2002')
 TIME ('12:00:00')
 TIME ('12.00.00')
   如果你想将当前日期格式转化成定制的格式（比如‘yyyymmdd’），那又该如何去做呢？按照我的经验，最好的办法就是编写一个自己定制的格式化函数。
 下面是这个UDF 的代码: 
 create function ts_fmt(TS timestamp,fmt varchar(20))
 returns varchar(50)
 return
 with tmp (dd,mm,yyyy,hh,mi,ss,nnnnnn)as
 (
     select
     substr(digits (day(TS)),9),
     substr(digits (month(TS)),9) ,
     rtrim(char(year(TS))),
     substr(digits (hour(TS)),9),
     substr(digits (minute(TS)),9),
     substr(digits (second(TS)),9),
     rtrim(char(microsecond(TS)))
     from sysibm.sysdummy1
     )
 select
 case fmt
     when'yyyymmdd'
         thenyyyy || mm || dd
     when'mm/dd/yyyy'
         thenmm || '/' || dd || '/' || yyyy
     when'yyyy/dd/mm hh:mi:ss'
         thenyyyy || '/' || mm || '/' || dd || ' ' || 
                hh|| ':' || mi || ':' || ss
     when 'nnnnnn'
         thennnnnnn
     else
         'dateformat ' || coalesce(fmt,'  ') || 
         'not recognized.'
     end
 from tmp
 SQL 日期和时间函数  
 DAYNAME        返回一个大小写混合的字符串，对于参数的日部分，用星期表示这一天的名称（例如，Friday）。  
 DAYOFWEEK    返回参数中的星期几，用范围在1-7 的整数值表示，其中1 代表星期日。  
 DAYOFWEEK_ISO 返回参数中的星期几，用范围在1-7 的整数值表示，其中1 代表星期一。  
 DAYOFYEAR         返回参数中一年中的第几天，用范围在1-366 的整数值表示。  
 DAYS                    返回日期的整数表示。  
 JULIAN_DAY        返回从公元前4712 年1 月1 日（儒略日历的开始日期）到参数中指定日期值之间的天数，用整数值表示。  
 MIDNIGHT_SECONDS  返回午夜和参数中指定的时间值之间的秒数，用范围在0 到86400 之间的整数值表示。  
 MONTHNAME              对于参数的月部分的月份，返回一个大小写混合的字符串（例如，January）。  
 TIMESTAMP_ISO        根据日期、时间或时间戳记参数而返回一个时间戳记值。  
 TIMESTAMP_FORMAT 从已使用字符模板解释的字符串返回时间戳记。  
 TIMESTAMPDIFF       根据两个时间戳记之间的时差，返回由第一个参数定义的类型表示的估计时差。  
 TO_CHAR                  返回已用字符模板进行格式化的时间戳记的字符表示。TO_CHAR 是VARCHAR_FORMAT 的同义词。 
 TO_DATE                 从已使用字符模板解释过的字符串返回时间戳记。TO_DATE 是TIMESTAMP_FORMAT 的同义词。 
 WEEK                    返回参数中一年的第几周，用范围在1-54 的整数值表示。以星期日作为一周的开始。  
 WEEK_ISO           返回参数中一年的第几周，用范围在1-53 的整数值表示。
 Example: 
 YEAR (current timestamp) 
 MONTH (current timestamp) 
 DAY (current timestamp) 
 HOUR (current timestamp) 
 MINUTE (current timestamp) 
 SECOND (current timestamp) 
 MICROSECOND (current timestamp)
   引用：
 mymm 回复于：2003-08-0817:01:11 
 DAYS函数返回日期的整数表示。  
  要计算两个日期之间的天数，可以： 
 days (current date) - days(date('2000-9-1'))
 要使用 SQL 获得当前的日期、时间及时间戳记，请参考适当的DB2 寄存器：
 SELECT current date FROMsysibm.sysdummy1 
 SELECT current time FROMsysibm.sysdummy1 
 SELECT current timestamp FROMsysibm.sysdummy1 
 sysibm.sysdummy1 表是一个特殊的内存中的表，用它可以发现如上面演示的DB2 寄存器的值。您也可以使用关键字VALUES 来对寄存器或表达式求值。例如，在DB2 命令行处理器（Command Line Processor，CLP）上，以下SQL 语句揭示了类似信息：  
 VALUES current date 
 VALUES current time 
 VALUES current timestamp   
 在余下的示例中，我将只提供函数或表达式，而不再重复SELECT ... FROM sysibm.sysdummy1 或使用VALUES 子句。 
 要使当前时间或当前时间戳记调整到GMT/CUT，则把当前的时间或时间戳记减去当前时区寄存器：  
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
 从时间戳记单独抽取出日期和时间也非常简单： 
 DATE (current timestamp)  
 TIME (current timestamp) 
 因为没有更好的术语，所以您还可以使用英语来执行日期和时间计算： 
 current date + 1 YEAR 
 current date + 3 YEARS + 2 MONTHS +15 DAYS 
 current time + 5 HOURS - 3 MINUTES +10 SECONDS 
 要计算两个日期之间的天数，您可以对日期作减法，如下所示： 
 days (current date) - days(date('1999-10-22')) 
 而以下示例描述了如何获得微秒部分归零的当前时间戳记： 
 CURRENT TIMESTAMP - MICROSECOND(current timestamp) MICROSECONDS 
 如果想将日期或时间值与其它文本相衔接，那么需要先将该值转换成字符串。为此，只要使用CHAR() 函数：   
 char(current date) 
 char(current time) 
 char(current date + 12 hours) 
 要将字符串转换成日期或时间值，可以使用：   
 TIMESTAMP('2002-10-20-12.00.00.000000') 
 TIMESTAMP ('2002-10-20 12:00:00') 
 DATE ('2002-10-20') 
 DATE ('10/20/2002') 
 TIME ('12:00:00') 
 TIME ('12.00.00') 
 TIMESTAMP()、DATE() 和TIME() 函数接受更多种格式。
