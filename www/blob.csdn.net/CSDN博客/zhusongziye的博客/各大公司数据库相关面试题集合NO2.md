# 各大公司数据库相关面试题集合NO2 - zhusongziye的博客 - CSDN博客





2018年11月10日 22:16:23[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：56








******✎✎✎***第一小节>>***



*1. *举出两个判断DDL改动的方法?

解答：你可以使用 Logminer 或 Streams



*2.*Coalescing做了什么?

解答：Coalescing针对于字典管理的tablespace进行碎片整理，将临近的小extents合并成单个的大extent.



*3.*解释冷备份和热备份的不同点以及各自的优点

解答：热备份针对归档模式的数据库，在数据库仍旧处于工作状态时进行备份。而冷备份指在数据库关闭后，进行备份，适用于所有模式的数据库。热备份的 优点在于当备份时，数据库仍旧可以被使用并且可以将数据库恢复到任意一个时间点。冷备份的优点在于它的备份和恢复操作相当简单，并且由于冷备份的数据库可以工作在非归档模式下,数据库性能会比归档模式稍好。(因为不必将archive log写入硬盘)*4. *解释data block , extent 和 segment的区别(这里建议用英文术语)

解答：data block是数据库中最小的逻辑存储单元。当数据库的对象需要更多的物理存储空间时，连续的data block就组成了extent . 一个数据库对象 拥有的所有extents被称为该对象的segment.



*5. *给出两个检查表结构的方法

解答：1. DESCRIBE命令

           2. DBMS_METADATA.GET_DDL 包



*6.* 怎样查看数据库引擎的报错

解答：alert log.



*7. *比较truncate和delete 命令

解答：两者都可以用来删除表中所有的记录。区别在于：truncate是DDL操作，它移动HWK，不需要 rollback segment .而Delete是DML操作, 需要rollback segment 且花费较长时间.



*8. *使用索引的理由

解答：快速访问表中的data block



*9. *给出在STAR SCHEMA中的两种表及它们分别含有的数据

解答：Fact tables 和dimension tables.fact table 包含大量的主要的信息而 dimension tables 存放对fact table 某些属性描述的信息



*10. *FACT Table上需要建立何种索引?

解答：位图索引 (bitmap index)



******✎✎✎***第二小节>>***



*11. *给出两种相关约束?

解答：主键和外键



*12. *如何在不影响子表的前提下，重建一个母表

解答：子表的外键强制实效，重建母表，激活外键



*13. *解释归档和非归档模式之间的不同和它们各自的优缺点

解答：归档模式是指你可以备份所有的数据库 transactions并恢复到任意一个时间点。非归档模式则相反，不能恢复到任意一个时间点。但是非归档模式可以带来数据库性能上的少许提高.



*14. *如何建立一个备份控制文件?

解答：Alter database backup control file to trace.



*15. *给出数据库正常启动所经历的几种状态 ?

解答：

STARTUP NOMOUNT – 数据库实例启动

STARTUP MOUNT – 数据库装载

STARTUP OPEN – 数据库打开



*16. *创建用户时，需要赋予新用户什么权限才能使它联上数据库。

解答：CONNECT



*17. *如何在tablespace里增加数据文件?

解答：ALTER TABLESPACE ADD DATAFILE SIZE



*18.*如何变动数据文件的大小?

解答：ALTER DATABASE DATAFILE RESIZE ;



*19.*哪个VIEW用来检查数据文件的大小?

解答： DBA_DATA_FILES



*20.*哪个VIEW用来判断tablespace的剩余空间

解答：DBA_FREE_SPACE



*21.*如何判断谁往表里增加了一条纪录?

解答：auditing



*22.*如何重构索引?

解答： ALTER INDEX REBUILD;



*23.*解释什么是Partitioning(分区)以及它的优点。

解答：Partition将大表和索引分割成更小，易于管理的分区。

![](https://img-blog.csdnimg.cn/2018111022145428.gif)

*24. *你刚刚编译了一个PL/SQL Package但是有错误报道，如何显示出错信息?

解答：SHOW ERRORS



*25.*如何搜集表的各种状态数据?

解答： ANALYZE

The ANALYZE command.



*26. *如何启动SESSION级别的TRACE

解答: DBMS_SESSION.SET_SQL_TRACE

ALTER SESSION SET SQL_TRACE = TRUE;



27. IMPORT和SQL*LOADER 这2个工具的不同点

解答：这两个ORACLE工具都是用来将数据导入数据库的。

区别是：IMPORT工具只能处理由另一个ORACLE工具EXPORT生成的数据。而SQL*LOADER可以导入不同的ASCII格式的数据源



*28.*用于网络连接的2个文件?

解答： TNSNAMES.ORA and SQLNET.ORA



