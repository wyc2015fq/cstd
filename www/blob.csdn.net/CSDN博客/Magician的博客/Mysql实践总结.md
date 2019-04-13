
# Mysql实践总结 - Magician的博客 - CSDN博客


2018年11月25日 14:36:09[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：195



### 文章目录
[前言：](#_1)
[1、mysql获取未来、现在、过去的时间：DATE_SUB（）、DATE_ADD()](#1mysqlDATE_SUBDATE_ADD_6)
[2、格式化日期：date_format（）](#2date_format_35)
[3、MySQL 日期、时间相减函数：datediff(date1,date2), timediff(time1,time2)](#3MySQL_datediffdate1date2_timedifftime1time2_66)
[4、SQL中CASE的用法](#4SQLCASE_82)
[5、SQL的nvl用法](#5SQLnvl_108)
[6、Mysql当前时间的几个函数](#6Mysql_116)
[7、Mysql删除表数据drop，truncate，delete区别](#7Mysqldroptruncatedelete_127)

# 前言：
工作中总会和mysql打交道，这里总结下一些常用的函数功能及需求实现
。后续会不断更新。
# 1、mysql获取未来、现在、过去的时间：DATE_SUB（）、DATE_ADD()
`#当前日期
select DATE_SUB(curdate(),INTERVAL 0 DAY) ;#或者 NOW()
#明天日期
select DATE_SUB(curdate(),INTERVAL -1 DAY) ;
#昨天日期
select DATE_SUB(curdate(),INTERVAL 1 DAY) ;
#前一个小时时间
select date_sub(now(), interval 1 hour);
#后一个小时时间
select date_sub(now(), interval -1 hour);
#前30分钟时间
select date_add(now(),interval -30 minute)
#后30分钟时间
select date_add(now(),interval 30 minute)
#对于Date_add（）同样
set @dt = now();
select date_add(@dt, interval 1 day);           - 加1天
select date_add(@dt, interval 1 hour);                -加1小时
select date_add(@dt, interval 1 minute);    - 加1分钟
select date_add(@dt, interval 1 second);             -加1秒
select date_add(@dt, interval 1 microsecond);     -加1毫秒
select date_add(@dt, interval 1 week);         -加1周
select date_add(@dt, interval 1 month);        -加1月
select date_add(@dt, interval 1 quarter);             -加1季
select date_add(@dt, interval 1 year);           -加1年`
# 2、格式化日期：date_format（）
`select date_format(now(),'%y-%m-%d');
根据format字符串格式化date值：
%S, %s 两位数字形式的秒（ 00,01, ..., 59）
%I, %i 两位数字形式的分（ 00,01, ..., 59）
%H 两位数字形式的小时，24 小时（00,01, ..., 23）
%h 两位数字形式的小时，12 小时（01,02, ..., 12）
%k 数字形式的小时，24 小时（0,1, ..., 23）
%l 数字形式的小时，12 小时（1, 2, ..., 12）
%T 24 小时的时间形式（hh:mm:ss）
%r 12 小时的时间形式（hh:mm:ss AM 或hh:mm:ss PM）
%p AM或PM
%W 一周中每一天的名称（Sunday, Monday, ..., Saturday）
%a 一周中每一天名称的缩写（Sun, Mon, ..., Sat）
%d 两位数字表示月中的天数（00, 01,..., 31）
%e 数字形式表示月中的天数（1, 2， ..., 31）
%D 英文后缀表示月中的天数（1st, 2nd, 3rd,...）
%w 以数字形式表示周中的天数（ 0 = Sunday, 1=Monday, ..., 6=Saturday）
%j 以三位数字表示年中的天数（ 001, 002, ..., 366）
%U 周（0, 1, 52），其中Sunday 为周中的第一天
%u 周（0, 1, 52），其中Monday 为周中的第一天
%M 月名（January, February, ..., December）
%b 缩写的月名（ January, February,...., December）
%m 两位数字表示的月份（01, 02, ..., 12）
%c 数字表示的月份（1, 2, ...., 12）
%Y 四位数字表示的年份
%y 两位数字表示的年份
%% 直接值“%”`
# 3、MySQL 日期、时间相减函数：datediff(date1,date2), timediff(time1,time2)
MySQL datediff(date1,date2)：两个日期相减 date1 date2，返回天数。
`select datediff('2008-08-08', '2008-08-01'); - 7
select datediff('2008-08-01', '2008-08-08'); -7
MySQL timediff(time1,time2)：两个日期相减 time1 time2，返回 time 差值。
select timediff('2008-08-08 08:08:08', '2008-08-08 00:00:00');- 08:08:08
select timediff('08:08:08', '00:00:00');                      - 08:08:08
注意：timediff(time1,time2) 函数的两个参数类型必须相同。`
# 4、SQL中CASE的用法
sql语言中的CASE类似C语言中的switch case
`#case语法
SELECT <myColumnSpec> =
CASE
WHEN <A> THEN <somethingA>
WHEN <B> THEN <somethingB>
ELSE <somethingE>
END 
#例子
SELECT
     Title,
    'Price Range' =
    CASE
        WHEN price IS NULL THEN 'Unpriced'
        WHEN price < 10 THEN 'Bargain'
        WHEN price BETWEEN 10 and 20 THEN 'Average'
        ELSE 'Gift to impress relatives'
    END
FROM titles
ORDER BY price`
# 5、SQL的nvl用法
**NVL函数是一个空值转换函数**
**NVL（表达式1，表达式2）**
如果表达式1为空值，NVL返回值为表达式2的值，否则返回表达式1的值。 该函数的目的是把一个空值（null）转换成一个实际的值。其表达式的值可以是数字型、字符型和日期型。但是表达式1和表达式2的数据类型必须为同一个类型。
# 6、Mysql当前时间的几个函数
NOW (); 当前具体的日期和时间，
CURDATE (); 当前日期
CURTIME(); 当前时间
DATE ( date ); 提取日期或日期/时间表达式的日期部分
DATE_ADD ( date, INTERVAL  expr  type); 给日期添加指定的时间间隔，
DATE_SUB (date, INTERVAL  expr  type);  从日期送去指定的时间间隔。\#expr 参数是您希望添加的时间间隔， type参数同DATE_ADD()。
DATEDIFF ( date1, date2 ); 返回两个日期之间的天数，只有值的日期部分参与计算。
DATE_FORMAT (date, format); 用不同的格式显示日期/时间。
# 7、Mysql删除表数据drop，truncate，delete区别
1、drop (删除表)：**删除内容和定义，释放空间**。简单来说就是把整个表去掉.以后要新增数据是不可能的,除非新增一个表。
`drop语句将删除表的结构被依赖的约束（constrain),触发器（trigger)索引（index);依赖于该表的存储过程/函数将被保留，但其状态会变为：invalid。`2、truncate (清空表中的数据)：删除内容、释放空间但不删除定义(保留表的数据结构)。**与drop不同的是,只是清空表数据而已**。
`注意:truncate 不能删除行数据,要删就要把表清空。`3、delete (删除表中的数据)：delete 语句用于删除表中的行。delete语句执行删除的过程是每次从表中删除一行，并且同时将该行的删除操作作为事务记录在日志中保存
`以便进行进行回滚操作。
       truncate与不带where的delete ：只删除数据，而不删除表的结构（定义）`参考：
[https://www.cnblogs.com/maanshancss/p/4036608.html](https://www.cnblogs.com/maanshancss/p/4036608.html)
[https://blog.csdn.net/anneqiqi/article/details/52061384](https://blog.csdn.net/anneqiqi/article/details/52061384)
[https://blog.csdn.net/liujava621/article/details/26599035](https://blog.csdn.net/liujava621/article/details/26599035)
[https://www.cnblogs.com/fjl0418/p/7929420.html](https://www.cnblogs.com/fjl0418/p/7929420.html)
[https://dev.mysql.com/doc/refman/5.5/en/date-and-time-functions.html\#function_date-format](https://dev.mysql.com/doc/refman/5.5/en/date-and-time-functions.html#function_date-format)

