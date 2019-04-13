
# Oracle基础：6:通过sqlplus执行sql语句后的结果判断 - 知行合一 止于至善 - CSDN博客

2018年11月11日 07:21:51[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：2043


![在这里插入图片描述](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这篇文章介绍一下如何对sqlplus执行的sql语句结果进行判断。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 环境准备
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)使用Oracle的精简版创建docker方式的demo环境，详细可参看：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[https://blog.csdn.net/liumiaocn/article/details/82733272](https://blog.csdn.net/liumiaocn/article/details/82733272)
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 常见问题
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)在sqlplus中执行sql语句，如果直接使用命令行的方式调用时会碰到两个问题：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)问题1:  需要进行交互性的输入
问题2：结果的判断不能通过返回值来确认
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 解决方式
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)在脚本调用里，解决方式如下
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)问题1可以通过前文提到的Here Document来解决。
问题2，则可以通过对输出的判断来进行，因为Oracle的错误代码基本以ORA-打头，所以根据输出中是否包含ORA-，基本可确认是否存在错误。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 执行结果判断示例
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)这里使用命令行的方式进行验证，直接拷贝到脚本中即可以使用脚本的方式与sqlplus进行集成。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`oracle@e871d42341c0:~$ sqlplus system/liumiao123@XE <<EOF
> desc student
> delete from student;
> select * from student;
> insert into student values (1001, 'liumiaocn');
> insert into student values (1001, 'liumiao');
> insert into student values (1003, 'michael');
> commit;
> select * from student;
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Mon Oct 22 05:18:51 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL>  Name					   Null?    Type
 ----------------------------------------- -------- ----------------------------
 STUID					   NOT NULL NUMBER(4)
 STUNAME					    VARCHAR2(50)
SQL> 
3 rows deleted.
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
Commit complete.
SQL> 
     STUID STUNAME
---------- --------------------------------------------------
      1001 liumiaocn
      1003 michael
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
oracle@e871d42341c0:~$ echo $?
0
oracle@e871d42341c0:~$`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)从上文可以看到，三行insert的sql语句由于第二行的主键重复，出现错误，但是最终的结果使用命令行的方式无法对结果进行判断，这是控制台方式的常见场景，比如sftp或者ftp等也有此特点，一般常用的对应方式无法通过返回值进行判断，只能通过输出来进行判断。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 输出信息
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)输出分为标准输出和标准错误两种，输入输出的FD分别为：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)标准输入：0
标准输出：1
标准错误：2
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)接下来我们看一下上文中的信息那些是标准输出，哪些是标准错误：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`oracle@e871d42341c0:~$ sqlplus system/abcd1234@XE <<EOF 2>output.error 1>output.info
> desc student
> delete from student;
> select * from student;
> insert into student values (1001, 'liumiaocn');
> insert into student values (1001, 'liumiao');
> insert into student values (1003, 'michael');
> commit;
> select * from student;
> EOF
oracle@e871d42341c0:~$ 
oracle@e871d42341c0:~$ cat output.error
oracle@e871d42341c0:~$ 
oracle@e871d42341c0:~$ cat output.info
SQL*Plus: Release 11.2.0.2.0 Production on Mon Oct 22 05:24:44 2018
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
Commit complete.
SQL> 
     STUID STUNAME
---------- --------------------------------------------------
      1001 liumiaocn
      1003 michael
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
oracle@e871d42341c0:~$`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)可以看到错误信息全在标准输出中，标准错误中没有信息。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 重定向标准输出与错误判断
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)虽然上述信息中可以看到，标准错误中没有信息，这里给出的方案是对应常见的控制台方式的错误控制，为了保证标准错误的信息不被遗漏，需要将标准错误和重定向到标准输出中，在bshell中写法如下：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> >输出文件名称 2>&1
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)结合本文的例子，使用方式如下：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`oracle@e871d42341c0:~$ sqlplus system/abcd1234@XE <<EOF >output.info 2>&1
> desc student
> delete from student;
> select * from student;
> insert into student values (1001, 'liumiaocn');
> insert into student values (1001, 'liumiao');
> insert into student values (1003, 'michael');
> commit;
> select * from student;
> EOF
oracle@e871d42341c0:~$ 
oracle@e871d42341c0:~$ cat output.info
SQL*Plus: Release 11.2.0.2.0 Production on Mon Oct 22 05:29:31 2018
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
Commit complete.
SQL> 
     STUID STUNAME
---------- --------------------------------------------------
      1001 liumiaocn
      1003 michael
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
oracle@e871d42341c0:~$`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 结果判断
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)使用grep确认是否存在ORA-相关的信息即可
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`oracle@e871d42341c0:~$ grep ORA- output.info 
ORA-00001: unique constraint (SYSTEM.SYS_C007024) violated
oracle@e871d42341c0:~$ echo $?
0
oracle@e871d42341c0:~$`[
            ](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

