
# expdp 导出 ORA-31638 ORA-25448 错误处理 - David Dai -- Focus on Oracle - CSDN博客


2015年03月05日 11:52:42[Dave](https://me.csdn.net/tianlesoftware)阅读数：11268



expdp 导出报如下错误：
[oracle@dg1 ~]$ expdp  directory=backup dumpfile=user.dmplogfile=user.log schemas=dave
Export: Release 11.2.0.3.0 - Production onThu Aug 28 00:28:34 2014
Copyright (c) 1982, 2011, Oracle and/or itsaffiliates.  All rights reserved.
Username: / as sysdba
Connected to: Oracle Database 11gEnterprise Edition Release 11.2.0.3.0 - 64bit Production
With the Partitioning, OLAP, Data Miningand Real Application Testing options
ORA-31626: job does not exist
ORA-31638: cannot attach to jobSYS_EXPORT_SCHEMA_01 for user SYS
ORA-06512: at"SYS.DBMS_SYS_ERROR", line 95
ORA-06512: at "SYS.KUPV$FT_INT",line 428
ORA-39077: unable to subscribe agentKUPC$A_1_002838232151000 to queue "KUPC$C_1_20140828002837"
ORA-06512: at"SYS.DBMS_SYS_ERROR", line 95
ORA-06512: at "SYS.KUPC$QUE_INT",line 250
ORA-25448: ruleSYS.KUPC$C_1_20140828002837$5 has errors
ORA-00376: file 2 cannot be read at thistime
ORA-01110: data file 2: '/u01/app/oracle/oradata/dave/sysaux01.dbf'

--查看数据文件状态：
SQL> col file_name for a50
SQL> select file_name,status fromdba_data_files;
FILE_NAME                   STATUS
-----------------------------------------------------------
/u01/app/oracle/oradata/dave/users01.dbf     AVAILABLE
/u01/app/oracle/oradata/dave/undotbs01.dbf      AVAILABLE
/u01/app/oracle/oradata/dave/sysaux01.dbf   AVAILABLE
/u01/app/oracle/oradata/dave/system01.dbf   AVAILABLE
/u01/dave01.dbf                AVAILABLE
/u01/dropspace02.dbf               AVAILABLE
/u01/dropspace03.dbf               AVAILABLE
/u01/dropspace04.dbf               AVAILABLE
/u01/fsf01.dbf                     AVAILABLE
/u01/dropspace01.dbf               AVAILABLE
10 rows selected.
toad 中也无法查看表空间，用sys用户连接的，权限不可能不够：
![](https://img-blog.csdn.net/20150305115143207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



查看数据库状态：
SQL> select open_mode from v$database;
OPEN_MODE
--------------------
READ WRITE
alert log 也没有错误。

开始重点放在如下部分：
ORA-31638: cannot attach to jobSYS_EXPORT_SCHEMA_01 for user SYS
ORA-06512: at"SYS.DBMS_SYS_ERROR", line 95
ORA-06512: at "SYS.KUPV$FT_INT",line 428
ORA-39077: unable to subscribe agentKUPC$A_1_002838232151000 to queue "KUPC$C_1_20140828002837"
ORA-06512: at"SYS.DBMS_SYS_ERROR", line 95
ORA-06512: at "SYS.KUPC$QUE_INT",line 250
ORA-25448: ruleSYS.KUPC$C_1_20140828002837$5 has errors
结果还是数据文件的问题， sysaux01.dbf 是offline的。 这个环境之前做过很多测试，也不清楚哪次给弄成offline了。

SQL> select file_name,online_status fromdba_data_files;
FILE_NAME                   ONLINE_
---------------------------------------------------------
/u01/app/oracle/oradata/dave/users01.dbf    ONLINE
/u01/app/oracle/oradata/dave/undotbs01.dbf      ONLINE
/u01/app/oracle/oradata/dave/sysaux01.dbf   OFFLINE
/u01/app/oracle/oradata/dave/system01.dbf    SYSTEM
/u01/dave01.dbf                ONLINE
/u01/dropspace02.dbf               RECOVER
/u01/dropspace03.dbf               RECOVER
/u01/dropspace04.dbf               RECOVER
/u01/fsf01.dbf                     ONLINE
/u01/dropspace01.dbf               OFFLINE
10 rows selected.
online之后，恢复正常：
SQL> alter database datafile'/u01/app/oracle/oradata/dave/sysaux01.dbf' online;
Database altered.
SQL> select file_name,online_status fromdba_data_files;
FILE_NAME                   ONLINE_
---------------------------------------------------------
/u01/app/oracle/oradata/dave/users01.dbf    ONLINE
/u01/app/oracle/oradata/dave/undotbs01.dbf      ONLINE
/u01/app/oracle/oradata/dave/sysaux01.dbf    ONLINE
/u01/app/oracle/oradata/dave/system01.dbf    SYSTEM
/u01/dave01.dbf                ONLINE
/u01/dropspace02.dbf               RECOVER
/u01/dropspace03.dbf               RECOVER
/u01/dropspace04.dbf               RECOVER
/u01/fsf01.dbf                     ONLINE
/u01/dropspace01.dbf               OFFLINE
10 rows selected.
Toad 也正常：
![](https://img-blog.csdn.net/20150305115124639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**AboutDave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1: 62697850 (空)    CNDBA_2: 62697716 (满)  CNDBA_3: 283816689
CNDBA_4: 391125754   CNDBA_5:104207940    CNDBA_6: 62697977   CNDBA_7: 142216823（满）


