# Oracle sql语句练习（转） - z69183787的专栏 - CSDN博客
2013年05月30日 14:35:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：805
**1. 创建TableSpace、用户及给用户分派权限**
      可以通过Oracle客户端工具Enterprise Manager Console连接相应数据库，通过图形化操作来完成，这里我记录通过sqlplus连接到Oracle，通过命令来完成创建TableSpace、用户及给用户分派权限：
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **create** tablespace testall datafile 'c:\oracle\product\oradata\datafile.dbf'**size** 100M;
   
- **create**user kylin identified **by** ADMIN **default** tablespace testall;
   
- **grant****connect**,resource,dba **to** kylin;  
** 2.简单的表操作**
      以上面创建的用户连接到数据库，创建一个表，向表中插入一条数据，查询数据，删除表，如下所示：
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **create****table** student(   
- **name** varchar2(20),   
- age number(3)   
- );   
- 
- **insert****into** student **values**('Kobe Bryant',32);
   
- 
- **select** * **from** student;   
- 
- **desc** student;   
- 
- **drop****table** student;  
**3.用Scott用户登录，练习一组查询语句**
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** empno,ename,job,sal,sal*12+200 **from** emp;   
- **select** empno,hiredate, hiredate+10 **from** emp;   
- 
- **select** ename||' is a '||job **from** emp;   
- **select** ename||' ''s annual salary is '|| sal*12 **from** emp;
   
- 
- **select** empno **as** employeeId, ename **name**, job "Job",sal*12 "Annual Salary"**from** emp;
   
- **select** empno "Empno",ename "Ename",sal, comm, ename||' - '|| comm, sal+comm **from** emp;  
用**DISTINCT** 去除重复行
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** deptno **from** emp;   
- **select****distinct** deptno **from** emp;   
- **select****distinct** deptno, job **from** emp;  
 查询结果排序**ORDER BY**，**ASC**（升序，缺省），**DESC**（降序）
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** * **from** emp **order****by** sal;
   
- **select** * **from** emp **order****by** sal **desc**;
   
- **select** ename,sal,deptno **from** emp;   
- **select** ename,sal,deptno **from** emp **order****by** sal **desc**,deptno;
   
- **select** ename,sal,deptno **from** emp **order****by** sal **desc**,deptno **desc**;  
条件查询
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** * **from** emp **where** ename='SMITH';
   
- **select** * **from** emp **where** deptno=10;   
- **select** * **from** emp **where** sal >= 3000;   
- **select** * **from** emp **where** sal >= 3000;  
获取当前系统时间：
select sysdate from dual;
 比较查询
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** * **from** emp **where** sal between 1200 and 1900;
   
- **select** * **from** emp **where** ename in ('SCOTT','WARD');
   
- **select** * **from** emp **where** deptno <> 30;  
模糊查询（**%**表示一个或多个字符，**_**表示一个字符，对于特殊字符可以用**ESCAPE**来查找）
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** * **from** emp **where** ename like'S%';
   
- **select** * **from** emp **where** ename like'_A%';
   
- **select** * **from** emp **where** ename like'%\_%'**escape**'\';  
 空值判断
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** * **from** emp **where** comm **is**null;
   
- **select** * **from** emp **where** comm **is**notnull;  
 逻辑运算符
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** * **from** emp **where** sal>2000 and deptno=20;
   
- **select** * **from** emp **where** sal>2000 or deptno=20;
   
- **select** * **from** emp **where** ename notin('SIMITH','JONES','KING','SCOTT');  
** 4.Oracle主要数据类型**
|char|字符型，最大长度2000B，缺省长度1B|
|----|----|
|nchar|基于NLS（National Language Support）的字符型，最大长度2000B，缺省长度1B|
|**varchar2**|**可变长度字符型，最大长度4000B**|
|nvarchar2|基于NLS，其余同varchar2|
|varchar|同varchar2|
|**number(m,n)**|**数值型，m为总位数，n为小树位数，m最大为38**|
|**date**|**日期型，保存日期和时间**|
|long|变长字符型，最大长度2GB，不支持对字符串进行搜索|
|raw|变长二进制数据类型，最大长度2000B|
|long raw|变长二进制数据类型，最大长度2GB|
|**blob**|**二进制大对象类型，最大长度4GB**|
|**clob**|**字符大对象类型，最大长度4GB**|
|nclob|基于NLS，同nclob|
|bfile|在数据库外部保存的大型二进制文件的大对象类型，最大长度4GB|

