# hdfs创建文件出错 - BlueSky - CSDN博客
2015年11月17日 16:18:57[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1565
hadoop fs 下的命令不能使用，使用hadoop fs -ls提示：ls: `.’: No such file or directory。 
**解决方案：**
 bin/hadoop hdfs fs -mkdir -p /in 
  bin/hadoop hdfs fs  -put /home/du/input   /in
如果Apache hadoop版本是2.x. 
  bin/hdfs  dfs  -mkdir -p /in 
  bin/hdfs  dfs   -put /home/du/input   /in
