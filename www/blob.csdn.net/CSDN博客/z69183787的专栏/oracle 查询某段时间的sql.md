# oracle 查询某段时间的sql - z69183787的专栏 - CSDN博客
2014年01月28日 13:56:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1645
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
 where to_char(last_load_time) > '2014-01-28/13:39:00'
 order by last_load_time desc;
