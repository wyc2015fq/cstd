# 为什么忘记commit也会造成select查询的性能问题 - 文章 - 伯乐在线
原文出处： [潇湘隐者](http://www.cnblogs.com/kerrycode/p/5836015.html)
今天遇到一个很有意思的问题，一个开发人员反馈在测试服务器ORACLE数据库执行的一条简单SQL语句非常缓慢，他写的一个SQL没有返回任何数据，但是耗费了几分钟的时间。让我检查分析一下原因，分析解决过后，发现事情的真相有点让人哭笑不得，但是也是非常有意思的。我们先简单构造一下类似的案例，当然只是简单模拟。
假设一个同事A，创建了一个表并初始化了数据（实际环境数据量较大，有1G多的数据），但是他忘记提交了。我们简单模拟如下：

MySQL
```
SQL> create table test_uncommit
  2  as
  3  select * from dba_objects where 1=0;
 
Table created.
 
SQL> declare rowIndex number;
  2  begin
  3     for rowIndex in 1..70 loop
  4     insert into test_uncommit
  5     select * from dba_objects;
  6     end loop;
  7  end;
  8  /
 
PL/SQL procedure successfully completed.
 
SQL>
```
![clip_image001](http://jbcdn2.b0.upaiyun.com/2016/09/0763c1dd8056d13e708a37f6a2151713.png)
另外一个同事B对这个表做一些简单查询操作，但是他不知道同事A的没有提交INSERT语句，如下所示，查询时间用了大概5秒多（这个因为构造的数据量不是非常大的缘故。实际场景耗费了几分钟）

MySQL
```
SQL> SET TIMING ON;
SQL> SET AUTOTRACE ON;
SQL> SELECT COUNT(1) FROM SYS.TEST_UNCOMMIT WHERE OBJECT_ID=39;
 
  COUNT(1)
----------
         0
 
Elapsed: 00:00:05.38
 
Execution Plan
----------------------------------------------------------
Plan hash value: 970680813
 
------------------------------------------------------------------------------------
| Id  | Operation          | Name          | Rows  | Bytes | Cost (%CPU)| Time     |
------------------------------------------------------------------------------------
|   0 | SELECT STATEMENT   |               |     1 |    13 |  6931   (3)| 00:00:10 |
|   1 |  SORT AGGREGATE    |               |     1 |    13 |            |          |
|*  2 |   TABLE ACCESS FULL| TEST_UNCOMMIT |     1 |    13 |  6931   (3)| 00:00:10 |
------------------------------------------------------------------------------------
Predicate Information (identified by operation id):
---------------------------------------------------
   2 - filter("OBJECT_ID"=39)
 
Note
-----
   - dynamic sampling used for this statement
 
 
Statistics
----------------------------------------------------------
          4  recursive calls
          0  db block gets
     229304  consistent gets
      61611  physical reads
    3806792  redo size
        514  bytes sent via SQL*Net to client
        492  bytes received via SQL*Net from client
          2  SQL*Net roundtrips to/from client
          0  sorts (memory)
          0  sorts (disk)
          1  rows processed
 
SQL>
```
![clip_image002](http://jbcdn2.b0.upaiyun.com/2016/09/a35efab8909a02bc0b3f5562f1dab93c.png)
当时是在SQL Developer工具里面分析SQL的执行计划，并没有注意到redo size非常大的情况。刚开始怀疑是统计信息不准确导致，手工收集了一下该表的统计信息，执行的时间和执行计划依然如此，没有任何变化。 如果我们使用SQL*Plus，查看执行计划，就会看到redo size异常大，你就会有所察觉（见后面分析）

MySQL
```
SQL> exec dbms_stats.gather_table_stats('SYS','TEST_UNCOMMIT');
 
PL/SQL procedure successfully completed.
 
Elapsed: 00:00:12.29
```
因为ORACLE里面的写不阻塞读，所以不可能是因为SQL阻塞的缘故，然后我想查看这个表到底有多少记录，结果亮瞎了我的眼睛，记录数为0，但是空间用掉了852 个数据块

MySQL
```
SQL> SELECT TABLE_NAME, NUM_ROWS, BLOCKS FROM DBA_TABLES WHERE TABLE_NAME='TEST_UNCOMMIT';
 
TABLE_NAME                       NUM_ROWS     BLOCKS
------------------------------ ---------- ----------
TEST_UNCOMMIT                           0        852
 
SQL>
```
![clip_image003](http://jbcdn2.b0.upaiyun.com/2016/09/0de723188d226c17ab8ad8c009886dbe.png)
于是我使用Tom大师的show_space脚本检查、确认该表的空间使用情况，如下所示，该表确实使用852个数据块。

MySQL
```
SQL> set serverout on;
SQL> exec show_space('TEST_UNCOMMIT');
Free Blocks.............................             852
Total Blocks............................             896
Total Bytes.............................       7,340,032
Total MBytes............................               7
Unused Blocks...........................              43
Unused Bytes............................         352,256
Last Used Ext FileId....................               1
Last Used Ext BlockId...................          88,201
Last Used Block.........................              85
 
PL/SQL procedure successfully completed.
 
SQL>
```
![clip_image004](http://jbcdn2.b0.upaiyun.com/2016/09/47a0efe37608ebca522be6be113e0c2d.png)
分析到这里，那么肯定是遇到了插入数据操作,却没有提交的缘故。用下面脚本检查发现一个会话ID为883的对这个表有一个ROW级排他锁，而且会话还有一个事务排他锁，那么可以肯定这个会话执行了DML操作，但是没有提交。

MySQL
```
SET linesize 190 
COL osuser format a15 
COL username format a20 wrap 
COL object_name format a20 wrap 
COL terminal format a25 wrap 
COL req_mode format a20 
SELECT B.SID, 
       C.USERNAME, 
       C.OSUSER, 
       C.TERMINAL, 
       DECODE(B.ID2, 0, A.OBJECT_NAME, 
                     'TRANS-' 
                     ||TO_CHAR(B.ID1)) OBJECT_NAME, 
       B.TYPE, 
       DECODE(B.LMODE, 0, 'WAITING', 
                       1, 'NULL', 
                       2, 'Row-S(SS)', 
                       3, 'ROW-X(SX)', 
                       4, 'SHARE', 
                       5, 'S/ROW-X(SSX)', 
                       6, 'EXCLUSIVE', 
                       ' OTHER')       "LOCK MODE", 
       DECODE(B.REQUEST, 0, '', 
                         1, 'NULL', 
                         2, 'Row-S(SS)', 
                         3, 'ROW-X(SX)', 
                         4, 'SHARE', 
                         5, 'S/ROW-X(SSX)', 
                         6, 'EXCLUSIVE', 
                         'OTHER')      "REQ_MODE" 
FROM   DBA_OBJECTS A, 
       V$LOCK B, 
       V$SESSION C 
WHERE  A.OBJECT_ID(+) = B.ID1 
       AND B.SID = C.SID 
       AND C.USERNAME IS NOT NULL 
ORDER  BY B.SID, 
          B.ID2;
```
![clip_image005](http://jbcdn2.b0.upaiyun.com/2016/09/c13e1c9863fbaa7d92e79d77a60bf74f.png)
我们在会话里面提交后，然后重新执行这个SQL，你会发现执行计划里面redo size为0，这是因为redo size表示DML生成的redo log的大小，其实从上面的执行计划分析redo size异常，就应该了解到一个七七八八了，因为一个正常的SELECT查询是不会在redo log里面生成相关信息的。那么肯定是遇到了DML操作，但是没有提交。
![clip_image006](http://jbcdn2.b0.upaiyun.com/2016/09/d6f0d9564756e3a8ef1e7c3f24c68903.png)
分析到这里，我们已经知道事情的前因后果了，解决也很容易，找到那个会话的信息，然后定位到哪个同事，让其提交即可解决。但是，为什么没有提交与提交过后的差距那么大呢？是什么原因呢？ 我们可以在这个案例，提交前与提交后跟踪执行的SQL语句，如下所示。

MySQL
```
SQL> ALTER SESSION SET SQL_TRACE=TRUE;
 
Session altered.
 
SQL> SELECT COUNT(1) FROM SYS.TEST_UNCOMMIT WHERE OBJECT_ID=39;
 
  COUNT(1)
----------
         0
SQL> 
 
SQL> ALTER SESSION SET SQL_TRACE=FALSE;
 
Session altered.
```
提交前上面SQL生成的跟踪文件为scm2_ora_8444.trc，我们使用TKPROF格式化如下： tkprof scm2_ora_8444.trc out_uncommit.txt 如下所示
![clip_image007](http://jbcdn2.b0.upaiyun.com/2016/09/bdd2a63975df9547fb9730d6837b2cc2.png)
提交后，在另外一个会话执行上面的SQL，然后格式化跟踪文件如下所示：
![clip_image008](http://jbcdn2.b0.upaiyun.com/2016/09/b4f63e9785dba426af2c7e63d6e5e05d.png)
我们发现提交前与提交后两者的物理读、一致性读有较大差别（尤其是一致性读相差3倍多）。这个主要是因为ORACLE的一致性读需要构造cr块，产生了大量的逻辑读的缘故。相关理论与概念如下：
为什么要一致性读,为了保持数据的一致性。如果一个事务需要修改数据块中数据，会先在回滚段中保存一份修改前数据和SCN的数据块，然后再更新Buffer Cache中的数据块的数据及其SCN，并标识其为“脏”数据。
当其他进程读取数据块时，会先比较数据块上的SCN和进程自己的SCN。如果数据块上的SCN小于等于进程本身的SCN，则直接读取数据块上的数据；
如果数据块上的SCN大于进程本身的SCN，则会从回滚段中找出修改前的数据块读取数据。通常，普通查询都是一致性读。
一致性读什么时候需要cr块呢，那就是select语句在发现所查询的时间点对应的scn，与数据块当前所的scn不一致的时候。构造cr块的时候，首先去data buffer中去找包含数据库前镜像的undo块，如果有直接取出构建CR块，这时候是逻辑读，产生逻辑IO；但是data buffer将undo信息写出后，就没有需要的undo信息，就会去undo段找所需要的前镜像的undo信息，这时候从磁盘上读出block到buffer中，这时候产生物理读(物理IO)
