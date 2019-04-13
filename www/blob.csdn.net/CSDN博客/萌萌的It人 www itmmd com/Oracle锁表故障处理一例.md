
# Oracle锁表故障处理一例 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月25日 17:44:05[Jlins](https://me.csdn.net/dyllove98)阅读数：1584


一次开发人员直接在数据库服务器上做delete table操作系统，由时在上班高峰运行，造成长时间执行无反应，进而把SQLPLUS客户端关闭掉，再次运行删除语句，造成数据库运行缓慢。
询问操作过程知道了是由锁表造成，最终处理；本文只是模拟锁表处理过程，下次遇到相同问题可以快速处理。
## 1. 查看数据库的等待事件
如果不清楚原因，可以通过先通过等待事件进行分析。
SQL> select sid,EVENT from v$session_wait where wait_class<>'Idle';
SID EVENT
---------- ----------------------------------------------------------------
158 SQL*Net message to client
159 enq: TX - row lock contention
SQL>

## 2. 查看数据库是否有锁
SQL> select * from v$lock where block=1;

## 3. 查询lock锁, 看谁锁谁
说明：BLOCK为1的行，表示资源由它锁定。REQUEST表示需要这个锁。
SQL> SELECT sid, id1, id2, lmode, block,request, type
FROM V$LOCK
WHERE id1 IN (SELECT id1 FROM V$LOCK WHERE lmode = 0)
ORDER BY id1, request;
SID        ID1        ID2      LMODE      BLOCK    REQUEST TY
---------- ---------- ---------- ---------- ---------- ---------- --
153     655397        308          6          1          0 TX
157     655397        308          0          0          6 TX

## 4. 查询锁表的SID,Serial\#，用户、开始时间，然后就可以杀掉该进程了。
SQL> select t1.sid,t1.serial\#,t1.username,t1.logon_time from v$session t1,v$locked_object t2 where t1.sid = t2.session_id order by t1.logon_time ;
SID    SERIAL\# USERNAME                       LOGON_TIME
---------- ---------- ------------------------------ -------------------
153         12 ABC                            2014-11-17 09:19:33
157        106 ABC                            2014-11-17 09:54:24

## 5. 可以进一步查询出SID，SPID等信息
SQL> select s.username, s.osuser, s.sid, s.serial\#, p.spid, s.program,s.STATUS
from v$session s,v$process p
where s.paddr = p.addr and s.username is not null;
USERNAME                       OSUSER                                SID    SERIAL\# SPID         PROGRAM                                          STATUS
------------------------------ ------------------------------ ---------- ---------- ------------ ------------------------------------------------ --------
ABC                            oracle                                153         12 4290         sqlplus@oradb (TNS V1-V3)                        INACTIVE
SYS                            oracle                                150         12 4417         sqlplus@oradb (TNS V1-V3)                        ACTIVE
ABC                            oracle                                157        119 4830         sqlplus@oradb (TNS V1-V3)                        ACTIVE
要么在oracle进行杀掉，以及在操作系统下杀掉，对于status为killed的语句，应用通过操作系统kill命令才能最终最快杀掉。
alter system kill session  '153,12';
alter system kill session  '157,119';
或操作系统中
kill -9 4290
kill -9 4830

## 6.  查询被锁的表的SID及相关资源
说明：出被锁的表及SQL，还是要结合上面的查询进行手工处理, 也可以通过SID找到SQL语句。
set pagesize 999;
set linesize 200;
col PROGRAM for a25;
col TERMINAL for a10;
SQL> select s.status,s.sid,s.serial\#,p.spid,
s.last_call_et as exec_seconds,t.sql_text as curr_sql
from gv$session s,v$process p,v$instance i,v$lock k, v$sqltext t
where s.paddr = p.addr
and s.type != 'BACKGROUND'
and s.lockwait = k.kaddr
and s.sql_hash_value = t.hash_value
and s.username is not null;
STATUS          SID    SERIAL\# SPID         EXEC_SECONDS CURR_SQL
-------- ---------- ---------- ------------ ------------ ----------------------------------------------------------------
ACTIVE          159         28 4072                 1285 update test_lock set name='aa4' where id = 1
SQL>

