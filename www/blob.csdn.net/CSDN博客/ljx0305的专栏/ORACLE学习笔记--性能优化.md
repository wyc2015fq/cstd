# ORACLE学习笔记--性能优化 - ljx0305的专栏 - CSDN博客
2008年10月02日 13:35:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：633
1. 查询正在执行语句的执行计划(也就是实际语句执行计划)
select * from v$sql_plan where hash_value = (select sql_hash_value from v$session where sid = 1111);
其中id和parent_id表示了执行数的结构,数值最大的为最先执行
比如
IDPARENT_ID
-------------
[**IXDBA.NET社区论坛**](http://bbs.ixdba.net/)
0 
1 0
2 1
3 2
**4 3**
5 4
**6 3**
则执行计划树为
0
1
2
3
64
5
**2.****如何设置自动跟踪**
用system登录
执行$ORACLE_HOME/rdbms/admin/utlxplan.sql创建计划表
执行$ORACLE_HOME/sqlplus/admin/plustrce.sql创建plustrace角色
如果想计划表让每个用户都能使用，则
SQL>create public synonym plan_table for plan_table;
SQL> grant all on plan_table to public;
如果想让自动跟踪的角色让每个用户都能使用，则
SQL> grant plustrace to public;
通过如下语句开启/停止跟踪
**SET AUTOTRACE **
ON |OFF | ON EXPLAIN | ON STATISTICS | TRACEONLY | TRACEONLY EXPLAIN
**3.****如何跟踪自己的会话或者是别人的会话**
跟踪自己的会话很简单
**Alter session set sql_trace true|false**
Or
Exec dbms_session.set_sql_trace(TRUE);
如果跟踪别人的会话，需要调用一个包
exec dbms_system.set_sql_trace_in_session(sid,serial#,true|false)
跟踪的信息在user_dump_dest 目录下可以找到或通过如下脚本获得文件名称（适用于Win环境，如果是unix需要做一定修改）
SELECT p1.value||'/'||p2.value||'_ora_'||p.spid||'.ora' filename
FROM
v$process p,
v$session s,
v$parameter p1,
v$parameter p2
WHERE p1.name = 'user_dump_dest'
AND p2.name = 'db_name'
AND p.addr = s.paddr
AND s.audsid = USERENV ('SESSIONID')
**最后，可以通过****tkprof****来解析跟踪文件，如**
**tkprof ****原文件****[目标](javascript:void(0))文件**** sys=n**
**4.****怎么设置整个数据库系统跟踪**
其实文档上的alter system set sql_trace=true是不成功的，但是可以通过设置事件来完成这个工作，作用相等
**alter system set events**
**'10046 trace name context forever,level 1';**
如果关闭跟踪，可以用如下语句
**alter system set events**
**'10046 trace name context off';**
其中的level 1与上面的8都是跟踪级别
level 1：跟踪SQL语句，等于sql_trace=true
level 4：包括变量的详细信息
level 8：包括等待事件
level 12：包括绑定变量与等待事件
**5.****怎么样根据****OS****进程快速获得****DB****进程信息与正在执行的语句**
有些时候，我们在OS上操作，象TOP之后我们得到的OS进程，怎么快速根据OS信息获得DB信息呢？
我们可以编写如下脚本：
$more whoit.sh
#!/bin/sh
sqlplus /nolog 100,cascade=> TRUE);
dbms_stats.gather_table_stats(User,TableName,degree => 4,cascade => true);
这是对命令与工具包的一些总结
**<1>****、**对于分区表，建议使用DBMS_STATS，而不是使用Analyze语句。
a) 可以并行进行，对多个用户，多个Table 
b) 可以得到整个分区表的数据和单个分区的数据。
c) 可以在不同级别上Compute Statistics：单个分区，子分区，全表，所有分区
d) 可以倒出统计信息
e) 可以用户自动收集统计信息
**<2>****、**DBMS_STATS的缺点
a) 不能Validate Structure 
b) 不能收集CHAINED ROWS, 不能收集CLUSTER TABLE的信息，这两个仍旧需要使用Analyze语句。
c) DBMS_STATS 默认不对索引进行Analyze，因为默认Cascade是False，需要手工指定为True 
**<3>****、**对于oracle 9里面的External Table，Analyze不能使用，只能使用DBMS_STATS来收集信息。
**6.****怎么样快速重整索引**通过rebuild语句，可以快速重整或移动索引到别的表空间
rebuild有重建整个索引数的功能，可以在不删除原始索引的情况下改变索引的存储参数
语法为
alter index index_name rebuild tablespace ts_name
storage(......);
如果要快速重建整个用户下的索引，可以用如下脚本，当然，需要根据你自己的情况做相应修改
SQL> set heading off
SQL> set feedback off
SQL> spool d:/index.sql
SQL> SELECT 'alter index ' || index_name || ' rebuild '
||'tablespace INDEXES storage(initial 256K next 256K pctincrease 0);'
FROM all_indexes
WHERE ( tablespace_name != 'INDEXES'
OR next_extent != ( 256 * 1024 )
)
AND owner = USER
SQL>spool off
**另外一个合并索引的语句是****alter index index_name coalesce**
这个语句仅仅是合并索引中同一级的leaf block，消耗不大，对于有些索引中存在大量空间浪费的情况下，有一些作用。
**7.****如何使用****Hint****提示**
在select/delete/update后写/*+ hint */
如select /*+ index(TABLE_NAME INDEX_NAME) */ col1...
注意/*和+之间不能有空格，如用hint指定使用某个索引
select /*+ index(cbotab) */ col1 from cbotab;
select /*+ index(cbotab cbotab1) */ col1 from cbotab;
select /*+ index(a cbotab1) */ col1 from cbotab a;
其中
TABLE_NAME是必须要写的，且如果在查询中使用了表的别名，在hint也要用表的别名来代替表名;
INDEX_NAME可以不必写，Oracle会根据统计值选一个索引;
如果索引名或表名写错了，那这个hint就会被忽略;
**8.****怎么样快速复制表或者是插入数据**
快速复制表可以指定Nologging选项
如：Create table t1 nologging 
as select * from t2;
快速插入数据可以指定append提示，但是需要注意noarchivelog模式下，默认用了append就是nologging模式的。在archivelog下，需要把表设置程Nologging模式。
如insert /*+ append */ into t1 
select * from t2
注意：如果在9i环境中并设置了FORCE LOGGING，则以上操作是无效的，并不会加快，当然，可以通过如下语句设置为NO FORCE LOGGING。
Alter database no force logging;
是否开启了FORCE LOGGING，可以用如下语句查看
SQL> select force_logging from v$database;
在很多时候，Oracle会错误的使用索引而导致效率的明显下降，我们可以使用一点点[技巧](javascript:void(0))而避免使用不该使用的索引，如：
表test,有字段a,b,c,d，在a,b,c上建立联合索引inx_a(a,b,c)，在b上单独建立了一个索引Inx_b(b)。
在正常情况下，where a=? and b=? and c=?会用到索引inx_a，where b=?会用到索引inx_b，但是，where a=? and b=? and c=? group by b会用到哪个索引呢？在分析数据不正确（很长时间没有分析）或根本没有分析数据的情况下，oracle往往会使用索引inx_b。通过执行计划的分析，这个索引的使用，将大大耗费查询时间。
当然，我们可以通过如下的技巧避免使用inx_b，而使用inx_a。
where a=? and b=? and c=? group by b||'' --如果b是字符
where a=? and b=? and c=? group by b+0 --如果b是数字
通过这样简单的改变，往往可以是查询时间提交很多倍
当然，我们也可以使用no_index提示，相信很多人没有用过，也是一个不错的方法：
select /*+ no_index(t,inx_b) */ * from test t
where a=? and b=? and c=? group by b
举例：
本来在CM_USER上有索引IDX_CM_USER4(ACC_ID)和IDX_CM_USER8(BILL_ID),可是执行如下语句的时候很慢。
select * from CM_USER where  acc_id =1200007175 
and user_status>0 and bill_id like '13%' order by acc_id,bill_id
用explain分析，发现执行计划是用IDX_CM_USER8.如下查询
select * from user_indexes where table_name ='CM_USER'发现IDX_CM_USER8没有分析过。
用下面语句执行计划改变
select /*+INDEX(CM_USER IDX_CM_USER4)*/* from CM_USER where  acc_id =1200007175 and user_status>0 and bill_id like '13%' order by acc_id,bill_id
或者分析索引
exec dbms_stats.gather_index_stats(ownname => 'QACS1',indname => 'IDX_CM_USER8',estimate_percent => 5 );
可以发现执行计划恢复正常。
**10.Oracle****什么时候会使用跳跃式索引扫描**
这是9i的一个新特性跳跃式索引扫描(Index Skip Scan).
例如表有索引index(a,b,c)，当查询条件为where b=?的时候，可能会使用到索引index(a,b,c)，如，执行计划中出现如下计划：
INDEX (SKIP SCAN) OF 'TEST_IDX' (NON-UNIQUE)
Oracle的优化器(这里指的是CBO)能对查询应用Index Skip Scans至少要有几个条件:
**<1>**优化器认为是合适的。
**<2>**索引中的前导列的唯一值的数量能满足一定的条件（如重复值很多）。
**<3>**优化器要知道前导列的值分布(通过分析/统计表得到)。
**<4>**合适的SQL语句
等。
**11.****怎么样创建使用虚拟索引**可以使用nosegment选项，如
create index virtual_index_name on table_name(col_name) nosegment;
如果在哪个session需要测试虚拟索引，可以利用隐含参数来处理
alter session set "_use_nosegment_indexes" = true;
就可以利用explain plan for select ……来看虚拟索引的[效果](javascript:void(0))，利用@$ORACLE_HOME/rdbms/admin/utlxpls查看执行计划，最后，根据需要，我们可以删除虚拟索引，如普通索引一样
drop index virtual_index_name;
注意：虚拟索引并不是物理存在的，所以虚拟索引并不等同于物理索引，不要用自动跟踪去测试虚拟索引，因为那是实际执行的效果，是用不到虚拟索引的。
**12.****怎样监控无用的索引**Oracle 9i以上，可以监控索引的使用情况，如果一段时间内没有使用的索引，一般就是无用的索引
语法为：
开始监控：alter index index_name monitoring usage;
检查使用状态：select * from v$object_usage;
停止监控：alter index index_name nomonitoring usage;
当然，如果想监控整个用户下的索引，可以采用如下的脚本：
set heading off
set echo off
set feedback off
set pages 10000
spool start_index_monitor.sql
SELECT 'alter index '||owner||'.'||index_name||' monitoring usage;'
FROM dba_indexes
WHERE owner = USER; 
spool off 
set heading on
set echo on
set feedback on
------------------------------------------------
set heading off
set echo off
set feedback off
set pages 10000
spool stop_index_monitor.sql
SELECT 'alter index '||owner||'.'||index_name||' nomonitoring usage;'
FROM dba_indexes
WHERE owner = USER; 
spool off 
set heading on
set echo on
set feedback on
**13.****怎么样能固定我的执行计划**
可以使用OUTLINE来固定SQL语句的执行计划，用如下语句可以创建一个OUTLINE
Create oe replace outline OutLn_Name on
Select Col1,Col2 from Table
where .......
如果要删除Outline，可以采用
Drop Outline OutLn_Name;
对于已经创建了的OutLine，存放在OUTLN用户的OL$HINTS表下面，对于有些语句，你可以使用update outln.ol$hints来更新outline，如
update outln.ol$hints(ol_name,'TEST1','TEST2','TEST2','TEST1)
where ol_name in ('TEST1','TEST2');
这样，你就把Test1 OUTLINE与Test2 OUTLINE互换了，如果想利用已经存在的OUTLINE，需要设置以下参数
Alter system/session set Query_rewrite_enabled = true
Alter system/session set use_stored_outlines = true
**14.v$sysstat****中的****class****分别代表什么**
统计类别
1 代表事例活动
2 代表Redo buffer活动
4 代表锁
8 代表数据缓冲活动
16 代表OS活动
32 代表并行活动
64 代表表访问
128 代表调试信息
**15.****怎么杀掉特定的数据库会话**
Alter system kill session 'sid,serial#';
或者
alter system disconnect session 'sid,serial#' immediate;
在win上，还可以采用oracle提供的orakill杀掉一个线程（其实就是一个Oracle进程）
在Linux/Unix上，可以直接利用kill杀掉数据库进程对应的OS进程
**16.****怎么快速查找锁与锁等待**
数据库的锁是比较耗费资源的，特别是发生锁等待的时候，我们必须找到发生等待的锁，有可能的话，杀掉该进程。
这个语句将查找到数据库中所有的DML语句产生的锁，还可以发现，任何DML语句其实产生了两个锁，一个是表锁，一个是行锁。
可以通过alter system kill session ‘sid,serial#’来杀掉会话
SELECT /*+ rule */ s.username,
decode(l.type,'TM','TABLE LOCK','TX','ROW LOCK',NULL) LOCK_LEVEL,
o.owner,o.object_name,o.object_type,
s.sid,s.serial#,s.terminal,s.machine,s.program,s.osuser
FROM v$session s,v$lock l,dba_objects o
WHERE l.sid = s.sid
AND l.id1 = o.object_id(+)
AND s.username is NOT NULL
如果发生了锁等待，我们可能更想知道是谁锁了表而引起谁的等待，以下的语句可以查询到谁锁了表，而谁在等待。
SELECT /*+ rule */ lpad(' ',decode(l.xidusn ,0,3,0))||l.oracle_username User_name,
o.owner,o.object_name,o.object_type,s.sid,s.serial#
FROM v$locked_object l,dba_objects o,v$session s
WHERE l.object_id=o.object_id
AND l.session_id=s.sid
ORDER BY o.object_id,xidusn DESC
以上查询结果是一个树状结构，如果有子节点，则表示有等待发生。如果想知道锁用了哪个回滚段，还可以关联到V$rollname，其中xidusn就是回滚段的USN
[Q] 如何有效的删除一个大表(extent数很多的表)
[A] 一个有很多(100k)extent的表，如果只是简单地用drop table的话，会很大量消耗CPU（Oracle要对fet$、uet$数据字典进行操作），可能会用上几天的时间，较好的方法是分多次删除extent，以减轻这种消耗：
1. truncate table big-table reuse storage;
2. alter table big-table deallocate unused keep 2000m ( 原来大小的n-1/n);
3. alter table big-table deallocate unused keep 1500m ;
....
4. drop table big-table;
**17.****如何收缩临时数据文件的大小**
9i以下版本采用
ALTER DATABASE DATAFILE 'file name' RESIZE 100M类似的语句
9i以上版本采用
ALTER DATABASE TEMPFILE 'file name' RESIZE 100M
注意，临时数据文件在使用时，一般不能收缩，除非关闭数据库或断开所有会话，停止对临时数据文件的使用。
**18.****怎么清理临时段**
可以使用如下办法
<1>、使用如下语句查看一下认谁在用临时段
SELECT username,sid,serial#,sql_address,machine,program,
tablespace,segtype, contents 
FROM v$session se,v$sort_usage su
WHERE se.saddr=su.session_addr
 <2>、那些正在使用临时段的进程
SQL>Alter system kill session 'sid,serial#';
<3>、把TEMP表空间回缩一下
SQL>Alter tablespace TEMP coalesce;
还可以使用诊断事件
<1>、确定TEMP表空间的ts#
SQL> select ts#, name FROM v$tablespace;
TS# NAME 
----------------------- 
0 SYSYEM 
1 RBS 
2 USERS 
3* TEMP 
...
<2>、执行清理操作
alter session set events 'immediate trace name DROP_SEGMENTS level TS#+1'
说明：
temp表空间的TS# 为 3*, So TS#+ 1= 4，如果想清除所有表空间的临时段，则，TS# = 2147483647
**19.****怎么样****dump****数据库内部结构，如上面显示的控制文件的结构**
常见的有
1、分析数据文件块，转储数据文件n的块m
alter system dump datafile n block m
2、分析日志文件
alter system dump logfile logfilename;
3、分析控制文件的内容
alter session set events 'immediate trace name CONTROLF level 10'
4、分析所有数据文件头
alter session set events 'immediate trace name FILE_HDRS level 10'
5、分析日志文件头
alter session set events 'immediate trace name REDOHDR level 10'
6、分析系统状态，最好每10分钟一次，做三次对比
alter session set events 'immediate trace name SYSTEMSTATE level 10'
7、分析进程状态
alter session set events 'immediate trace name PROCESSSTATE level 10'
8、分析Library Cache的详细情况
alter session set events 'immediate trace name library_cache level 10'
**20.****如何获得所有的事件代码**
事件代码范围一般从10000 to 10999，以下列出了这个范围的事件代码与信息
SET SERVEROUTPUT ON
DECLARE 
err_msg VARCHAR2(120);
BEGIN
dbms_output.enable (1000000);
FOR err_num IN 10000..10999
LOOP
err_msg := SQLERRM (-err_num);
IF err_msg NOT LIKE '%Message '||err_num||' not found%' THEN
dbms_output.put_line (err_msg);
END IF;
END LOOP;
END;
/
在Unix系统上，事件信息放在一个文本文件里
$ORACLE_HOME/rdbms/mesg/oraus.msg
可以用如下脚本查看事件信息
event=10000
while [ $event -ne 10999 ]
do
event=`expr $event + 1`
oerr ora $event
done
对于已经确保的/正在跟踪的事件，可以用如下脚本获得
SET SERVEROUTPUT ON
DECLARE
l_level NUMBER;
BEGIN
FOR l_event IN 10000..10999
LOOP
dbms_system.read_ev (l_event,l_level);
IF l_level > 0 THEN
dbms_output.put_line ('Event '||TO_CHAR (l_event)||
' is set at level '||TO_CHAR (l_level));
END IF;
END LOOP;
END;
/
Statspack是Oracle 8i以上提供的一个非常好的性能监控与诊断工具，基本上全部包含了BSTAT/ESTAT的功能，更多的信息，可以参考附带文档$ORACLE_HOME/rdbms/admin/spdoc.txt。
安装Statspack: 
cd $ORACLE_HOME/rdbms/admin
sqlplus "/ as sysdba" @spdrop.sql -- 卸载，第一次可以不需要
sqlplus "/ as sysdba" @spcreate.sql -- 需要根据提示输入表空间名
使用Statspack: 
sqlplus perfstat/perfstat
exec statspack.snap; -- 进行信息收集统计，每次运行都将产生一个快照号
-- 获得快照号，必须要有两个以上的快照，才能生成报表
select SNAP_ID, SNAP_TIME from STATS$SNAPSHOT; 
@spreport.sql -- 输入需要查看的开始快照号与结束快照号
其他相关脚本s: 
spauto.sql - 利用dbms_job提交一个作业，自动的进行STATPACK的信息收集统计
sppurge.sql - 清除一段范围内的统计信息，需要提供开始快照与结束快照号
sptrunc.sql - 清除(truncate) 
所有统计信息
**22. SQL****语句的优化方法**
<1> /*+ALL_ROWS*/
表明对语句块选择基于开销的优化方法,并获得最佳吞吐量,使资源消耗最小化.
例如:
SELECT /*+ALL+_ROWS*/ EMP_NO,EMP_NAM,DAT_IN FROM BSEMPMS WHERE EMP_NO='CCBZZP';
<2>. /*+FIRST_ROWS*/
表明对语句块选择基于开销的优化方法,并获得最佳响应时间,使资源消耗最小化.
例如:
SELECT /*+FIRST_ROWS*/ EMP_NO,EMP_NAM,DAT_IN FROM BSEMPMS WHERE    EMP_NO='CCBZZP';
<3>. /*+CHOOSE*/
[**www.ixdba.net**](http://www.ixdba.net/)
表明如果数据字典中有访问表的统计信息,将基于开销的优化方法,并获得最佳的吞吐量;
表明如果数据字典中没有访问表的统计信息,将基于规则开销的优化方法;
例如:
SELECT /*+CHOOSE*/ EMP_NO,EMP_NAM,DAT_IN FROM BSEMPMS WHERE EMP_NO='CCBZZP';
<4>. /*+RULE*/
表明对语句块选择基于规则的优化方法.
例如:
SELECT /*+ RULE */ EMP_NO,EMP_NAM,DAT_IN FROM BSEMPMS WHERE EMP_NO='CCBZZP';
<5>. /*+FULL(TABLE)*/
表明对表选择全局扫描的方法.
例如:
SELECT /*+FULL(A)*/ EMP_NO,EMP_NAM FROM BSEMPMS A WHERE EMP_NO='CCBZZP';
<6>. /*+ROWID(TABLE)*/
提示明确表明对指定表根据ROWID进行访问.
例如:
SELECT /*+ROWID(BSEMPMS)*/ * FROM BSEMPMS WHERE ROWID>='AAAAAAAAAAAAAA'
   AND EMP_NO='CCBZZP';
<7>. /*+CLUSTER(TABLE)*/ 
提示明确表明对指定表选择簇扫描的访问方法,它只对簇对象有效.
例如:
SELECT  /*+CLUSTER */ BSEMPMS.EMP_NO,DPT_NO FROM BSEMPMS,BSDPTMS
WHERE DPT_NO='TEC304' AND BSEMPMS.DPT_NO=BSDPTMS.DPT_NO;
<8>. /*+INDEX(TABLE INDEX_NAME)*/
表明对表选择索引的扫描方法.
例如:
SELECT /*+INDEX(BSEMPMS SEX_INDEX) USE SEX_INDEX BECAUSE THERE ARE FEWMALE    BSEMPMS */  FROM BSEMPMS WHERE SEX='M';
<9>. /*+INDEX_ASC(TABLE INDEX_NAME)*/
表明对表选择索引升序的扫描方法.
例如:
SELECT /*+INDEX_ASC(BSEMPMS PK_BSEMPMS) */  FROM BSEMPMS WHERE DPT_NO='CCBZZP';
<10>. /*+INDEX_COMBINE*/
为指定表选择位图访问路经,如果INDEX_COMBINE中没有提供作为参数的索引,将选择出位图索引的
布尔组合方式.
例如:
SELECT /*+INDEX_COMBINE(BSEMPMS SAL_BMI HIREDATE_BMI)*/ * FROM BSEMPMS
WHERE SAL<5000000 AND HIREDATE<SYSDATE;
<11>. /*+INDEX_JOIN(TABLE INDEX_NAME)*/
提示明确命令优化器使用索引作为访问路径.
例如:
SELECT /*+INDEX_JOIN(BSEMPMS SAL_HMI HIREDATE_BMI)*/ SAL,HIREDATE
FROM BSEMPMS WHERE SAL<60000;
<12>. /*+INDEX_DESC(TABLE INDEX_NAME)*/
表明对表选择索引降序的扫描方法.
例如:
SELECT /*+INDEX_DESC(BSEMPMS PK_BSEMPMS) */  FROM BSEMPMS WHERE    DPT_NO='CCBZZP';
<13>. /*+INDEX_FFS(TABLE INDEX_NAME)*/
对指定的表执行快速全索引扫描,而不是全表扫描的办法.
例如:
SELECT /*+INDEX_FFS(BSEMPMS IN_EMPNAM)*/ * FROM BSEMPMS WHERE DPT_NO='TEC305';
<14>. /*+ADD_EQUAL TABLE INDEX_NAM1,INDEX_NAM2,...*/
提示明确进行执行规划的选择,将几个单列索引的扫描合起来.
例如:
SELECT /*+INDEX_FFS(BSEMPMS IN_DPTNO,IN_EMPNO,IN_SEX)*/ * FROM BSEMPMS WHERE EMP_NO='CCBZZP' AND DPT_NO='TDC306';
<15>. /*+USE_CONCAT*/
对查询中的WHERE后面的OR条件进行转换为UNION ALL的组合查询.
例如:
SELECT /*+USE_CONCAT*/ * FROM BSEMPMS WHERE DPT_NO='TDC506' AND SEX='M';
<16>. /*+NO_EXPAND*/
对于WHERE后面的OR 或者IN-LIST的查询语句,NO_EXPAND将阻止其基于优化器对其进行扩展.
例如:
SELECT /*+NO_EXPAND*/ * FROM BSEMPMS WHERE  DPT_NO='TDC506' AND SEX='M';
<17>. /*+NOWRITE*/
禁止对查询块的查询重写操作.
<18>. /*+REWRITE*/
可以将视图作为参数.
<19>. /*+MERGE(TABLE)*/
能够对视图的各个查询进行相应的合并.
例如:
SELECT /*+MERGE(V) */ A.EMP_NO,A.EMP_NAM,B.DPT_NO FROM BSEMPMS A (SELET DPT_NO,AVG(SAL) AS AVG_SAL FROM BSEMPMS B GROUP BY DPT_NO) V WHERE A.DPT_NO=V.DPT_NOAND A.SAL>V.AVG_SAL;
<20>. /*+NO_MERGE(TABLE)*/
对于有可合并的视图不再合并.
例如:
SELECT /*+NO_MERGE(V) */ A.EMP_NO,A.EMP_NAM,B.DPT_NO FROM BSEMPMS A (SELET DPT_NO,AVG(SAL) AS AVG_SAL FROM BSEMPMS B GROUP BY DPT_NO) V WHERE A.DPT_NO=V.DPT_NOAND A.SAL>V.AVG_SAL;
<21>. /*+ORDERED*/
根据表出现在FROM中的顺序,ORDERED使ORACLE依此顺序对其连接.
例如:
SELECT /*+ORDERED*/ A.COL1,B.COL2,C.COL3 FROM TABLE1 A,TABLE2 B,TABLE3 C
WHERE A.COL1=B.COL1 AND B.COL1=C.COL1;
<22>. /*+USE_NL(TABLE)*/
将指定表与嵌套的连接的行源进行连接,并把指定表作为内部表.
例如:
SELECT /*+ORDERED USE_NL(BSEMPMS)*/ BSDPTMS.DPT_NO,BSEMPMS.EMP_NO,BSEMPMS.EMP_NAM FROM BSEMPMS,BSDPTMS WHERE BSEMPMS.DPT_NO=BSDPTMS.DPT_NO;
<23>. /*+USE_MERGE(TABLE)*/
将指定的表与其他行源通过合并排序连接方式连接起来.
例如:
SELECT /*+USE_MERGE(BSEMPMS,BSDPTMS)*/ * FROM BSEMPMS,BSDPTMS WHERE
BSEMPMS.DPT_NO=BSDPTMS.DPT_NO;
<24>. /*+USE_HASH(TABLE)*/
将指定的表与其他行源通过哈希连接方式连接起来.
例如:
SELECT /*+USE_HASH(BSEMPMS,BSDPTMS)*/ * FROM BSEMPMS,BSDPTMS WHERE
BSEMPMS.DPT_NO=BSDPTMS.DPT_NO;
<25>. /*+DRIVING_SITE(TABLE)*/
强制与ORACLE所选择的位置不同的表进行查询执行.
例如:
SELECT /*+DRIVING_SITE(DEPT)*/ * FROM BSEMPMS,DEPT@BSDPTMS WHERE BSEMPMS.DPT_NO=DEPT.DPT_NO;
<26>. /*+LEADING(TABLE)*/
将指定的表作为连接次序中的首表.
<27>. /*+CACHE(TABLE)*/
当进行全表扫描时,CACHE提示能够将表的检索块放置在缓冲区缓存中最近最少列表LRU的最近使用端
例如:
SELECT /*+FULL(BSEMPMS) CAHE(BSEMPMS) */ EMP_NAM FROM  BSEMPMS;
<28>. /*+NOCACHE(TABLE)*/
当进行全表扫描时,CACHE提示能够将表的检索块放置在缓冲区缓存中最近最少列表LRU的最近使用端
例如:
SELECT /*+FULL(BSEMPMS) NOCAHE(BSEMPMS) */ EMP_NAM FROM  BSEMPMS;
<29>. /*+APPEND*/
直接插入到表的最后,可以提高速度.
insert /*+append*/ into test1  select * from test4 ;
insert /*+append */ into emp nologging
<30>. /*+NOAPPEND*/
通过在插入语句生存期内停止并行模式来启动常规插入.
insert /*+noappend*/ into test1  select * from test4 ;
<31>.parallel direct-load insert 
sql> alter session enable parallel dml; 
sql> insert /*+parallel(emp,2) */ into emp nologging 
sql> select * from emp_old; 
未完,待续!
转自:http://www.ixdba.net/article/30/370$2.html
