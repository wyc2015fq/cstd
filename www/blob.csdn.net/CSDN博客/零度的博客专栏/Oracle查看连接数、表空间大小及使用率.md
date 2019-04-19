# Oracle查看连接数、表空间大小及使用率 - 零度的博客专栏 - CSDN博客
2016年04月28日 13:41:38[零度anngle](https://me.csdn.net/zmx729618)阅读数：615
       目前总结的语句，在查看数据的连接情况很有用，写完程序一边测试代码一边查看数据库连接的释放情况有助于分析优化出一个健壮的系统程序来。
1.当前的数据库连接数
select count(*) from v$process where program=‘ORACLE.EXE(SHAD)’; --当前的数据库连接数
2.数据库允许的最大连接数
 select value from v$parameter where name ='processes'--数据库允许的最大连接数
3.修改最大连接数
alter system set processes = 300 scope = spfile;--修改最大连接数
4.重启数据库
shutdown immediate;
startup;--重启数据库
5.查看当前有哪些用户正在使用数据
SELECT osuser, a.username,cpu_time/executions/1000000||'s',b.sql_text,machine
from v$session a, v$sqlarea b
where a.sql_address =b.address order by cpu_time/executions desc;
--查看当前有哪些用户正在使用数据
6.当前的session连接数
 select count(*) from v$session  --当前的session连接数
7.并发连接数
 select count(*) from v$session where status='ACTIVE'　--并发连接数
8.最大连接
 show parameter processes  --最大连接
9.查看表空间大小及使用率
select  
         sysdate,c.tablespace_name,round(used_mb,1),round(total_mb,1),round(used_mb/total_mb,3)*100||'%'
from
         (select tablespace_name, sum(bytes)/1024/1024 used_mb from dba_segments group by tablespace_name) a,
         (select tablespace_name,SUM(case autoextensible  when 'YES' THEN
              CASE WHEN maxbytes > bytes THEN maxbytes ELSE bytes END
              ELSE bytes END)/1024/1024 total_mb
         from dba_data_files group by tablespace_name) b,
         dba_tablespaces c
where
         c.tablespace_name = b.tablespace_name
         and c.tablespace_name=a.tablespace_name(+)
         and c.contents = 'PERMANENT'
         and c.tablespace_name not like '%UNDO%'
order by used_mb/total_mb;
