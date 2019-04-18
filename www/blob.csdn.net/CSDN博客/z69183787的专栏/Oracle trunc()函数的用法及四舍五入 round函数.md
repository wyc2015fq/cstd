# Oracle trunc()函数的用法及四舍五入 round函数 - z69183787的专栏 - CSDN博客
2013年11月05日 19:43:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7157
   --Oracle trunc()函数的用法
/**************日期********************/
1.select trunc(sysdate) from dual  --2011-3-18 
 今天的日期为2011-3-18
2.select trunc(sysdate, 'mm')   from   dual  --2011-3-1   
 返回当月第一天.
3.select trunc(sysdate,'yy') from dual 
 --2011-1-1       返回当年第一天
4.select trunc(sysdate,'dd') from dual 
 --2011-3-18    返回当前年月日
5.select trunc(sysdate,'yyyy') from dual 
 --2011-1-1   返回当年第一天
6.select trunc(sysdate,'d') from dual 
 --2011-3-13 (星期天)返回当前星期的第一天
7.select trunc(sysdate, 'hh') from dual  
 --2011-3-18 14:00:00   当前时间为14:41   
8.select trunc(sysdate, 'mi') from dual  --2011-3-18
 14:41:00   TRUNC()函数没有秒的精确
/***************数字********************/
/*
TRUNC（number,num_digits） 
Number 需要截尾取整的数字。 
Num_digits 用于指定取整精度的数字。Num_digits 的默认值为 0。
TRUNC()函数截取时不进行四舍五入
*/
9.select trunc(123.458) from dual --123
10.select trunc(123.458,0) from dual --123
11.select trunc(123.458,1) from dual --123.4
12.select trunc(123.458,-1) from dual --120
13.select trunc(123.458,-4) from dual --0
14.select trunc(123.458,4) from dual 
 --123.458
15.select trunc(123) from dual 
 --123
16.select trunc(123,1) from dual --123
17.select trunc(123,-1) from dual --120
SELECT ROUND( number, [ decimal_places ] ) FROM DUAL
说明：
number : 将要处理的数值
decimal_places : 四舍五入，小数取几位,不填默认为0
Sample :
select round(123.456) from dual;              结果： 123
select round(123.456, 0) from dual;          结果： 123
select round(123.456, 1) from dual;          结果： 123.5
select round(123.456, 2) from dual;          结果：123.46
select round(123.456, 3) from dual;          结果： 123.456
select round(-123.456, 2) from dual;         结果：-123.46
