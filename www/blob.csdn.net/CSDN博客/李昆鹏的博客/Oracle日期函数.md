
# Oracle日期函数 - 李昆鹏的博客 - CSDN博客


2018年05月26日 21:48:22[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：136


-------------------------------------------Oracle日期函数------------------------------------------
### 日期函数
Oracle中提供了很多和日期相关的函数，包括日期的加减，在日期加减时有一些规律
日期 – 数字 = 日期
日期 + 数字 = 日期
日期 – 日期 = 数字(天)
1.    范例：查询雇员的进入公司的周数。
分析：查询雇员进入公司的天数(sysdate – 入职日期)/7就是周数
![](https://img-blog.csdn.net/20180526214719578?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.    获得两个时间段中的月数：MONTHS_BETWEEN()
范例：查询所有雇员进入公司的月数
![](https://img-blog.csdn.net/20180526214739815?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.    获得几个月后的日期：ADD_MONTH()
范例：求出三个月后的日期
![](https://img-blog.csdn.net/20180526214747624?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.    指定给出下次某日期数NEXT_DATE()
范例：求出下一个星期一是什么日期
![](https://img-blog.csdn.net/20180526214753414?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.    求出一个日期的最后一天
范例：求出本月的最后一天是几号
![](https://img-blog.csdn.net/20180526214758618?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select sysdate from dual --当前日期
|select ename,trunc((sysdate-hiredate)/7)  from emp --计算员工入职的周数
|select  ename,trunc(months_between(sysdate,hiredate)) from emp --计算员工入职的月份
|select add_months(sysdate,3) from dual --在指定时间上添加月份
|select next_day(sysdate,'星期二') from dual --下一个星期几是多少号
|select last_day(sysdate) from dual --当月最后一天是几号
|


