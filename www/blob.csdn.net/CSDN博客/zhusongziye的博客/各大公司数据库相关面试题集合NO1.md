# 各大公司数据库相关面试题集合NO1 - zhusongziye的博客 - CSDN博客





2018年11月10日 22:14:14[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：110








******✎✎✎***第一小节>>***



*1.*你一定得利用备份恢复数据库，但是你没有控制文件，该如何解决问题呢?

解答：重建控制文件，用带backup control file 子句的recover命令恢复数据库。



*2**.*如何转换init.ora到spfile?

解答：使用create spfile from pfile 命令.



*3*.哪个column可以用来区别V$视图和GV$视图?

解答： INST_ID 指明集群环境中具体的 某个instance 。



*4*. 如何生成explain plan?

解答：运行utlxplan.sql. 建立plan 表,针对特定SQL语句，使用 explain plan set statement_id= ‘tst1′ into plan_table 运行utlxplp.sql 或 utlxpls.sql察看explain plan



*5*.如何增加buffer cache的命中率?

解答：在数据库较繁忙时，适用buffer cache advisory 工具，查询v$db_cache_advice . 如果有必要更改，可以使用 altersystem set db_cache_size 命令



*6*.ORA-01555的应对方法?

解答：具体的出错信息是snapshot too old within rollback seg , 通常可以通过

增大rollback seg来解决问题。当然也需要察看一下具体造成错误的SQL文本



*7*.解释$ORACLE_HOME和$ORACLE_BASE的区别?

解答：ORACLE_BASE是oracle的根目录，ORACLE_HOME是oracle产品的目录。



*8*.如何判断数据库的时区?

解答：SELECT DBTIMEZONE FROM DUAL;



*9*.解释GLOBAL_NAMES设为TRUE的用途

解答：GLOBAL_NAMES指明联接数据库的方式。如果这个参数设置为TRUE,在建立数据库链接时就必须用相同的名字连结远程数据库



*10.*如何加密PL/SQL程序?

解答：WRAP



***✎✎✎第二小节>>***



*11.*解释FUNCTION,PROCEDURE和PACKAGE区别

解答：function 和procedure是PL/SQL代码的集合，通常为了完成一个任务。procedure 不需要返回任何值而function将返回一个值在另一方面，Package是为了完成一个商业功能的一组function和proceudre的集合



*12.*解释TABLE Function的用途

解答：TABLE Function是通过PL/SQL逻辑返回一组纪录，用于普通的表/视图。他们也用于pipeline和ETL过程。



*13.*举出3种可以收集three advisory statistics

解答：Buffer Cache Advice, Segment Level Statistics, TimedStatistics



*14.*Audit trace 存放在哪个oracle目录结构中?

解答：unix $ORACLE_HOME/rdbms/audit Windows the event viewer



*15.*解释materialized views的作用

解答：Materialized views 用于减少那些汇总，集合和分组的信息的集合数量。它们通常适合于数据仓库和DSS系统。



*16.*当用户进程出错，哪个后台进程负责清理它

解答： PMON



*17.*哪个后台进程刷新materialized views?

解答：The Job Queue Processes.



*18.*如何判断哪个session正在连结以及它们等待的资源?

解答：V$SESSION / V$SESSION_WAIT



*19.*描述什么是 redo logs

解答：Redo Logs 是用于存放数据库数据改动状况的物理和逻辑结构。可以用来修复数据库.



*20.*如何进行强制LOG SWITCH?

解答：ALTER SYSTEM SWITCH LOGFILE;



