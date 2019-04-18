# oracle日期时间型timestamp相关学习 - z69183787的专栏 - CSDN博客
2014年12月16日 13:49:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1030
**1、获取系统时间的语句(ssxff6获取小数点后面六位)**
select sysdate,systimestamp,to_char(systimestamp, 'yyyymmdd hh24:mi:ssxff6'),
  to_char(systimestamp, 'yyyymmdd hh24:mi:ss.ff6') from dual;  
**2、字符型转成timestamp **
select to_timestamp('2011-09-14 12:52:42.123456789', 'syyyy-mm-dd hh24:mi:ss.ff') from dual;
**3、timestamp转成date型**
select cast(to_timestamp('2011-09-14 12:52:42.123456789', 'syyyy-mm-dd hh24:mi:ss.ff') as date) timestamp_to_date from dual; 
**4、date型转成timestamp**
select cast(sysdate as timestamp) date_to_timestamp from dual;  
**5、两date的日期相减得出的是天数，而两timestamp的日期相减得出的是完整的年月日时分秒小数秒**
select sysdate-sysdate,systimestamp-systimestamp from dual;
select extract(day from inter) * 24 * 60 * 60 +   
  extract(hour from inter) * 60 * 60 + extract(minute from inter) * 60 +   
    extract(second from inter) "seconds" from 
(
  select to_timestamp('2011-09-14 12:34:23.281000000', 'yyyy-mm-dd hh24:mi:ss.ff') -  
    to_timestamp('2011-09-14 12:34:22.984000000', 'yyyy-mm-dd hh24:mi:ss.ff') inter from dual
); 
select extract(second from to_timestamp('2011-09-14 12:34:23.281000000', 'yyyy-mm-dd hh24:mi:ss.ff'))-
  extract(second from to_timestamp('2011-09-14 12:34:22.984000000', 'yyyy-mm-dd hh24:mi:ss.ff')) from dual;
注：所以，timestamp要算出两日期间隔了多少秒，要用函数转换一下。
to_char函数支持date和timestamp,但是trunc却不支持TIMESTAMP数据类型。
            