**5.函数**
字符函数
|函数|功能|用法|返回结果|
|----|----|----|----|
|lower()|转化为小写|lower(KOBE BRYANT)|kobe bryant|
|upper()|转化为大写|upper(kobe bryant)|KOBE BRYANT|
|initcap()|字母首字母大写|initcap(KOBE BRYANT)|Kobe Bryant|
|concat()|字符串连接|concat('Kobe','Bryant')|Kobe Bryant|
|substr()|字符串截取|substr('Kobe Bryant',6,6)|Bryant|
|length()|返回字符串长度|length('Kobe Bryant')|11|
|instr()|定位字符串|instr('Kobe Bryant','be')|3|
|lpad()|左侧填充|lpad('kobe',10,'*')|******kobe|
|rpad()|右侧填充|rpad('kobe',10,'*')|kobe******|
|trim()|过滤首尾空格|trim('    Kobe Bryant   ')|Kobe Bryant|
|replace()|替换|replace('Kobe Bryant','Kobe','Kylin')|Kylin Bryant|

SQL语句：
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** empno,lower(ename),job **from** emp;   
- **select** empno,initcap(ename),job **from** emp;   
- **select** concat('Kobe','Bryant') **from** dual;
   
- **select** substr('Kobe Bryant',6,6) **from** dual;   
- **select** length('Kobe Bryant') **from** dual;   
- **select** instr('Kobe Bryant','be') **from** dual;
   
- **select** lpad('kobe',10,'*') **from** dual;   
- **select** rpad('kobe',10,'*') **from** dual;   
- **select** trim('    Kobe Bryant   ') **from** dual;
   
- **select**replace('Kobe Bryant','Kobe','Kylin') **from** dual;  
 数值函数
|函数|用法|功能|返回结果|
|----|----|----|----|
|abs()|取绝对值|abs(-3.14)|3.14|
|round()|四舍五入|round(3.1415)round(3.1415,3)round(314.1592,-2)|33.142300|
|trunc()|截断|trunc(3.1415,3)|3.141|
|ceil()|向上取整|ceil(3.14)|4|
|floor()|向下取整|floor(3.14)|3|
|sign()|判断数值正负|sign(-3.14)sign(0)sign(3.14)|-101|
|sin()|三角函数|sin(3.14)|.001592653|
|power()|冥运算|power(4.5,2)||
|sqrt()|开平方根|sqrt(9)|3|
|mod()|取模|mod(10,3)|1|
|exp()|基数为e的冥运算|exp(1)||


相关SQL语句
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** ename,sign(sal-1250) "Salary Info"**from** emp;   
- **select** sin(3.14) **from** dual;  
 日期类型相关函数
|函数|功能|用法|返回结果|
|----|----|----|----|
|add_months(x,y)|计算在日期x上加上y个月后的日期|add_months(sysdate,2)||
|last_day(x)|返回日期x当月最后一天的日期|last_day(sysdate)||
|months_between(x,y)|返回日期x和y之间相差的月数|months_between(sysdate,sysdate)||
|round(x,y)|将日期x四舍五入到y所指定的日期单位（月或年）的第一天|round(sysdate,'month')round(sysdate,'year')|01-10月 -1001-1月 -11|
|trunc(x,y)|将日期x截断到y所指定的日期单位（月或年）的第一天|trunc(sysdate,'month')trunc(sysdate,'year')|01-10月 -1001-1月 -10|
|next_day(x,y)|计算指定日期x后的第一个星期几（由参数y指定）对应的日期|next_day(sysdate,星期二)||

 相关测试SQL语句
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** add_months(sysdate,2) **from** dual;   
- **select** last_day(sysdate) **from** dual;   
- **select** ename,months_between(sysdate,hiredate) **from** emp;    
- **select** round(sysdate,'month') **from** dual;   
- **select** round(sysdate,'year') **from** dual;   
- **select** trunc(sysdate,'month') **from** dual;   
- **select** trunc(sysdate,'year') **from** dual;   
- **select** next_day(sysdate,'星期二') **from** dual;  
转换函数
如下图所示，为常用几个转换函数
 常用日期格式符:
|格式符|说明|举例|
|----|----|----|
|yyyy|年份|2010|
|mm|用数字显示月份|10|
|dd|在当月中是第几天|5|
|day|星期几|星期日|
|am/pm|显示上午或下午|上午|
|hh/hh12/hh24|小时|7:41 21:41|
|mi|分钟|41|
|ss|秒|46|
      除上述格式符外，Oracle中日期模式还常用："- : ; /"，如要显示其他文本字符串则需要使用双引号括起来；也可以模式串的开头使用"fm"标记以去掉数字前面的零。
SQL描述
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** empno,ename,job, to_char(hiredate,'yyyy-mm-dd') **from** emp;   
- **select** to_char(sysdate,'yyyy-mm-dd hh:mi,ss am') **from** dual;   
- 
- **select** to_char(sysdate,'yyyy"年"mm"月"dd"日" hh:mi,ss am') **from** dual;   
- 
- **select** to_date('2010-10-02','yyyy-mm-dd') **from** dual;  
 常用数字描述符
