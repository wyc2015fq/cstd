# hadoop 迭代 - zj360202的专栏 - CSDN博客





2013年02月01日 15:57:46[zj360202](https://me.csdn.net/zj360202)阅读数：891








1. MapReduce，组合式，迭代式，链式  [**http://www.cnblogs.com/liqizhou/archive/2012/05/14/2499653.html**](http://www.cnblogs.com/liqizhou/archive/2012/05/14/2499653.html)

这个

```java
Configuration job1conf = new Configuration();
Job job1 = new Job(job1conf);
.........//job1 其他设置
Configuration job2conf = new Configuration();
Job job2 = new Job(job2conf);
.........//job2 其他设置
Configuration job3conf = new Configuration();
Job job3 = new Job(job3conf);
.........//job3 其他设置
job3.addDepending(job1);//设置job3和job1的依赖关系
job3.addDepending(job2);
JobControl JC = new JobControl("123");
JC.addJob(job1);//把三个job加入到jobcontorl中
JC.addJob(job2);
JC.addJob(job3);
JC.run();
```



这个地方有一个错误的，这里的JOB是**org.apache.hadoop.mapred.jobcontrol.Job;**

**同时Job job3 = new Job(job3conf,"Job3");这个地方job3conf应该是JobConf，而不是configuratioin**

**感谢 一朋友的测试中发现这个问题！！！**



