# Oracle数据库——触发器的创建与应用 - weixin_33985507的博客 - CSDN博客
2016年05月25日 18:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
一、涉及内容
1.理解触发器的概念、作用和类型。
2.练习触发器的创建和使用。
二、具体操作
（实验）
  1.利用触发器对在scott.emp表上执行的DML操作进行安全性检查，只有scott用户登录数据库后才能向该表中执行DML操作。（第1题中，user是系统函数，返回当前用户。字符串中使用两个单引号表示一个单引号。）
要求：分别以system用户和scott用户对emp 表执行DML操作，试验触发器的运行效果。
（1）在scott用户下创建触发器
语句：
```
create or replace trigger tri_dm1
 before insert or update or delete on scott.emp
 begin
   if user <>'SCOTT' then
     raise_application_error(-20001,'You don''t have access to modify this table.');
   end if;
 end;
 /
```
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183254194-1836324125.png)
（2）以system 用户连接，并对emp表执行DML操作
语句：
```
conn system/orcl1234;
insert into scott.emp(empno,ename)  values(8888,'shenxiao');
```
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183329381-1656323972.png)
（3）以scott用户连接，并对emp表执行DML操作
语句：
```
conn scott/tiger;
insert into scott.emp(empno,ename)  values(8888,'shenxiao');
```
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183405881-437053821.png)
  2.利用触发器进行表和备份表之间的同步复制。
（1）在scott 用户下创建scott.emp 表的复本 employee。
语句：
 conn scott/tiger;
create table employee as select * from scott.emp;
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183442147-1981245669.png)
（2）在scott用户下创建能实现scott.emp和employee 两表之间同步复制的DML触发器。
语句：
```
create or replace trigger duplicate_emp
 after update or insert or delete on scott.emp
 for each row
 begin
   if inserting then
     insert into  employee values (:new.empno,:new.ename,:new.job,:new.mgr,
                                   :new.hiredate,:new.sal,:new.comm,:new.deptno);
   elsif deleting then
     delete from employee where empno=:old.empno;
   else
     update employee set empno=:new.empno,ename=:new.ename,job=:new.job,
                         mgr=:new.mgr,hiredate=:new.hiredate,sal=:new.sal,comm=:new.comm,
                          deptno=:new.deptno
     where empno=:old.empno;
   end if;
  end;
 /
```
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183517741-163941828.png)
（3）对scott.emp表进行插入、删除和更新操作。
删除：delete from scott.emp where empno=7934;
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183540209-1338377578.png)
插入：
Insert into scott.emp(empno,ename,job,sal) values(1111,'zhangsan','ANALYST',2900)
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183559131-584507446.png)
更新：
 update scott.emp set sal=3900 where empno=1111;
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183624256-2080063341.png)
（4）查询scott.emp表和employee表中插入、删除和更新的记录。
语句：select * from scott.emp;
        select * from employee；
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183648897-1508733677.png)
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183718381-2140446051.png)
 3.建立触发器，对scott.emp 表进行DML操作时的时间、用户进行日志记录。
 （1）建立日志表emp_log。
语句：create table emp_log(who varchar2(30),when date,oper varchar2(10));
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183818053-1992299985.png)
 （2）在emp表上建立语句级触发器，将对emp表执行的操作记录到emp_log表中。
语句：
```
create or replace trigger dm1_log
 after insert or update or delete on scott.emp
 declare 
   oper emp_log.oper%type;
  begin 
    if inserting then 
       oper:='insert';
    elsif deleting then 
       oper:='delete';
    else 
       oper:='update';
    end if;
    insert into emp_log  values(user,sysdate,oper);
 end;
 /
```
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183854459-205007211.png)
 （3）对scott.emp 执行DML操作，查看emp_log 表中的数据。
语句：
```
insert into scott.emp(empno)  values(1112);
select * from emp_log;
```
截图：
![](https://images2015.cnblogs.com/blog/818082/201605/818082-20160525183931397-1827228494.png)
（习题）
1.简述Oracle 数据库中触发器的类型及触发条件。
答：触发器的类型及触发条件如下表所示：
|按划分类型|触发器的类型|触发条件|
|----|----|----|
|按照触发的时间|BEFORE触发器，指事前触发器|在触发语句执行前触发器被触发|
|AFTER触发器，指事后触发器|在触发语句执行以后触发器被触发| |
|INSTEAD OF触发器，指替代触发器|触发语句被触发器操作替代| |
|按照触发的事件|DML触发器|对表或视图执行DML操作时触发的触发器|
|DDL触发器|在数据库中执行DDL操作时触发的触发器| |
|用户事件触发器|与用户执行的DCL操作或LOGON/LOGOFF操作相关的触发器| |
|系统事件触发器|是指由数据库系统事件触发的触发器| |
其中，DML触发器，按照触发时DML操作影响的记录多少，又可分为：
行级触发器 ：DML语句每操作一行，行级触发器就会被调用一次
语句级触发器 ：DML语句不论影响多少行数据，语句级触发器只被调用一次
DDL触发器又可以分为：
数据库级DDL触发器 ：数据库中任何用户执行了相应的DDL操作该类触发器都被触发。
用户级DDL触发器 ：只有在创建触发器时指定方案的用户执行相应的DDL操作时触发器才被触发，其他用户执行该DDL操作时触发器不会被触发。
2.描述一个触发器的组成部分及其作用。
答：在Oracle系统中，触发器包括以下几个组成部分：
|组成部分|作用|
|----|----|
|1.触发器名称|触发器名是在创建触发器为触发器起的名称。一般包括：触发器执行的时间、执行的操作、涉及的表、涉及的列等。|
|2.触发语句|触发语句是导致Oracle执行触发器操作的诱因，它包括对触发时间、触发事件和触发对象的定义。只有用户对数据库执行的操作满足触发语句中定义的所有内容后，触发器才有可能被系统自动调用。|
|3.触发限制条件|触发限制条件是决定触发器是否被系统自动调用的另一个因素。当用户的操作满足触发语句时，触发器不一定被调用，此时，系统还要检查触发器中是否定义了触发限制条件，如果存在，还要检查当前的操作是否满足限制条件。|
|4.触发器操作|触发器操作是触发器的主体，是被系统自动执行的PL/SQL 程序块。当触发语句和触发限制条件都满足时，系统将自动执行触发器操作部分的代码。|
3.简述替代触发器的作用。
答：
创建触发器时若选择了INSTEAD OF子句，那么该触发器就是替代触发器。 替代触发器只能建立在视图上不能建立在表上。用户在视图上执行的DML操作将被替代触发器中的操作代替。
替代触发器主要解决对不可更新视图执行更新操作时带来的问题。在定义视图时，如果视图中没有选择基础表的主键咧，或者视图中的数据来自多个基础表，那么用户将无法对这样的视图直接执行插入、修改、删除操作。这种情况下，用户可以针对是视图创建一个替代触发器，将对视图的更新操作转换为对基础表的操作。
