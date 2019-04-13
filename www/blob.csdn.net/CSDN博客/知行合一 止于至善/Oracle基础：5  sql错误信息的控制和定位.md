
# Oracle基础：5: sql错误信息的控制和定位 - 知行合一 止于至善 - CSDN博客

2018年11月11日 01:40:18[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2022


![在这里插入图片描述](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在sqlplus中执行的sql出错之后应该如何处理和对应，多行sql语句或者存储过程的信息如何进行错误定位，这篇文章将结合实例进行简单地说明。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 环境准备
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)使用Oracle的精简版创建docker方式的demo环境，详细可参看：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[https://blog.csdn.net/liumiaocn/article/details/82733272](https://blog.csdn.net/liumiaocn/article/details/82733272)
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 如何进行错误定位
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 场景：

> 假如有3行insert的sql语句，中间一行出错之后，后续继续执行的情况下，如何定位到第二行？

> dbms_utility.format_error_backtrace

> 通过使用dbms_utility.format_error_backtrace可以得到ERROR at line xxx:的信息，这对我们较为有用，我们接下来进行确认

> oracle@e871d42341c0:~$ sqlplus system/abcd1234@XE <<EOF
> SET SERVEROUTPUT ON
> desc student
> delete from student;
> select * from student;
> insert into student values (1001, 'liumiaocn');
> insert into student values (1001, 'liumiao');
> insert into student values (1003, 'michael');
> select * from student;
> commit;
> exec dbms_output.put_line(dbms_utility.format_error_backtrace);
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Sun Oct 21 13:06:07 2018
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

SQL> 
1 row created.
SQL> 
     STUID STUNAME
---------- --------------------------------------------------
      1001 liumiaocn
      1003 michael
SQL> 
Commit complete.
SQL> 
PL/SQL procedure successfully completed.
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
oracle@e871d42341c0:~$
> 可以看到，报错的时候提示了行号，但是行号是1，这是因为这种写法以一行为单位，自然是如此，如果是单个多行的存储过程，将会更加清晰。

> ERROR at line 1:
ORA-00001: unique constraint (SYSTEM.SYS_C007024) violated
> 所以我们将这个例子进行改造，三行insert的sql放到文件之中，然后在使用dbms_utility.format_error_backtrace来进行确认

> oracle@e871d42341c0:~$ cat /tmp/sqltest1.sql 
desc student
delete from student;
select * from student;
insert into student values (1001, 'liumiaocn');
insert into student values (1001, 'liumiao');
insert into student values (1003, 'michael');
select * from student;
commit;
oracle@e871d42341c0:~$
> 然后在尝试一下是否能够确认行号，会发现仍然不能精确定位：

> oracle@e871d42341c0:~$ sqlplus system/abcd1234@XE <<EOF
> SET SERVEROUTPUT ON
> @/tmp/sqltest1.sql
> exec dbms_output.put_line(dbms_utility.format_error_backtrace);
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Sun Oct 21 13:08:27 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL> SQL>  Name					   Null?    Type
 ----------------------------------------- -------- ----------------------------
 STUID					   NOT NULL NUMBER(4)
 STUNAME					    VARCHAR2(50)

2 rows deleted.

no rows selected

1 row created.
insert into student values (1001, 'liumiao')
*
ERROR at line 1:
ORA-00001: unique constraint (SYSTEM.SYS_C007024) violated

1 row created.

     STUID STUNAME
---------- --------------------------------------------------
      1001 liumiaocn
      1003 michael

Commit complete.
SQL> 
PL/SQL procedure successfully completed.
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
oracle@e871d42341c0:~$
> 因为dbms_utility.format_error_backtrace更多的场景是在于存储过程的错误定位，接下来我们使用一个简单的存储过程例子来进行确认错误行号定位, 先看一个正常的存储过程，把上面的内容稍微修改一下：

> oracle@e871d42341c0:~$ cat /tmp/addstudent.sql 
create or replace PROCEDURE addstudents
IS
student_count number;
BEGIN
delete from student;
select count(*) into student_count from student;
dbms_output.put('sql set count before :');
dbms_output.put_line(student_count);
insert into student values (1001, 'liumiaocn');
insert into student values (1002, 'liumiao');
insert into student values (1003, 'michael');
select count(*) into student_count from student;
dbms_output.put('sql set count after :');
dbms_output.put_line(student_count);
END;
/
exec addstudents();
oracle@e871d42341c0:~$
> 结果执行信息如下

> oracle@e871d42341c0:~$ sqlplus system/liumiao123 <<EOF
set serveroutput on;
@/tmp/addstudent.sql 
EOF
SQL*Plus: Release 11.2.0.2.0 Production on Mon Oct 22 04:42:11 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL> SQL> 
Procedure created.
sql set count before :0
sql set count after :3
PL/SQL procedure successfully completed.
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
oracle@e871d42341c0:~$
> 接下来我们修改一下内容，使得第二行主键重复

> oracle@e871d42341c0:~$ cat /tmp/addstudent.sql
create or replace PROCEDURE addstudents
IS
student_count number;
BEGIN
delete from student;
select count(*) into student_count from student;
dbms_output.put('sql set count before :');
dbms_output.put_line(student_count);
insert into student values (1001, 'liumiaocn');
insert into student values (1001, 'liumiao');
insert into student values (1003, 'michael');
select count(*) into student_count from student;
dbms_output.put('sql set count after :');
dbms_output.put_line(student_count);
END;
/
exec addstudents();
oracle@e871d42341c0:~$
> 再次执行，自然会出错，但是可以看到，正确报出了所在行数，这是procedure的机制提示的信息

> oracle@e871d42341c0:~$ sqlplus system/liumiao123 <<EOF
set serveroutput on;
@/tmp/addstudent.sql 
EOF
SQL*Plus: Release 11.2.0.2.0 Production on Mon Oct 22 04:44:25 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL> SQL> 
Procedure created.
sql set count before :0
BEGIN addstudents(); END;
*
ERROR at line 1:
ORA-00001: unique constraint (SYSTEM.SYS_C007024) violated
ORA-06512: at "SYSTEM.ADDSTUDENTS", line 10
ORA-06512: at line 1

SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
oracle@e871d42341c0:~$
> 可以看到，ORA-06512: at “SYSTEM.ADDSTUDENTS”, line 10的信息就是我们期待的信息，提示出在这个存储过程的第10行执行出现问题，而实际可以使用dbms_utility.format_error_backtrace结合exception给出更为清晰地方式，比如：

> oracle@e871d42341c0:~$ cat /tmp/addstudent.sql 
create or replace PROCEDURE addstudents
IS
student_count number;
BEGIN
delete from student;
select count(*) into student_count from student;
dbms_output.put('sql set count before :');
dbms_output.put_line(student_count);
insert into student values (1001, 'liumiaocn');
insert into student values (1001, 'liumiao');
insert into student values (1003, 'michael');
select count(*) into student_count from student;
dbms_output.put('sql set count after :');
dbms_output.put_line(student_count);
exception
when others then
dbms_output.put('exception happend with line info : ');
dbms_output.put_line(dbms_utility.format_error_backtrace);
END;
/
exec addstudents();
oracle@e871d42341c0:~$
> 执行结果确认：

> oracle@e871d42341c0:~$ sqlplus system/liumiao123 <<EOF
set serveroutput on;
@/tmp/addstudent.sql 
EOF
SQL*Plus: Release 11.2.0.2.0 Production on Mon Oct 22 04:49:27 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL> SQL> 
Procedure created.
sql set count before :0
exception happend with line info : ORA-06512: at "SYSTEM.ADDSTUDENTS", line 10

PL/SQL procedure successfully completed.
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
oracle@e871d42341c0:~$
> 这样则可以看出能够比较清晰地进行错误的定位了，但是由于功能受限，所以实际使用场景仍然较为有限，但是定位存储过程的信息则可以使用dbms_utility.format_error_backtrace等进行确认。

> 总结

> 多行sql执行定位可以考虑拆成单行来确认，而存储过程则可结合format_error_backtrace等进行确认以提供问题出现的所在行号。

[
						](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
