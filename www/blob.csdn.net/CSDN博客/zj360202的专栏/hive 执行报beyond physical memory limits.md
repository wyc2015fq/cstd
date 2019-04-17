# hive 执行报beyond physical memory limits - zj360202的专栏 - CSDN博客





2014年08月08日 15:43:30[zj360202](https://me.csdn.net/zj360202)阅读数：4592标签：[hive																[reduce																[physical memory limi																[物理内存](https://so.csdn.net/so/search/s.do?q=物理内存&t=blog)
个人分类：[Hive](https://blog.csdn.net/zj360202/article/category/1298103)





报错内容



```java
hive> INSERT OVERWRITE TABLE recommend_user_log_detail_show_old_device PARTITION (dt ='2014-08-08' )
    > select /*+ mapjoin(recommend_user_score)*/ c2.udid,c2.ad_id,c2.app_id,'0' as log_type,c2.dt,c2.hour,c2.time_minute,c2.time_second,concat(c2.dt,' ',c2.hour,':',c2.time_minute,':',c2.time_second) as log_time
    > from recommend_user_score_old_device_all a2
    > left outer join ios_show_log c2 on c2.udid=a2.userid and c2.dt ='2014-08-07'
    > where a2.dt ='2014-08-08'  and a2.userid not in ('9027E4556534','9027E44234D8','000000000000','28E02C3975C5');
Total jobs = 1
Stage-1 is selected by condition resolver.
Launching Job 1 out of 1
Number of reduce tasks not specified. Estimated from input data size: 9
In order to change the average load for a reducer (in bytes):
  set hive.exec.reducers.bytes.per.reducer=<number>
In order to limit the maximum number of reducers:
  set hive.exec.reducers.max=<number>
In order to set a constant number of reducers:
  set mapreduce.job.reduces=<number>
Starting Job = job_1407388927414_0274, Tracking URL = http://master:8088/proxy/application_1407388927414_0274/
Kill Command = /usr/local/hadoop-2.2.0/bin/hadoop job  -kill job_1407388927414_0274
Hadoop job information for Stage-1: number of mappers: 46; number of reducers: 9
2014-08-08 15:11:29,634 Stage-1 map = 0%,  reduce = 0%
2014-08-08 15:11:43,536 Stage-1 map = 1%,  reduce = 0%, Cumulative CPU 156.32 sec
2014-08-08 15:11:45,630 Stage-1 map = 3%,  reduce = 0%, Cumulative CPU 324.1 sec
2014-08-08 15:11:46,679 Stage-1 map = 5%,  reduce = 0%, Cumulative CPU 382.63 sec
2014-08-08 15:11:47,730 Stage-1 map = 7%,  reduce = 0%, Cumulative CPU 443.74 sec
2014-08-08 15:11:49,822 Stage-1 map = 12%,  reduce = 0%, Cumulative CPU 523.69 sec
2014-08-08 15:11:51,928 Stage-1 map = 15%,  reduce = 0%, Cumulative CPU 609.18 sec
2014-08-08 15:11:52,985 Stage-1 map = 18%,  reduce = 0%, Cumulative CPU 660.47 sec
2014-08-08 15:11:54,035 Stage-1 map = 23%,  reduce = 0%, Cumulative CPU 699.21 sec
2014-08-08 15:11:55,085 Stage-1 map = 29%,  reduce = 0%, Cumulative CPU 735.32 sec
2014-08-08 15:11:56,133 Stage-1 map = 35%,  reduce = 0%, Cumulative CPU 769.8 sec
2014-08-08 15:11:57,183 Stage-1 map = 40%,  reduce = 0%, Cumulative CPU 819.56 sec
2014-08-08 15:11:58,229 Stage-1 map = 43%,  reduce = 0%, Cumulative CPU 846.05 sec
2014-08-08 15:11:59,278 Stage-1 map = 48%,  reduce = 0%, Cumulative CPU 880.56 sec
2014-08-08 15:12:00,327 Stage-1 map = 52%,  reduce = 0%, Cumulative CPU 909.65 sec
2014-08-08 15:12:01,377 Stage-1 map = 54%,  reduce = 0%, Cumulative CPU 920.33 sec
2014-08-08 15:12:02,429 Stage-1 map = 56%,  reduce = 0%, Cumulative CPU 928.36 sec
2014-08-08 15:12:03,475 Stage-1 map = 59%,  reduce = 0%, Cumulative CPU 958.43 sec
2014-08-08 15:12:04,528 Stage-1 map = 60%,  reduce = 0%, Cumulative CPU 964.78 sec
2014-08-08 15:12:06,625 Stage-1 map = 62%,  reduce = 0%, Cumulative CPU 982.74 sec
2014-08-08 15:12:08,720 Stage-1 map = 62%,  reduce = 2%, Cumulative CPU 1025.58 sec
2014-08-08 15:12:09,768 Stage-1 map = 62%,  reduce = 3%, Cumulative CPU 1048.97 sec
2014-08-08 15:12:11,863 Stage-1 map = 62%,  reduce = 4%, Cumulative CPU 1109.31 sec
2014-08-08 15:12:12,914 Stage-1 map = 64%,  reduce = 6%, Cumulative CPU 1260.66 sec
2014-08-08 15:12:15,018 Stage-1 map = 64%,  reduce = 7%, Cumulative CPU 1344.31 sec
2014-08-08 15:12:16,064 Stage-1 map = 67%,  reduce = 3%, Cumulative CPU 1389.85 sec
2014-08-08 15:12:17,109 Stage-1 map = 68%,  reduce = 3%, Cumulative CPU 1407.49 sec
2014-08-08 15:12:19,203 Stage-1 map = 71%,  reduce = 3%, Cumulative CPU 1442.24 sec
2014-08-08 15:12:20,249 Stage-1 map = 73%,  reduce = 3%, Cumulative CPU 1513.36 sec
2014-08-08 15:12:22,347 Stage-1 map = 74%,  reduce = 0%, Cumulative CPU 1455.33 sec
2014-08-08 15:12:23,395 Stage-1 map = 75%,  reduce = 0%, Cumulative CPU 1470.49 sec
2014-08-08 15:12:24,445 Stage-1 map = 78%,  reduce = 0%, Cumulative CPU 1483.32 sec
2014-08-08 15:12:25,496 Stage-1 map = 80%,  reduce = 0%, Cumulative CPU 1509.45 sec
2014-08-08 15:12:26,545 Stage-1 map = 82%,  reduce = 0%, Cumulative CPU 1519.35 sec
2014-08-08 15:12:27,596 Stage-1 map = 83%,  reduce = 2%, Cumulative CPU 1535.38 sec
2014-08-08 15:12:28,654 Stage-1 map = 85%,  reduce = 0%, Cumulative CPU 1540.42 sec
2014-08-08 15:12:29,701 Stage-1 map = 85%,  reduce = 1%, Cumulative CPU 1548.09 sec
2014-08-08 15:12:30,745 Stage-1 map = 85%,  reduce = 3%, Cumulative CPU 1557.82 sec
2014-08-08 15:12:31,793 Stage-1 map = 87%,  reduce = 0%, Cumulative CPU 1633.62 sec
2014-08-08 15:12:32,848 Stage-1 map = 88%,  reduce = 0%, Cumulative CPU 1639.17 sec
2014-08-08 15:12:33,895 Stage-1 map = 90%,  reduce = 0%, Cumulative CPU 1644.45 sec
2014-08-08 15:12:34,939 Stage-1 map = 92%,  reduce = 0%, Cumulative CPU 1660.19 sec
2014-08-08 15:12:37,035 Stage-1 map = 92%,  reduce = 2%, Cumulative CPU 1677.26 sec
2014-08-08 15:12:38,079 Stage-1 map = 97%,  reduce = 0%, Cumulative CPU 1681.32 sec
2014-08-08 15:12:40,164 Stage-1 map = 98%,  reduce = 0%, Cumulative CPU 1691.46 sec
2014-08-08 15:12:41,208 Stage-1 map = 98%,  reduce = 1%, Cumulative CPU 1719.14 sec
2014-08-08 15:12:42,452 Stage-1 map = 99%,  reduce = 4%, Cumulative CPU 1741.44 sec
2014-08-08 15:12:43,495 Stage-1 map = 100%,  reduce = 6%, Cumulative CPU 1748.1 sec
2014-08-08 15:12:44,542 Stage-1 map = 100%,  reduce = 4%, Cumulative CPU 1715.24 sec
2014-08-08 15:12:45,588 Stage-1 map = 100%,  reduce = 6%, Cumulative CPU 1728.56 sec
2014-08-08 15:12:47,683 Stage-1 map = 100%,  reduce = 4%, Cumulative CPU 1718.33 sec
2014-08-08 15:12:48,729 Stage-1 map = 100%,  reduce = 2%, Cumulative CPU 1708.28 sec
2014-08-08 15:12:49,776 Stage-1 map = 100%,  reduce = 100%, Cumulative CPU 1697.59 sec
MapReduce Total cumulative CPU time: 28 minutes 17 seconds 590 msec
Ended Job = job_1407388927414_0274 with errors
Error during job, obtaining debugging information...
Examining task ID: task_1407388927414_0274_m_000003 (and more) from job job_1407388927414_0274
Examining task ID: task_1407388927414_0274_m_000004 (and more) from job job_1407388927414_0274
Examining task ID: task_1407388927414_0274_m_000020 (and more) from job job_1407388927414_0274
Examining task ID: task_1407388927414_0274_r_000002 (and more) from job job_1407388927414_0274
Examining task ID: task_1407388927414_0274_r_000004 (and more) from job job_1407388927414_0274
Examining task ID: task_1407388927414_0274_r_000002 (and more) from job job_1407388927414_0274
Examining task ID: task_1407388927414_0274_r_000000 (and more) from job job_1407388927414_0274
Examining task ID: task_1407388927414_0274_r_000003 (and more) from job job_1407388927414_0274

Task with the most failures(4): 
-----
Task ID:
  task_1407388927414_0274_r_000000

URL:
  http://master:8088/taskdetails.jsp?jobid=job_1407388927414_0274&tipid=task_1407388927414_0274_r_000000
-----
Diagnostic Messages for this Task:
Container [pid=17643,containerID=container_1407388927414_0274_01_000097] is running beyond physical memory limits. Current usage: 1.1 GB of 1 GB physical memory used; 1.7 GB of 2.1 GB virtual memory used. Killing container.
Dump of the process-tree for container_1407388927414_0274_01_000097 :
	|- PID PPID PGRPID SESSID CMD_NAME USER_MODE_TIME(MILLIS) SYSTEM_TIME(MILLIS) VMEM_USAGE(BYTES) RSSMEM_USAGE(PAGES) FULL_CMD_LINE
	|- 17643 8200 17643 17643 (bash) 2 2 108646400 301 /bin/bash -c /usr/local/jdk1.7.0_60/bin/java -Djava.net.preferIPv4Stack=true -Dhadoop.metrics.log.level=WARN  -Xmx1024m -Djava.io.tmpdir=/usr/local/hadoop-2.2.0/temp/nm-local-dir/usercache/root/appcache/application_1407388927414_0274/container_1407388927414_0274_01_000097/tmp -Dlog4j.configuration=container-log4j.properties -Dyarn.app.container.log.dir=/usr/local/hadoop-2.2.0/logs/userlogs/application_1407388927414_0274/container_1407388927414_0274_01_000097 -Dyarn.app.container.log.filesize=0 -Dhadoop.root.logger=INFO,CLA org.apache.hadoop.mapred.YarnChild 192.168.11.25 52043 attempt_1407388927414_0274_r_000000_3 97 1>/usr/local/hadoop-2.2.0/logs/userlogs/application_1407388927414_0274/container_1407388927414_0274_01_000097/stdout 2>/usr/local/hadoop-2.2.0/logs/userlogs/application_1407388927414_0274/container_1407388927414_0274_01_000097/stderr  
	|- 17742 17643 17643 17643 (java) 1643 266 1733853184 287434 /usr/local/jdk1.7.0_60/bin/java -Djava.net.preferIPv4Stack=true -Dhadoop.metrics.log.level=WARN -Xmx1024m -Djava.io.tmpdir=/usr/local/hadoop-2.2.0/temp/nm-local-dir/usercache/root/appcache/application_1407388927414_0274/container_1407388927414_0274_01_000097/tmp -Dlog4j.configuration=container-log4j.properties -Dyarn.app.container.log.dir=/usr/local/hadoop-2.2.0/logs/userlogs/application_1407388927414_0274/container_1407388927414_0274_01_000097 -Dyarn.app.container.log.filesize=0 -Dhadoop.root.logger=INFO,CLA org.apache.hadoop.mapred.YarnChild 192.168.11.25 52043 attempt_1407388927414_0274_r_000000_3 97 

Container killed on request. Exit code is 143


FAILED: Execution Error, return code 2 from org.apache.hadoop.hive.ql.exec.mr.MapRedTask
MapReduce Jobs Launched: 
Job 0: Map: 46  Reduce: 9   Cumulative CPU: 1697.59 sec   HDFS Read: 12383832954 HDFS Write: 0 FAIL
Total MapReduce CPU Time Spent: 28 minutes 17 seconds 590 msec
```

解决方案



```java
set mapreduce.map.memory.mb=1025;//只要大于1024，hive默认分配的内存分大一倍，也就是2048M
set mapreduce.reduce.memory.mb=1025;
```


在执行的hive之前加上上面的配置就好了](https://so.csdn.net/so/search/s.do?q=physical memory limi&t=blog)](https://so.csdn.net/so/search/s.do?q=reduce&t=blog)](https://so.csdn.net/so/search/s.do?q=hive&t=blog)




