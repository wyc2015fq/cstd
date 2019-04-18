# Oracle查看表空间大小和 使用率 （DBA 给的） - z69183787的专栏 - CSDN博客
2014年03月14日 10:33:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1993
                
select  sysdate,c.tablespace_name,round(used_mb,1),round(total_mb,1),round(used_mb/total_mb,3)*100||'%' from
(select tablespace_name, sum(bytes)/1024/1024 used_mb from dba_segments group by tablespace_name) a,
(select tablespace_name,SUM(case autoextensible  when 'YES' THEN
              CASE WHEN maxbytes > bytes THEN maxbytes ELSE bytes END
              ELSE bytes END)/1024/1024 total_mb
from dba_data_files group by tablespace_name) b,
dba_tablespaces c
where c.tablespace_name = b.tablespace_name
and c.tablespace_name=a.tablespace_name(+)
and c.contents = 'PERMANENT'
and c.tablespace_name not like '%UNDO%'
order by used_mb/total_mb;
            
