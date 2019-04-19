# 如何查看某个查询用了多少TempDB空间 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [宋沄剑](http://www.jobbole.com/members/CareySon) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
最近帮助客户调优的过程中，发现客户的TempDB存在非常大的压力，经过排查是发现某些语句对TempDB的巨量使用所导致。
在SQL Server中，TempDB主要负责供下述三类情况使用：
- 内部使用（排序、hash join、work table等)
- 外部使用（临时表，表变量等）
- 行版本控制（乐观并发控制）
而对于内部使用，一些比较复杂的查询中由于涉及到了大量的并行、排序等操作时就需要大量的内存空间，每一个查询在开始时都会由SQL Server预估需要多少内存，在具体的执行过程中，如果授予的内存不足，则需要将多出来的部分由TempDB处理，这也就是所谓的Spill to TempDB。
通过下述语句可以观察到某个查询对TempDB造成了多少读写：

MySQL
```
DECLARE @read   BIGINT, 
        @write  BIGINT
;        
SELECT  @read = SUM(num_of_bytes_read), 
        @write = SUM(num_of_bytes_written) 
FROM    tempdb.sys.database_files AS DBF
JOIN    sys.dm_io_virtual_file_stats(2, NULL) AS FS
        ON FS.file_id = DBF.file_id
WHERE   DBF.type_desc = 'ROWS'
--这里放入需要测量的语句
SELECT  tempdb_read_MB = (SUM(num_of_bytes_read) - @read) / 1024. / 1024., 
        tempdb_write_MB = (SUM(num_of_bytes_written) - @write) / 1024. / 1024.,
        internal_use_MB = 
            (
            SELECT  internal_objects_alloc_page_count / 128.0
            FROM    sys.dm_db_task_space_usage
            WHERE   session_id = @@SPID
            )
FROM    tempdb.sys.database_files AS DBF
JOIN    sys.dm_io_virtual_file_stats(2, NULL) AS FS
        ON FS.file_id = DBF.file_id
WHERE   DBF.type_desc = 'ROWS'
```
最近在一个客户那里看到的烂查询所导致的TempDB使用结果如下:
![sqlconsume](http://jbcdn2.b0.upaiyun.com/2015/12/28c8edde3d61a0411511d3b1866f06366.png)
使用该查询就可以帮助了解某个语句使用了多少TempDB。
