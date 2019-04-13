
# Oracle基础：3: 程序中调用sqlplus的方式 - 知行合一 止于至善 - CSDN博客

2018年11月10日 06:07:35[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1591


![在这里插入图片描述](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过sqlplus可以连接数据库根据用户权限进行数据或者设定操作，但是需要交互操作并返回结果，这篇文章介绍一下如何在程序中使用sqlplus。
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 环境准备
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)使用Oracle的精简版创建docker方式的demo环境，详细可参看：
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[https://blog.csdn.net/liumiaocn/article/details/82733272](https://blog.csdn.net/liumiaocn/article/details/82733272)
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# Here Document
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)因为sqlplus是控制台的方式与用户进行交互式的输入/输出对应，而在程序执行的过程中显然是需要预先定好的输入，这样可以考虑使用Here Document，比如希望通过sqlplus来确认数据库版本信息，则可以这样
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`# sqlplus system/liumiao123 <<EOF
> select * from v\$version;
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Sun Oct 21 11:06:42 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL> 
BANNER
--------------------------------------------------------------------------------
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
PL/SQL Release 11.2.0.2.0 - Production
CORE	11.2.0.2.0	Production
TNS for Linux: Version 11.2.0.2.0 - Production
NLSRTL Version 11.2.0.2.0 - Production
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
#`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)注意：需要注意v$version中的$需要转义
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 创建table
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)接下来使用Here Document的方式调用sqlplus来创建table
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`# sqlplus system/liumiao123 <<EOF
> create table student (
> stuid  number(4),
> stuname varchar2(50),
> primary key (stuid)
> );
> desc student;
> EOF
SQL*Plus: Release 11.2.0.2.0 Production on Sun Oct 21 11:11:52 2018
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
SQL>   2    3    4    5  
Table created.
SQL>  Name					   Null?    Type
 ----------------------------------------- -------- ----------------------------
 STUID					   NOT NULL NUMBER(4)
 STUNAME					    VARCHAR2(50)
SQL> Disconnected from Oracle Database 11g Express Edition Release 11.2.0.2.0 - 64bit Production
#`[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 总结
[
](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)sqlplus结合Here Document即可实现在程序中调用sqlplus。
[
            ](https://img-blog.csdn.net/20180917101847329?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdW1pYW9jbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

