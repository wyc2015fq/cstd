# Oracle用户密码含有特殊字符应当如何处理 - z69183787的专栏 - CSDN博客
2013年12月27日 17:12:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16822
通常我们在设置Oracle密码时会将密码设置的比较复杂，一般有字母数字特殊符号组成
但是在有特殊符号时，修改密码或着导入导出数据时会遇到麻烦，下文便是对该情况下的解释说明：
本文以特殊符号“/”作为例子
一、Linux环境，数据库为Oracle10g
1、修改密码
[oracle@olivenan oracle]$ sqlplus '/as sysdba'
SQL*Plus: Release 10.2.0.1.0 - Production on Wed May 17 14:33:32 2006
Copyright (c) 1982, 2005, Oracle.  All rights reserved.
Connected to:
Oracle Database 10g Enterprise Edition Release 10.2.0.1.0 - Production
With the Partitioning, OLAP and Data Mining options
SQL> alter user test identified by aa/aa;
alter user test identified by aa/aa
                                  *
ERROR at line 1:
ORA-00922: missing or invalid option
此处应该使用""
SQL> alter user test identified by "aa/aa";
User altered.
SQL>
[oracle@olivenan oracle]$ exp test/aa/aa file=test.dmp buffer=65536
Export: Release 10.2.0.1.0 - Production on Wed May 17 14:32:10 2006
Copyright (c) 1982, 2005, Oracle.  All rights reserved.
EXP-00004: invalid username or password
Username:
此处应该进行转义使用 " "处理密码，使用 ' '处理用户名和密码
[oracle@olivenan oracle]$ exp 'test/"aa/aa"' file=test.dmp buffer=65536
Export: Release 10.2.0.1.0 - Production on Wed May 17 14:32:52 2006
Copyright (c) 1982, 2005, Oracle.  All rights reserved.
Connected to: Oracle Database 10g Enterprise Edition Release 10.2.0.1.0 - Production
With the Partitioning, OLAP and Data Mining options
Export done in WE8ISO8859P1 character set and AL16UTF16 NCHAR character set
About to export specified users ...
. exporting pre-schema procedural objects and actions
...............
导出成功，此处省略
二、Windows Xp下，数据库为Oracle9i
C:\Documents and Settings\w>sqlplus /nolog
SQL*Plus: Release 9.2.0.1.0 - Production on Wed May 17 14:56:34 2006
Copyright (c) 1982, 2002, Oracle Corporation.  All rights reserved.
SQL> connect sys/olivenan as sysdba
Connected.
SQL> alter user test identified by aa/aa;
alter user test identified by aa/aa
                                      *
ERROR at line 1:
ORA-00922: missing or invalid option
此处使用""
SQL> alter user test identified by "aa/aa";
User altered.
SQL>
Microsoft Windows XP [版本 5.1.2600]
(C) 版权所有 1985-2001 Microsoft Corp.
C:\Documents and Settings\w>exp test/aa/aa file=aa.dmp buffer=65536
Export: Release 9.2.0.1.0 - Production on Wed May 17 14:58:50 2006
Copyright (c) 1982, 2002, Oracle Corporation.  All rights reserved.
EXP-00004: invalid username or password
Username:
Password:
EXP-00056: ORACLE error 1017 encountered
ORA-01017: invalid username/password; logon denied
Username: ^Z^Z
EXP-00030: Unexpected End-Of-File encountered while reading input
EXP-00000: Export terminated unsuccessfully
此处使用"""   """来处理用户密码
C:\Documents and Settings\w>exp test/"""aa/aa""" file=aa.dmp buffer=65536
Export: Release 9.2.0.1.0 - Production on Wed May 17 14:59:10 2006
Copyright (c) 1982, 2002, Oracle Corporation.  All rights reserved.
Connected to: Oracle9i Enterprise Edition Release 9.2.0.1.0 - Production
With the Partitioning, OLAP and Oracle Data Mining options
JServer Release 9.2.0.1.0 - Production
Export done in WE8ISO8859P1 character set and AL16UTF16 NCHAR character set
server uses ZHS16GBK character set (possible charset conversion)
About to export specified users ...
. exporting pre-schema procedural objects and actions
. exporting foreign function library names for user test
. exporting PUBLIC type synonyms
. exporting private type synonyms
. exporting object type definitions for user test
About to export test's objects ...
. exporting database links
. exporting sequence numbers
