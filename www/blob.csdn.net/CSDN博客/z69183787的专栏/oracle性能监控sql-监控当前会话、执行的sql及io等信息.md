# oracle性能监控sql-监控当前会话、执行的sql及io等信息 - z69183787的专栏 - CSDN博客
2014年01月28日 13:47:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8953
监控当前数据库谁在运行什么SQL语句
SELECT osuser, username, sql_text from v$session a, v$sqltext b
where a.sql_address =b.address order by address, piece;
　　分析表 
analyze table tablename compute statistics for all indexes;
analyze table tablename compute statistics for all indexed columns;
analyze table tablename compute statistics for table;
　　监控事例的等待 
select event,sum(decode(wait_Time,0,0,1)) "Prev",
sum(decode(wait_Time,0,1,0)) "Curr",count(*) "Tot"
from v$session_Wait
group by event order by 4;
　　查看碎片程度高的表 
SELECT segment_name table_name , COUNT(*) extents
FROM dba_segments WHERE owner NOT IN (SYS, SYSTEM) GROUP           BY segment_name
HAVING COUNT(*) = (SELECT MAX( COUNT(*) FROM dba_segments GROUP BY segment_name);
　　表、索引的存储情况检查 
select segment_name,sum(bytes),count(*) ext_quan from dba_extents where
tablespace_name=&tablespace_name and segment_type=TABLE group by tablespace_name,segment_name;
select segment_name,count(*) from dba_extents where segment_type=INDEX and owner=&owner
group by segment_name;
　　找使用CPU多的用户session
12是cpu used by this session
select a.sid,spid,status,substr(a.program,1,40)   prog,a.terminal,osuser,value/60/100 value
from v$session a,v$process b,v$sesstat c
where c.statistic#=12 and c.sid=a.sid and a.paddr=b.addr order by value desc;
　　监控表空间的 I/O 比例
select df.tablespace_name name,df.file_name "file",f.phyrds pyr,
f.phyblkrd pbr,f.phywrts pyw, f.phyblkwrt pbw
from v$filestat f, dba_data_files df
where f.file# = df.file_id
order by df.tablespace_name;
　　回滚段的争用情况 
select name, waits, gets, waits/gets "Ratio"
from v$rollstat a, v$rollname b
where a.usn = b.usn;
　　在某个用户下找所有的索引 
select user_indexes.table_name, user_indexes.index_name,uniqueness, column_name
from user_ind_columns, user_indexes
where user_ind_columns.index_name = user_indexes.index_name
and user_ind_columns.table_name = user_indexes.table_name
order by user_indexes.table_type, user_indexes.table_name,
user_indexes.index_name, column_position;
　　监控文件系统的 I/O 比例
select substr(a.file#,1,2) "#", substr(a.name,1,30) "Name",
a.status, a.bytes, b.phyrds, b.phywrts
from v$datafile a, v$filestat b
where a.file# = b.file#;
　　监控 SGA 中字典缓冲区的命中率
select parameter, gets,Getmisses , getmisses/(gets+getmisses)*100 "miss ratio",
(1-(sum(getmisses)/ (sum(gets)+sum(getmisses))))*100 "Hit ratio"
from v$rowcache
where gets+getmisses <>0
group by parameter, gets, getmisses;
　　监控 SGA 中共享缓存区的命中率，应该小于1％
select sum(pins) "Total Pins", sum(reloads) "Total Reloads",
sum(reloads)/sum(pins) *100 libcache
from v$librarycache;
select sum(pinhits-reloads)/sum(pins) "hit radio",sum(reloads)/sum(pins) "reload percent" 
from v$librarycache;
　　监控 SGA 的命中率
select a.value + b.value "logical_reads", c.value "phys_reads",
round(100 * ((a.value+b.value)-c.value) / (a.value+b.value)) "BUFFER HIT RATIO"
from v$sysstat a, v$sysstat b, v$sysstat c
where a.statistic# = 38 and b.statistic# = 39
and c.statistic# = 40;
　　监控 SGA 中重做日志缓存区的命中率，应该小于1％
SELECT name, gets, misses, immediate_gets, immediate_misses,
Decode(gets,0,0,misses/gets*100) ratio1,
Decode(immediate_gets+immediate_misses,0,0,
immediate_misses/(immediate_gets+immediate_misses)*100) ratio2
FROM v$latch WHERE name IN (redo allocation, redo copy);
　　显示所有数据库对象的类别和大小 
select count(name) num_instances ,type ,sum(source_size) source_size ,
sum(parsed_size) parsed_size ,sum(code_size) code_size ,sum(error_size) error_size,
sum(source_size) +sum(parsed_size) +sum(code_size) +sum(error_size) size_required
from dba_object_size
group by type order by 2;
　　监控内存和硬盘的排序比率，最好使它小于 .10，增加 sort_area_size
SELECT name, value FROM v$sysstat WHERE name IN (sorts (memory), sorts (disk));
　　监控字典缓冲区 
SELECT (SUM(PINS - RELOADS)) / SUM(PINS) "LIB CACHE" FROM V$LIBRARYCACHE;
SELECT (SUM(GETS - GETMISSES - USAGE - FIXED)) / SUM(GETS) "ROW CACHE" FROM V$ROWCACHE;
SELECT SUM(PINS) "EXECUTIONS", SUM(RELOADS) "CACHE MISSES WHILE EXECUTING" FROM V$LIBRARYCACHE;
　　后者除以前者,此比率小于1％,接近0％为好。
SELECT SUM(GETS) "DICTIONARY GETS",SUM(GETMISSES) "DICTIONARY CACHE GET MISSES"
FROM V$ROWCACHE
　　监控 MTS
select busy/(busy+idle) "shared servers busy" from v$dispatcher;
　　此值大于0.5时，参数需加大
select sum(wait)/sum(totalq) "dispatcher waits" from v$queue where type=dispatcher;
select count(*) from v$dispatcher;
select servers_highwater from v$mts;
servers_highwater接近mts_max_servers时，参数需加大
　　碎片程度 
select tablespace_name,count(tablespace_name) from dba_free_space group by tablespace_name
having count(tablespace_name)>10;
alter tablespace name coalesce;
alter table name deallocate unused;
create or replace view ts_blocks_v as
select tablespace_name,block_id,bytes,blocks,free space segment_name from dba_free_space
union all
select tablespace_name,block_id,bytes,blocks,segment_name from dba_extents;
select * from ts_blocks_v;
select tablespace_name,sum(bytes),max(bytes),count(block_id) from dba_free_space
group by tablespace_name;
四、SQL语句执行效率问题
1．检查占用CPU时间比较长的sql语句:
select sql_text,cpu_time from v$sql where cpu_time >1e7 order by cpu_time
2．执行效率最差的10条sql语句
SELECT * FROM ( SELECT PARSING_USER_ID EXECUTIONS SORTS,
COMMAND_TYPE, DISK_READS, sql_text FROM v$sqlarea
ORDER BY disk_reads DESC) WHERE ROWNUM<10 ;
3．利用V_$SQLAREA视图提供了执行的细节。（执行、读取磁盘和读取缓冲区的次数）
s select SQL_TEXT,EXECUTIONS,DISK_READS,COMMAND_TYPE,OPTIMIZER_MODE, SHARABLE_MEM,BUFFER_GETS from v$sqlarea
•
　　数 数据列 
EXECUTIONS：执行次数
DISK_READS：读盘次数
COMMAND_TYPE：命令类型（3:select,2:insert;6:update;7delete;47:pl/sql程序单元）
OPTIMIZER_MODE：优化方式
SQL_TEXT：Sql语句
SHARABLE_MEM：占用shared pool的内存多少
BUFFER_GETS：读取缓冲区的次数
• 用途
1、帮忙找出性能较差的SQL语句
2、帮忙找出最高频率的SQL
3、帮忙分析是否需要索引或改善联接
4. 监控当前Oracle的session，如出现时钟的标志，表示此进程中的sql运行时间较长。
oracle SQL语句执行的监控脚本!
--查找bad sql的方法: 
select * from (select buffer_gets, sql_text 
from v$sqlarea 
where buffer_gets >500000 
order by buffer_gets desc) where rownum<=30;
--執行次數SQL
select sql_text,executions from 
(select sql_text,executions from v$sqlarea order by executions desc) 
where rownum<81;
--讀磁盤多的SQL
select sql_text,disk_reads from 
(select sql_text,disk_reads from v$sqlarea order by disk_reads desc) 
where rownum<21;
--排序多的SQL
select sql_text,sorts from 
(select sql_text,sorts from v$sqlarea order by sorts desc) 
where rownum<21;
--分析次數太多，執行次數太少，要用綁定綁定變量寫sql
set pagesize 600; 
set linesize 120; 
select substr(sql_text,1,80) "sql", count(*), sum(executions) "totexecs" 
from v$sqlarea 
where executions < 5 
group by substr(sql_text,1,80) 
having count(*) > 30 
order by 2;
--遊標的觀察
set pages 300; 
select sum(a.value), b.name 
from v$sesstat a, v$statname b 
where a.statistic# = b.statistic# 
and b.name = 'opened cursors current' 
group by b.name; 
select count(0) from v$open_cursor; 
select user_name,sql_text,count(0) from v$open_cursor 
group by user_name,sql_text having count(0)>30;
--查看當前用戶執行的SQL
select sql_text from v$sqltext_with_newlines where (hash_value,address) in 
(select sql_hash_value,sql_address from v$session where username='&username') 
order by address,piece;
