# linux下根据进程号PID查找程序路径 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月26日 11:06:47[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：9766








1、执行ps -u hdfs查看hdfs用户下在运行的进程；

     如：

```
ps -u hdfs
  PID TTY          TIME CMD
27939 ?        16:07:09 java
31211 ?        00:23:16 HwChrDecode
```


2、进入/proc相应进程PID的文件夹



```
#cd /proc/27939
#ls –ail
```


可以看到对应的程序路径



```
ls -ail
总计 0
1831010306 dr-xr-xr-x   5 hdfs hadoop 0 03-23 09:13 .
         1 dr-xr-xr-x 280 root root   0 03-18 10:18 ..
1831010327 dr-xr-xr-x   2 hdfs hadoop 0 05-26 10:33 attr
1831010315 -r--------   1 hdfs hadoop 0 05-26 10:28 auxv
1831010316 -r--r--r--   1 hdfs hadoop 0 05-26 04:18 cmdline
1831010337 -rw-r--r--   1 hdfs hadoop 0 05-26 10:28 coredump_filter
1831010326 -r--r--r--   1 hdfs hadoop 0 05-26 10:28 cpuset
1831010310 lrwxrwxrwx   1 hdfs hadoop 0 05-26 10:28 cwd -> /data1/hadoop/uts2-agent
1831010314 -r--------   1 hdfs hadoop 0 05-26 10:28 environ
1831010312 lrwxrwxrwx   1 hdfs hadoop 0 05-26 10:28 exe -> /usr/java/jdk1.6.0_35/bin/java
1831010313 dr-x------   2 hdfs hadoop 0 05-26 10:33 fd
1831010370 -r--r--r--   1 hdfs hadoop 0 05-26 10:28 io
1831010368 -r--------   1 hdfs hadoop 0 05-26 10:28 limits
1831010334 -rw-r--r--   1 hdfs hadoop 0 05-26 10:28 loginuid
1831010319 -r--r--r--   1 hdfs hadoop 0 05-26 10:28 maps
1831010309 -rw-------   1 hdfs hadoop 0 05-26 10:28 mem
1831010321 -r--r--r--   1 hdfs hadoop 0 05-26 04:17 mounts
1831010322 -r--------   1 hdfs hadoop 0 05-26 10:28 mountstats
1831010320 -r--r--r--   1 hdfs hadoop 0 05-26 10:28 numa_maps
1831010336 -rw-r--r--   1 hdfs hadoop 0 05-26 10:28 oom_adj
1831010335 -r--r--r--   1 hdfs hadoop 0 05-26 10:28 oom_score
1831010311 lrwxrwxrwx   1 hdfs hadoop 0 05-26 10:28 root -> /
1831010325 -r--r--r--   1 hdfs hadoop 0 05-26 10:28 schedstat
1831010324 -r--r--r--   1 hdfs hadoop 0 05-26 10:28 smaps
1831010317 -r--r--r--   1 hdfs hadoop 0 05-26 04:18 stat
1831010318 -r--r--r--   1 hdfs hadoop 0 05-26 10:21 statm
1831010308 -r--r--r--   1 hdfs hadoop 0 05-26 04:18 status
1831010307 dr-xr-xr-x  32 hdfs hadoop 0 05-26 10:33 task
1831010323 -r--r--r--   1 hdfs hadoop 0 05-26 10:28 wchan
```





