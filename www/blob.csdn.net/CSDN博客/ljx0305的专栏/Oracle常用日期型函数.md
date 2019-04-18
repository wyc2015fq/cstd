# Oracle常用日期型函数 - ljx0305的专栏 - CSDN博客
2009年06月30日 09:37:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：668标签：[oracle																[date																[sql																[integer																[工作																[function](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=integer&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
### [Oracle常用日期型函数](blog/117249)
1。Sysdate 当前日期和时间
SQL> Select sysdate from dual;
SYSDATE
----------
21-6月 -05
2。Last_day 本月最后一天 
SQL> Select last_day(sysdate) from dual;
LAST_DAY(S
----------
30-6月 -05
3。Add_months(d,n) 当前日期d后推n个月 
用于从一个日期值增加或减少一些月份 
date_value:=add_months(date_value,number_of_months)
SQL> Select add_months(sysdate,2) from dual;
ADD_MONTHS
----------
21-8月 -05
4。Months_between(f,s) 日期f和s间相差月数 
SQL> select months_between(sysdate,to_date('2005-11-12','yyyy-mm-dd'))from dual;
MONTHS_BETWEEN(SYSDATE,TO_DATE('2005-11-12','YYYY-MM-DD'))
----------------------------------------------------------
                                                 -4.6966741 
5。NEXT_DAY(d, day_of_week)
返回由"day_of_week"命名的，在变量"d"指定的日期之后的第一个工作日的日期。参数"day_of_week"必须为该星期中的某一天。
SQL> SELECT next_day(to_date('20050620','YYYYMMDD'),1) FROM dual;
NEXT_DAY(T
----------
26-6月 -05
6。current_date()返回当前会话时区中的当前日期 
date_value:=current_date 
SQL> column sessiontimezone for a15 
SQL> select sessiontimezone,current_date from dual; 
SESSIONTIMEZONE CURRENT_DA 
--------------- ---------- 
+08:00           13-11月-03 
SQL> alter session set time_zone='-11:00' 2   / 
会话已更改。 
SQL> select sessiontimezone,current_timestamp from dual; 
SESSIONTIMEZONE CURRENT_TIMESTAMP 
--------------- ------------------------------------ 
-11:00           12-11月-03 04.59.13.668000 下午 -11:00 
7。current_timestamp()以timestamp with time zone数据类型返回当前会话时区中的当前日期
SQL> select current_timestamp from dual;
CURRENT_TIMESTAMP
---------------------------------------------------------------------------
21-6月 -05 10.13.08.220589 上午 +08:00
8。dBTimezone()返回时区
SQL> select dbtimezone from dual;
DBTIME
------
-08:00
9。extract()找出日期或间隔值的字段值 
date_value:=extract(date_field from [datetime_value|interval_value]) 
SQL> select extract(month from sysdate) "This Month" from dual;
This Month
----------
          6
SQL> select extract(year from add_months(sysdate,36)) " Years" from dual;
      Years
----------
       2008
10。localtimestamp()返回会话中的日期和时间 
SQL> select localtimestamp from dual;
LOCALTIMESTAMP
---------------------------------------------------------------------------
21-6月 -05 10.18.15.855652 上午
常用日期数据格式（该段为摘抄）
Y或YY或YYY 年的最后一位，两位或三位 Select to_char(sysdate,’YYY’) from dual； 002表示2002年 
SYEAR或YEAR SYEAR使公元前的年份前加一负号 Select to_char(sysdate,’SYEAR’) from dual； -1112表示公元前111 2年 
Q 季度，1～3月为第一季度 Select to_char(sysdate,’Q’) from dual； 2表示第二季度① 
MM 月份数 Select to_char(sysdate,’MM’) from dual； 12表示12月 
RM 月份的罗马表示 Select to_char(sysdate,’RM’) from dual； IV表示4月 
Month 用9个字符长度表示的月份名 Select to_char(sysdate,’Month’) from dual； May后跟6个空格表示5月 
WW 当年第几周 Select to_char(sysdate,’WW’) from dual； 24表示2002年6月13日为第24周 
W 本月第几周 Select to_char(sysdate,’W’) from dual； 2002年10月1日为第1周 
DDD 当年第几, 1月1日为001，2月1日为032 Select to_char(sysdate,’DDD’) from dual； 363 2002年1 2月2 9日为第363天 
DD 当月第几天 Select to_char(sysdate,’DD’) from dual； 04 10月4日为第4天 
D 周内第几天 Select to_char(sysdate,’D’) from dual； 5 2002年3月14日为星期一 
DY 周内第几天缩写 Select to_char(sysdate,’DY’) from dual； SUN 2002年3月24日为星期天 
HH或HH12 12进制小时数 Select to_char(sysdate,’HH’) from dual； 02 午夜2点过8分为02 
HH24 24小时制 Select to_char(sysdate,’HH24’) from dual； 14 下午2点08分为14 
MI 分钟数(0～59) Select to_char(sysdate,’MI’) from dual； 17下午4点17分 
SS 秒数(0～59) Select to_char(sysdate,’SS’) from dual； 22 11点3分22秒 
提示注意不要将MM格式用于分钟(分钟应该使用MI)。MM是用于月份的格式，将它用于分钟也能工作，但结果是错误的。 
现在给出一些实践后的用法：
1。上月末天：
SQL> select to_char(add_months(last_day(sysdate),-1),'yyyy-MM-dd') LastDay from
dual;
LASTDAY
----------
2005-05-31
2。上月今天
SQL> select to_char(add_months(sysdate,-1),'yyyy-MM-dd') PreToday from dual;
PRETODAY
----------
2005-05-21
3.上月首天
SQL> select to_char(add_months(last_day(sysdate)+1,-2),'yyyy-MM-dd') firstDay from dual;
FIRSTDAY
----------
2005-05-01
4.按照每周进行统计
SQL> select to_char(sysdate,'ww') from dual group by to_char(sysdate,'ww');
TO
--
25
5。按照每月进行统计
SQL> select to_char(sysdate,'mm') from dual group by to_char(sysdate,'mm');
TO
--
06
6。按照每季度进行统计
SQL> select to_char(sysdate,'q') from dual group by to_char(sysdate,'q');
T
-
2
7。按照每年进行统计
SQL> select to_char(sysdate,'yyyy') from dual group by to_char(sysdate,'yyyy');
TO_C
----
2005
8.要找到某月中所有周五的具体日期 
select to_char(t.d,'YY-MM-DD') from ( 
select trunc(sysdate, 'MM')+rownum-1 as d 
from dba_objects 
where rownum < 32) t 
where to_char(t.d, 'MM') = to_char(sysdate, 'MM') --找出当前月份的周五的日期 
and trim(to_char(t.d, 'Day')) = '星期五' 
-------- 
03-05-02 
03-05-09 
03-05-16 
03-05-23 
03-05-30  
如果把where to_char(t.d, 'MM') = to_char(sysdate, 'MM')改成sysdate-90，即为查找当前月份的前三个月中的每周五的日期。
9.oracle中时间运算
内容如下： 
1、oracle支持对日期进行运算 
2、日期运算时是以天为单位进行的 
3、当需要以分秒等更小的单位算值时，按时间进制进行转换即可 
4、进行时间进制转换时注意加括号，否则会出问题 
SQL> alter session set nls_date_format='yyyy-mm-dd hh:mi:ss'; 
会话已更改。 
SQL> set serverout on 
SQL> declare 
   2 DateValue date; 
   3 begin 
   4 select sysdate into DateValue from dual; 
   5 dbms_output.put_line('源时间:'||to_char(DateValue)); 
   6 dbms_output.put_line('源时间减1天:'||to_char(DateValue-1)); 
   7 dbms_output.put_line('源时间减1天1小时:'||to_char(DateValue-1-1/24)); 
   8 dbms_output.put_line('源时间减1天1小时1分:'||to_char(DateValue-1-1/24-1/(24*60))); 
   9 dbms_output.put_line('源时间减1天1小时1分1秒:'||to_char(DateValue-1-1/24-1/(24*60)-1/(24*60*60))); 
10 end; 
11 / 
源时间:2003-12-29 11:53:41 
源时间减1天:2003-12-28 11:53:41 
源时间减1天1小时:2003-12-28 10:53:41 
源时间减1天1小时1分:2003-12-28 10:52:41 
源时间减1天1小时1分1秒:2003-12-28 10:52:40 
PL/SQL 过程已成功完成。
在Oracle中实现时间相加处理
-- 名称：Add_Times
-- 功能：返回d1与NewTime相加以后的结果，实现时间的相加
-- 说明：对于NewTime中的日期不予考虑
-- 日期：2004-12-07
-- 版本：1.0
-- 作者：Kevin
create or replace function Add_Times(d1 in date,NewTime in date) return date 
is
   hh    number;
   mm    number;
   ss    number;
   hours number;
   dResult   date;  
begin
   -- 下面依次取出时、分、秒
   select to_number(to_char(NewTime,'HH24')) into hh from dual;
   select to_number(to_char(NewTime,'MI')) into mm from dual;
   select to_number(to_char(NewTime,'SS')) into ss from dual;
   -- 换算出NewTime中小时总和，在一天的百分几
   hours := (hh + (mm / 60) + (ss / 3600))/ 24;
   -- 得出时间相加后的结果
   select d1 + hours into dResult from dual;
   return(dResult);
end Add_Times;
-- 测试用例
-- select Add_Times(sysdate,to_date('2004-12-06 03:23:00','YYYY-MM-DD HH24:MI:SS')) from dual
在Oracle9i中计算时间差
计算时间差是Oracle DATA数据类型的一个常见问题。Oracle支持日期计算，你可以创建诸如“日期1－日期2”这样的表达式来计算这两个日期之间的时间差。 
一旦你发现了时间差异，你可以使用简单的技巧来以天、小时、分钟或者秒为单位来计算时间差。为了得到数据差，你必须选择合适的时间度量单位，这样就可以进行数据格式隐藏。 
使用完善复杂的转换函数来转换日期是一个诱惑，但是你会发现这不是最好的解决方法。 
round(to_number(end-date-start_date))- 消逝的时间（以天为单位） 
round(to_number(end-date-start_date)*24)- 消逝的时间（以小时为单位） 
round(to_number(end-date-start_date)*1440)- 消逝的时间（以分钟为单位） 
显示时间差的默认模式是什么？为了找到这个问题的答案，让我们进行一个简单的SQL *Plus查询。 
SQL> select sysdate-(sysdate-3) from dual; 
SYSDATE-(SYSDATE-3) 
------------------- 
                    3  
这里，我们看到了Oracle使用天来作为消逝时间的单位，所以我们可以很容易的使用转换函数来把它转换成小时或者分钟。然而，当分钟数不是一个整数时，我们就会遇到放置小数点的问题。 
Select 
     (sysdate-(sysdate-3.111))*1440 
from 
     dual; 
(SYSDATE-(SYSDATE-3.111))*1440 
------------------------------ 
                      4479.83333  
当然，我们可以用ROUND函数（即取整函数）来解决这个问题，但是要记住我们必须首先把DATE数据类型转换成NUMBER数据类型。 
Select 
     round(to_number(sysdate-(sysdate-3.111))*1440) 
from 
     dual; 
ROUND(TO_NUMBER(SYSDATE-(SYSDATE-3.111))*1440) 
---------------------------------------------- 
                                            4480  
我们可以用这些函数把一个消逝时间近似转换成分钟并把这个值写入Oracle表格中。在这个例子里，我们有一个离线（logoff）系统级触发机制来计算已经开始的会话时间并把它放入一个Oracle STATSPACK USER_LOG扩展表格之中。 
Update 
     perfstat.stats$user_log 
set 
     elapsed_minutes = 
     round(to_number(logoff_time-logon_time)*1440) 
where 
     user = user_id 
and 
     elapsed_minutes is NULL; 
查出任一年月所含的工作日
CREATE OR REPLACE FUNCTION Get_WorkingDays(
   ny IN VARCHAR2
) RETURN INTEGER IS
/*------------------------------------------------------------------------------------------
函数名称：Get_WorkingDays
中文名称：求某一年月中共有多少工作日
作者姓名: XINGPING
编写时间: 2004-05-22
输入参数：NY:所求包含工作日数的年月，格式为yyyymm,如200405
返 回 值：整型值，包含的工作日数目。
算法描述：
     1).列举出参数给出的年月中的每一天。这里使用了一个表（ljrq是我的库中的一张表。这个表可以是有权访问的、记录条数至少为31的任意一张表或视图）来构造出某年月的每一天。
     2).用这些日期和一个已知星期几的日期相减(2001-12-30是星期天)，所得的差再对7求模。如果所求年月在2001-12-30以前，那么所得的差既是负数，求模后所得值范围为大于-6,小于0，如-1表示星期六,故先将求模的结果加7，再求7的模.
     3).过滤掉结果集中值为0和6的元素，然后求count,所得即为工作日数目。      
-------------------------------------------------------------------------------------------------*/
   Result INTEGER;
BEGIN
   SELECT COUNT(*) INTO Result
     FROM (SELECT MOD(MOD(q.rq-to_date('2001-12-30','yyyy-mm-dd'),7),7) weekday
             FROM ( SELECT to_date(ny||t.dd,'yyyymmdd') rq
                      FROM (SELECT substr(100+ROWNUM,2,2) dd 
                              FROM ljrq z WHERE Rownum<=31
                           ) t
                      WHERE to_date(ny||t.dd,'yyyymmdd') 
                        BETWEEN to_date(ny,'yyyymm') 
                            AND last_day(to_date(ny,'yyyymm'))
                  )q
          ) a   
     WHERE a.weekday NOT IN(0,6);    
   RETURN Result;  
END Get_WorkingDays;
______________________________________
还有一个版本
CREATE OR REPLACE FUNCTION Get_WorkingDays(
   ny IN VARCHAR2
) RETURN INTEGER IS
/*-----------------------------------------------------------------------------------------
函数名称：Get_WorkingDays
中文名称：求某一年月中共有多少工作日
作者姓名: XINGPING
编写时间: 2004-05-23
输入参数：NY:所求包含工作日数的年月，格式为yyyymm,如200405
返 回 值：整型值，包含的工作日数目。
算法描述：使用Last_day函数计算出参数所给年月共包含多少天，根据这个值来构造一个循环。在这个循环中先求这个月的每一天与一个已知是星期天的日期(2001-12-30是星期天)的差，所得的差再对7求模。如果所求日期在2001-12-30以前，那么所得的差既是负数，求模后所得值范围为大于-6,小于0，如-1表示星期六,故先将求模的结果加7，再求7的模. 如过所得值不等于0和6（即不是星期六和星期天），则算一个工作日。      
----------------------------------------------------------------------------------------*/
   Result INTEGER := 0;
   myts INTEGER;       --所给年月的天数
   scts INTEGER;       --某天距2001-12-30所差的天数
   rq    DATE;
   djt INTEGER := 1;    -- 
BEGIN
   myts := to_char(last_day(to_date(ny,'yyyymm')),'dd');  
   LOOP 
     rq := TO_date(ny||substr(100+djt,2),'yyyymmdd');
     scts := rq - to_date('2001-12-30','yyyy-mm-dd');
     IF MOD(MOD(scts,7)+7,7) NOT IN(0,6) THEN
       Result := Result + 1;
     END IF;
     djt := djt + 1;  
     EXIT WHEN djt>myts;
   END LOOP;  
   RETURN Result;  
END Get_WorkingDays;
以上两个版本的比较
第一个版本一条SQL语句就可以得出结果，不需要编程就可以达到目的。但需要使用任意一张有权访问的、记录条数至少为31的一张表或视图。
     第二个版本需要编程，但不需要表或者视图。
     这两个版本都还存在需要完善的地方，即没有考虑节日，如五一、十一、元旦、春节这些节假期都没有去除。这些节假日应该维护成一张表，然后通过查表来去除这些节假日。
