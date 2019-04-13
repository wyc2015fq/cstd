
# Oracle基础：4: sql执行错误时的控制方式 - 知行合一 止于至善 - CSDN博客

2018年11月10日 06:48:50[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1587标签：[oracle																](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)[docker																](https://so.csdn.net/so/search/s.do?q=docker&t=blog)[sqlplus																](https://so.csdn.net/so/search/s.do?q=sqlplus&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=docker&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)个人分类：[数据库																](https://blog.csdn.net/liumiaocn/article/category/6328292)
[
																								](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)


![在这里插入图片描述](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
多条sql执行时如果在中间的语句出现错误，后续会不会直接执行，如何进行设定，以及其他数据库诸如Mysql是如何对应的，这篇文章将会进行简单的整理和说明。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 环境准备
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)使用Oracle的精简版创建docker方式的demo环境，详细可参看：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[https://blog.csdn.net/liumiaocn/article/details/82733272](https://blog.csdn.net/liumiaocn/article/details/82733272)
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 多行语句的正常执行
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)对上篇文章创建的两个字段的学生信息表，正常添加三条数据，详细如下：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`# sqlplus system/liumiao123@XE <<EOF
> desc student
> select * from student;
> insert into student values (1001, 'liumiaocn');
> insert into student values (1002, 'liumiao');
> insert into student values (1003, 'michael');
> commit;
> select * from student;
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Sun Oct 21 12:08:35 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL>  Name					   Null?    Type
 ----------------------------------------- -------- ----------------------------
 STUID					   NOT NULL NUMBER(4)
 STUNAME					    VARCHAR2(50)
SQL> 
no rows selected
SQL> 
1 row created.
SQL> 
1 row created.
SQL> 
1 row created.
SQL> 
Commit complete.
SQL> 
     STUID STUNAME
---------- --------------------------------------------------
      1001 liumiaocn
      1002 liumiao
      1003 michael
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
#`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 多行语句中间出错时的缺省动作
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 问题：

> 三行insert语句，如果中间的一行出错，缺省的状况下第三行会不会被插入进去？

> 我们将第二条insert语句的主键故意设定重复，然后进行确认第三条数据是否会进行插入即可。

> \# sqlplus system/liumiao123@XE <<EOF
desc student
delete from student;
select * from student;
insert into student values (1001, 'liumiaocn');
insert into student values (1001, 'liumiao');
insert into student values (1003, 'michael');
select * from student;
commit;> > > > >       
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Sun Oct 21 12:15:16 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL>  Name					   Null?    Type
 ----------------------------------------- -------- ----------------------------
 STUID					   NOT NULL NUMBER(4)
 STUNAME					    VARCHAR2(50)
SQL> 
2 rows deleted.
SQL> 
no rows selected
SQL> 
1 row created.
SQL> insert into student values (1001, 'liumiao')
*
ERROR at line 1:
ORA-00001: unique constraint (SYSTEM.SYS_C007024) violated

SQL> 
1 row created.
SQL> 
     STUID STUNAME
---------- --------------------------------------------------
      1001 liumiaocn
      1003 michael
SQL> SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
\#
> 结果非常清晰地表明是会继续执行的，在oracle中通过什么来对其进行控制呢？

> WHENEVER SQLERROR

> 答案很简单，在oracle中通过WHENEVER SQLERROR来进行控制。

> WHENEVER SQLERROR {EXIT [SUCCESS | FAILURE | WARNING | n | variable | :BindVariable] [COMMIT | ROLLBACK] | CONTINUE [COMMIT | ROLLBACK | NONE]}

> WHENEVER SQLERROR EXIT

> 添加此行设定，即会在失败的时候立即推出，接下来我们进行确认：

> \# sqlplus system/liumiao123@XE <<EOF
WHENEVER SQLERROR EXIT
desc student
delete from student;
select * from student;
insert into student values (1001, 'liumiaocn');
insert into student values (1001, 'liumiao');
insert into student values (1003, 'michael');
select * from student;
commit;> > > > > > > > > 
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Sun Oct 21 12:27:15 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL> SQL>  Name					   Null?    Type
 ----------------------------------------- -------- ----------------------------
 STUID					   NOT NULL NUMBER(4)
 STUNAME					    VARCHAR2(50)
SQL> 
2 rows deleted.
SQL> 
no rows selected
SQL> 
1 row created.
SQL> insert into student values (1001, 'liumiao')
*
ERROR at line 1:
ORA-00001: unique constraint (SYSTEM.SYS_C007024) violated

Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
\#
> WHENEVER SQLERROR CONTINUE

> 使用CONTINUE则和缺省方式下的行为一致，出错仍然继续执行

> \# sqlplus system/liumiao123@XE <<EOF
WHENEVER SQLERROR CONTINUE
desc student
delete from student;
select * from student;
insert into student values (1001, 'liumiaocn');
insert into student values (1001, 'liumiao');
insert into student values (1003, 'michael');
select * from student;
commit;> > > > > > > > > 
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Sun Oct 21 12:31:54 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL> SQL>  Name					   Null?    Type
 ----------------------------------------- -------- ----------------------------
 STUID					   NOT NULL NUMBER(4)
 STUNAME					    VARCHAR2(50)
SQL> 
1 row deleted.
SQL> 
no rows selected
SQL> 
1 row created.
SQL> insert into student values (1001, 'liumiao')
*
ERROR at line 1:
ORA-00001: unique constraint (SYSTEM.SYS_C007024) violated

SQL> 
1 row created.
SQL> 
     STUID STUNAME
---------- --------------------------------------------------
      1001 liumiaocn
      1003 michael
SQL> 
Commit complete.
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
\#
> Mysql中类似的机制

> 在2014年7月，mysql中使用source是否提供相关的类似机制的问题中，最终引入了Oracle此项功能在mysql中引入的建议，详细请参看：

> [https://bugs.mysql.com/bug.php?id=73177](https://bugs.mysql.com/bug.php?id=73177)

> 所以目前这只是一个sqlplus端的强化功能，并非标准，不同数据库需要确认相应的功能是否存在。

> 总结

> Oracle中使用WHENEVER SQLERROR进行出错控制是否继续，本文给出的例子非常简单，详细功能的使用可根据文中列出的Usage进行自行验证和探索。

> 参考内容

> [https://docs.oracle.com/cd/E25178_01/server.1111/e16638/advisor.htm](https://docs.oracle.com/cd/E25178_01/server.1111/e16638/advisor.htm)

> [https://docs.oracle.com/cd/B19306_01/server.102/b14357/ch12052.htm](https://docs.oracle.com/cd/B19306_01/server.102/b14357/ch12052.htm)

[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
