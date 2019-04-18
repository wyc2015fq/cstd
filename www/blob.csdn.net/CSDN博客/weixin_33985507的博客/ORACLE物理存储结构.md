# ORACLE物理存储结构 - weixin_33985507的博客 - CSDN博客
2014年12月10日 23:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
**1.查看数据库实例基本信息：**
```
SQL> SELECT DBID,NAME,CREATED,LOG_MODE,OPEN_MODE,FORCE_LOGGING,CURRENT_SCN,FLASHBACK_ON FROM V$DATABASE;
DBID        NAME        CREATED   LOG_MODE    OPEN_MODE             FORCE_LOGGING CURRENT_SCN FLASHBACK_ON
---------- --------- ----------- ------------ -------------------- ------------- ----------- ------------------
1392118573  ORCL       2014/11/22 ARCHIVELOG   READ WRITE            NO    1236387 NO
```
**2.控制文件：**
控制文件是一个二进制文件。ORACLE数据库实例启动后（即启动到NOMOUNT模式），要通过加载控制文件确定数据文件、重做日志文件的路（进入到MOUNT模式），然后才能打开数据库（OPEN）。
控制文件还包括数据库名称、数据库创建信息、表空间信息、数据文件状态、日志谁的信息、备份信息、检查点信息等。
```
SQL> SHOW PARAMETER CONTROL_FILES;
NAME TYPE VALUE
------------------------------------ ----------- ------------------------------
control_files string D:\APP\ADMINISTRATOR\ORADATA\ORCL\CONTROL01.CTL, D:\APP\ADMINISTRATOR\FLASH_RECOVERY_AREA\ORCL\CONTROL02.CTL
SQL> SELECT * FROM V$CONTROLFILE;
STATUS NAME IS_RECOVERY_DEST_FILE BLOCK_SIZE FILE_SIZE_BLKS
------- -------------------------------------------------------------------------------- --------------------- ---------- --------------
D:\APP\ADMINISTRATOR\ORADATA\ORCL\CONTROL01.CTL NO 16384 594
D:\APP\ADMINISTRATOR\FLASH_RECOVERY_AREA\ORCL\CONTROL02.CTL NO 16384 594
```
**3.数据文件：**
```
SQL> SELECT NAME FROM V$DATAFILE;
NAME
--------------------------------------------------------------------------------
D:\APP\ADMINISTRATOR\ORADATA\ORCL\SYSTEM01.DBF
D:\APP\ADMINISTRATOR\ORADATA\ORCL\SYSAUX01.DBF
D:\APP\ADMINISTRATOR\ORADATA\ORCL\UNDOTBS01.DBF
D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF
D:\APP\ADMINISTRATOR\ORADATA\ORCL\EXAMPLE01.DBF
```
**3.1.数据文件的状态：**
ONLINE,OFFLINE,SYSTEM,RECOVER, 只有SYSTEM表空间数据文件的状态会显示为SYSTEM。
```
SQL> ALTER DATABASE DATAFILE 'D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF' OFFLINE;
Database altered
SQL> ALTER DATABASE DATAFILE 'D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF' ONLINE;
ALTER DATABASE DATAFILE 'D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF' ONLINE
ORA-01113: 文件 4 需要介质恢复
ORA-01110: 数据文件 4: 'D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF'
SQL> SELECT STATUS,NAME FROM V$DATAFILE;
STATUS NAME
------- --------------------------------------------------------------------------------
SYSTEM D:\APP\ADMINISTRATOR\ORADATA\ORCL\SYSTEM01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\SYSAUX01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\UNDOTBS01.DBF
RECOVER D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\EXAMPLE01.DBF
SQL> conn sys/PASSWD@orcl as sysdba;
已连接。
SQL> shutdown immediate;
数据库已经关闭。
已经卸载数据库。
ORACLE 例程已经关闭。
SQL> startup mount;
ORACLE 例程已经启动。
Total System Global Area 647204864 bytes
Fixed Size 2178536 bytes
Variable Size 478151192 bytes
Database Buffers 159383552 bytes
Redo Buffers 7491584 bytes
数据库装载完毕。
SQL> recover datafile 'D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF';
完成介质恢复。
SQL> alter database open;
数据库已更改。
SQL> SELECT STATUS,NAME FROM V$DATAFILE;
STATUS NAME
------- --------------------------------------------------------------------------------
SYSTEM D:\APP\ADMINISTRATOR\ORADATA\ORCL\SYSTEM01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\SYSAUX01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\UNDOTBS01.DBF
OFLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\EXAMPLE01.DBF
SQL> ALTER DATABASE DATAFILE 'D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF' ONLINE;
数据库已更改。
SQL> SELECT STATUS,NAME FROM V$DATAFILE;
STATUS NAME
------- --------------------------------------------------------------------------------
SYSTEM D:\APP\ADMINISTRATOR\ORADATA\ORCL\SYSTEM01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\SYSAUX01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\UNDOTBS01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\USERS01.DBF
ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\EXAMPLE01.DBF
```
**4、临时文件**
当ORACLE在执行大规模排序时，如果PGA内存中无法存放，就会将数据存储到临时表空间中作为中转，当操作执行完后，这部分空间又会随机的被其它操作临时使用。
临时表空间必须处于Nologging模式。
临时表空间不能被置为只读。
RMAN备份时不会备份临时表空间数据文件，恢复时也不进行恢复。
查看临时表空间数据文件信息：
```
SQL> SELECT STATUS,ENABLED,BYTES/1024/1024 MB,NAME FROM V$TEMPFILE;
STATUS     ENABLED     MB         NAME
------- ---------- ---------- --------------------------------------------------------------------------------
ONLINE    READ WRITE    29           D:\APP\ADMINISTRATOR\ORADATA\ORCL\TEMP01.DBF
```
**5、日志文件**
在ORACLE数据库中，执行数据库修改操作后，并不是马上写入数据文件，而是首先生成重做信息，并写入SGA中的一块叫做LOG_BUFFER的固定区域，当这个区域中的数据满足一定的条件后，会有相应的进程
将LOG_BUFFER中的内容写入到重做日志文件中。例如：当日志切换时，相关进程会从重做日志文件中读取用户已提交的数据，并写入到数据文件。
重做日志文件至少两个，循环使用。
**5.1查看日志文件信息：**
```
SQL> SELECT * FROM V$LOGFILE;
GROUP# STATUS TYPE MEMBER IS_RECOVERY_DEST_FILE
---------- ------- ------- ------------------------------------------------- ---------------------
3 ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\REDO03.LOG NO
2 ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\REDO02.LOG NO
1 ONLINE D:\APP\ADMINISTRATOR\ORADATA\ORCL\REDO01.LOG NO
4 ONLINE D:\TESTADDLOG.LOG NO
SQL> SELECT * FROM V$LOG;
GROUP# THREAD# SEQUENCE# BYTES BLOCKSIZE MEMBERS ARCHIVED STATUS FIRST_CHANGE# FIRST_TIME NEXT_CHANGE# NEXT_TIME
---------- ---------- ---------- ---------- ---------- ---------- -------- ---------------- ------------- ----------- ------------ -----------
1 1 14 52428800 512 1 YES INACTIVE 1188864 2014/12/1 2 1208046 2014/12/9 2
2 1 16 52428800 512 1 YES INACTIVE 1208411 2014/12/9 2 1234202 2014/12/10
3 1 17 52428800 512 1 NO CURRENT 1234202 2014/12/10 281474976710 
4 1 15 8388608 512 1 YES INACTIVE 1208046 2014/12/9 2 1208411 2014/12/9 2
```
**5.2 日志文件的STATUS:**
UNUSED:还未写入过数据，刚创建的联机重做日志文件就是这种状态。
CURRENT: 当前正在使用。
ACTIVE:表示虽然当前未使用，不过该文件中内容尚未归档，或者文件中的内容还淌有全部写入数据文件中，实例恢复时必须借助该文件中的内容。
INACTIVE:表示该文件中的内容已被妥善处理，此时正处于空闲状态。
CLEARING:表示该日志文件正在被重建。
CLEARING_CURRENT:表示该日志文件重建时出现错误。
如果用户在执行某些DDL语句时附加NOLOGING子句或执行DML时指定HINT APPEND方式操作数据，则这种操作不会产生重做日志记录。
如果想让所有操作均记录重做日志，可以将数据库属性修改为Force Logging.         
```
SQL> SELECT FORCE_LOGGING FROM V$DATABASE;
FORCE_LOGGING
-------------
NO
```
默认情况下，数据处于NOForce Logging状态。要修改为Foce Logging模式，执行下列操作：
```
ALTER DATABASE FORCE LOGGING;
ALTER DATABASE NO FORCE LOGGING;--取消该模式。
```
**6.归档日志文件**
联机重做日志文件保存了数据库所做的修改。所谓归档就是ORACLE自动将写满或被切换的联机重做日志文件复制一份到指定的路径下，并按照相应的规则重新命名。
每个联机重做日志文件组中可以有多个成员，但是归档时每组仅会产生一个归档文件。
数据库只有处于归档模式才会产生归档日志文件。
查看数据库是否处理归档模式：
```
SQL> ARCHIVE LOG LIST;
数据库日志模式 存档模式
自动存档 启用
存档终点 USE_DB_RECOVERY_FILE_DEST
最早的联机日志序列 14
下一个存档日志序列 17
当前日志序列 17
SQL> SELECT NAME,LOG_MODE FROM V$DATABASE;
NAME LOG_MODE
--------- ------------
ORCL ARCHIVELOG
```
如果数据库处理非归档模式，可用如下的语句修改为归档模式：
```
ALTER SYSTEM SET LOG_ARCHIVE_DEST_1='LOCATION=E:\ORACLE\ORCL\ARCHIVE'; --修改路径档路径，将归档文件存放在用户自己定义的目录下。也可以不指定路径，则存放在安装路径的默认的路径下。
SHUTDOWN IMMEDIATE;
STARTUP MOUNT;
ALTER DATABASE ARCHIVELOG;--通过该命令将数据库置于归档。
ALTER DATABASE OPEN;
```
在默认情况下，ORACLE会等第一组联机重做日志文件被写满后再触发归档操作，DBA也可执行下列命令进行**手动归档：**
ALTER SYSTEM ARCHIVE LOG ALL;
