# Oracle数据库性能监控常用Sql - z69183787的专栏 - CSDN博客
2014年01月28日 14:03:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3740
监控sga内存分配信息
`select * from v$sgainfo;`
监控每个用户的磁盘io及io命中率
```
select v$sess_io.*,(block_gets+consistent_gets) reads,
((block_gets+consistent_gets-physical_reads)/(block_gets+consistent_gets)) ratio
from v$sess_io
where (block_gets+consistent_gets)>=1000
order by (block_gets+consistent_gets) desc;
```
查询最近执行的sql语句
```
select last_load_time,
disk_reads,
sorts,
fetches,
buffer_gets,
optimizer_cost,
cpu_time,
sql_fulltext,
sql_text
from v$sql
where to_char(last_load_time)> '2011-05-10/14:00:00' 
order by last_load_time desc;
```
查询会话信息和会话io
```
select a.sid,
block_gets,
consistent_gets,
physical_reads,
block_changes,
consistent_changes,
b.serial#,
b.username,
b.command,
b.server,
b.machine,
b.terminal,
b.program
from v$sess_io a,
v$session b
where a.sid=b.sid
order by 5 desc;
```
查询当前正在执行的sql及io信息
```
select a.sql_text,
a.sql_fulltext,
a.cpu_time,
b.sid,
b.serial#,
b.username,
b.machine,
b.terminal,
b.program,
c.block_gets,
c.consistent_gets,
c.physical_reads,
c.block_changes,
c.consistent_changes
from v$sql a,
v$session b,
v$sess_io c
where a.address=b.sql_address
and b.sid=c.sid
order by c.block_changes desc;
```
查询刚执行过的sql和io信息
```
select a.sql_text,
a.sql_fulltext,
a.cpu_time,
b.sid,
b.serial#,
b.username,
b.machine,
b.terminal,
b.program,
c.block_gets,
c.consistent_gets,
c.physical_reads,
c.block_changes,
c.consistent_changes
from v$sql a,
v$session b,
v$sess_io c
where a.address=b.prev_sql_addr
and b.sid=c.sid
order by c.block_changes desc;
```
查询表空间每个文件的空闲空间
```
select a.tablespace_name,b.file_name,sum(a.bytes/1024/1024) free_mb,b.bytes/1024/1024
size_mb,b.autoextensible,b.maxbytes/1024/1024 max_mb
from dba_free_space a,dba_data_files b where a.file_id=b.file_id
group by a.tablespace_name,b.file_name,b.bytes,b.autoextensible,b.maxbytes order by
a.tablespace_name,b.file_name;
```
