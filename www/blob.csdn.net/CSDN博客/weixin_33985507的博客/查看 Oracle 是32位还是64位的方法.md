# 查看 Oracle 是32位还是64位的方法 - weixin_33985507的博客 - CSDN博客
2011年09月25日 09:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
## 方法一：使用sqlplus
64位：
[oracle@qs-wg-db2 ~]$ sqlplus / as sysdba;
SQL*Plus: Release 10.2.0.5.0 - Productionon Sun Sep 25 08:57:22 2011
Copyright (c) 1982, 2010, Oracle.  All Rights Reserved.
Connected to:
Oracle Database 10g Enterprise EditionRelease 10.2.0.5.0 - 64bit Production
With the Partitioning, OLAP, Data Miningand Real Application Testing options
SQL>
如果是64位，用sqlplus 连上之后会显示具体的位数信息，32位则不会显示。
32位：
C:\Users\Administrator.DavidDai>sqlplus/ as sysdba;
SQL*Plus: Release 11.2.0.1.0 Production onSun Sep 25 08:55:48 2011
Copyright (c) 1982, 2010, Oracle.  All rights reserved.
Connected to:
Oracle Database 11g Enterprise EditionRelease 11.2.0.1.0 - Production
With the Partitioning, OLAP, Data Miningand Real Application Testing options
## 方法二： 查看v$version 视图
32位：
SQL> select * from v$version;
BANNER
--------------------------------------------------------------------------
Oracle Database 11g Enterprise EditionRelease 11.2.0.1.0 - Production
PL/SQL Release 11.2.0.1.0 - Production
CORE   11.2.0.1.0      Production
TNS for 32-bit Windows: Version 11.2.0.1.0- Production
NLSRTL Version 11.2.0.1.0 - Production
64位：
SQL> select * from v$version;
BANNER
----------------------------------------------------------------
Oracle Database 10g Enterprise EditionRelease 10.2.0.5.0 - 64bi
PL/SQL Release 10.2.0.5.0 - Production
CORE   10.2.0.5.0      Production
TNS for Linux: Version 10.2.0.5.0 -Production
NLSRTL Version 10.2.0.5.0 – Production
和第一种方法一样，64位会显示具体的位数信息，32位不会显示。
## 方法三：查看v$sql 视图
32位：输出为8位16进制数
SQL> select address from v$sql whererownum<2;
ADDRESS
--------
B50ACCAC
64位：输出为16位16进制数
SQL> select address from v$sql whererownum<2;
ADDRESS
----------------
0000000196FDF7D8
-------------------------------------------------------------------------------------------------------
Blog： http://blog.csdn.net/tianlesoftware 
Weibo: http://weibo.com/tianlesoftware
Email: dvd.dba@gmail.com
DBA1 群：62697716(满);   DBA2 群：62697977(满)  DBA3 群：62697850(满)  
DBA 超级群：63306533(满);  DBA4 群：83829929(满) DBA5群： 142216823(满) 
DBA6 群：158654907(满)   DBA7 群：69087192(满)  DBA8 群：172855474
DBA 超级群2：151508914  DBA9群：102954821     聊天 群：40132017(满)
--加群需要在备注说明Oracle表空间和数据文件的关系，否则拒绝申请
