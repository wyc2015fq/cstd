# hive中的set hive.exec.parallel=true 参数说明 - 数据之美的博客 - CSDN博客
2018年03月07日 11:15:52[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1579
个人分类：[hive/sql](https://blog.csdn.net/oppo62258801/article/category/6961227)
hive.exec.parallel参数控制在同一个sql中的不同的job是否可以同时运行,默认为false.
下面是对于该参数的测试过程:
测试sql:
select r1.a
from (select t.a from sunwg_10 t join sunwg_10000000 s on t.a=s.b) r1 join (select s.b from sunwg_100000 t join sunwg_10 s on t.a=s.b) r2 on (r1.a=r2.b);
1
Set hive.exec.parallel=false;
当参数为false的时候,三个job是顺序的执行
**[html]**[view plain](http://blog.csdn.net/jiedushi/article/details/7965604#)[copy](http://blog.csdn.net/jiedushi/article/details/7965604#)
- hive> set hive.exec.parallel=false;  
- hive> select r1.a  
- > from (select t.a from sunwg_10 t join sunwg_10000000 s on t.a=s.b) r1 join (select s.b from sunwg_100000 t join sunwg_10 s on t.a=s.b) r2 on (r1.a=r2.b);  
- Total MapReduce jobs = 3
- Launching Job 1 out of 3  
- Number of reduce tasks not specified. Estimated from input data size: 1  
- In order to change the average load for a reducer (in bytes):  
-   set hive.exec.reducers.bytes.per.reducer=<number>
- In order to limit the maximum number of reducers:  
-   set hive.exec.reducers.max=<number>
- In order to set a constant number of reducers:  
-   set mapred.reduce.tasks=<number>
- Cannot run job locally: Input Size (= 397778060) is larger than hive.exec.mode.local.auto.inputbytes.max (= -1)  
- Starting Job = job_201208241319_2001905, Tracking URL = http://hdpjt:50030/jobdetails.jsp?jobid=job_201208241319_2001905
- Kill Command = /dhwdata/hadoop/bin/../bin/hadoop job  -Dmapred.job.tracker=hdpjt:9001 -kill job_201208241319_2001905  
- Hadoop job information for Stage-1: number of mappers: 7; number of reducers: 1  
- 2012-09-07 17:55:40,854 Stage-1 map = 0%,  reduce = 0%  
- 2012-09-07 17:55:55,663 Stage-1 map = 14%,  reduce = 0%  
- 2012-09-07 17:56:00,506 Stage-1 map = 56%,  reduce = 0%  
- 2012-09-07 17:56:10,254 Stage-1 map = 100%,  reduce = 0%  
- 2012-09-07 17:56:19,871 Stage-1 map = 100%,  reduce = 29%  
- 2012-09-07 17:56:30,000 Stage-1 map = 100%,  reduce = 75%  
- 2012-09-07 17:56:34,799 Stage-1 map = 100%,  reduce = 100%  
- Ended Job = job_201208241319_2001905
- Launching Job 2 out of 3  
- Number of reduce tasks not specified. Estimated from input data size: 1  
- In order to change the average load for a reducer (in bytes):  
-   set hive.exec.reducers.bytes.per.reducer=<number>
- In order to limit the maximum number of reducers:  
-   set hive.exec.reducers.max=<number>
- In order to set a constant number of reducers:  
-   set mapred.reduce.tasks=<number>
- Cannot run job locally: Input Size (= 3578060) is larger than hive.exec.mode.local.auto.inputbytes.max (= -1)  
- Starting Job = job_201208241319_2002054, Tracking URL = http://hdpjt:50030/jobdetails.jsp?jobid=job_201208241319_2002054
- Kill Command = /dhwdata/hadoop/bin/../bin/hadoop job  -Dmapred.job.tracker=hdpjt:9001 -kill job_201208241319_2002054  
- Hadoop job information for Stage-4: number of mappers: 2; number of reducers: 1  
- 2012-09-07 17:56:43,343 Stage-4 map = 0%,  reduce = 0%  
- 2012-09-07 17:56:48,124 Stage-4 map = 50%,  reduce = 0%  
- 2012-09-07 17:56:55,816 Stage-4 map = 100%,  reduce = 0%  
- Ended Job = job_201208241319_2002054
- Launching Job 3 out of 3  
- Number of reduce tasks not specified. Estimated from input data size: 1  
- In order to change the average load for a reducer (in bytes):  
-   set hive.exec.reducers.bytes.per.reducer=<number>
- In order to limit the maximum number of reducers:  
-   set hive.exec.reducers.max=<number>
- In order to set a constant number of reducers:  
-   set mapred.reduce.tasks=<number>
- Cannot run job locally: Input Size (= 596) is larger than hive.exec.mode.local.auto.inputbytes.max (= -1)  
- Starting Job = job_201208241319_2002120, Tracking URL = http://hdpjt:50030/jobdetails.jsp?jobid=job_201208241319_2002120
- Kill Command = /dhwdata/hadoop/bin/../bin/hadoop job  -Dmapred.job.tracker=hdpjt:9001 -kill job_201208241319_2002120  
- Hadoop job information for Stage-2: number of mappers: 2; number of reducers: 1  
- 2012-09-07 17:57:12,641 Stage-2 map = 0%,  reduce = 0%  
- 2012-09-07 17:57:19,571 Stage-2 map = 50%,  reduce = 0%  
- 2012-09-07 17:57:25,199 Stage-2 map = 100%,  reduce = 0%  
- 2012-09-07 17:57:29,210 Stage-2 map = 100%,  reduce = 100%  
- Ended Job = job_201208241319_2002120
- OK  
- abcdefghijk_0  
- abcdefghijk_1  
- abcdefghijk_2  
- abcdefghijk_3  
- abcdefghijk_4  
- abcdefghijk_5  
- abcdefghijk_6  
- abcdefghijk_7  
- abcdefghijk_8  
- abcdefghijk_9  
- Time taken: 135.944 seconds  
2
但是可以看出来其实两个子查询中的sql并无关系,可以并行的跑
**[html]**[view plain](http://blog.csdn.net/jiedushi/article/details/7965604#)[copy](http://blog.csdn.net/jiedushi/article/details/7965604#)
- hive> set hive.exec.parallel=true;  
- hive> select r1.a  
- > from (select t.a from sunwg_10 t join sunwg_10000000 s on t.a=s.b) r1 join (select s.b from sunwg_100000 t join sunwg_10 s on t.a=s.b) r2 on (r1.a=r2.b);  
- Total MapReduce jobs = 3
- Launching Job 1 out of 3  
- Launching Job 2 out of 3  
- Number of reduce tasks not specified. Estimated from input data size: 1  
- In order to change the average load for a reducer (in bytes):  
-   set hive.exec.reducers.bytes.per.reducer=<number>
- In order to limit the maximum number of reducers:  
-   set hive.exec.reducers.max=<number>
- In order to set a constant number of reducers:  
-   set mapred.reduce.tasks=<number>
- Cannot run job locally: Input Size (= 397778060) is larger than hive.exec.mode.local.auto.inputbytes.max (= -1)  
- Number of reduce tasks not specified. Estimated from input data size: 1  
- In order to change the average load for a reducer (in bytes):  
-   set hive.exec.reducers.bytes.per.reducer=<number>
- In order to limit the maximum number of reducers:  
-   set hive.exec.reducers.max=<number>
- In order to set a constant number of reducers:  
-   set mapred.reduce.tasks=<number>
- Cannot run job locally: Input Size (= 3578060) is larger than hive.exec.mode.local.auto.inputbytes.max (= -1)  
- Starting Job = job_201208241319_2001452, Tracking URL = http://hdpjt:50030/jobdetails.jsp?jobid=job_201208241319_2001452
- Kill Command = /dhwdata/hadoop/bin/../bin/hadoop job  -Dmapred.job.tracker=hdpjt:9001 -kill job_201208241319_2001452  
- Starting Job = job_201208241319_2001453, Tracking URL = http://hdpjt:50030/jobdetails.jsp?jobid=job_201208241319_2001453
- Kill Command = /dhwdata/hadoop/bin/../bin/hadoop job  -Dmapred.job.tracker=hdpjt:9001 -kill job_201208241319_2001453  
- Hadoop job information for Stage-4: number of mappers: 2; number of reducers: 1  
- Hadoop job information for Stage-1: number of mappers: 7; number of reducers: 1  
- 2012-09-07 17:52:10,558 Stage-4 map = 0%,  reduce = 0%  
- 2012-09-07 17:52:10,588 Stage-1 map = 0%,  reduce = 0%  
- 2012-09-07 17:52:22,827 Stage-1 map = 14%,  reduce = 0%  
- 2012-09-07 17:52:22,880 Stage-4 map = 100%,  reduce = 0%  
- 2012-09-07 17:52:27,678 Stage-1 map = 22%,  reduce = 0%  
- 2012-09-07 17:52:28,701 Stage-1 map = 36%,  reduce = 0%  
- 2012-09-07 17:52:31,137 Stage-1 map = 93%,  reduce = 0%  
- 2012-09-07 17:52:33,551 Stage-1 map = 100%,  reduce = 0%  
- 2012-09-07 17:52:36,427 Stage-4 map = 100%,  reduce = 100%  
- Ended Job = job_201208241319_2001453
- 2012-09-07 17:52:42,883 Stage-1 map = 100%,  reduce = 33%  
- 2012-09-07 17:52:45,431 Stage-1 map = 100%,  reduce = 70%  
- 2012-09-07 17:52:47,526 Stage-1 map = 100%,  reduce = 76%  
- 2012-09-07 17:52:51,829 Stage-1 map = 100%,  reduce = 84%  
- Ended Job = job_201208241319_2001452
- Launching Job 3 out of 3  
- Number of reduce tasks not specified. Estimated from input data size: 1  
- In order to change the average load for a reducer (in bytes):  
-   set hive.exec.reducers.bytes.per.reducer=<number>
- In order to limit the maximum number of reducers:  
-   set hive.exec.reducers.max=<number>
- In order to set a constant number of reducers:  
-   set mapred.reduce.tasks=<number>
- Cannot run job locally: Input Size (= 596) is larger than hive.exec.mode.local.auto.inputbytes.max (= -1)  
- Starting Job = job_201208241319_2001621, Tracking URL = http://hdpjt:50030/jobdetails.jsp?jobid=job_201208241319_2001621
- Kill Command = /dhwdata/hadoop/bin/../bin/hadoop job  -Dmapred.job.tracker=hdpjt:9001 -kill job_201208241319_2001621  
- Hadoop job information for Stage-2: number of mappers: 2; number of reducers: 1  
- 2012-09-07 17:53:07,081 Stage-2 map = 0%,  reduce = 0%  
- 2012-09-07 17:53:10,351 Stage-2 map = 50%,  reduce = 0%  
- 2012-09-07 17:53:11,380 Stage-2 map = 100%,  reduce = 0%  
- 2012-09-07 17:53:18,132 Stage-2 map = 100%,  reduce = 100%  
- Ended Job = job_201208241319_2001621
- OK  
- abcdefghijk_0  
- abcdefghijk_1  
- abcdefghijk_2  
- abcdefghijk_3  
- abcdefghijk_4  
- abcdefghijk_5  
- abcdefghijk_6  
- abcdefghijk_7  
- abcdefghijk_8  
- abcdefghijk_9  
- Time taken: 108.301 seconds  
总结:
在资源充足的时候hive.exec.parallel会让那些存在并发job的sql运行得更快,但同时消耗更多的资源
可以评估下hive.exec.parallel对我们的刷新任务是否有帮助.
转载 ：[http://www.oratea.net/?p=1377](http://www.oratea.net/?p=1377)
