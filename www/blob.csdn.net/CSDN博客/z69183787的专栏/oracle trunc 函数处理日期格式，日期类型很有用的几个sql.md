# oracle trunc 函数处理日期格式，日期类型很有用的几个sql - z69183787的专栏 - CSDN博客
2014年06月25日 10:30:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1989
**select to_char(sysdate,'yyyy-mm-dd hh24:mi:ss') from dual;  --显示当前时间**2011-12-29 16:24:34
**select trunc(sysdate,'year') from dual; --截取到年（本年的第一天）**2011-1-1
**select trunc(sysdate,'q') from dual; --截取到季度（本季度的第一天）**2011-10-1
**select trunc(sysdate,'month') from dual; --截取到月（本月的第一天）**2011-12-1
**select trunc(sysdate,'') from dual; **空
**select to_char(trunc(sysdate),'yyyymmdd hh24:mi:ss') from dual; --默认截取到日（当日的零点零分零秒）**20111229 00:00:00
**select trunc(sysdate-1,'w') from dual;  -- 离当前时间最近的周四，若当天为周四则返回当天，否则返回上周四**2011-12-22
**select trunc(sysdate,'ww') from dual;  --截取到上周末（上周周六）**2011-12-24
**select trunc(sysdate,'day') from dual; --截取到周（本周第一天，即上周日）**2011-12-25
**select trunc(sysdate,'iw') from dual; --本周第2天，即本周一**2011-12-26
**select to_char(trunc(sysdate,'dd'),'yyyymmdd hh24:mi:ss') from dual;--截取到日（当日的零点零分零秒）**20111229 00:00:00
**select trunc(sysdate,'hh24') from dual;  --截取到小时（当前小时，零分零秒）**2011-12-29 16:00:00
**select trunc(sysdate,'mi') from dual; --截取到分（当前分，零秒）**2011-12-29 16:24:00
**select trunc(sysdate,'ss') from dual ;--报错，没有精确到秒的格式**
```
[sql]
```
```
--
 Oracle 取上周一到周末的sql
```
`  `
```
--
 这样取的是 在一周内第几天，是以周日为开始的
```
`select`
```
to_char(to_date(
```
`'20130906'``,``'yyyymmdd'``),``'d'`
```
)
```
`from`
```
dual;
```
`       `
```
--结果：6
 注释：2013.09.06是周五，为本周的第六天
```
`  `
`select`
```
to_char(sysdate+(2-to_char(sysdate,
```
`'d'``))-7,``'yyyymmdd'`
```
)
```
`from`
```
dual;
```
```
---上周一
```
`select`
```
to_char(sysdate+(2-to_char(sysdate,
```
`'d'``))-1,``'yyyymmdd'`
```
)
```
`from`
```
dual;
```
```
---上周日
```
`  `
```
--
 一个更简单的写法 ， 返回date类型
```
`select`
```
trunc(sysdate,
```
`'iw'`
```
)
 - 7
```
`from`
```
dual;
```
```
---上周一
```
`select`
```
trunc(sysdate,
```
`'iw'`
```
)
 - 1
```
`from`
```
dual;
```
```
--上周日
```
`  `
```
--
 这样查出来是本周一
```
`select`
```
trunc(sysdate,
```
`'iw'`
```
)
```
`from`
```
dual;
```
`  `
`select`
```
trunc(to_date(
```
`'20130915'``,``'yyyymmdd'``),``'iw'`
```
)
```
`from`
```
dual;
```
`       `
```
--
 结果：2013/9/9  注释：20130915 为周日
```
`  `
```
--
 返回char类型
```
`select`
```
to_char(trunc(sysdate,
```
`'iw'`
```
)
 - 7,
```
`'yyyymmdd'`
```
)
```
`from`
```
dual;
```
```
--上周一
```
`select`
```
to_char(trunc(sysdate,
```
`'iw'`
```
)
 - 1,
```
`'yyyymmdd'`
```
)
```
`from`
```
dual;
```
```
--上周日
```
`  `
```
--
 获取上周一的函数
```
`create`
```
or
```
```
replace
```
```
function
```
```
fun_acc_getlastweekstart(systemdate
```
`in`
```
date
```
```
)
```
`  ``return`
```
varchar2
```
`is`
`  `
```
result_str
 varchar2(15);
```
`begin`
`  ``select`
```
to_char(trunc(systemdate,
```
`'iw'`
```
)
 - 7,
```
`'yyyymmdd'`
```
)
```
`    ``into`
```
result_str
```
`    ``from`
```
dual;
```
`  ``return`
```
result_str;
```
`end`
```
fun_acc_getlastweekstart;
```
`  `
```
--
 获取上周日的函数
```
`create`
```
or
```
```
replace
```
```
function
```
```
fun_acc_getlastweekend(systemdate
```
`in`
```
date
```
```
)
```
`return`
```
varchar2
```
`is`
`  `
```
result_str
 varchar2(15);
```
`begin`
`  ``select`
```
to_char(trunc(systemdate,
```
`'iw'`
```
)
 - 1,
```
`'yyyymmdd'`
```
)
```
`    ``into`
```
result_str
```
`    ``from`
```
dual;
```
`  ``return`
```
result_str;
```
`end`
```
fun_acc_getlastweekend;
```
`  `
```
--
 测试这个函数
```
`select`
```
fun_acc_getlastweekstart(sysdate)
```
`from`
```
dual;
```
`select`
```
fun_acc_getlastweekend(sysdate)
```
`from`
```
dual;
```
`select`
```
fun_acc_getlastweekstart(to_date(
```
`'20130915'``,``'yyyymmdd'`
```
))
```
`from`
```
dual;
```
`select`
```
fun_acc_getlastweekend(to_date(
```
`'20130915'``,``'yyyymmdd'`
```
))
```
`from`
```
dual;
```
`       `
```
--查询结果：20130826、20130901、20130902、20130908
```
```
--
 注：
```
`select`
```
sysdate
```
`from`
```
dual;
```
`       `
```
--查询结果：2013/9/6
 9:45:14
```
