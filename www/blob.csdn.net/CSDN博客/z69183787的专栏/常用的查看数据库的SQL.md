# 常用的查看数据库的SQL - z69183787的专栏 - CSDN博客
2014年01月28日 13:18:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：681
1、查看表空间的名称及大小 
　　select t.tablespace_name, round(sum(bytes/(1024*1024)),0) ts_size 
　　from dba_tablespaces t, dba_data_files d 
　　where t.tablespace_name = d.tablespace_name 
　　group by t.tablespace_name; 
2、查看表空间物理文件的名称及大小 
　　select tablespace_name, file_id, file_name, 
　　round(bytes/(1024*1024),0) total_space 
　　from dba_data_files 
　　order by tablespace_name; 
3、查看回滚段名称及大小 
　　select segment_name, tablespace_name, r.status, 
　　(initial_extent/1024) InitialExtent,(next_extent/1024) NextExtent, 
　　max_extents, v.curext CurExtent 
　　From dba_rollback_segs r, v$rollstat v 
　　Where r.segment_id = v.usn(+) 
　　order by segment_name ; 
4、查看控制文件 
　　select name from v$controlfile; 
5、查看日志文件 
　　select member from v$logfile; 
6、查看表空间的使用情况 
　　select sum(bytes)/(1024*1024) as free_space,tablespace_name 
　　from dba_free_space 
　　group by tablespace_name; 
　　SELECT A.TABLESPACE_NAME,A.BYTES TOTAL,B.BYTES USED, C.BYTES FREE, 
　　(B.BYTES*100)/A.BYTES "% USED",(C.BYTES*100)/A.BYTES "% FREE" 
　　FROM SYS.SM$TS_AVAIL A,SYS.SM$TS_USED B,SYS.SM$TS_FREE C 
　　WHERE A.TABLESPACE_NAME=B.TABLESPACE_NAME AND A.TABLESPACE_NAME=C.TABLESPACE_NAME; 
7、查看数据库库对象 
　　select owner, object_type, status, count(*) count# from all_objects group by owner, object_type, status; 
8、查看数据库的版本　 
　　Select version FROM Product_component_version 
　　Where SUBSTR(PRODUCT,1,6)='[Oracle](http://oracle.chinaitlab.com/)'; 
9、查看数据库的创建日期和归档方式 
　　Select Created, Log_Mode, Log_Mode From V$Database;