||||||||
|----|----|----|----|----|----|----|
|格式符|说明| | | | | |
|9|代表一位数字| | | | | |
|0|代表一位数字，强制显示0| | | | | |
|$|放置一个美元符| | | | | |
|L|放置一个本地货币符| | | | | |
|.|小数点| | | | | |
|,|千位指示符| | | | | |
SQL示例：
Sql代码[](http://blog.csdn.net/topviewers/article/details/6769574#)
- **select** to_char(123123.123456) **from** dual;   
- **select** to_char(123456.123456,'$999,999.0000000') **from** dual;   
- **select** to_number('123456') **from** dual;   
- **select** to_number('985,365.1245','999,999.000000') **from** dual;  
------------------------------下面是oracle的练习题-------------------------------------
使用oracle自带的scott用户
1、求部门中那些人的薪水最高
**[sql]**[view
 plain](http://blog.csdn.net/topviewers/article/details/6769574#)[copy](http://blog.csdn.net/topviewers/article/details/6769574#)
- select ename,sal from emp  
- join
-  (  
- select deptno,max(sal) max_sal  
- from emp groupby deptno ) t  
- on
-  (emp.deptno = t.deptno and sal = t.max_sal);  
2 求部门平均薪水的等级
**[sql]**[view
 plain](http://blog.csdn.net/topviewers/article/details/6769574#)[copy](http://blog.csdn.net/topviewers/article/details/6769574#)
- select deptno,grade,avg_sal from
- (select deptno,avg(sal) avg_sal from emp groupby deptno) t    
- join salgrade on
-  ( t.avg_sal between salgrade.losal and salgrade.hisal);
  
3、求部门平均的薪水等级
**[sql]**[view
 plain](http://blog.csdn.net/topviewers/article/details/6769574#)[copy](http://blog.csdn.net/topviewers/article/details/6769574#)
- select deptno,avg(grade) from
- (  
- select deptno,ename,grade from emp   
- join salgrade s on
-   (emp.sal between s.losal and s.hisal)  
- )  
- groupby deptno;
  
4、雇员中有哪些人是经理人
**[sql]**[view
 plain](http://blog.csdn.net/topviewers/article/details/6769574#)[copy](http://blog.csdn.net/topviewers/article/details/6769574#)
- select ename from emp where empno in
- (selectdistinct mgr from emp)
  
5、不准用组函数，求薪水的最高值
**[sql]**[view
 plain](http://blog.csdn.net/topviewers/article/details/6769574#)[copy](http://blog.csdn.net/topviewers/article/details/6769574#)
- selectdistinct sal from emp where sal notin
- (  
- selectdistinct e1.sal from emp e1  
- join emp e2  
- on e1.sal < e2.sal  
- )  
6、求平均薪水最高的部门的部门编号
**[sql]**[view
 plain](http://blog.csdn.net/topviewers/article/details/6769574#)[copy](http://blog.csdn.net/topviewers/article/details/6769574#)
- select deptno,avg_sal from
- (  
- selectavg(sal) avg_sal,deptno from emp  
- groupby deptno  
- )   
- where avg_sal =  
- (  
- selectmax(avg_sal) from
-    (  
- selectavg(sal) avg_sal,deptno from emp  
- groupby deptno  
-    )  
- )  
7、求平均薪水最高的部门的部门名称
**[sql]**[view
 plain](http://blog.csdn.net/topviewers/article/details/6769574#)[copy](http://blog.csdn.net/topviewers/article/details/6769574#)
- select dname ,deptno from dept where deptno =   
- (  
- select deptno from
-   (  
- selectavg(sal) avg_sal,deptno from emp  
- groupby deptno  
-   )  
- where avg_sal =  
-   (  
- selectmax(avg_sal) from
-      (  
- selectavg(sal) avg_sal,deptno from emp  
- groupby deptno  
-      )  
-   )  
- )  
8、求平均薪水的等级最低的部门的部门名称
**[sql]**[view
 plain](http://blog.csdn.net/topviewers/article/details/6769574#)[copy](http://blog.csdn.net/topviewers/article/details/6769574#)
- select dname, t1.deptno,grade,avg_sal from
- (  
- select deptno ,grade,avg_sal from
-  (  
- selectavg(sal) avg_sal,deptno from emp groupby deptno)t  
- join salgrade s on
-      (t.avg_sal between s.losal and s.hisal)  
-  ) t1  
- join dept on (t1.deptno = dept.deptno)  
- where t1.grade =  
-  (  
- selectmin(grade) from
-    (  
- select avg_sal,deptno,grade from salgrade  
- join
-      (selectavg(sal) avg_sal,deptno from emp groupby deptno) t  
- on (t.avg_sal between salgrade.losal and hisal )  
-    )  
- )  
