# oracle优化-----监控指标 - weixin_33985507的博客 - CSDN博客
2010年08月29日 16:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
oracle优化-----监控指标
author：skate
在想优化数据库前，首先要确认数据库是否需要优化，这就需要一些监控指标了，如，事务响应时间，数据库的逻辑读，数据库的物理读/物理写等。日常监控这些指标，从这些数值的波动可以观察数据库的负载，再配合os的统计信息
如下sql运行两次，取其差值，就是系统的总的逻辑读
 select * from v$sysstat s where s.NAME like 'consistent gets'
如下sql运行两次，取其差值，就是系统的总的物理读（应用的，不是实例本身的）
 select * from v$sysstat s where s.NAME like 'physical reads'
如下sql运行两次，取其差值，就是系统的总的物理写
 select * from v$sysstat s where s.NAME like 'physical writes'
和物理写有关的统计（10g和11g）
SQL>  select name,value from v$sysstat s where s.NAME like 'physical write%';
NAME                                                                  VALUE
---------------------------------------------------------------- ----------
physical write total IO requests                                    6585571
physical write total multi block requests                            443341
physical write total bytes                                       8686156185
physical writes                                                     1144890
physical writes direct                                                22960
physical writes from cache                                          1121930
physical write IO requests                                           726044
physical write bytes                                             9378938880
physical writes non checkpoint                                       229713
physical writes direct (lob)                                            722
physical writes direct temporary tablespace                           15727
11 rows selected
和物理写有关的统计（9i）
SQL> select name,value from v$sysstat s where s.NAME like 'physical write%';
NAME                                                                  VALUE
---------------------------------------------------------------- ----------
physical writes                                                      338284
physical writes non checkpoint                                       224500
physical writes direct                                               124584
physical writes direct (lob)                                              0
SQL>
和物理读有关的统计（10g和11g）
SQL>  select name,value from v$sysstat s where s.NAME like 'physical read%';
NAME                                                                  VALUE
---------------------------------------------------------------- ----------
physical read total IO requests                                     1333640
physical read total multi block requests                             206363
physical read total bytes                                        3652049715
physical reads                                                      2357552
physical reads cache                                                2353036
physical reads direct                                                  4515
physical read IO requests                                            262704
physical read bytes                                              1931306598
physical reads cache prefetch                                       2094848
physical reads prefetch warmup                                         9008
physical reads retry corrupt                                              0
physical reads direct (lob)                                               0
physical reads direct temporary tablespace                                6
physical reads for flashback new                                          0
14 rows selected
SQL>
和物理读有关的统计（9i）
SQL> select name,value from v$sysstat s where s.NAME like 'physical read%';
NAME                                                                  VALUE
---------------------------------------------------------------- ----------
physical reads                                                       163790
physical reads direct                                                124164
physical reads direct (lob)                                               0
SQL>
和逻辑读有关的统计（10g和11g）
SQL>  select name,value  from v$sysstat s where s.NAME like 'consistent%';
NAME                                                                  VALUE
---------------------------------------------------------------- ----------
consistent gets                                                  1131603028
consistent gets from cache                                       1131603021
consistent gets - examination                                     278726379
consistent gets direct                                                  701
consistent changes                                                  6655643
和逻辑读有关的统计（9i）
SQL> select name,value  from v$sysstat s where s.NAME like 'consistent%';
NAME                                                                  VALUE
---------------------------------------------------------------- ----------
consistent gets                                                   504873168
consistent changes                                                  5300217
consistent gets - examination                                     203110671
SQL>
consistent gets：数据请求总数在回滚段Buffer中
consistent gets from cache：直接从buffer读consistent数据
consistent gets direct：direct load operation（直接从回滚段中装载数据到pga中）
physical reads cache prefetch：instance reads，包括backup and recovery, and other utilities  是 non-application workload
physical reads prefetch warmup：预先随机从disk读的数据
consistent gets - examination：这个是干嘛的不清楚，谁知道请告诉下
v$sysstat统计信息项的描述
[http://youngcow.net/doc/oracle10g/server.102/b14237/stats002.htm](http://youngcow.net/doc/oracle10g/server.102/b14237/stats002.htm)
oracle10（over）常用的优化参数，9i没有v$sys_optimizer_env
SQL>  select name,isdefault,value,default_value from v$sys_optimizer_env;
NAME                                     ISDEFAULT VALUE                     DEFAULT_VALUE
---------------------------------------- --------- ------------------------- -------------------------
parallel_execution_enabled               YES       true                      true
optimizer_features_enable                YES       10.2.0.1                  10.2.0.1
cpu_count                                YES       2                         2
active_instance_count                    YES       1                         1
parallel_threads_per_cpu                 YES       2                         2
hash_area_size                           YES       131072                    131072
bitmap_merge_area_size                   YES       1048576                   1048576
sort_area_size                           YES       65536                     65536
sort_area_retained_size                  YES       0                         0
pga_aggregate_target                     YES       394240 KB                 394240 KB
parallel_query_mode                      YES       enabled                   enabled
parallel_dml_mode                        YES       disabled                  disabled
parallel_ddl_mode                        YES       enabled                   enabled
optimizer_mode                           YES       all_rows                  all_rows
cursor_sharing                           NO        similar                   exact
star_transformation_enabled              YES       false                     false
optimizer_index_cost_adj                 YES       100                       100
optimizer_index_caching                  YES       0                         0
query_rewrite_enabled                    YES       true                      true
query_rewrite_integrity                  YES       enforced                  enforced
workarea_size_policy                     YES       auto                      auto
optimizer_dynamic_sampling               YES       2                         2
statistics_level                         YES       typical                   typical
skip_unusable_indexes                    YES       true                      true
optimizer_secure_view_merging            YES       true                      true
25 rows selected
SQL>
数据库有哪些表和视图（v$fixed_table，v_$fixed_table，gv$fixed_table，gv_$fixed_table）
SQL> select count(1) from v_$fixed_table;
  COUNT(1)
----------
      1383
查看数据库视图的定义（字面含义就是固定视图的定义）
SQL> select * from v_$fixed_view_definition where view_name = upper('gv$sys_optimizer_env');
VIEW_NAME                      VIEW_DEFINITION
------------------------------ --------------------------------------------------------------------------------
GV$SYS_OPTIMIZER_ENV           select INST_ID,                        PNUM_QKSCESYROW,                PNAME_QKS
从上面可以查到，gv$sys_optimizer_env是在基表X$QKSCESYS上构建的，而oracle9i没有这个基表
小记：
系统缓存游标的使用情况
SQL> select * from v$system_cursor_cache;
     OPENS       HITS  HIT_RATIO
---------- ---------- ----------
  26259588   26259588          1
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wyzxg/archive/2010/03/24/5413079.aspx](http://blog.csdn.net/wyzxg/archive/2010/03/24/5413079.aspx)
